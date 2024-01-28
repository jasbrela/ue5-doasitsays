// Copyright Epic Games, Inc. All Rights Reserved.

#include "DoAsItSaysCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InteractionWidget.h"
#include "Interactive.h"
#include "LookDeathZone.h"
#include "PickUpObject.h"
#include "Villain.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

ADoAsItSaysCharacter::ADoAsItSaysCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	// Create a CameraComponent	
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	CameraComponent->SetupAttachment(GetCapsuleComponent());
	CameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	CameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(CameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));
}

void ADoAsItSaysCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentInteractionRange = InteractionRange;
	
	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraShake(CameraShake);

	PrimaryActorTick.bCanEverTick = true;

	if (const UWorld* World = GetWorld(); InteractionClass && World)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(UGameplayStatics::GetPlayerController(World, 0),
		                                                     InteractionClass, TEXT("InteractionUI"));
		InteractionWidget->AddToViewport(0);
	}
}

void ADoAsItSaysCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	InteractionLineTrace();
}

void ADoAsItSaysCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// if CastChecked fails, the game crashes
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ADoAsItSaysCharacter::Move);
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ADoAsItSaysCharacter::Look);
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this,
		                                   &ADoAsItSaysCharacter::Interact);
		EnhancedInputComponent->BindAction(DropAction, ETriggerEvent::Triggered, this, &ADoAsItSaysCharacter::Drop);
	}
}

void ADoAsItSaysCharacter::OnMissionStatusChanged(int ID, bool Completed)
{
	if (AffectedAfterMissionCompletedID == ID)
	{
		CurrentInteractionRange = Completed ? MaxInteractionRange : InteractionRange;
	}
}

void ADoAsItSaysCharacter::Move(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D MovementVector = Value.Get<FVector2D>();
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ADoAsItSaysCharacter::Look(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		const FVector2D LookAxisVector = Value.Get<FVector2D>();
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADoAsItSaysCharacter::Interact(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		if (Value.Get<bool>())
		{
			// Not carrying anything, but looking to a pick up actor
			if (CurrentPickUpActor != nullptr && CarriedPickUpActor == nullptr)
			{
				CarriedPickUpActor = CurrentPickUpActor;
				CurrentPickUpActor = nullptr;
				const FAttachmentTransformRules rules = FAttachmentTransformRules(EAttachmentRule::SnapToTarget,
					EAttachmentRule::SnapToTarget, EAttachmentRule::KeepWorld, false);
				CarriedPickUpActor->OnPickUpObject();
				CarriedPickUpActor->AttachToComponent(Mesh1P, rules, TEXT("socket"));
				InteractionWidget->ToggleTooltip(false);
			}
			// Looking at a interactive actor
			else if (CurrentInteractiveActor != nullptr)
			{
				EInteractionEffect Effect = EInteractionEffect::None;
				
				if (CarriedPickUpActor != nullptr)
				{
					Effect = CarriedPickUpActor->Effect;
				}
				
				if (Villain)
				{
					if (Villain->GetRequiredEffect() == Effect && Effect != EInteractionEffect::None)
					{
						APickUpObject* Temp = CarriedPickUpActor;
						Drop(FInputActionValue(true));
						if (Temp)
						{
							Temp->OnDeliverObject();
						} else
						{
							UE_LOG(LogTemp, Warning, TEXT("didnt work"));
						}
					}
				}

				CurrentInteractiveActor->Interact(Effect);
			}
		}
	}
}

void ADoAsItSaysCharacter::Drop(const FInputActionValue& Value)
{
	if (Controller != nullptr)
	{
		if (Value.Get<bool>())
		{
			if (CarriedPickUpActor != nullptr)
			{
				WasCarryingObject = true;
				const FDetachmentTransformRules rules = FDetachmentTransformRules(
					EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, EDetachmentRule::KeepWorld, false);
				CarriedPickUpActor->DetachFromActor(rules);
				CarriedPickUpActor->OnDropObject();
				CarriedPickUpActor = nullptr;
			}
		}
	}
}


void ADoAsItSaysCharacter::InteractionLineTrace()
{
	if (const UWorld* World = GetWorld())
	{
		FHitResult Hit;

		FCollisionQueryParams QueryParams;
		QueryParams.AddIgnoredActor(this);

		FVector TraceStart = GetFirstPersonCameraComponent()->GetComponentLocation();
		FVector TraceEnd = TraceStart + GetFirstPersonCameraComponent()->GetForwardVector() * CurrentInteractionRange;

		World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams);

		//DrawDebugLine(World, TraceStart, TraceEnd, FColor::Orange);

		AActor* ActorHit = Hit.GetActor();

		if (Hit.bBlockingHit && IsValid(ActorHit))
		{
			if (IInteractive* Interactive = Cast<IInteractive>(ActorHit))
			{
				SetPickUpObject(nullptr);

				if (Interactive->bIsInteractive && CurrentInteractiveActor == nullptr)
				{
					SetInteractiveObject(Interactive);
				}
				else
				{
					SetInteractiveObject(nullptr);
				}
			}
			else if (APickUpObject* PickUpObject = Cast<APickUpObject>(ActorHit))
			{
				SetInteractiveObject(nullptr);
				SetPickUpObject(PickUpObject);
			}
			else if (Cast<ALookDeathZone>(ActorHit))
			{
				// TODO: Game Over
				UGameplayStatics::OpenLevel(this, TEXT("BadEnding_Level"));
			}
			else
			{
				SetInteractiveObject(nullptr);
				SetPickUpObject(nullptr);
			}
		}
		else
		{
			SetInteractiveObject(nullptr);
			SetPickUpObject(nullptr);
		}
	}
}

void ADoAsItSaysCharacter::SetInteractiveObject(IInteractive* Interactive)
{
	if (CurrentInteractiveActor != Interactive)
	{
		if (CurrentInteractiveActor != nullptr)
		{
			CurrentInteractiveActor->OnExitRange();
		}

		CurrentInteractiveActor = Interactive;

		if (CurrentInteractiveActor != nullptr)
		{
			CurrentInteractiveActor->OnEnterRange();
			
			if (Villain == nullptr)
			{
				if (AVillain* CurrentActor = Cast<AVillain>(CurrentInteractiveActor))
				{
					Villain = CurrentActor;
				}
			}
		}
		
		FString TooltipText = CurrentInteractiveActor != nullptr ? TEXT("[E] " + Interactive->Tooltip) : TEXT("");

		if (Villain != nullptr && CurrentInteractiveActor == Villain)
		{
			const EInteractionEffect RequiredEffect = Villain->GetRequiredEffect();
			
			if (RequiredEffect != EInteractionEffect::None)
			{
				if (CarriedPickUpActor)
				{
					if (RequiredEffect == CarriedPickUpActor->Effect)
					{
						TooltipText = TEXT("[E] Deliver");
					} else
					{
						CurrentInteractiveActor = nullptr;
					}
				}
			}
		}

		InteractionWidget->ToggleTooltip(CurrentInteractiveActor != nullptr, TooltipText);
	}
}

void ADoAsItSaysCharacter::SetPickUpObject(APickUpObject* PickUp)
{
	if (CurrentPickUpActor != PickUp)
	{
		CurrentPickUpActor = PickUp;

		const FString Instruction = CarriedPickUpActor ? TEXT("Full hands\n[G] Drop item") : TEXT("[E] Pick Up");
		InteractionWidget->ToggleTooltip(PickUp != nullptr, Instruction);
	}
	else if (WasCarryingObject && !CarriedPickUpActor)
	{
		if (PickUp != nullptr)
		{
			WasCarryingObject = false;
			CurrentPickUpActor = PickUp;
			InteractionWidget->ToggleTooltip(true, TEXT("[E] Pick Up"));
		}
	}
}

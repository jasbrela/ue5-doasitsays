// Copyright Epic Games, Inc. All Rights Reserved.

#include "DoAsItSaysCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "InteractionWidget.h"
#include "Interactive.h"
#include "Blueprint/UserWidget.h"
#include "Engine/LocalPlayer.h"
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

	UGameplayStatics::GetPlayerCameraManager(this, 0)->StartCameraShake(CameraShake);
	
	PrimaryActorTick.bCanEverTick = true;

	if (const UWorld* World = GetWorld(); InteractionClass && World)
	{
		InteractionWidget = CreateWidget<UInteractionWidget>(UGameplayStatics::GetPlayerController(World, 0), InteractionClass, TEXT("InteractionUI"));
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
		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &ADoAsItSaysCharacter::Interact);
	}
}

void ADoAsItSaysCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void ADoAsItSaysCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void ADoAsItSaysCharacter::Interact(const FInputActionValue& Value)
{
	const bool Pressed = Value.Get<bool>();
	
	if (Controller != nullptr)
	{
		if (Pressed)
		{
			if (CurrentInteractiveActor != nullptr)
			{
				CurrentInteractiveActor->Interact();
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
		FVector TraceEnd = TraceStart + GetFirstPersonCameraComponent()->GetForwardVector() * InteractionRange;
		
		World->LineTraceSingleByChannel(Hit, TraceStart, TraceEnd, ECollisionChannel::ECC_Visibility, QueryParams);
		
		//DrawDebugLine(World, TraceStart, TraceEnd, FColor::Orange);

		AActor* ActorHit = Hit.GetActor();
		
		if (Hit.bBlockingHit && IsValid(ActorHit))
		{
			if (IInteractive* Interactive = Cast<IInteractive>(ActorHit))
			{
				if (CurrentInteractiveActor != Interactive) {
					if (CurrentInteractiveActor != nullptr)
					{
						CurrentInteractiveActor->OnExitRange();
					}
					
					if (Interactive->bIsInteractive && CurrentInteractiveActor == nullptr)
					{
						CurrentInteractiveActor = Interactive;
						InteractionWidget->ToggleInteraction(true, CurrentInteractiveActor->Tooltip);
						CurrentInteractiveActor->OnEnterRange();
						//UE_LOG(LogTemp, Warning, TEXT("New Interactive Set"));
					} else
					{
						CurrentInteractiveActor = nullptr;
						InteractionWidget->ToggleInteraction(false);
					}
				}
				else if (!CurrentInteractiveActor->bIsInteractive)
				{
					CurrentInteractiveActor->OnExitRange();
					CurrentInteractiveActor = nullptr;
					InteractionWidget->ToggleInteraction(false);
				}
			}
		} else
		{
			if (CurrentInteractiveActor != nullptr)
			{
				CurrentInteractiveActor->OnExitRange();
				CurrentInteractiveActor = nullptr;
				InteractionWidget->ToggleInteraction(false);
				
				//UE_LOG(LogTemp, Warning, TEXT("No Interactive"));
			}
		}
	}
}

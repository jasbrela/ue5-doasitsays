// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "AffectedByMission.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DoAsItSaysCharacter.generated.h"

class AVillain;
class APickUpObject;
class IInteractive;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInteractionWidget;
class UInputMappingContext;
struct FInputActionValue;

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class ADoAsItSaysCharacter : public ACharacter, public IAffectedByMission
{
	GENERATED_BODY()
public:
	ADoAsItSaysCharacter();
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return CameraComponent; }

protected:
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	virtual void OnMissionStatusChanged(int ID, bool Completed = true) override;
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void Drop(const FInputActionValue& Value);
	void InteractionLineTrace();
	void SetInteractiveObject(IInteractive* Interactive);
	void SetPickUpObject(APickUpObject* PickUp);
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	TSubclassOf<UUserWidget> InteractionClass;
	
	UPROPERTY()
	UInteractionWidget* InteractionWidget;

private:
	UPROPERTY(EditAnywhere)
	int AffectedAfterMissionCompletedID = 0;
	
	UPROPERTY(EditDefaultsOnly, Category=Interaction)
	int InteractionRange;

	UPROPERTY(EditDefaultsOnly, Category=Interaction)
	int MaxInteractionRange;
	
	int CurrentInteractionRange = 0;

	bool WasCarryingObject = false;
	
	UPROPERTY()
	AVillain* Villain;
	
	IInteractive* CurrentInteractiveActor;
	/// the Pick Up Actor the Character is currently looking at
	UPROPERTY(VisibleInstanceOnly)
	APickUpObject* CurrentPickUpActor;
	/// the Pick Up Actor the Character is currently carrying
	UPROPERTY(VisibleInstanceOnly)
	APickUpObject* CarriedPickUpActor;
	
	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(VisibleDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;
	
	UPROPERTY(VisibleAnywhere, Category = Camera)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(EditDefaultsOnly, Category=Input)
	UInputAction* MoveAction;
	
	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* LookAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* InteractAction;

	UPROPERTY(EditDefaultsOnly, Category = Input)
	UInputAction* DropAction;

	UPROPERTY(EditDefaultsOnly, Category = Camera)
	TSubclassOf<UCameraShakeBase> CameraShake;
};


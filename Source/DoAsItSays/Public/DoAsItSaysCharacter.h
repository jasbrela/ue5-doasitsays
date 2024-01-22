// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "DoAsItSaysCharacter.generated.h"

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
class ADoAsItSaysCharacter : public ACharacter
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
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void InteractionLineTrace();
	
	UPROPERTY(EditDefaultsOnly, Category="Interaction")
	TSubclassOf<UUserWidget> InteractionClass;
	
	UPROPERTY()
	UInteractionWidget* InteractionWidget;

private:
	UPROPERTY(EditDefaultsOnly, Category=Interaction)
	int InteractionRange = 100;

	IInteractive* CurrentInteractiveActor;
	
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
};


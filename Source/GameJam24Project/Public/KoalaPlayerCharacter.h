// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KoalaBaseCharacter.h"
#include "KoalaPlayerCharacter.generated.h"

/**
 * 
 */
struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class GAMEJAM24PROJECT_API AKoalaPlayerCharacter : public AKoalaBaseCharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKoalaPlayerCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	APlayerController* PlayerController;


public:
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USpringArmComponent* SpringArmComp;

public:
	// Interactions
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactions | Player")
		float InteractionRange = 100.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Interactions | Player")
		bool bIsOnTree;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interactions | Player")
		AActor* ItemCarriedOnBack;  // For carrying koalas on the back or items etc. Should be AActor because Koala is Character and ABaseInteractableObject is another class itself
	bool IsCarryingItem() const { return ItemCarriedOnBack != nullptr; }

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void PlayerJump(const FInputActionValue& Value);

private:
	// Actions
	UPROPERTY(EditAnywhere, Category = "Input")
		UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* InteractAction;
};

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

	virtual void Die() override;


public:
	// Components
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class UCameraComponent* CameraComp;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		class USpringArmComponent* SpringArmComp;

	UPROPERTY(EditDefaultsOnly, Category = "Shooting");
		TSubclassOf<class AGun>  GunClass;
	

public:
	// Interactions
	UPROPERTY(EditAnywhere, Category = "Interactions | Player")
		FName ItemCarryBoneNameOnMesh;
	UPROPERTY(EditAnywhere, Category = "Interactions | Player")
		FName ItemCarryBoneNameOnMeshWEAPON;
	UPROPERTY(EditAnywhere, Category = "Interactions | Player")
		float InteractionRange = 100.f;
	UPROPERTY(EditAnywhere, Category = "Interactions | Player")
		float ItemLaunchForceAmount = 100.f;  // For launching the object IF we are doing it
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Interactions | Player")
		AActor* ItemCarriedOnBack;  // For carrying koalas on the back or items etc. Should be AActor because Koala is Character and ABaseInteractableObject is another class itself
	
	UFUNCTION(BlueprintCallable)
		bool IsCarryingItem() const { return ItemCarriedOnBack != nullptr; }
	void CarryItemOnBack(AActor* ItemToCarry);
	void DropCurrentCarriedItem();
	void ReloadGun();
	UPROPERTY()
		class AGun* Gun;

	

public:
	// Climbing
	UPROPERTY(EditAnywhere, Category = "Climbing")
		float TreeClimbingSpeed = 20.f;
	UPROPERTY(EditAnywhere, Category = "Climbing")
		float TreeAttachmentRadius = 20.f;  // Distance between player and tree when player is climbing the tree. For the initial teleportation
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Climbing")
		bool bIsOnTree;
	class ABaseTree* CurrentClimbingTree;
	void DetachFromCurrentTree();

public:
	// HUD and Widgets
	void RemoveAllPlayerWidgets();

	UFUNCTION(BlueprintCallable)
		void MakeStartingWidgets();
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		class UMissionObjectivesWidget* ObjectivesWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UMissionObjectivesWidget> ObjectivesWidgetClass;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Widgets")
		class UBasicPlayerWidget* BasicPlayerWidget;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Widgets")
		TSubclassOf<class UBasicPlayerWidget> BasicPlayerWidgetClass;
	UFUNCTION(BlueprintCallable)
		float GetTimeLeftProportional() const;
	UFUNCTION(BlueprintCallable)
		int GetBabyKoalasLeft() const;
	UFUNCTION(BlueprintCallable)
		float GetGunAmmoProportional() const;

	void UpdateKoalasAliveWidget();

public:
	// Animations
	float ClimbingDir;

	UPROPERTY(EditAnywhere, Category = "Animations | Locomotion")
		UAnimMontage* TreeStartClimbingMontage;

private:
	void Move(const FInputActionValue& Value);
	void NotMoving(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interact(const FInputActionValue& Value);
	void PickupAndCarryItem(const FInputActionValue& Value);
	void PlayerJump(const FInputActionValue& Value);
	void Shoot(const FInputActionValue& Value);
	void StopShoot(const FInputActionValue& Value);
	void StopShootNotPressed(const FInputActionValue& Value);

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
	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* CarryItemAction;
	UPROPERTY(EditAnywhere, Category = "Input")
		UInputAction* ShootAction;

};

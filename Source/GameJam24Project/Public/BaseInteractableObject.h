// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "InteractableObjectInterface.h"
#include "BaseInteractableObject.generated.h"

UCLASS()
class GAMEJAM24PROJECT_API ABaseInteractableObject : public AActor, public IInteractableObjectInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseInteractableObject();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// It is better to not have the interact function here. Because it is done by player or AI not by itself
	// Unless we are doing timing and delays

	class AKoalaGameModeBase* MGameMode;

	virtual void Interact() override;
	UFUNCTION(BlueprintCallable)
		bool IsConsumable() const;
	UPROPERTY(EditAnywhere)
		UStaticMeshComponent* BaseMeshComp;  // This is the base mesh for the object

};

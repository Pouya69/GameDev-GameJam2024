// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractableObject.h"
#include "Consumable.generated.h"

/**
 * 
 */
UENUM(BlueprintType)
enum class EConsumableType {
	HEALTH_ONLY				UMETA(DisplayName="Health Only"),
	STAMINA_ONLY			UMETA(DisplayName = "Stamina Only"),
	STAMINA_AND_HEALTH		UMETA(DisplayName = "Health and Stamina"),
	WATER		UMETA(DisplayName = "Water"),
};

UCLASS()
class GAMEJAM24PROJECT_API AConsumable : public ABaseInteractableObject
{
	GENERATED_BODY()
	
public:
	AConsumable();

public:
	void Consume(class AKoalaBaseCharacter* Consumer);
	void DestroyItemHandleFire();  // For example when the item gets set on fire?
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
		float AdditionToConsumer = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
		EConsumableType ItemType;

private:
	
};

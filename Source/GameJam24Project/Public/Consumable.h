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
	HEALTH_ONLY							UMETA(DisplayName="Health Only"),
	STAMINA_ONLY						UMETA(DisplayName = "Stamina Only"),
	STAMINA_AND_HEALTH					UMETA(DisplayName = "Health and Stamina"),
	STAMINA_AND_HEALTH_AND_WATER		UMETA(DisplayName = "Health and Stamina and Water"),
	WATER								UMETA(DisplayName = "Water"),
	POOP								UMETA(DisplayName = "Poop"),
};

UCLASS()
class GAMEJAM24PROJECT_API AConsumable : public ABaseInteractableObject
{
	GENERATED_BODY()
	
public:
	AConsumable();


protected:
	virtual void BeginPlay() override;

public:
	void Consume(class AKoalaBaseCharacter* Consumer);
	UFUNCTION()
		void DestroyItemHandleFire(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser);  // For example when the item gets set on fire?
	
	UPROPERTY(EditAnywhere, Category = "Effects")
		class UNiagaraComponent* NiagaraEffect;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
		float AdditionToConsumer = 20.f;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
		EConsumableType ItemType;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Player")
		float HealthReductionFire = 20;

	UPROPERTY(EditAnywhere, Category = "Death")
		UMaterialInterface* DeathMaterial;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		USoundBase* ConsumeSound;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Sound")
		USoundBase* DestroySound;

private:
	float Health = 100.f;
};

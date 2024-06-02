// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "BabyAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API UBabyAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UBabyAnimInstance();

public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;
protected:
	virtual void NativeBeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float CharacterSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float CharacterDirection;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bIsFalling;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bIsOnFire;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bIsDead;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bIsSleeping;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bIsBeingCarried;
};

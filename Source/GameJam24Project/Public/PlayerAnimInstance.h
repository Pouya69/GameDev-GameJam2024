// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "PlayerAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API UPlayerAnimInstance : public UAnimInstance
{
	GENERATED_BODY()

public:
	UPlayerAnimInstance();

public:
	virtual void NativeUpdateAnimation(float DeltaTime) override;
protected:
	virtual void NativeBeginPlay() override;

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float CharacterSpeed;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float OnGroundCharacterDirection;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bIsClimbingTree;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float ClimbingCharacterDirection;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		float CharacterPitch;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bIsFalling;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bIsOnFire;
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
		bool bIsDead;
};

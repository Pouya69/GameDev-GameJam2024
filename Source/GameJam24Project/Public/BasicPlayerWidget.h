// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "BasicPlayerWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API UBasicPlayerWidget : public UUserWidget
{
	GENERATED_BODY()

protected:
	virtual void NativeConstruct() override;
	
private:
	class AKoalaPlayerCharacter* PlayerCharacter;

	UFUNCTION(BlueprintCallable)
		float GetHealth() const;
	UFUNCTION(BlueprintCallable)
		float GetStamina() const;
	UFUNCTION(BlueprintCallable)
		bool IsOnFire() const;
	UFUNCTION(BlueprintCallable)
		float GetTimeLeft() const;
};

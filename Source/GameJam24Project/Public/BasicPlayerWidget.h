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
	
public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int TotalBabyKoalas;

	DECLARE_EVENT(FLayerViewModel, FMakeKoalaItems)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void MakeKoalaItemsWidget();

	FMakeKoalaItems& OnMakeKoalaItems() { return MakeKoalaItems; }

	virtual void MakeKoalaItemsWidget_Implementation();
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
	UFUNCTION(BlueprintCallable)
		int GetBabyKoalasLeft() const;

	FMakeKoalaItems MakeKoalaItems;

};

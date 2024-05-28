// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "MissionObjectiveItemWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API UMissionObjectiveItemWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		class AMissionObjective* MissionObjectiveLinked;
	virtual void NativeConstruct() override;
	//UFUNCTION(BlueprintCallable)
		//void ObjectiveDone();
	UFUNCTION(BlueprintCallable)
		FString& GetTitle() const;
	UFUNCTION(BlueprintCallable)
		FString& GetDescription() const;
	UFUNCTION(BlueprintCallable)
		void MakeObjectiveItemWidget(class AMissionObjective* MissionObjective);
};

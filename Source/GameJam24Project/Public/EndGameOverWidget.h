// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "EndGameOverWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API UEndGameOverWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

	void BuildGameOverWidget(bool bWon, const FString& Message, const TSubclassOf<UUserWidget>& InNextLevelIntroWidgetClass, int InStars);

	DECLARE_EVENT(FLayerViewModel, FDisplayMyWidget)
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
		void DisplayMyWidget();

	virtual void DisplayMyWidget_Implementation();

	FDisplayMyWidget& OnDisplayMyWidget() { return DisplayMyWidgetEvent; }

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		bool bWonGame = false;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		FString EndGameMessage;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		TSubclassOf<UUserWidget> NextLevelIntroWidgetClass;
	UPROPERTY(EditAnywhere, BlueprintReadOnly)
		int Stars;

private:
	FDisplayMyWidget DisplayMyWidgetEvent;
};

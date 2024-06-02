// Fill out your copyright notice in the Description page of Project Settings.


#include "EndGameOverWidget.h"

void UEndGameOverWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UEndGameOverWidget::BuildGameOverWidget(bool bWon, const FString& Message, const FString& InNextLevelName, int InStars)
{
	EndGameMessage = Message;
	bWonGame = bWon;
	NextLevelName = InNextLevelName;
	Stars = InStars;
	DisplayMyWidget_Implementation();
	DisplayMyWidget();
	// UGameplayStatics::OpenLevel(NextLevelName);
}

void UEndGameOverWidget::DisplayMyWidget_Implementation()
{
}

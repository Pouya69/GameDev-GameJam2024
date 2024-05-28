// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayerWidget.h"
#include "KoalaPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

void UBasicPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerCharacter = Cast<AKoalaPlayerCharacter>(GetOwningPlayerPawn());
}


float UBasicPlayerWidget::GetHealth() const
{
	return PlayerCharacter->GetHealth() / 100;
}

float UBasicPlayerWidget::GetStamina() const
{
	// UE_LOG(LogTemp, Warning, TEXT("STAMINA WIDGET: %f"), PlayerCharacter->GetStamina());
	return PlayerCharacter->GetStamina() / 100;
}

bool UBasicPlayerWidget::IsOnFire() const
{
	return PlayerCharacter->IsOnFire();
}

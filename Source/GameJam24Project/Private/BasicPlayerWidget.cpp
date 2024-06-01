// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicPlayerWidget.h"
#include "KoalaPlayerCharacter.h"

void UBasicPlayerWidget::NativeConstruct()
{
	Super::NativeConstruct();
	PlayerCharacter = Cast<AKoalaPlayerCharacter>(GetOwningPlayerPawn());
	TotalBabyKoalas = PlayerCharacter->GetBabyKoalasLeft();
}


void UBasicPlayerWidget::MakeKoalaItemsWidget_Implementation()
{
}

float UBasicPlayerWidget::GetHealth() const
{
	return (PlayerCharacter != nullptr) ? PlayerCharacter->GetHealth() / 100 : 0.f;
}

float UBasicPlayerWidget::GetStamina() const
{
	return (PlayerCharacter != nullptr) ? PlayerCharacter->GetStamina() / 100 : 0.f;
}

bool UBasicPlayerWidget::IsOnFire() const
{
	return (PlayerCharacter != nullptr) ? PlayerCharacter->IsOnFire() : false;
}

bool UBasicPlayerWidget::IsHavingWeapon() const
{
	return (PlayerCharacter != nullptr) ? !PlayerCharacter->bIsOnTree && !PlayerCharacter->IsDead() : false;
}

float UBasicPlayerWidget::GetTimeLeft() const
{
	return (PlayerCharacter != nullptr) ? PlayerCharacter->GetTimeLeftProportional() : 0.f;
}

float UBasicPlayerWidget::GetGunAmmo() const
{
	return (PlayerCharacter != nullptr) ? PlayerCharacter->GetGunAmmoProportional() : 0.f;
}

int UBasicPlayerWidget::GetBabyKoalasLeft() const
{
	return (PlayerCharacter != nullptr) ? PlayerCharacter->GetBabyKoalasLeft() : 0;
}

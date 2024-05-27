// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaBabyCharacter.h"
#include "Components/CapsuleComponent.h"
AKoalaBabyCharacter::AKoalaBabyCharacter()
{
	CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
}

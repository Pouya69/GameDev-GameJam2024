// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaBabyCharacter.h"
#include "Components/CapsuleComponent.h"


AKoalaBabyCharacter::AKoalaBabyCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
}

void AKoalaBabyCharacter::BeginPlay()
{
	Super::BeginPlay();
}

void AKoalaBabyCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddStamina(FMath::RandRange(-StaminaDeductionRateRandomness, StaminaDeductionRateRandomness));
}

void AKoalaBabyCharacter::Die()
{
	/*// SetRootComponent(GetMesh());
	CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	// CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Ignore);
	// CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	// CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);
	
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->AddImpulse(DeathMeshImpulse);
	// GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);*/
	

	// GetController()->Destroy();
	Super::Die();
}
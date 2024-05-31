// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "BaseTree.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Consumable.h"
#include "KoalaPlayerCharacter.h"
#include "Fire.h"

// Sets default values
AKoalaBaseCharacter::AKoalaBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = Cast<UCapsuleComponent>(GetRootComponent());
	// CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
	
	
}

// Called when the game starts or when spawned
void AKoalaBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//OnTakeAnyDamage.AddDynamic(this, &AKoalaBaseCharacter::DamageTakenHandle);
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	
	
}

// Called every frame
void AKoalaBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsBeingCarried || bIsDead) return;  // Stamina will not reduce if it is being carried
	if (Stamina <= 0) {
		Sleep();
		return;
	}
	float StaminaFinalDeducationAmount = IsCharacterMoving() ? (StaminaDeductionRate * StaminaDeductionMultiplierMoving) : StaminaDeductionRate;
	Stamina -= StaminaFinalDeducationAmount;
	// UE_LOG(LogTemp, Warning, TEXT("Stamina: %f"), GetStamina());

}

bool AKoalaBaseCharacter::AreThereAnyTreesAround(FHitResult& OutHitResult) const
{
	bool bResult = GetObjectAround(OutHitResult, TreeDistanceCheck);
	if (bResult) {
		// To check if the hit actor was a tree specifically
		bResult = OutHitResult.GetActor()->IsA(ABaseTree::StaticClass());
	}
	return bResult;
}

bool AKoalaBaseCharacter::GetObjectAround(FHitResult& OutHitResult, float RangeCheck) const
{
	FVector Start = GetActorLocation();
	FVector End = Start + (RangeCheck * GetActorForwardVector());
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);
	bool bResult = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity, ECollisionChannel::ECC_GameTraceChannel1, FCollisionShape::MakeSphere(RangeCheck), Params);
	if (bResult) {
		// To check whether the interacted object is the ABaseInteractableObject OR a character (baby koala for example)
		bResult = OutHitResult.GetActor()->IsA(ABaseInteractableObject::StaticClass()) || OutHitResult.GetActor()->IsA(ACharacter::StaticClass());
	}

	return bResult;
}

bool AKoalaBaseCharacter::IsOnFire() const
{
	/*TODO: Implement it*/
	TArray<AActor*> OverlapActors;
	CapsuleComp->GetOverlappingActors(OverlapActors, FireClass);

	return !OverlapActors.IsEmpty();

	// Delete line below
	// return false;
}

bool AKoalaBaseCharacter::IsCharacterMoving() const
{
	FVector Velocity = GetVelocity();
	if (GetCharacterMovement()->IsFalling()) {
		// If character is in air we are not checking the Z velocity. We will only check Z velocity when going up trees and etc.
		Velocity.Z = 0;
	}
	return Velocity.Length() >= MovementCheckSpeed;
}

// Calling TakeDamage in this way caused the engine to crash (Something wrong from fire?). But using the Take damage below the problem is fixed 

/* void AKoalaBaseCharacter::DamageTakenHandle(AActor* DamagedActor, float Damage, const UDamageType* DamageType, AController* InstigatedBy, AActor* DamageCauser)
{
	// We need to do ApplyDamage in Fire class or whatever
	FDamageEvent DamageEvent = FDamageEvent(UDamageType::StaticClass());
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, InstigatedBy, DamageCauser);
	Health -= Damage;
	if (Health <= 0) {
		Die();
	}
} */

float AKoalaBaseCharacter::TakeDamage(float Damage, FDamageEvent const &DamageEvent,  AController* InstigatedBy, AActor* DamageCauser)
{	
	// UE_LOG(LogTemp, Warning, TEXT("Taking Damage: %s"), *GetName());
	if (bIsDead) return 0.f;
	const bool bShouldTakeDamage =  GetWorld()->GetTimeSeconds()  - LastDamageTime > TimeBetweenDamage;
	if(!bShouldTakeDamage)
	{
		return 0;
	}
	LastDamageTime = GetWorld()->GetTimeSeconds();
	// float ActualDamage = Super::TakeDamage(Damage, DamageEvent, InstigatedBy, DamageCauser);
	Health -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("New Health, %s: %f"), *GetName(), Health);
	if (Health <= 0) {
		Die();
	}

	return Damage;
}

void AKoalaBaseCharacter::Die()
{
	// TODO: Death stuff
	// This line below alerts other listeners of the event that character has died
	if (bIsDead) return;
	CapsuleComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);  // WIll no longer be pawn
	if (DeathMaterial != nullptr) {
		GetMesh()->SetMaterial(0, DeathMaterial);
	}
	bIsDead = true;
	SetActorTickEnabled(false);
	DeathEvent.Broadcast();
	
	
}

void AKoalaBaseCharacter::ConsumeItem(AConsumable* Consumable)
{
	if (!Consumable) return;
	Consumable->Consume(this);
}

void AKoalaBaseCharacter::AddHealth(float Amount)
{
	// TODO: Any effects and sounds will play here
	Health += Amount;
	if (Health > 100) Health = 100;
}

void AKoalaBaseCharacter::AddStamina(float Amount)
{
	// TODO: Any effects and sounds will play here
	Stamina += Amount;
	if (Stamina > 100) Stamina = 100;
}

void AKoalaBaseCharacter::Sleep()
{
	if (IsSleeping() || IsDead()) return;  // Don't do anything if already sleeping or dead
	GetWorldTimerManager().ClearTimer(SleepTimerHandle);  // Clear already existing timer just in case.
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {
		if (!bIsDead) {
			// Wake up after sleeping for X seconds
			if (GetController()->IsPlayerController()) {
				EnableInput(Cast<APlayerController>(GetController()));
			}
			bIsSleeping = false;
			Stamina = StaminaAfterSleep;
			// UE_LOG(LogTemp, Warning, TEXT("Awake: %s"), *GetFullName());
		}
		
	});
	if (GetController()->IsPlayerController()) {
		DisableInput(Cast<APlayerController>(GetController()));
		AKoalaPlayerCharacter* PlayerCharacter = Cast<AKoalaPlayerCharacter>(this);
		PlayerCharacter->DropCurrentCarriedItem();
		PlayerCharacter->DetachFromCurrentTree();
	}
	bIsSleeping = true;
	// UE_LOG(LogTemp, Warning, TEXT("Sleeping: %s"), *GetFullName());
	GetWorldTimerManager().SetTimer(SleepTimerHandle, TimerDelegate, SleepDelay, false);
}

void AKoalaBaseCharacter::StopCharacterMovement()
{
	GetCharacterMovement()->StopMovementImmediately();
}
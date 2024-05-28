// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "BaseTree.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Consumable.h"
#include "KoalaPlayerCharacter.h"

// Sets default values
AKoalaBaseCharacter::AKoalaBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = Cast<UCapsuleComponent>(GetRootComponent());

	
}

// Called when the game starts or when spawned
void AKoalaBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	//OnTakeAnyDamage.AddDynamic(this, &AKoalaBaseCharacter::DamageTakenHandle);
	
	
}

// Called every frame
void AKoalaBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (bIsBeingCarried) return;  // Stamina will not reduce if it is being carried
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
	// TArray<AActor*> OverlapActors;
	// GetOverlappingActors(OverlapActors, AFire::StaticClass());

	// return !OverlapActors.IsEmpty();

	// Delete line below
	return false;
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
	float ActualDamage = Super::TakeDamage(Damage, DamageEvent, InstigatedBy, DamageCauser);
	Health -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("Damage taken. New Health %f"), Health);
	if (Health <= 0) {
		Die();
	}

	return ActualDamage;
}

void AKoalaBaseCharacter::Die()
{
	// TODO: Death stuff
	SetActorTickEnabled(false);
	if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
		PlayerController->DisableInput(PlayerController);
	}
	// This line below alerts other listeners of the event that character has died
	CapsuleComp->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);  // WIll no longer be pawn
	bIsDead = true;
	DeathEvent.Broadcast();
	
}

void AKoalaBaseCharacter::ConsumeItem(AConsumable* Eucalyptus)
{
	if (!Eucalyptus) return;
	Eucalyptus->Consume(this);
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
	if (IsSleeping()) return;  // Don't do anything if already sleeping
	GetWorldTimerManager().ClearTimer(SleepTimerHandle);  // Clear already existing timer just in case.
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {
		// Wake up after sleeping for X seconds
		if (GetController()->IsPlayerController()) {
			EnableInput(Cast<APlayerController>(GetController()));
		}
		bIsSleeping = false;
		Stamina = StaminaAfterSleep;
		UE_LOG(LogTemp, Warning, TEXT("Awake: %s"), *GetFullName());
	});
	if (GetController()->IsPlayerController()) {
		DisableInput(Cast<APlayerController>(GetController()));
		AKoalaPlayerCharacter* PlayerCharacter = Cast<AKoalaPlayerCharacter>(this);
		PlayerCharacter->DropCurrentCarriedItem();
		PlayerCharacter->DetachFromCurrentTree();
	}
	bIsSleeping = true;
	UE_LOG(LogTemp, Warning, TEXT("Sleeping: %s"), *GetFullName());
	GetWorldTimerManager().SetTimer(SleepTimerHandle, TimerDelegate, SleepDelay, false);
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaBaseCharacter.h"
#include "Components/CapsuleComponent.h"
#include "BaseTree.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Consumable.h"
#include "KoalaPlayerCharacter.h"
#include "Fire.h"
#include "Gun.h"
#include "KoalaGameModeBase.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraComponent.h"
#include "Components/AudioComponent.h"

// Sets default values
AKoalaBaseCharacter::AKoalaBaseCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	// PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bCanEverTick = true;
	CapsuleComp = Cast<UCapsuleComponent>(GetRootComponent());
	NiagaraSleeping = CreateDefaultSubobject<UNiagaraComponent>(FName("Sleeping Effect"));
	NiagaraSleeping->SetupAttachment(GetMesh(), FName("sleeping"));
	// CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel2, ECollisionResponse::ECR_Overlap);
	
	
}

// Called when the game starts or when spawned
void AKoalaBaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	InitialMeshRotation = GetMesh()->GetRelativeRotation();
	//OnTakeAnyDamage.AddDynamic(this, &AKoalaBaseCharacter::DamageTakenHandle);
	GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
	GetCharacterMovement()->MaxWalkSpeed = NormalSpeed;
	GameMode = Cast<AKoalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	if (NiagaraSleeping) {
		NiagaraSleeping->SetHiddenInGame(true);
	}
	SoundPitches = this->IsA(AKoalaPlayerCharacter::StaticClass()) ? 1.f : 5.f;
}

// Called every frame
void AKoalaBaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (!GameMode || GameMode->bGameIsOver || bIsDead) {
		GetWorld()->GetTimerManager().ClearTimer(SleepTimerHandle);
		// GetWorld()->GetTimerManager().ClearTimer(SleepSoundTimerHandle);
		GetWorld()->GetTimerManager().ClearAllTimersForObject(this);
		SetActorTickEnabled(false);
		return;
	}  // Stamina will not reduce if it is being carried
	// if (bIsBeingCarried || bIsDead) {
	if (Stamina <= 0) {
		if (!IsSleeping()) {
			Sleep();
		}
		return;
	}
	const float StaminaFinalDeducationAmount = (IsCharacterMoving() ? (StaminaDeductionRate * StaminaDeductionMultiplierMoving) : StaminaDeductionRate);
	Stamina -= StaminaFinalDeducationAmount;
	// UE_LOG(LogTemp, Warning, TEXT("Stamina: %f"), GetStamina());

}

bool AKoalaBaseCharacter::AreThereAnyTreesAround(FHitResult& OutHitResult, FCollisionQueryParams InParams)
{
	bool bResult = GetObjectAround(OutHitResult, TreeDistanceCheck, InParams);
	if (bResult) {
		// To check if the hit actor was a tree specifically
		UE_LOG(LogTemp, Warning, TEXT("Check: %s"), *OutHitResult.GetActor()->GetName());
		bResult = OutHitResult.GetActor()->IsA(ABaseTree::StaticClass());
	}
	return bResult;
}

bool AKoalaBaseCharacter::GetObjectAround(FHitResult& OutHitResult, float RangeCheck, FCollisionQueryParams InParams)
{
	FVector Start = GetActorLocation() + FVector::UpVector * RangeCheck;
	FVector End = Start;
	if (AKoalaPlayerCharacter* PlayerCharacterThis = Cast<AKoalaPlayerCharacter>(this)) {
		InParams.AddIgnoredActor(PlayerCharacterThis->Gun);
	}
	// FVector End = Start + (RangeCheck * GetActorForwardVector());// + (-RangeCheck * GetActorUpVector());
	InParams.AddIgnoredActor(this);
	bool bResult = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity,ECC_GameTraceChannel1, FCollisionShape::MakeSphere(RangeCheck),InParams);
	if (!bResult) {
		Start = GetActorLocation() + FVector::UpVector * RangeCheck;
		End = Start + (RangeCheck * GetActorForwardVector());// + (-RangeCheck * GetActorUpVector());
		bResult = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity,ECC_GameTraceChannel1, FCollisionShape::MakeSphere(RangeCheck),InParams);
		if (!bResult) {
			Start = GetActorLocation();
			End = Start + (RangeCheck * GetActorForwardVector());// + (-RangeCheck * GetActorUpVector());
			bResult = GetWorld()->SweepSingleByChannel(OutHitResult, Start, End, FQuat::Identity,ECC_GameTraceChannel1, FCollisionShape::MakeSphere(RangeCheck),InParams);
			if (bResult) {
				if (OutHitResult.GetActor()->IsA(ABaseTree::StaticClass())) {
					return false;

				}
			}
		}
	}
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
	if (OnFireSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), OnFireSound, GetActorLocation(), 1.5, SoundPitches);
	}
	Health -= Damage;
	UE_LOG(LogTemp, Warning, TEXT("New Health, %s: %f"), *GetName(), Health);
	if (Health <= 0) {
		Die();
	}

	return Damage;
}

void AKoalaBaseCharacter::Die()
{
	if (DeathSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), DeathSound, GetActorLocation(), 1.5, SoundPitches);
	}
	NiagaraSleeping->SetHiddenInGame(true);
	// This line below alerts other listeners of the event that character has died
	GetWorld()->GetTimerManager().ClearTimer(SleepTimerHandle);
	// GetWorld()->GetTimerManager().ClearTimer(SleepSoundTimerHandle);
	GetWorldTimerManager().ClearAllTimersForObject(this);
	if (bIsDead) return;
	// SetRootComponent(GetMesh());
	CapsuleComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	// CapsuleComp->SetCollisionResponseToChannel(Channel, ECollisionResponse::ECR_Ignore);
	// CapsuleComp->SetCollisionResponseToAllChannels(ECR_Ignore);
	// CapsuleComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	GetMesh()->SetCollisionEnabled(ECollisionEnabled::PhysicsOnly);

	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->AddImpulse(DeathMeshImpulse);
	// GetMesh()->SetCollisionResponseToChannel(Channel, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Block);

	StopCharacterMovement();
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
	if (Health < 0) Health = 0;
}

void AKoalaBaseCharacter::AddStamina(float Amount)
{
	// TODO: Any effects and sounds will play here
	Stamina += Amount;
	if (Stamina > 100) Stamina = 100;
	else if (Stamina < 0) Stamina = 0;
}

void AKoalaBaseCharacter::Sleep()
{
	if (!GameMode || GameMode->bGameIsOver || IsDead()) {
		GetWorldTimerManager().ClearAllTimersForObject(this);
		return;
	}
	if (GetWorldTimerManager().TimerExists(SleepTimerHandle)) {
		GetWorld()->GetTimerManager().ClearTimer(SleepTimerHandle);
		// GetWorld()->GetTimerManager().ClearTimer(SleepSoundTimerHandle);
		return;
	}
	if (AKoalaPlayerCharacter* PlayerCharacter = Cast<AKoalaPlayerCharacter>(this)) {
		PlayerCharacter->PlayerController->SetControlRotation(FRotator(-50, 0, 0));
		DisableInput(PlayerCharacter->PlayerController);
		if (PlayerCharacter->Gun->ShootWaterAudioComp) {
			if (PlayerCharacter->Gun->ShootWaterAudioComp->IsActive()) {
				PlayerCharacter->Gun->ShootWaterAudioComp->SetActive(false);
			}
		}
		PlayerCharacter->Gun->ReleaseTrigger();
		PlayerCharacter->DropCurrentCarriedItem();
		PlayerCharacter->DetachFromCurrentTree();
	}
	if (NiagaraSleeping) {
		NiagaraSleeping->SetHiddenInGame(false);
	}
	bIsSleeping = true;
	FTimerDelegate TimerDelegate;
	TimerDelegate.BindLambda([&]() {
		if (GameMode && !GameMode->bGameIsOver) {
			if (!bIsDead) {
				// Wake up after sleeping for X seconds
				if (APlayerController* PlayerController = Cast<APlayerController>(GetController())) {
					EnableInput((PlayerController));
				}
				bIsSleeping = false;
				if (NiagaraSleeping) {
					NiagaraSleeping->SetHiddenInGame(true);
				}
				Stamina = StaminaAfterSleep;
				// GetWorldTimerManager().ClearTimer(SleepSoundTimerHandle);
				// UE_LOG(LogTemp, Warning, TEXT("Awake: %s"), *GetFullName());
			}
		}
		
		/*else {
			GetWorldTimerManager().ClearTimer(SleepTimerHandle);
		}*/
	});
	/*if (SleepingSound) {
		FTimerDelegate TimerDelegateSleepingSound;
		TimerDelegateSleepingSound.BindLambda([&]() {
			if (GameMode && !GameMode->bGameIsOver) {
				if (!bIsDead && !IsOnFire()) {
					if (SleepingSound) {
						if (IsA(AKoalaPlayerCharacter::StaticClass())) {
							UGameplayStatics::PlaySound2D(GetWorld(), SleepingSound, 1.5, SoundPitches);
						}
						else {
							UGameplayStatics::PlaySoundAtLocation(GetWorld(), SleepingSound, GetActorLocation(), 1.5, SoundPitches);
						}
					}
				}
				
			}
		});
		GetWorldTimerManager().SetTimer(SleepSoundTimerHandle, TimerDelegateSleepingSound, MakeSleepingSoundEverySeconds, true, 0);
	}*/
	
	// UE_LOG(LogTemp, Warning, TEXT("Sleeping: %s"), *GetFullName());
	GetWorldTimerManager().SetTimer(SleepTimerHandle, TimerDelegate, SleepDelay, false);
	
}

void AKoalaBaseCharacter::StopCharacterMovement()
{
	GetCharacterMovement()->StopMovementImmediately();
}

void AKoalaBaseCharacter::ChangeCharacterSpeed(bool bShouldRun)
{
	GetCharacterMovement()->MaxWalkSpeed = bShouldRun ? RunningSpeed : NormalSpeed;
}

void AKoalaBaseCharacter::ResetCharacterMesh()
{
	GetMesh()->SetWorldRotation(InitialMeshRotation);
}

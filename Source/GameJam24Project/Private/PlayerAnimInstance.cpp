// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerAnimInstance.h"
#include "KoalaPlayerCharacter.h"
#include "GameFramework/PawnMovementComponent.h"
#include "KismetAnimationLibrary.h"

UPlayerAnimInstance::UPlayerAnimInstance()
{
}

void UPlayerAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);

	AKoalaPlayerCharacter* OwnerPawn = Cast<AKoalaPlayerCharacter>(TryGetPawnOwner());

	if (!OwnerPawn) {
		UE_LOG(LogTemp, Warning, TEXT("Owner Pawn Invalid For animation instance: %s"), *GetFullName());
		return;
	}
	bIsDead = OwnerPawn->IsDead();
	if (bIsDead) {
		return;
	}
	UPawnMovementComponent* MovementComp = OwnerPawn->GetMovementComponent();
	// const FVector CharacterVel = MovementComp->Velocity;
	CharacterSpeed = MovementComp->Velocity.Length();
	OnGroundCharacterDirection = UKismetAnimationLibrary::CalculateDirection(MovementComp->Velocity, OwnerPawn->GetActorRotation());
	bIsFalling = MovementComp->IsFalling();
	bIsClimbingTree = OwnerPawn->bIsOnTree;
	if (!bIsClimbingTree) {
		ClimbingCharacterDirection = 0;
	}
	else {
		ClimbingCharacterDirection = OwnerPawn->ClimbingDir;
	}
	const float CharPitch = OwnerPawn->GetBaseAimRotation().Pitch;
	CharacterPitch = CharPitch >= 180 ? CharPitch - 360 : CharPitch;
}

void UPlayerAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

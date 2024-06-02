// Fill out your copyright notice in the Description page of Project Settings.


#include "BabyAnimInstance.h"

#include "AIController.h"
#include "GameFramework/PawnMovementComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "KoalaBabyCharacter.h"
#include "KismetAnimationLibrary.h"

UBabyAnimInstance::UBabyAnimInstance()
{

}

void UBabyAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();
}

void UBabyAnimInstance::NativeUpdateAnimation(float DeltaTime)
{
	Super::NativeUpdateAnimation(DeltaTime);
	AKoalaBabyCharacter* OwnerPawn = Cast<AKoalaBabyCharacter>(TryGetPawnOwner());

	if (!OwnerPawn) {
		UE_LOG(LogTemp, Warning, TEXT("Owner Pawn Invalid For animation instance: %s"), *GetFullName());
		return;
	}
	bIsDead = OwnerPawn->IsDead();
	if (bIsDead) {
		return;
	}
	UPawnMovementComponent* MovementComp = OwnerPawn->GetMovementComponent();
	CharacterSpeed = MovementComp->Velocity.Length();
	CharacterDirection = UKismetAnimationLibrary::CalculateDirection(MovementComp->Velocity, OwnerPawn->GetActorRotation());
	bIsFalling = MovementComp->IsFalling();
	bIsSleeping = OwnerPawn->IsSleeping();
	bIsBeingCarried = OwnerPawn->bIsBeingCarried;
	AAIController* MyAIController = Cast<AAIController>(OwnerPawn->GetController());
	if (!MyAIController) {
		// UE_LOG(LogTemp, Warning, TEXT("AI Controller invalid for: %s"), *GetFullName());
		return;
	}
	bIsOnFire = MyAIController->GetBlackboardComponent()->GetValueAsBool(FName("Is on fire"));;

}

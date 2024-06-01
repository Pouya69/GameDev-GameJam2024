// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_StopMovement.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "KoalaBabyCharacter.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_StopMovement::UBTTask_StopMovement()
{
	NodeName = TEXT("Stop Movement");
}

EBTNodeResult::Type UBTTask_StopMovement::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAIController* OwnerAIController = OwnerComp.GetAIOwner();
	AKoalaBabyCharacter* OwnerPawn = Cast<AKoalaBabyCharacter>(OwnerAIController->GetPawn());
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	// TODO: Set the speed and stuff for the koala when on fire

	BlackBoardComp->ClearValue(FName("IsMovingToLocation"));
	BlackBoardComp->ClearValue(FName("MovingToLocation"));
	OwnerAIController->StopMovement();
	// OwnerPawn->StopCharacterMovement();
	return EBTNodeResult::Succeeded;
}

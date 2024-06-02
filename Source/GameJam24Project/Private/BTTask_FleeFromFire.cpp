// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_FleeFromFire.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "KoalaBabyCharacter.h"
#include "Navigation/PathFollowingComponent.h"

UBTTask_FleeFromFire::UBTTask_FleeFromFire()
{
	NodeName = TEXT("Flee from fire");
}

EBTNodeResult::Type UBTTask_FleeFromFire::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* OwnerAIController = OwnerComp.GetAIOwner();
	AKoalaBabyCharacter* OwnerPawn = Cast<AKoalaBabyCharacter>(OwnerAIController->GetPawn());

	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	OwnerPawn->ChangeCharacterSpeed(true);

	bool bAlreadyHasMovingLocation = BlackBoardComp->GetValueAsBool(FName("IsMovingToLocation"));
	if (bAlreadyHasMovingLocation) {
		return EBTNodeResult::Failed;
	}
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	FNavLocation MoveLocationNav;
	bool bFoundLocationToGoto = NavSystem->GetRandomReachablePointInRadius(OwnerPawn->GetActorLocation(), ReachablePointRadius, MoveLocationNav);
	if (bFoundLocationToGoto) {
		BlackBoardComp->SetValueAsVector(GetSelectedBlackboardKey(), MoveLocationNav.Location);
		BlackBoardComp->SetValueAsBool(FName("IsMovingToLocation"), true);
		// DrawDebugSphere(GetWorld(), MoveLocationNav.Location, 20.f, 50, FColor::Red, true);
		// UE_LOG(LogTemp, Warning, TEXT("Moving To: %s"), *MoveLocationNav.Location.ToCompactString());
		// OwnerAIController->MoveToLocation(MoveLocationNav.Location, DestinationAcceptableRadius);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can't find a location"));
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::Succeeded;
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BTTask_BabyDefaultBehaviour.h"
#include "NavigationSystem.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "KoalaBabyCharacter.h"


UBTTask_BabyDefaultBehaviour::UBTTask_BabyDefaultBehaviour()
{
	NodeName = TEXT("Baby default behaviour");
}

EBTNodeResult::Type UBTTask_BabyDefaultBehaviour::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);
	AAIController* OwnerAIController = OwnerComp.GetAIOwner();
	AKoalaBabyCharacter* OwnerPawn = Cast<AKoalaBabyCharacter>(OwnerAIController->GetPawn());
	UBlackboardComponent* BlackBoardComp = OwnerComp.GetBlackboardComponent();
	// TODO: Set the speed and stuff for the koala when on fire

	bool bAlreadyHasMovingLocation = BlackBoardComp->GetValueAsBool(FName("IsMovingToLocation"));
	if (bAlreadyHasMovingLocation) {
		return EBTNodeResult::Succeeded;
	}
	UNavigationSystemV1* NavSystem = FNavigationSystem::GetCurrent<UNavigationSystemV1>(this);
	FNavLocation MoveLocationNav;
	bool bFoundLocationToGoto = NavSystem->GetRandomReachablePointInRadius(OwnerPawn->GetActorLocation(), ReachablePointRadius, MoveLocationNav);
	if (bFoundLocationToGoto) {
		BlackBoardComp->SetValueAsVector(GetSelectedBlackboardKey(), MoveLocationNav.Location);
		BlackBoardComp->SetValueAsBool(FName("IsMovingToLocation"), true);
		// DrawDebugSphere(GetWorld(), MoveLocationNav.Location, 20.f, 50, FColor::Red, true);
		// UE_LOG(LogTemp, Warning, TEXT("Moving To: %s"), *MoveLocationNav.Location.ToCompactString());
		OwnerAIController->MoveToLocation(MoveLocationNav.Location, DestinationAcceptableRadius);
	}
	else {
		UE_LOG(LogTemp, Warning, TEXT("Can't find a location"));
		return EBTNodeResult::Failed;
	}

	return EBTNodeResult::Succeeded;
}

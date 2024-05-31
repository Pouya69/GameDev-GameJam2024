// Fill out your copyright notice in the Description page of Project Settings.


#include "BTService_CheckForDanger.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "KoalaBabyCharacter.h"


UBTService_CheckForDanger::UBTService_CheckForDanger()
{
	NodeName = TEXT("Check for danger");
}

void UBTService_CheckForDanger::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AKoalaBabyCharacter* OwnerPawn = Cast<AKoalaBabyCharacter>(OwnerComp.GetAIOwner()->GetPawn());
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(GetSelectedBlackboardKey(), OwnerPawn->IsOnFire());
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsSleeping"), OwnerPawn->IsSleeping());
	OwnerComp.GetBlackboardComponent()->SetValueAsBool(FName("IsBeingCarried"), OwnerPawn->bIsBeingCarried);

}

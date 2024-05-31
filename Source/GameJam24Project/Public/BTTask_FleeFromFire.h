// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_FleeFromFire.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API UBTTask_FleeFromFire : public UBTTask_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTTask_FleeFromFire();

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

public:
	UPROPERTY(EditAnywhere)
	float ReachablePointRadius;
	UPROPERTY(EditAnywhere)
	float DestinationAcceptableRadius;
};

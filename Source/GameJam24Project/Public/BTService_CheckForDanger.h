// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/Services/BTService_BlackboardBase.h"
#include "BTService_CheckForDanger.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API UBTService_CheckForDanger : public UBTService_BlackboardBase
{
	GENERATED_BODY()

public:
	UBTService_CheckForDanger();

protected:
	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};

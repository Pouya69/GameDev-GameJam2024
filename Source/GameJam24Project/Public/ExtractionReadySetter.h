// This is a free project

#pragma once

#include "CoreMinimal.h"
#include "BaseInteractableObject.h"
#include "ExtractionReadySetter.generated.h"

UCLASS()
class GAMEJAM24PROJECT_API AExtractionReadySetter : public ABaseInteractableObject
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExtractionReadySetter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void ExtractNow();
	UPROPERTY(EditAnywhere)
		int ExtractionNewTime = 20;
};

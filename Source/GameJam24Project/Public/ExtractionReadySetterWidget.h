// This is a free project

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "ExtractionReadySetterWidget.generated.h"

/**
 * 
 */
UCLASS()
class GAMEJAM24PROJECT_API UExtractionReadySetterWidget : public UUserWidget
{
	GENERATED_BODY()
	
public:
	virtual void NativeConstruct() override;

public:
	class AExtractionReadySetter* ExtractionReadySetter;
	void BuildMyWidget(class AKoalaPlayerCharacter* InPlayerCharacter, class AExtractionReadySetter* InExtractionReadySetter);
	class AKoalaPlayerCharacter* PlayerCharacter;

	UFUNCTION(BlueprintCallable)
		void SetExtract(bool bShouldExtract);
};

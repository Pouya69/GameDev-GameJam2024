// This is a free project


#include "KoalaPlayerCharacter.h"
#include "ExtractionReadySetterWidget.h"
#include "ExtractionReadySetter.h"

void UExtractionReadySetterWidget::NativeConstruct()
{
	Super::NativeConstruct();
}

void UExtractionReadySetterWidget::BuildMyWidget(AKoalaPlayerCharacter* InPlayerCharacter, AExtractionReadySetter* InExtractionReadySetter)
{
	ExtractionReadySetter = InExtractionReadySetter;
	PlayerCharacter = InPlayerCharacter;
}

void UExtractionReadySetterWidget::SetExtract(bool bShouldExtract)
{
	if (ExtractionReadySetter && PlayerCharacter) {
		if (bShouldExtract) {
			ExtractionReadySetter->ExtractNow();
		}
		PlayerCharacter->ExtractionReadySetterWidget = nullptr;
		PlayerCharacter->PlayerController->bShowMouseCursor = false;
		PlayerCharacter->EnableInput(PlayerCharacter->PlayerController);
		RemoveFromParent();
	}
}

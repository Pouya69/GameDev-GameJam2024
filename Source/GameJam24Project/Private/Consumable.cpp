// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumable.h"
#include "KoalaBaseCharacter.h"

AConsumable::AConsumable()
{
	BaseMeshComp->SetSimulatePhysics(true);
	BaseMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AConsumable::Consume(AKoalaBaseCharacter* Consumer)
{
	if (Consumer == nullptr) return;
	// TODO: Also the effects and sounds will play here
	Super::Interact();
	if (ItemType == EConsumableType::STAMINA_ONLY) {
		Consumer->AddStamina(AdditionToConsumer);
	}
	else if (ItemType == EConsumableType::HEALTH_ONLY) {
		Consumer->AddHealth(AdditionToConsumer);
	}
	else if (ItemType == EConsumableType::STAMINA_AND_HEALTH) {
		Consumer->AddStamina(AdditionToConsumer);
		Consumer->AddHealth(AdditionToConsumer);
	}
	else if (ItemType == EConsumableType::WATER) {
		// TODO: Water getting added
		// Something like the line below
		// Consumer->Weapon->AddWater(AdditionToConsumer);
	}
	
	Destroy();  // The item will be destroyed after getting consumed

}

void AConsumable::DestroyItemHandleFire()
{
	// TODO: Any destruction sounds and effects will play here
	Destroy();
}

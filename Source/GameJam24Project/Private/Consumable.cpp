// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumable.h"
#include "KoalaBaseCharacter.h"
#include "KoalaPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"

AConsumable::AConsumable()
{
	BaseMeshComp->SetSimulatePhysics(true);
	BaseMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
}

void AConsumable::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AConsumable::DestroyItemHandleFire);
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
		if (AKoalaPlayerCharacter* PlayerCharacter = Cast<AKoalaPlayerCharacter>(Consumer)) {
			PlayerCharacter->ReloadGun();
		}
		
	}
	else if (ItemType == EConsumableType::STAMINA_AND_HEALTH_AND_WATER) {
		Consumer->AddStamina(AdditionToConsumer);
		Consumer->AddHealth(AdditionToConsumer);
		if (AKoalaPlayerCharacter* PlayerCharacter = Cast<AKoalaPlayerCharacter>(Consumer)) {
			PlayerCharacter->ReloadGun();
		}
	}
	else if (ItemType == EConsumableType::POOP) {
		Consumer->AddStamina(AdditionToConsumer);
		UGameplayStatics::ApplyDamage(Consumer, AdditionToConsumer, Consumer->GetController(), Consumer, UDamageType::StaticClass());
	}
	
	Destroy();  // The item will be destroyed after getting consumed

}

void AConsumable::DestroyItemHandleFire(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0) return;
	// TODO: Any destruction sounds and effects will play here
	Health -= HealthReductionFire;
	if (Health <= 0) {
		if (DeathMaterial) {
			BaseMeshComp->SetMaterial(0, DeathMaterial);
		}
		
	}
	// Destroy();
}
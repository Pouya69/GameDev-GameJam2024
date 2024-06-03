// Fill out your copyright notice in the Description page of Project Settings.


#include "Consumable.h"
#include "KoalaBaseCharacter.h"
#include "KoalaPlayerCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "KoalaGameModeBase.h"
#include "NiagaraComponent.h"

AConsumable::AConsumable()
{
	BaseMeshComp->SetSimulatePhysics(true);
	BaseMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	NiagaraEffect = CreateDefaultSubobject<UNiagaraComponent>(FName("Effect"));
	NiagaraEffect->SetupAttachment(BaseMeshComp);
}

void AConsumable::BeginPlay()
{
	Super::BeginPlay();

	OnTakeAnyDamage.AddDynamic(this, &AConsumable::DestroyItemHandleFire);
}

void AConsumable::Consume(AKoalaBaseCharacter* Consumer)
{
	if (Consumer == nullptr || Health <= 0) return;
	// TODO: Also the effects and sounds will play here
	Super::Interact();
	if (ItemType == EConsumableType::STAMINA_ONLY) {
		if (ConsumeSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ConsumeSound, Consumer->GetActorLocation(), 1.5);
		}
		Consumer->AddStamina(AdditionToConsumer);
	}
	else if (ItemType == EConsumableType::HEALTH_ONLY) {
		if (ConsumeSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ConsumeSound, Consumer->GetActorLocation(), 1.5);
		}
		Consumer->AddHealth(AdditionToConsumer);
	}
	else if (ItemType == EConsumableType::STAMINA_AND_HEALTH) {
		if (ConsumeSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ConsumeSound, Consumer->GetActorLocation(), 1.5);
		}
		Consumer->AddStamina(AdditionToConsumer);
		Consumer->AddHealth(AdditionToConsumer);
	}
	else if (ItemType == EConsumableType::WATER) {
		if (AKoalaPlayerCharacter* PlayerCharacter = Cast<AKoalaPlayerCharacter>(Consumer)) {
			PlayerCharacter->ReloadGun();
		}
		
	}
	else if (ItemType == EConsumableType::STAMINA_AND_HEALTH_AND_WATER) {
		if (ConsumeSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ConsumeSound, Consumer->GetActorLocation(), 1.5);
		}
		Consumer->AddStamina(AdditionToConsumer);
		Consumer->AddHealth(AdditionToConsumer);
		if (AKoalaPlayerCharacter* PlayerCharacter = Cast<AKoalaPlayerCharacter>(Consumer)) {
			PlayerCharacter->ReloadGun();
		}
	}
	else if (ItemType == EConsumableType::POOP) {
		if (ConsumeSound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), ConsumeSound, Consumer->GetActorLocation(), 1.5);
		}
		Consumer->AddStamina(AdditionToConsumer);
		UGameplayStatics::ApplyDamage(Consumer, AdditionToConsumer, Consumer->GetController(), Consumer, UDamageType::StaticClass());
	}
	AKoalaGameModeBase* GameMode = Cast<AKoalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
	GameMode->ConsumablesInLevel--;
	Destroy();  // The item will be destroyed after getting consumed

}

void AConsumable::DestroyItemHandleFire(AActor* DamagedActor, float Damage, const class UDamageType* DamageType, class AController* InstigatedBy, AActor* DamageCauser)
{
	if (Health <= 0) return;
	// TODO: Any destruction sounds and effects will play here
	Health -= Damage;
	if (Health <= 0) {
		if (DeathMaterial) {
			BaseMeshComp->SetMaterial(0, DeathMaterial);
		}
		if (DestroySound) {
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), DestroySound, GetActorLocation());
		}
		AKoalaGameModeBase* GameMode = Cast<AKoalaGameModeBase>(UGameplayStatics::GetGameMode(GetWorld()));
		GameMode->ConsumablesInLevel--;
		// Destroy();
	}
	
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseInteractableObject.h"
#include "Consumable.h"

// Sets default values
ABaseInteractableObject::ABaseInteractableObject()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	BaseMeshComp = CreateDefaultSubobject<UStaticMeshComponent>(FName("Base Mesh"));
	BaseMeshComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_GameTraceChannel1, ECollisionResponse::ECR_Block);
	BaseMeshComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);

}

// Called when the game starts or when spawned
void ABaseInteractableObject::BeginPlay()
{
	Super::BeginPlay();

	// TODO instanciate mesh and capsule collision here
	
}

// Called every frame
void ABaseInteractableObject::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


// See the header file
void ABaseInteractableObject::Interact()
{
	// TODO: Any effects and sounds will play here
	UE_LOG(LogTemp, Warning, TEXT("Interacting..."));
}

bool ABaseInteractableObject::IsConsumable() const
{
	// We can do further checks for here
	return this->IsA(AConsumable::StaticClass());
}

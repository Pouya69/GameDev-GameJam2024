// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root")); 
	SetRootComponent(Root);


	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGun::PullTrigger()
{
	
	UE_LOG(LogTemp, Warning, TEXT("Ammo: %f"), Ammunition);
	if(!Ammunition)
	{
		return;
	}
	
	Ammunition -= AmmoConsumeRate;
	FHitResult HitResult;
	AController* Controller = GetOwnerController();

	// Uncomment this line when weapon ready and muzzle socket created on it
	// FVector Start = GetMesh()->GetSocketLocation(FName("muzzle"));
	FVector Start = GetActorLocation();
	FVector End = Start + (Controller->GetControlRotation().Vector() * FireRange);
	// SpawnEmitterJetEffect(Start, End);
	DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f);
	if(GunTrace(HitResult))
	{
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, ShotDirection.Rotation());
		//UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);		
		AActor* ResultActor = HitResult.GetActor();

		if(ResultActor != nullptr)
		{
			UE_LOG(LogTemp, Warning, TEXT("Hitting %s"),*ResultActor->GetName());
			UGameplayStatics::ApplyDamage(ResultActor, Damage, nullptr, this, UDamageType::StaticClass());
		}
	}
}


bool AGun::GunTrace(FHitResult& Hit)
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	AController* Controller = GetOwnerController();

	if (Controller == nullptr) return false;
	
	Controller->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());

	FVector End = PlayerLocation + PlayerRotation.Vector() * FireRange;

	return GetWorld()->LineTraceSingleByChannel(Hit, PlayerLocation, End , ECollisionChannel::ECC_GameTraceChannel1, params);
	
}


 void AGun::SpawnEmitterJetEffect(const FVector& Start, const FVector& End)
 {
	if(Niagara)
	{
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Niagara, Start);
		if(NiagaraComponent)
		{
			NiagaraComponent->SetVectorParameter(FName("BeamEnd"), End);
		}
	}
 }



AController* AGun::GetOwnerController() const
{
	APawn* OwnerPawn = Cast<APawn>(GetOwner());

	if ( OwnerPawn == nullptr)
	{
		return nullptr;
	}
	return OwnerPawn -> GetController();
}

void AGun::ReloadAmmunition()
{
	Ammunition = AmmunitionCapacity;
}
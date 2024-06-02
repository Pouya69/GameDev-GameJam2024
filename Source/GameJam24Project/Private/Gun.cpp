// Fill out your copyright notice in the Description page of Project Settings.


#include "Gun.h"
#include "Components/StaticMeshComponent.h"
#include "NiagaraComponent.h"
#include "NiagaraFunctionLibrary.h"
#include "Engine/DamageEvents.h"
#include "Engine/DamageEvents.h"
#include "Kismet/GameplayStatics.h"
#include "Fire.h"
#include "KoalaPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"

// Sets default values
AGun::AGun()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root")); 
	SetRootComponent(Root);

	MeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(RootComponent);

	GunBeamMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Beam"));
	GunBeamMesh->SetupAttachment(MeshComponent);

}

// Called when the game starts or when spawned
void AGun::BeginPlay()
{
	Super::BeginPlay();
	Niagara = FindComponentByTag<UNiagaraComponent>(FName("StartEffect"));
	NiagaraEnd = FindComponentByTag<UNiagaraComponent>(FName("EndEffect"));
	NotHittingScale3D = GunBeamMesh->GetComponentScale();
	InitialRotationBeam = Niagara->GetRelativeRotation();
	
}

// Called every frame
void AGun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void AGun::PullTrigger()
{
	
	// UE_LOG(LogTemp, Warning, TEXT("Ammo: %f"), Ammunition);
	if(!Ammunition)
	{
		ReleaseTrigger();
		return;
	}
	// GunBeamMesh->SetHiddenInGame(false);
	Niagara->SetHiddenInGame(false);
	Ammunition -= AmmoConsumeRate;
	FHitResult HitResult;

	// Uncomment this line when weapon ready and muzzle socket created on it
	FVector Start = MeshComponent->GetSocketLocation(FName("muzzle"));
	FVector End = Start + (PlayerCharacter->PlayerController->GetControlRotation().Vector() * FireRange);
	// DrawDebugLine(GetWorld(), Start, End, FColor::Blue, false, 1.f);
	// FVector BeamScale = NotHittingScale3D;
	FRotator BeamRotation = InitialRotationBeam;
	if(GunTrace(HitResult))
	{
		FVector NewEnd = HitResult.ImpactPoint;
		
		NiagaraEnd->SetWorldLocation(NewEnd);
		
		// BeamRotation = UKismetMathLibrary::GetDirectionUnitVector(Start, NewEnd).Rotation();
		NiagaraEnd->SetHiddenInGame(false);
		// BeamScale *= (FVector::Dist(Start, End) / FVector::Dist(Start, NewEnd));
		//UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), ImpactEffect, HitResult.Location, ShotDirection.Rotation());
		//UGameplayStatics::SpawnSoundAtLocation(GetWorld(), ImpactSound, HitResult.Location);		
		AActor* ResultActor = HitResult.GetActor();
		// DrawDebugSphere(GetWorld(), HitResult.ImpactPoint, 5, 10, FColor::Blue, true);
		if(ResultActor != nullptr)
		{
			// UE_LOG(LogTemp, Warning, TEXT("Hitting %s"), *HitResult.GetComponent()->GetName());
			
			// UGameplayStatics::ApplyDamage(ResultActor, Damage, nullptr, this, UDamageType::StaticClass());
			if (AFire* FireObjectHit = Cast<AFire>(ResultActor)) {
				if (NiagaraHitObject) {
					UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraHitFire, HitResult.ImpactPoint);
				}
				FireObjectHit->DestroyFire(HitResult.GetComponent());
			}
			else {
				// UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), NiagaraHitObject, HitResult.ImpactPoint);
			}
			
		}
		End = NewEnd;
	}
	else {
		NiagaraEnd->SetHiddenInGame(true);
	}
	Niagara->SetRelativeRotation(BeamRotation);
	// GunBeamMesh->SetWorldRotation(BeamRotation);
	// GunBeamMesh->SetRelativeScale3D(BeamScale);
	SpawnEmitterJetEffect(Start, End);
}

void AGun::ReleaseTrigger()
{
	GunBeamMesh->SetHiddenInGame(true);
	Niagara->SetHiddenInGame(true);
	NiagaraEnd->SetHiddenInGame(true);
}


bool AGun::GunTrace(FHitResult& Hit)
{
	FVector PlayerLocation;
	FRotator PlayerRotation;
	if (PlayerCharacter == nullptr) return false;
	PlayerCharacter->PlayerController->GetPlayerViewPoint(PlayerLocation, PlayerRotation);
	
	FCollisionQueryParams params;
	params.AddIgnoredActor(this);
	params.AddIgnoredActor(GetOwner());

	FVector End = PlayerLocation + PlayerRotation.Vector() * FireRange;

	return GetWorld()->LineTraceSingleByChannel(Hit, PlayerLocation, End , ECollisionChannel::ECC_WorldStatic, params);
	
}


 void AGun::SpawnEmitterJetEffect(const FVector& Start, const FVector& End)
 {
	/*if (Niagara)
	{
		UNiagaraComponent* NiagaraComponent = UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), Niagara, GunBeamMesh->GetComponentLocation(), (End - Start).Rotation());
		if(NiagaraComponent)
		{
			NiagaraComponent->SetVectorParameter(FName("BeamEnd"), End);
		}
		Niagara->SetHiddenInGame(false);
	}
	if (GunBeamMesh) {
		GunBeamMesh->SetHiddenInGame(false);
	}*/
 }

void AGun::ReloadAmmunition()
{
	Ammunition = AmmunitionCapacity;
}
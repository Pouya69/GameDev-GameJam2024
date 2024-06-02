// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "BaseInteractableObject.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "BaseTree.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Consumable.h"
#include "Components/CapsuleComponent.h"
#include "KoalaBabyCharacter.h"
#include "MissionObjectivesWidget.h"
#include "BasicPlayerWidget.h"
#include "Gun.h"
#include "Kismet/GameplayStatics.h"
#include "KoalaGameModeBase.h"

AKoalaPlayerCharacter::AKoalaPlayerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(FName("Spring Arm Comp"));
	CameraComp = CreateDefaultSubobject<UCameraComponent>(FName("Camera Comp"));
	SpringArmComp->AttachToComponent(GetRootComponent(), FAttachmentTransformRules::KeepRelativeTransform);
	CameraComp->AttachToComponent(SpringArmComp, FAttachmentTransformRules::KeepRelativeTransform);
}

void AKoalaPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	PlayerController = GetLocalViewingPlayerController();
	RemoveAllPlayerWidgets();
	MakeStartingWidgets();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMapping, 0);
	}
	if(GunClass)
	{ 
		UE_LOG(LogTemp, Warning, TEXT("Spawning %s"), *GunClass->GetName());
		Gun = GetWorld()->SpawnActor<AGun>(GunClass);
		Gun->AttachToComponent( GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, TEXT("weapon_socket"));
		Gun->SetOwner(this);
		Gun->PlayerCharacter = this;
	}

	UE_LOG(LogTemp, Warning, TEXT("MESH: %S"), *GetMesh()->GetFullName());
}

void AKoalaPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (IsCarryingItem()) {
		// const FTransform Trans = GetMesh()->GetSocketTransform(ItemCarryBoneNameOnMesh);
		/*if (AKoalaBabyCharacter* KoalaCharacter = Cast<AKoalaBabyCharacter>(ItemCarriedOnBack)) {
			// KoalaCharacter->GetMesh()->Location
		}*/
		ItemCarriedOnBack->SetActorTransform(GetMesh()->GetSocketTransform(ItemCarryBoneNameOnMesh));
		// ItemCarriedOnBack
	}
	/*if (bIsOnTree) {
		if (IsOnFire()) {
			DetachFromCurrentTree();
		}
	}*/

}

void AKoalaPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	if (UEnhancedInputComponent* EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedPlayerInputComponent->BindAction(MoveAction, ETriggerEvent::None, this, &AKoalaPlayerCharacter::NotMoving);
		EnhancedPlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKoalaPlayerCharacter::Move);
		EnhancedPlayerInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKoalaPlayerCharacter::Look);
		EnhancedPlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AKoalaPlayerCharacter::PlayerJump);
		EnhancedPlayerInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AKoalaPlayerCharacter::Interact);
		EnhancedPlayerInputComponent->BindAction(CarryItemAction, ETriggerEvent::Started, this, &AKoalaPlayerCharacter::PickupAndCarryItem);
		EnhancedPlayerInputComponent->BindAction(ShootAction, ETriggerEvent::Triggered, this, &AKoalaPlayerCharacter::Shoot);
		EnhancedPlayerInputComponent->BindAction(ShootAction, ETriggerEvent::None, this, &AKoalaPlayerCharacter::StopShoot);
	}

}

void AKoalaPlayerCharacter::Die()
{
	Super::Die();
}

void AKoalaPlayerCharacter::CarryItemOnBack(AActor* ItemToCarry)
{
	UPrimitiveComponent* PrimRootComp = Cast<UPrimitiveComponent>(ItemToCarry->GetRootComponent());
	PrimRootComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
	PrimRootComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	if (AKoalaBabyCharacter* BabyCharacter = Cast<AKoalaBabyCharacter>(ItemToCarry)) {
		BabyCharacter->ResetCharacterMesh();
		BabyCharacter->bIsBeingCarried = true;
		BabyCharacter->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		BabyCharacter->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
		BabyCharacter->GetCharacterMovement()->StopMovementImmediately();
		BabyCharacter->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Flying;
		// BabyCharacter->GetMesh()->SetSimulatePhysics(true);

	}
	else {
		PrimRootComp->SetSimulatePhysics(false);
	}
	PrimRootComp->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemCarryBoneNameOnMesh);
	
	ItemCarriedOnBack = ItemToCarry;
	
	UE_LOG(LogTemp, Warning, TEXT("Comp: %s"), *ItemCarriedOnBack->GetRootComponent()->GetName());
}

void AKoalaPlayerCharacter::DropCurrentCarriedItem()
{
	if (!IsCarryingItem()) return;

	ItemCarriedOnBack->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
	UPrimitiveComponent* PrimRootComp = Cast<UPrimitiveComponent>(ItemCarriedOnBack->GetRootComponent());
	PrimRootComp->SetWorldRotation(FRotator::ZeroRotator);
	if (AKoalaBabyCharacter* BabyCharacter = Cast<AKoalaBabyCharacter>(ItemCarriedOnBack)) {
		BabyCharacter->ResetCharacterMesh();
		PrimRootComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		BabyCharacter->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
		BabyCharacter->GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Block);
		BabyCharacter->GetCharacterMovement()->MovementMode = EMovementMode::MOVE_Walking;
		BabyCharacter->bIsBeingCarried = false;
		// BabyCharacter->GetMesh()->SetSimulatePhysics(false);

	}
	else {
		PrimRootComp->SetSimulatePhysics(true);
		PrimRootComp->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	}
	// We can put the launching the object as well
	// Works for normal objects for now
	// BABY KOALAS STILL NEEDS TESTING. uncomment to check
	/*
	FVector LaunchImpulse = CameraComp->GetForwardVector() * ItemLaunchForceAmount * PrimRootComp->GetMass();
	PrimRootComp->AddImpulse(LaunchImpulse);
	*/
	UE_LOG(LogTemp, Warning, TEXT("Dropping: %s"), *ItemCarriedOnBack->GetFullName());
	ItemCarriedOnBack = nullptr;
}

void AKoalaPlayerCharacter::ReloadGun()
{
	if (Gun)
	{
		Gun->ReloadAmmunition();
	}
}

void AKoalaPlayerCharacter::Move(const FInputActionValue& Value)
{
	if (Super::IsSleeping()) return;  // Character cannot move but can look around when asleep

	const FVector2D Input = Value.Get<FVector2D>();

	const FRotator MyControlRotation = PlayerController->GetControlRotation();
	const FRotator MovementRotation(0.0f, MyControlRotation.Yaw, 0.0f);

	if (Input.X != 0.0f)
	{
		if (bIsOnTree) {
			if (Input.Y != 0.0f) {
				ClimbingDir = 0;
				return;
			}
			ClimbingDir = Input.X > 0 ? 90 : -90;
			if (CurrentClimbingTree == nullptr) return;
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
			FVector CurrentTreeLocation = CurrentClimbingTree->GetActorLocation();
			CurrentTreeLocation.Z = GetActorLocation().Z;
			FVector ActorRightTargetLocation = GetActorLocation() + (Input.X * (TreeClimbingSpeed*3) * MovementDirection);
			FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(CurrentTreeLocation, ActorRightTargetLocation);
			FVector TargetLocation = CurrentTreeLocation + (Direction * TreeAttachmentRadius);
			SetActorLocation(TargetLocation, true);
			FRotator FinalRotation = (-1 * Direction).Rotation();
			FinalRotation.Pitch = GetControlRotation().Pitch;
			PlayerController->SetControlRotation(FinalRotation);
			// AddMovementInput(MovementDirection, Input.X);
			return;
		}
		else {
			ClimbingDir = 0;
		}
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, Input.X);
	}

	if (Input.Y != 0.0f)
	{
		if (bIsOnTree) {
			if (Input.X != 0.0f) { 
				ClimbingDir = 0;
				return;
			}
			ClimbingDir = Input.Y > 0 ? 180 : -180;
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::UpVector);
			FVector CurrentTreeLocation = CurrentClimbingTree->GetActorLocation();
			CurrentTreeLocation.Z = GetActorLocation().Z;
			FVector ActorUpTargetLocation = GetActorLocation() + (Input.Y * (TreeClimbingSpeed * 3) * MovementDirection);
			FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(CurrentTreeLocation, ActorUpTargetLocation);
			FVector TargetLocation = CurrentTreeLocation + (Direction * TreeAttachmentRadius);
			SetActorLocation(TargetLocation, true);
			FRotator FinalRotation = (-1 * Direction).Rotation();
			FinalRotation.Pitch = GetControlRotation().Pitch;
			PlayerController->SetControlRotation(FinalRotation);
			// AddMovementInput(MovementDirection, Input.X);
			return;
		}
		else {
			ClimbingDir = 0;
		}
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, Input.Y);
	}
	ClimbingDir = 0;


}

void AKoalaPlayerCharacter::NotMoving(const FInputActionValue& Value)
{
	ClimbingDir = 0;
}

void AKoalaPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	if (!bIsOnTree) {
		// Player will free look up and down
		AddControllerYawInput(Input.X);
	}
	
	AddControllerPitchInput(Input.Y);
}

void AKoalaPlayerCharacter::DetachFromCurrentTree() {
	if (!bIsOnTree || CurrentClimbingTree == nullptr) {
		bIsOnTree = false;
		CurrentClimbingTree = nullptr;
		return;
	}
	if (Gun) {
		Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("weapon_socket"));
	}
	if (JumpingSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), JumpingSound, GetActorLocation(), SoundPitches);
	}
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);  // Character will go back to normal movement mode
	// Super::Jump();
	bIsOnTree = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	CurrentClimbingTree = nullptr;
}

float AKoalaPlayerCharacter::GetTimeLeftProportional() const
{
	return (float) GameMode->TimeLeftExtraction / GameMode->ExtractionTimeSinceStart;
}

int AKoalaPlayerCharacter::GetBabyKoalasLeft() const
{
	return GameMode->BabyKoalasAlive;
}

float AKoalaPlayerCharacter::GetGunAmmoProportional() const
{
	if (!Gun) return 0.f;
	return Gun->GetAmmoLeft() / Gun->AmmunitionCapacity;
}

void AKoalaPlayerCharacter::UpdateKoalasAliveWidget()
{
	if (BasicPlayerWidget == nullptr) return;
	BasicPlayerWidget->MakeKoalaItemsWidget_Implementation();
	BasicPlayerWidget->MakeKoalaItemsWidget();
}

void AKoalaPlayerCharacter::RemoveAllPlayerWidgets()
{
	// Destroy all the widgets
	if (BasicPlayerWidget)	BasicPlayerWidget->RemoveFromParent();
	if (ObjectivesWidget)	ObjectivesWidget->RemoveFromParent();
}

void AKoalaPlayerCharacter::MakeStartingWidgets()
{
	
	if (ObjectivesWidgetClass &&! GameMode->MissionObjectives.IsEmpty()) {
		ObjectivesWidget = CreateWidget<UMissionObjectivesWidget>(PlayerController, ObjectivesWidgetClass);
		ObjectivesWidget->AddToViewport();
		ObjectivesWidget->MakeObjectivesWidget_Implementation();
		ObjectivesWidget->MakeObjectivesWidget();
	}
	if (BasicPlayerWidgetClass) {
		BasicPlayerWidget = CreateWidget<UBasicPlayerWidget>(PlayerController, BasicPlayerWidgetClass);
		BasicPlayerWidget->AddToViewport();
		UpdateKoalasAliveWidget();
	}
}

void AKoalaPlayerCharacter::Interact(const FInputActionValue& Value)
{
	// This Interact function is for picking things up, interacting with objects using the USE KEY
	// This is more optimized I think.
	FHitResult HitResult;
	if (!GetObjectAround(HitResult, InteractionRange)) return;

	if (ABaseTree* TreeObject = Cast<ABaseTree>(HitResult.GetActor())) {
		// NOTE: This is just an example. The tree attachment is on PlayerJump already
		UE_LOG(LogTemp, Warning, TEXT("Tree Interacted"));
	}
	else if (AConsumable* Consumable = Cast<AConsumable>(HitResult.GetActor())) {
		Super::ConsumeItem(Consumable);

		// TODO This is temporary! Cannot operate on something that will affect this class from the parent.
	}





	/*
	// Check if overlaps with something
	TArray<AActor*> OverlappingActors;
	// TODO for now filtered by ABaseInteractableObject, next will remove this filter and instead check if interface is implemented.
	GetOverlappingActors(OverlappingActors, ABaseInteractableObject::StaticClass());
	if (!OverlappingActors.IsEmpty())
	{
		for (auto OverlappingActor : OverlappingActors)
		{
			ABaseInteractableObject* Actor = Cast<ABaseInteractableObject>(OverlappingActor);
			Actor->Interact();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Not Overlap"));
	}
	*/
}

void AKoalaPlayerCharacter::PickupAndCarryItem(const FInputActionValue& Value)
{
	if (IsCarryingItem()) {
		DropCurrentCarriedItem();
		return;
	}
	FHitResult HitResult;
	if (!GetObjectAround(HitResult, InteractionRange)) return;
	UE_LOG(LogTemp, Warning, TEXT("Picking up: %s"), *HitResult.GetActor()->GetName());
	AActor* InteractedActor = HitResult.GetActor();
	// Accepted items that can be carried on the back are below
	if (InteractedActor->IsA(AKoalaBaseCharacter::StaticClass()) || InteractedActor->IsA(AConsumable::StaticClass())) {
		CarryItemOnBack(InteractedActor);
	}
}

void AKoalaPlayerCharacter::PlayerJump(const FInputActionValue& Value)
{
	// This PlayerJump function is for jumping AND interacting with trees as a result of player pressing JUMP KEY
	// TODO: We should also check if the player is already on tree
	if (bIsOnTree) {
		DetachFromCurrentTree();
		return;
	}
	FHitResult HitResult;
	if (AreThereAnyTreesAround(HitResult)) {
		/*if (TreeStartClimbingMontage) {
			DisableInput(PlayerController);
			PlayAnimMontage(TreeStartClimbingMontage);
		}*/
		if (Gun) {
			Gun->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, ItemCarryBoneNameOnMeshWEAPON);
		}
		// TODO: Attach to tree
		// We first teleport the player to the tree
		CurrentClimbingTree = Cast<ABaseTree>(HitResult.GetActor());
		FVector CurrentTreeLocation = CurrentClimbingTree->GetActorLocation();
		CurrentTreeLocation.Z = GetActorLocation().Z;
		FVector Direction = UKismetMathLibrary::GetDirectionUnitVector(CurrentTreeLocation, GetActorLocation());
		FVector TargetLocation = CurrentTreeLocation + (Direction *TreeAttachmentRadius);
		// Player will face the tree
		GetCharacterMovement()->StopMovementImmediately();
		SetActorLocation(TargetLocation);
		PlayerController->SetControlRotation((-1 * Direction).Rotation());
		GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);  // To allow the character to go up and down freely
		bIsOnTree = true;  // It is set in Anim Montage
		GetCharacterMovement()->bOrientRotationToMovement = false;
		return;
	}
	if (JumpingSound) {
		UGameplayStatics::PlaySoundAtLocation(GetWorld(), JumpingSound, GetActorLocation(), SoundPitches);
	}
	
	

	Super::Jump();
}


void AKoalaPlayerCharacter::Shoot(const FInputActionValue& Value)
{
	if (bIsOnTree) return;
	if(Gun)
	{
		Gun->PullTrigger();
	}
	
}

void AKoalaPlayerCharacter::StopShoot(const FInputActionValue& Value)
{
	if (bIsOnTree) return;
	if (Gun)
	{
		Gun->ReleaseTrigger();
	}
}

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
	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMapping, 0);
	}
}

void AKoalaPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AKoalaPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);



	if (UEnhancedInputComponent* EnhancedPlayerInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedPlayerInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AKoalaPlayerCharacter::Move);
		EnhancedPlayerInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AKoalaPlayerCharacter::Look);
		EnhancedPlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AKoalaPlayerCharacter::PlayerJump);
		EnhancedPlayerInputComponent->BindAction(InteractAction, ETriggerEvent::Started, this, &AKoalaPlayerCharacter::Interact);

	}

}

void AKoalaPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();

	const FRotator MyControlRotation = PlayerController->GetControlRotation();
	const FRotator MovementRotation(0.0f, MyControlRotation.Yaw, 0.0f);

	if (Input.X != 0.0f)
	{
		if (bIsOnTree) {
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
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, Input.X);
	}

	if (Input.Y != 0.0f)
	{
		if (bIsOnTree) {
			const FVector MovementDirection = MovementRotation.RotateVector(FVector::UpVector);
			AddMovementInput(MovementDirection, Input.Y*TreeClimbingSpeed);
			return;
		}
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, Input.Y);
	}


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
	if (!bIsOnTree || CurrentClimbingTree == nullptr) return;
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);  // Character will go back to normal movement mode
	// Super::Jump();
	bIsOnTree = false;
	GetCharacterMovement()->bOrientRotationToMovement = false;
	CurrentClimbingTree = nullptr;
}

void AKoalaPlayerCharacter::Interact(const FInputActionValue& Value)
{
	// This Interact function is for picking things up, interacting with objects using the USE KEY
	// This is more optimized I think.
	FHitResult HitResult;
	if (!GetObjectsAround(HitResult, InteractionRange)) return;


	if (ABaseTree* TreeObject = Cast<ABaseTree>(HitResult.GetActor())) {
		// NOTE: This is just an example. The tree attachment is on PlayerJump already
		UE_LOG(LogTemp, Warning, TEXT("Tree Interacted"));
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
		bIsOnTree = true;
		GetCharacterMovement()->bOrientRotationToMovement = false;
		return;
	}
	
	

	Super::Jump();
}

// Fill out your copyright notice in the Description page of Project Settings.


#include "KoalaPlayerCharacter.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "BaseInteractableObject.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"

AKoalaPlayerCharacter::AKoalaPlayerCharacter()
{
	Super::AKoalaBaseCharacter();
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
		EnhancedPlayerInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
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
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, Input.X);
	}

	if (Input.Y != 0.0f)
	{
		const FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, Input.Y);
	}


}

void AKoalaPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}

void AKoalaPlayerCharacter::Interact(const FInputActionValue& Value)
{
	// This Interact function is for picking things up, interacting with objects using the USE KEY

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
}

void AKoalaPlayerCharacter::PlayerJump(const FInputActionValue& Value)
{
	// This PlayerJump function is for jumping AND interacting with trees as a result of player pressing JUMP KEY
	// TODO: Should check if there are any trees around
	

	Super::Jump();
}

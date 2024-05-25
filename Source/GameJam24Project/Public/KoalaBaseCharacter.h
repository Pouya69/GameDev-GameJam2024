// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "KoalaBaseCharacter.generated.h"

struct FInputActionValue;
class UInputMappingContext;
class UInputAction;

UCLASS()
class GAMEJAM24PROJECT_API AKoalaBaseCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AKoalaBaseCharacter();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);

	UPROPERTY(EditAnywhere, Category="Input")
	UInputMappingContext* InputMapping;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere, Category="Input")
	UInputAction* JumpAction;

};




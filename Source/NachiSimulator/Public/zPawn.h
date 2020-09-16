// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"

#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"

#include "zPawn.generated.h"

UCLASS()
class NACHISIMULATOR_API AzPawn : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	AzPawn();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void Restart() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


protected:

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USceneComponent* Root;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		UCameraComponent* Camera;


public:

	UFUNCTION()
		void ZoomCamera(float Axis);

	UFUNCTION()
		void MoveMouseRight(float Axis);

	UFUNCTION()
		void MoveMouseUp(float Axis);

	UFUNCTION()
		void AllowOrbit();

	UFUNCTION()
		void FreezeOrbit();

	UFUNCTION()
		void AllowPan();

	UFUNCTION()
		void FreezePan();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsRightPressed = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
		bool IsMiddlePressed = false;

};

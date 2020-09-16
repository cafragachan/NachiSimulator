// Fill out your copyright notice in the Description page of Project Settings.


#include "zPawn.h"
#include "Components/InputComponent.h"
#include "GameFramework/Controller.h"

// Sets default values
AzPawn::AzPawn()
{
	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>("Root");
	SetRootComponent(Root);

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	SpringArm->SetupAttachment(RootComponent);

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");
	Camera->SetupAttachment(SpringArm);
	//Camera->bUsePawnControlRotation = true;
}

// Called when the game starts or when spawned
void AzPawn::BeginPlay()
{
	Super::BeginPlay();

	AController* MyController = GetController();
	if (!MyController) return;
	//MyController->SetControlRotation(FRotator(-10, 0, 0));

	if (!ensure(Camera)) Camera = FindComponentByClass<UCameraComponent>();

}

void AzPawn::Restart()
{
	Super::Restart();

	AController* MyController = GetController();
	if (!MyController) return;
	//MyController->SetControlRotation(FRotator(-10, 0, 0));
}

// Called every frame
void AzPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AzPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("Zoom", this, &AzPawn::ZoomCamera);

	PlayerInputComponent->BindAxis("MoveMouseRight", this, &AzPawn::MoveMouseRight);
	PlayerInputComponent->BindAxis("MoveMouseUp", this, &AzPawn::MoveMouseUp);

	PlayerInputComponent->BindAction("Orbit", IE_Pressed, this, &AzPawn::AllowOrbit);
	PlayerInputComponent->BindAction("Orbit", IE_Released, this, &AzPawn::FreezeOrbit);

	PlayerInputComponent->BindAction("Pan", IE_Pressed, this, &AzPawn::AllowPan);
	PlayerInputComponent->BindAction("Pan", IE_Released, this, &AzPawn::FreezePan);
}

void AzPawn::ZoomCamera(float Axis)
{
	SpringArm->TargetArmLength -= Axis * 300;
}

void AzPawn::MoveMouseRight(float Axis)
{
	if (IsRightPressed) AddControllerYawInput(Axis);

	else if (IsMiddlePressed && GetController())
	{
		if (Camera) Camera->AddLocalOffset(FVector(0, Axis * -30, 0));
		else UE_LOG(LogTemp, Warning, TEXT("no camera"));
	}

	else return;
}

void AzPawn::MoveMouseUp(float Axis)
{
	if (IsRightPressed) AddControllerPitchInput(Axis * -1);

	else if (IsMiddlePressed && GetController())
	{
		if (Camera) Camera->AddLocalOffset(FVector(0, 0, Axis * -30));
		else UE_LOG(LogTemp, Warning, TEXT("no camera"));
	}

	else return;
}

void AzPawn::AllowOrbit()
{
	UE_LOG(LogTemp, Warning, TEXT("pressing Right"));
	IsRightPressed = true;
}

void AzPawn::FreezeOrbit()
{
	IsRightPressed = false;
}

void AzPawn::AllowPan()
{
	UE_LOG(LogTemp, Warning, TEXT("pressing Middle"));

	IsMiddlePressed = true;
}

void AzPawn::FreezePan()
{
	IsMiddlePressed = false;
}
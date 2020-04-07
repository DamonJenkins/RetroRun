// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerBase.h"

// Sets default values
APlayerBase::APlayerBase()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void APlayerBase::BeginPlay()
{
	Super::BeginPlay();

	dashTimer = dashDuration + 1.0f;

	JumpMaxHoldTime = timerMax;
	JumpMaxCount = 2;

	capsule = GetCapsuleComponent();
	skelMesh = GetMesh();

	skelMesh->SetRelativeRotation(FRotator(0.0f, 0.0f, 0.0f));
}

// Called every frame
void APlayerBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (dashTimer < dashDuration + dashCooldown) {
		dashTimer += DeltaTime;
	}

	if (dashing) {
		GetCharacterMovement()->GravityScale = 0.0f;

		//Lerp
		SetActorLocation(
			FMath::Lerp(dashStart, dashTarget, dashTimer / dashDuration),
			true
		);

		if (dashTimer >= dashDuration) dashing = false;
	} else {
		GetCharacterMovement()->GravityScale = 1.0f;
	}
}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(class UInputComponent* _inComp)
{
	Super::SetupPlayerInputComponent(_inComp);

	_inComp->BindAxis("plrMoveHorizontal", this, &APlayerBase::MoveHorizontal);
	_inComp->BindAxis("plrMoveVertical");
	
	_inComp->BindAction("plrJump", IE_Pressed, this, &APlayerBase::Jump);
	_inComp->BindAction("plrJump", IE_Released, this, &APlayerBase::StopJumping);

	_inComp->BindAction("plrDash", IE_Pressed, this, &APlayerBase::Dash);
}

void APlayerBase::MoveHorizontal(float _lrAxis) {
	//if (dashing) return;

	if (FMath::Abs(_lrAxis) > 0.05f) {
		AddMovementInput(FVector(0, FMath::Sign(_lrAxis), 0), FMath::Abs(_lrAxis));
		skelMesh->SetRelativeRotation(FRotator(0.0f, _lrAxis > 0.0f ? 0.0f : 180.0f, 0.0f));
	}

}

void APlayerBase::Dash() {
	if (dashing) return;
	if (dashTimer < dashDuration + dashCooldown) return;

	FVector aimDir = FVector(0.0f, GetInputAxisValue(FName("plrMoveHorizontal")), GetInputAxisValue(FName("plrMoveVertical")));

	if (aimDir.Size() < 0.2f) return;

	dashing = true;
	dashTimer = 0.0f;

	dashStart = capsule->GetComponentLocation();

	dashTarget = dashStart + (aimDir * dashLength);

	FVector currVel = capsule->GetPhysicsLinearVelocity();

	float hComp = (aimDir.Y < 0.0f) == (currVel.Y < 0.0f) ? currVel.Y : 0.0f;
	float vComp = (aimDir.Z < 0.0f) == (currVel.Z < 0.0f) ? currVel.Z : 0.0f;

	capsule->SetPhysicsLinearVelocity(FVector(0.0f, hComp, vComp));
}


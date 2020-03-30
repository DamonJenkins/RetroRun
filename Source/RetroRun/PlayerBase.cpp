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
	
	TArray<UStaticMeshComponent*> prims;
	GetComponents(prims);

	mesh = prims[0];

	dashTimer = dashDuration + 1.0f;
}

// Called every frame
void APlayerBase::Tick(float DeltaTime) {
	Super::Tick(DeltaTime);

	if (dashTimer < dashDuration + dashCooldown) {
		dashTimer += DeltaTime;
	}

	if (dashing) {
		//Lerp
		SetActorLocation(
			FMath::Lerp(dashStart, dashTarget, dashTimer / dashDuration),
			true
		);

		if (dashTimer >= dashDuration) dashing = false;
	} else {

		FVector addForce(0.0f, 0.0f, -1.0f * gravityForce * gravityScale);

		mesh->AddForce(addForce);

		if (timer < timerMax) {
			timer += DeltaTime;
			if (timer >= timerMax) {
				EndJump();
			}
		}

		FVector newVel = mesh->GetPhysicsLinearVelocity();
		newVel.Y = FMath::Clamp(newVel.Y, -maxSpeed, maxSpeed);
		mesh->SetPhysicsLinearVelocity(newVel);
	}
}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(class UInputComponent* _inComp)
{
	Super::SetupPlayerInputComponent(_inComp);

	_inComp->BindAxis("plrMoveHorizontal", this, &APlayerBase::MoveHorizontal);
	
	_inComp->BindAction("plrJump", IE_Pressed, this, &APlayerBase::StartJump);
	_inComp->BindAction("plrJump", IE_Released, this, &APlayerBase::EndJump);

	_inComp->BindAction("plrDash", IE_Pressed, this, &APlayerBase::Dash);
}

void APlayerBase::MoveHorizontal(float _lrAxis) {
	if (dashing) return;
	mesh->AddForce(FVector(0.0f, moveAccel * 1000.0f * _lrAxis, 0.0f));
}

void APlayerBase::StartJump() {
	walled = false;

	FHitResult hitRes;
	FVector startVec = mesh->GetComponentLocation() + FVector(0.0f, 0.0f, -60);
	FVector endVec = FVector(0.0f, 0.0f, -5.0f) + startVec;
	FCollisionQueryParams collisionParams(FName(TEXT("Jump Trace")), true, this);

	if (GetWorld()->LineTraceSingleByChannel(hitRes, startVec, endVec, ECC_WorldStatic, collisionParams)) {
		if (hitRes.bBlockingHit) {
			grounded = true;
			doubleJumped = false;
		}
	} else {
		grounded = false;

		FVector wallcheckL = FVector(0.0f, -70.0f, 0.0f) + startVec;
		FVector wallcheckR = FVector(0.0f,  70.0f, 0.0f) + startVec;
		FVector jumpVec = FVector(0.0f, 0.0f, 1.0f);
		if (GetWorld()->LineTraceSingleByChannel(hitRes, startVec, wallcheckL, ECC_WorldStatic, collisionParams)) {
			//Wall Jump off the wall to the left of the character
			if (hitRes.bBlockingHit) {
				walled = true;
				mesh->SetPhysicsLinearVelocity(jumpVec.RotateAngleAxis(-wallJumpAngle, FVector(1, 0, 0)) * jumpForce);

				timer = 0.0f;
			}
		} else if (GetWorld()->LineTraceSingleByChannel(hitRes, startVec, wallcheckR, ECC_WorldStatic, collisionParams)) {
			if (hitRes.bBlockingHit) {
				walled = true;

				mesh->SetPhysicsLinearVelocity(jumpVec.RotateAngleAxis(wallJumpAngle, FVector(1, 0, 0)) * jumpForce);

				timer = 0.0f;
			}
		}
	}

	if (grounded) {
		gravityScale = jumpGravityScale;
		
		FVector modVel = mesh->GetPhysicsLinearVelocity();
		modVel.Z = jumpForce;
		mesh->SetPhysicsLinearVelocity(modVel);

		timer = 0.0f;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "Jump");

		dashing = false;
	} else if (!doubleJumped && !walled) {
		gravityScale = doubleJumpGravityScale;
		
		
		FVector modVel = mesh->GetPhysicsLinearVelocity();
		modVel.Z = jumpForce;
		mesh->SetPhysicsLinearVelocity(modVel);

		timer = 0.0f;
		doubleJumped = true;
		GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, "DoubleJump");

		dashing = false;
	}
}

void APlayerBase::EndJump() {
	gravityScale = 1.0f;
}

void APlayerBase::Dash() {
	if (dashing) return;
	if (dashTimer < dashDuration + dashCooldown) return;
	dashing = true;
	dashTimer = 0.0f;

	dashStart = mesh->GetComponentLocation();

	FVector aimDir = FVector(0.0f, GetInputAxisValue(FName("plrMoveHorizontal")), GetInputAxisValue(FName("plrMoveVertical")));
	dashTarget = dashStart + (aimDir * dashLength);

	FVector currVel = mesh->GetPhysicsLinearVelocity();

	float hComp = (aimDir.Y < 0.0f) == (currVel.Y < 0.0f) ? currVel.Y : 0.0f;
	float vComp = (aimDir.Z < 0.0f) == (currVel.Z < 0.0f) ? currVel.Z : 0.0f;

	mesh->SetPhysicsLinearVelocity(FVector(0.0f, hComp, vComp));
}


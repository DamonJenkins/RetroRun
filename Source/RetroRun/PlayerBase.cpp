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
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	mesh->AddForce(FVector(0.0f, 0.0f, -9.8f * 5000.0f * gravityScale));

	FVector newVel = mesh->GetPhysicsLinearVelocity();
	newVel.Y = FMath::Clamp(newVel.Y, -maxSpeed, maxSpeed);
	mesh->SetPhysicsLinearVelocity(newVel);
}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(class UInputComponent* _inComp)
{
	Super::SetupPlayerInputComponent(_inComp);

	_inComp->BindAxis("plrMoveHorizontal", this, &APlayerBase::MoveHorizontal);
	
	_inComp->BindAction("plrJump", IE_Pressed, this, &APlayerBase::StartJump);
	_inComp->BindAction("plrJump", IE_Released, this, &APlayerBase::EndJump);
}

void APlayerBase::MoveHorizontal(float _lrAxis) {
	mesh->AddForce(FVector(0.0f, moveAccel * _lrAxis, 0.0f));
}

void APlayerBase::StartJump() {
	mesh->AddImpulse(FVector(0.0f, 0.0f, 50000.0f));
}

void APlayerBase::EndJump() {

}


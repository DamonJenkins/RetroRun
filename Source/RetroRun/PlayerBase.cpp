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
	
	TArray<UPrimitiveComponent*> prims;
	GetComponents<UPrimitiveComponent>(prims);

	physicsComp = prims[0];
}

// Called every frame
void APlayerBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	physicsComp->AddForce(FVector(0.0f, 0.0f, 9.8f * gravityScale));
}

// Called to bind functionality to input
void APlayerBase::SetupPlayerInputComponent(class UInputComponent* _inComp)
{
	Super::SetupPlayerInputComponent(_inComp);

	_inComp->BindAxis("MoveHorizontal", this, &APlayerBase::MoveHorizontal);
	
	_inComp->BindAction("Jump", IE_Pressed, this, &APlayerBase::StartJump);
	_inComp->BindAction("Jump", IE_Released, this, &APlayerBase::EndJump);
}

void APlayerBase::MoveHorizontal(float _lrAxis) {
	//AddMovementInput(FVector(0.0f, 200.0f, 0.0f), _lrAxis);

	

	AddActorWorldOffset(FVector(_lrAxis * 100.0f, 0.0f, 0.0f));
}

void APlayerBase::StartJump() {
	
}

void APlayerBase::EndJump() {

}


// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "PlayerBase.generated.h"

UCLASS()
class RETRORUN_API APlayerBase : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveHorizontal(float _lrAxis);
	void Dash();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	//Jump timer
	float timerMax = 0.4f;

	bool dashing;
	float dashTimer;

	FVector dashStart, dashTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		/*The amount of time the player can't dash for after dashing*/
		float dashCooldown = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		/*The amount of time a dash will take*/
		float dashDuration = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		/*The maximum distance (in cm) of a dash*/
		float dashLength = 150.0f;

private:
	UCapsuleComponent* capsule;
};

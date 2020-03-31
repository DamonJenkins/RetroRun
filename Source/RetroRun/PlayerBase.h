// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine.h"
#include "Math/UnrealMathUtility.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Pawn.h"
#include "PlayerBase.generated.h"

UCLASS()
class RETRORUN_API APlayerBase : public APawn
{
	GENERATED_BODY()

public:
	// Sets default values for this pawn's properties
	APlayerBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveHorizontal(float _lrAxis);
	void StartJump();
	void EndJump();
	void Dash();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	bool grounded = false;
	bool doubleJumped = false;
	bool walled = false;

	float gravityScale = 1.0f;
	
	//Jump timer
	float timer = 0.0f;
	float timerMax = 0.7f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float moveAccel = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float maxSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float gravityForce = 50000.0f;

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float jumpForce = 900.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float jumpGravityScale = 0.6f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float doubleJumpGravityScale = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		float wallJumpGravityScale = 0.8f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "PlayerStats")
		/*The angle (in degrees) the player jumps away from walls*/
		float wallJumpAngle = 45.0f;

private:
	UStaticMeshComponent* mesh;
	float meshHalfHeight = 0.0f;
};

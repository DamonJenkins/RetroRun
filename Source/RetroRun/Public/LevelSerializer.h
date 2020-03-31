// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Misc/FileHelper.h"

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"

#include "LevelSerializer.generated.h"

USTRUCT(Blueprintable)
struct FTileStruct {

	GENERATED_BODY()

		UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 containerIndex = 0;
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
		int32 instanceIndex = 0;
};

/**
 * 
 */
UCLASS()
class RETRORUN_API ULevelSerializer : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()


		UFUNCTION(BlueprintCallable)
		static TMap<FVector, FTileStruct> LoadLevel(FString levelName);

		UFUNCTION(BlueprintCallable)
		static void SaveLevel(FString levelName, TMap <FVector, FTileStruct> levelMap);
};


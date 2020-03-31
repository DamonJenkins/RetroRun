// Fill out your copyright notice in the Description page of Project Settings.


#include "LevelSerializer.h"

TMap<FVector, FTileStruct> ULevelSerializer::LoadLevel(FString levelName) {
	TMap<FVector, FTileStruct> temp = TMap<FVector, FTileStruct>();


	FString levelFile;
	FFileHelper::LoadFileToString(levelFile, *levelName);
	TArray<FString> values;
	levelFile.ParseIntoArrayLines(values);

	for (
		int32 i = 0;i < values.Num(); i += 5) 
	{
		FTileStruct str;
		str.containerIndex = FCString::Atoi(*values[i + 3]);
		str.instanceIndex = FCString::Atoi(*values[i + 4]);
		temp.Add(FVector(FCString::Atof(*values[i]), FCString::Atof(*values[i + 1]), FCString::Atof(*values[i + 2])), str);
	}

	return temp;
}

void ULevelSerializer::SaveLevel(FString levelName, TMap<FVector, FTileStruct> levelMap)
{
	FString text;
	TArray<FVector> keys;
	levelMap.GetKeys(keys);
	for(int32 i = 0; i < keys.Num(); i++)
	{
		text += FString::SanitizeFloat(keys[i].X) + "\n";
		text += FString::SanitizeFloat(keys[i].Y) + "\n";
		text += FString::SanitizeFloat(keys[i].Z) + "\n";
		text += FString::FromInt(levelMap[keys[i]].containerIndex) + "\n";
		text += FString::FromInt(levelMap[keys[i]].instanceIndex) + "\n";
	}
	FFileHelper::SaveStringToFile(text, *levelName);
}

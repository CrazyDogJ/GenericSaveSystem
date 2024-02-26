// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSS_ActorSaver.h"
#include "GSS_CharacterSaver.generated.h"
UCLASS()
class GENERICSAVESYSTEM_API UCharacterSaveData : public UObject
{
	GENERATED_BODY()

public:

	UPROPERTY(SaveGame)
	FRotator ControllerRotation;

	UPROPERTY(SaveGame)
	TArray<uint8> Data;
};

UCLASS(ClassGroup = GenericSaveSystem, meta = (BlueprintSpawnableComponent))
class GENERICSAVESYSTEM_API UGSS_CharacterSaver : public UGSS_ActorSaver
{
	GENERATED_BODY()

public:
	
	UPROPERTY()
	ACharacter* Character;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GenericSaveSystem")
	bool bSaveControllerRotation = true;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GenericSaveSystem")
	bool bSaveExtraData = true;

	/**
	 * Call it before you save your game.
	 */
	UFUNCTION(BlueprintCallable)
	void SetSaveObject(const TArray<uint8> InExtraData);

	/**
	 * Call it after you load your game.
	 */
	UFUNCTION(BlueprintCallable)
	TArray<uint8> GetSaveObject();
	
protected:
	virtual bool SerializeData(TArray<uint8>& SerializedData) override;
	virtual void DeserializeData(const TArray<uint8>& SerializedData) override;
	
	UPROPERTY()
	TArray<uint8> ExtraData;
	
	virtual void PostLoad() override;
	
};

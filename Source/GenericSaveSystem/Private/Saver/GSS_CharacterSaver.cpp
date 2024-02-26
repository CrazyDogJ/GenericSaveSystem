// Fill out your copyright notice in the Description page of Project Settings.


#include "Saver/GSS_CharacterSaver.h"

#include "GameFramework/Character.h"
#include "Serialization/GSS_Archive.h"


void UGSS_CharacterSaver::SetSaveObject(const TArray<uint8> InExtraData)
{
	ExtraData = InExtraData;
}

TArray<uint8> UGSS_CharacterSaver::GetSaveObject()
{
	return ExtraData;
}

bool UGSS_CharacterSaver::SerializeData(TArray<uint8>& SerializedData)
{
	//Character rotation
	UCharacterSaveData* CharacterSaveData = NewObject<UCharacterSaveData>();
	if (Character)
	{
		if (bSaveControllerRotation)
		{
			CharacterSaveData->ControllerRotation = Character->GetControlRotation();
		}
		if (bSaveExtraData)
		{
			CharacterSaveData->Data = ExtraData;
		}
	}
	
	//Actor保存到ActorRecord
	FGSS_ActorRecord ActorRecord;
	SerializeActor(GetOwner(), ActorRecord);

	//Write data
	TArray<uint8> Data;
	FMemoryWriter MemoryWriter(Data, true);
	FGSS_Archive Archive(MemoryWriter, false);
	CharacterSaveData->Serialize(Archive);
	ActorRecord.Serialize(Archive);
	MemoryWriter.Close();
	SerializedData = Data;
	
	return true;
}

void UGSS_CharacterSaver::DeserializeData(const TArray<uint8>& SerializedData)
{
	UCharacterSaveData* CharacterSaveData = NewObject<UCharacterSaveData>();
	FMemoryReader MemoryReader(SerializedData, true);
	FGSS_Archive Archive(MemoryReader, false);
	FGSS_ActorRecord ActorRecord;
	CharacterSaveData->Serialize(Archive);
	ActorRecord.Serialize(Archive);
	DeserializeActor(GetOwner(), ActorRecord);

	if (Character)
	{
		if (bSaveControllerRotation)
		{
			Character->Controller->SetControlRotation(CharacterSaveData->ControllerRotation);
		}
		if (bSaveExtraData)
		{
			ExtraData = CharacterSaveData->Data;
		}
	}
	
}

void UGSS_CharacterSaver::PostLoad()
{
	Super::PostLoad();

	Character = Cast<ACharacter>(GetOwner());
}

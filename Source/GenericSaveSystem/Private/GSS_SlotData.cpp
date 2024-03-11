// Copyright 2020-2024 luochuanyuewu. All Rights Reserved.

#include "GSS_SlotData.h"
#include "TimerManager.h"

#include "GSS_SavePreset.h"


/**
 * A macro for tidying up accessing of private members, through the above code
 *
 * @param InClass		The class being accessed (not a string, just the class, i.e. FStackTracker)
 * @param InObj			Pointer to an instance of the specified class
 * @param MemberName	Name of the member being accessed (again, not a string)
 * @return				The value of the member
 */
#define GET_PRIVATE(InClass, InObj, MemberName) (*InObj).*GetPrivate(InClass##MemberName##Accessor())

/////////////////////////////////////////////////////
// USlotData

void UGSS_SlotData::Serialize(FArchive& Ar)
{
	Super::Serialize(Ar);

	Ar << bStoreGameInstance;
	Ar << GameInstance;

	MainLevel.Serialize(Ar);
	Ar << SubLevels;
	Ar << SaverRecords;
}

bool UGSS_SlotData::FindSaverRecord(const FName& Name, FGSS_ObjectRecord*& Record)
{
	for (auto& SaverRecord : SaverRecords)
	{
		if (SaverRecord.Name.IsEqual(Name))
		{
			Record = &SaverRecord;
			return true;
		}
	}
	return false;
}

void UGSS_SlotData::CleanRecords(bool bKeepSublevels)
{
	//Clean Up serialization data
	GameInstance = {};

	MainLevel.CleanRecords();
	if (!bKeepSublevels)
	{
		SubLevels.Empty();
	}
}

bool UGSS_SlotData::CleanRecordByKey(const FName& Key)
{
	if (Key.IsValid())
	{
		TArray<int32> NeedToRemove;
		for (int32 i = 0; i < SaverRecords.Num(); i++)
		{
			auto SaverRecord = SaverRecords[i];
			if (SaverRecord.Name.IsEqual(Key))
			{
				NeedToRemove.Add(i);
			}
		}

		if (NeedToRemove.Num() > 0)
		{
			for (const auto Index : NeedToRemove)
			{
				SaverRecords.RemoveAtSwap(Index, 1, false);
			}
			SaverRecords.Shrink();
			return true;
		}
	}
	return false;
}
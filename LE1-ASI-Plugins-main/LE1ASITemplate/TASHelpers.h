#pragma once
#include "../LE1-SDK/SdkHeaders.h"

class ABioWorldHelper
{
public:
	void Update(ABioWorldInfo* bioWorldInfo)
	{

		bioWorldInfo->m_UIWorld; //class UBioUIWorld* //m_sMapFile
		bioWorldInfo->m_Subtitles; //class UBioSubtitles* //m_sSubtitle, 	void UpdateSubtitles ( float fDeltaT );

		bioWorldInfo->m_SkillGame; //class UBioSkillGame*

		bioWorldInfo->m_playerSquad; //class ABioPlayerSquad*
		bioWorldInfo->m_oQuestProgress; //class UBioQuestProgressionMap*
		bioWorldInfo->m_oPowerManager; //class UBioPowerManager*
		bioWorldInfo->m_oPlayerSquad; //class ABioBaseSquad*
		bioWorldInfo->m_oPendingLoot; //class UBioInventory*
		bioWorldInfo->m_oMomentaryLoot; //class UBioInventory*
		bioWorldInfo->m_oGlobalVariables; //class UBioGlobalVariableTable*
		bioWorldInfo->m_oDiscoveredCodex; //class UBioDiscoveredCodexMap*
		bioWorldInfo->m_oDefaultSquad; //class ABioBaseSquad*
		bioWorldInfo->m_oCurrentConversation; //class UBioConversation*
		bioWorldInfo->m_nCurrentTipID; //int
		bioWorldInfo->m_lstCinematicsSeen; //struct TArray<struct FString>
		bioWorldInfo->m_LevelType; //EBioLevelTypeObjectType
		bioWorldInfo->lstStateEventMaps; //struct TArray<class UBioStateEventMap*>
		bioWorldInfo->lstStateEventMapNames; //struct TArray<struct FString>
		bioWorldInfo->lstQuestMaps; //struct TArray<class UBioQuestMap*>
		bioWorldInfo->lstQuestMapNames; //struct TArray<struct FString>
		bioWorldInfo->lstOutcomeMaps; //struct TArray<class UBioOutcomeMap*>
		bioWorldInfo->lstOutcomeMapNames; //struct TArray<struct FString>
		bioWorldInfo->lstConsequenceMaps; //struct TArray<class UBioStateEventMap*>
		bioWorldInfo->lstConsequenceMapNames; //struct TArray<class UBioStateEventMap*>
		bioWorldInfo->lstCodexMaps; //struct TArray<class UBioCodexMap*>
		bioWorldInfo->lstCodexMapNames; //struct TArray<struct FString>
		bioWorldInfo->LocalPlayerController; //class ABioPlayerController*
		bioWorldInfo->Game; //class AGameInfo*
		bioWorldInfo->GameTimeSeconds; //float
		bioWorldInfo->EventNotifier; //class UBioEventNotifier*
		bioWorldInfo->CurrentGame; //class UBioSaveGame*
		bioWorldInfo->ControllerList; //class AController*
		bioWorldInfo->Children; //struct TArray<class AActor*>
		bioWorldInfo->ArtPlaceableList; //class ABioArtPlaceable*
		bioWorldInfo->StreamingLevels; //struct TArray<class ULevelStreaming*>
		bioWorldInfo->RealTimeSeconds; //float
		bioWorldInfo->PawnList; //class APawn*
	}



private:

};
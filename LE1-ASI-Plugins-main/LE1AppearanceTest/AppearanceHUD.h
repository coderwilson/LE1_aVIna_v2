#pragma once
#include <fstream>
#include <ostream>
#include <streambuf>
#include <sstream>
#include "../LE1-SDK/SdkHeaders.h"

/// <summary>
/// Abstract class for creating custom HUDs similar to StreamingLevelsHUD
/// </summary>
/// 

// Overload the << operator for FString
std::wostream& operator<<(std::wostream& os, const FString& str)
{
	if (str.Data)
	{
		os << str.Data;
	}
	return os;
}

ABioPawn* GetPlayer(AWorldInfo* worldInfo)
{
	if (!worldInfo) return nullptr;
	auto bioWorldInfo = (ABioWorldInfo*)worldInfo;
	auto pc = bioWorldInfo->LocalPlayerController;
	if (!pc) return nullptr;
	auto player = pc->Pawn;
	if (!player || !player->IsA(ABioPawn::StaticClass())) return nullptr;
	else return static_cast<ABioPawn*>(player);
}


class CustomHUD
{
public:
	bool ShouldDraw = true;
	CustomHUD(LPSTR windowName = "MassEffect")
	{
		_windowName = windowName;
	}
	virtual void Update(UCanvas* canvas)
	{
		_canvas = canvas;
	}
	virtual void Draw() = 0;

protected:
	UCanvas* _canvas;
	float textScale = 1.0f;
	float lineHeight = 12.0f;
	int yIndex;
	LPSTR _windowName;

	void SetTextScale()
	{
		HWND activeWindow = FindWindowA(NULL, _windowName);
		if (activeWindow)
		{
			RECT rcOwner;
			if (GetWindowRect(activeWindow, &rcOwner))
			{
				long width = rcOwner.right - rcOwner.left;
				long height = rcOwner.bottom - rcOwner.top;

				if (width > 2560 && height > 1440)
				{
					textScale = 2.0f;
				}
				else if (width > 1920 && height > 1080)
				{
					textScale = 1.5f;
				}
				else
				{
					textScale = 1.0f;
				}
				lineHeight = 12.0f * textScale;
			}
		}
	}

	void RenderText(std::wstring msg, const float x, const float y, const char r, const char g, const char b, const float alpha)
	{
		if (_canvas)
		{
			_canvas->SetDrawColor(r, g, b, alpha * 255);
			_canvas->SetPos(x, y + 64); //+ is Y start. To prevent overlay on top of the power bar thing
			_canvas->DrawTextW(FString{ const_cast<wchar_t*>(msg.c_str()) }, 1, textScale, textScale, nullptr);
		}
	}

	void RenderText(std::wstring msg, const float x, const float y, FColor color)
	{
		if (_canvas)
		{
			_canvas->SetDrawColor(color.R, color.G, color.B, color.A * 255);
			_canvas->SetPos(x, y + 64); //+ is Y start. To prevent overlay on top of the power bar thing
			_canvas->DrawTextW(FString{ const_cast<wchar_t*>(msg.c_str()) }, 1, textScale, textScale, nullptr);
		}
	}

	void RenderTextLine(std::wstring msg, const char r, const char g, const char b, const float alpha)
	{
		RenderText(msg, 5, lineHeight * yIndex, r, g, b, alpha);
		yIndex++;
	}

	void RenderTextLine(std::wstring msg, FColor color)
	{
		RenderText(msg, 5, lineHeight * yIndex, color);
		yIndex++;
	}

	void RenderName(const wchar_t* label, FName name)
	{
		wchar_t output[512];
		swprintf_s(output, 512, L"%s: %S", label, name.GetName());
		RenderTextLine(output, 0, 255, 0, 1.0f);
	}

	void RenderString(const wchar_t* label, FString str)
	{
		wchar_t output[512];
		swprintf_s(output, 512, L"%s: %s", label, str.Data);
		RenderTextLine(output, 0, 255, 0, 1.0f);
	}

	void RenderStringColor(const wchar_t* label, FString str, FColor color)
	{
		wchar_t output[512];
		swprintf_s(output, 512, L"%s: %s", label, str.Data);
		RenderTextLine(output, color);
	}

	void RenderInt(const wchar_t* label, int value)
	{
		wchar_t output[512];
		swprintf_s(output, 512, L"%s: %d", label, value);
		RenderTextLine(output, 0, 255, 0, 1.0f);
	}

	void RenderBool(const wchar_t* label, bool value)
	{
		wchar_t output[512];
		if (value)
		{
			swprintf_s(output, 512, L"%s: %s", label, L"True");
		}
		else
		{
			swprintf_s(output, 512, L"%s: %s", label, L"False");
		}
		RenderTextLine(output, 0, 255, 0, 1.0f);
	}


	FColor CalculatePawnColor(ABioPawn* player, UBioPawnBehavior* behavior) {
		unsigned char r = 0;
		unsigned char g = 0;
		unsigned char b = 0;

		if (behavior->IsHostile(player->Controller))
			r = 255;
		if (behavior->IsNeutral(player->Controller))
			b = 255;
		if (behavior->IsFriendly(player->Controller))
			g = 255;

		return { r, g, b, 1 };
	}

	void DrawPawnStuff(ABioPawn* player, ABioPawn* pawn) {
		if (pawn->IsDead())
			return;
		
		FColor Green = { 0, 255, 0, 1 };
		//bool isfriend = player->IsFriendly(pawn);
		//bool ishostile = player->IsHostile(pawn);
		//int pawn_hp = pawn->Health;

		UBioPawnBehavior* behavior = pawn->m_oBehavior;
		FColor pawnColor = CalculatePawnColor(player, behavior);

		//ABioBaseSquad* squad = behavior->Squad;

		UBioAttributesEpicPawn* attributes = behavior->m_PawnAttributes;

		int health = (int)attributes->m_HealthCurrent;

		int shield = (int)attributes->m_ShieldCurrent;
		if (behavior->m_oShield != nullptr && behavior->m_oShield->m_pAttributes != nullptr)
			shield = (int)behavior->m_oShield->m_pAttributes->m_DamageCapacity;

		//UBioAttributesShield* bioShield = behavior->m_oShield->m_pAttributes;
		//if (bioShield == nullptr)
		//	return;
		//int s1 = (int)bioShield->m_ShieldMax.m_Current;
		//int s2 = (int)bioShield->m_DamageCapacity;

		wchar_t output[512];
		swprintf_s(output, 512, L"%s, HP: %d, Shield: %d", pawn->m_oBehavior->m_oActorType->ActorGameName.Data, health, shield);
		
		RenderStringColor(L"Pawn Name", FString{ const_cast<wchar_t*>(output) }, pawnColor);
		
		////cout << *pawn->m_oBehavior->m_oActorType->ActorGameName.Data << "\n";
		//if (!wcscmp(pawn->m_oBehavior->m_oActorType->ActorGameName.Data, L"Ashley")) {
		//	Draw3DBoundingBox(_canvas, pawn, {0, 255, 0, 1}, player);
		//}
	}
};

class AppearanceHUD : public CustomHUD
{
public:
	AppearanceHUD() : CustomHUD("MassEffect") { }

	void Update(UCanvas* canvas, ABioHUD* biohud)
	{
		AWorldInfo* worldInfo = biohud->WorldInfo;

		ABioPawn* pawn = GetPlayer(worldInfo);
		__super::Update(canvas);
		_biohud = biohud;
		_pawn = pawn;
		_worldInfo = worldInfo;
	}

	void Draw() override
	{
		if (!ShouldDraw) return;

		SetTextScale();
		yIndex = 3;

		if (_worldInfo) {
			ABioPawn* pawnList = static_cast<ABioPawn*>(_worldInfo->PawnList);

			// Loop through the pawn list
			while (pawnList != nullptr) {
				DrawPawnStuff(_pawn, pawnList);
				pawnList = static_cast<ABioPawn*>(pawnList->NextPawn);
			}

			ABioWorldInfo* test = static_cast<ABioWorldInfo*>(_worldInfo->WorldInfo);

			// Retrieve and print the conversation subtitle
			UBioConversation* convo = test->eventGetConversation();
			if (convo) {
				wcout << convo->m_sCurrentSubTitle << "\n";
			}

			// Print the current conversation subtitle (if not nullptr)
			if (test->m_oCurrentConversation) {
				wcout << test->m_oCurrentConversation->m_sCurrentSubTitle << "\n";
			}

			// Uncomment the following lines later
			// if (_worldInfo->m_oCurrentConversation) {
			//     DrawConversationStuff(_worldInfo->m_oCurrentConversation);
			// }
		}
	}

private:
	ABioHUD* _biohud;
	ABioPawn* _pawn;
	AWorldInfo* _worldInfo;
};


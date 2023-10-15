#pragma once


class CHOKBAR_API PerGameSettings
{
private:

	static PerGameSettings* inst;
	static PerGameSettings* Instance() { return inst; }

public:

	PerGameSettings();
	~PerGameSettings();

private:
	/* PER GAME PRIVATE VARIABLES */
	WCHAR m_GameName[MAX_NAME_STRING];
	WCHAR m_ShortName[MAX_NAME_STRING];
	HICON m_MainICon;
	WCHAR m_BootTime[MAX_NAME_STRING];


public:

	static WCHAR* GameName() { return inst->m_GameName; }
	static void SetGameName(UINT id) { LoadString(HInstance(), id, inst->m_GameName, MAX_NAME_STRING); }

	static WCHAR* ShortName() { return inst->m_ShortName; }
	static void SetShortName(UINT id) { LoadString(HInstance(), id, inst->m_ShortName, MAX_NAME_STRING); }

	static HICON MainIcon() { return inst->m_MainICon; }
	static void SetMainIcon(UINT id) { LoadIcon(HInstance(), MAKEINTRESOURCE(id)); }

	static WCHAR* BootTime() { return inst->m_BootTime; }

};

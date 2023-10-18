#include "Chokbar.h"

PerGameSettings* PerGameSettings::inst;

PerGameSettings::PerGameSettings()
{
	inst = this;

	wcscpy_s(inst->m_GameName, L"undefined");
	wcscpy_s(inst->m_ShortName, L"undefined");
	wcscpy_s(inst->m_BootTime, Time::GetDateTimeString(true).c_str());

}

PerGameSettings::~PerGameSettings()
{
}

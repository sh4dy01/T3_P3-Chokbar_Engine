#pragma once

namespace Time
{

	std::wstring CHOKBAR_API GetTime(BOOL stripped = false);

	std::wstring CHOKBAR_API GetDate(BOOL stripped = false);

	std::wstring CHOKBAR_API GetDateTimeString(BOOL stripped = false);

}
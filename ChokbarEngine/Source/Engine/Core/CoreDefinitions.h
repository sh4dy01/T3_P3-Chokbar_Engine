#pragma once

#define MAX_NAME_STRING 256
#define HInstance() GetModuleHandle(NULL)

typedef std::wstring WSTRING;
typedef std::string  STRING;

#define DELPTR(ptr) if (ptr) { delete ptr; ptr = nullptr; }
#define NULLPTR(ptr) if (ptr) { ptr = nullptr; }
#define RELPTR(ptr) if (ptr) { ptr->Release(); ptr = nullptr; }
#define DELPTR(ptr) if (ptr != nullptr) { delete ptr; ptr = nullptr; }

#define I(x) x::GetInstance() 

#define _CRTDBG_MAP_ALLOC


#if defined(DEBUG) | defined(_DEBUG)
	#include <crtdbg.h>
	// Replace new to check for memory leaks
	#define new new(_NORMAL_BLOCK, __FILE__, __LINE__)

#endif

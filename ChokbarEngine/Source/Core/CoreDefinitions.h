#pragma once

#define MAX_NAME_STRING 256
#define HInstance() GetModuleHandle(NULL)
#define I(x) { return x::GetInstance() }

#ifdef _DEBUG
#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )
// Replace _NORMAL_BLOCK with _CLIENT_BLOCK if you want the
// allocations to be of _CLIENT_BLOCK type
#else
#define DBG_NEW new
#endif

typedef std::wstring WSTRING;
typedef std::string  STRING;
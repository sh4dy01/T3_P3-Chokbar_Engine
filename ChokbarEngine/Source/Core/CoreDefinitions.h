#pragma once

#define MAX_NAME_STRING 256
#define HInstance() GetModuleHandle(NULL)

typedef std::wstring WSTRING;
typedef std::string  STRING;

#define DELPTR(ptr) if (ptr) { delete ptr; ptr = nullptr; }
#define RELPTR(ptr) if (ptr) { ptr->Release(); ptr = nullptr; }
#define I(x) x::GetInstance() 
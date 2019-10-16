#pragma once

#ifdef DLL_IMPORT
#define CLASS_DECLSPEC __declspec(dllimport)
#else
#define CLASS_DECLSPEC __declspec(dllexport)
#endif
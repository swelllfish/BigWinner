#pragma once

#include <string>
#include <Windows.h>


typedef unsigned char U8;

U8 ASCII2U8(char *pbuf, int cnt);

std::string ASCII2String(char *pbuf, int cnt);

void StringToLPCWSTR(std::string orig, wchar_t *wcstring);

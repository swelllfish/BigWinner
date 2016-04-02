#pragma once

#include <string>


typedef unsigned char U8;

U8 ASCII2U8(char *pbuf, int cnt);

std::string ASCII2String(char *pbuf, int cnt);
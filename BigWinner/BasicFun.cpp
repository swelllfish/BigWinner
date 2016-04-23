#include "BasicFun.h"
#include <math.h>

U8 ASCII2U8(char *pbuf, int cnt)
{
	int i;
	U8 result = 0;
	for (i = 0; i < cnt; ++i)
	{
		result += (*pbuf - 0x30) * (U8)pow((double)10, cnt - i - 1);
		++pbuf;
	}
	return result;
}

std::string ASCII2String(char *pBuf, int cnt)
{
	int i;
	std::string result;

	for (i = 0; i < cnt; ++i)
	{
		result += *pBuf;
		++pBuf;
	}
	
	return result;
}

LPCWSTR StringToLPCWSTR(std::string orig)
{
	size_t origsize = orig.length() + 1;
	const size_t newsize = 100;
	size_t convertedChars = 0;
	wchar_t *wcstring = (wchar_t *) malloc(sizeof(wchar_t) *(orig.length() - 1));
	mbstowcs_s(&convertedChars, wcstring, origsize, orig.c_str(), _TRUNCATE);


	return wcstring;
}
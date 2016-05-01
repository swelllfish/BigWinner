#pragma once

#include "stdafx.h"
using namespace std;

#define SE_RED_BALL (0)
#define US_RED_BALL (1)
#define BLUE_BALL   (2)
#define DATA_NUM    (3)
#define DATA        (4)

typedef struct
{
	vector<U8> SequRed;
	vector<U8> UnseqRed;
	vector<U8> BlueBall;
	vector<string> DateNum;
	vector<string> Date;
}_Num_Data;


class OpFile
{
public:
	OpFile();
	~OpFile();
	void FileAnalise();
	char *LoadFile(const char *FilePath);
	void GetAllData(char *FileBuf);
	bool Exit();
	vector<U8>::iterator GetInfor_it(int SerNum, U8 BallNum, U8 Type);
	vector<string>::iterator GetInfor_it(int SerNum, U8 Type);
	int GetInfor_Capacity(U8 Type);
private:
	char *FileBuf;
	long FileSize;
	_Num_Data numinfor;	//the information of the file data

	const static int SequRedSize = 2;
	const static int UnseqRedSize = 2;
	const static int BlueBallSize = 2;
	const static int DateNumSize = 5;
	const static int DateSize = 10;

};
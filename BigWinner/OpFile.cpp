#include "stdafx.h"
#include "BasicFun.h"

const static char FilePath[] = "./History.txt";

OpFile::OpFile()
{

}

OpFile::~OpFile()
{
	
}

void OpFile::FileAnalise()
{
	//load file and distribution ram
	FileBuf = LoadFile(FilePath);
	GetAllData(FileBuf);
}

char *OpFile::LoadFile(const char *FilePath)
{
	using namespace std;

	char *buffer;
	filebuf *pbuf;

	//using binary to open the file
	ifstream file;
	file.open(FilePath, ios::binary);

// 	if (file.is_open())	//to be sure the file is exit
// 	{
// 		MessageBox(NULL, TEXT("Open Successed"), TEXT("Successed"), MB_ICONQUESTION);
// 	}
// 	else
// 	{
// 		MessageBox(NULL, TEXT("Open Filed"), TEXT("Failed"), MB_ICONERROR);
// 		return buffer;
// 	}

	pbuf = file.rdbuf();	//get the pointer of the file's buff

	FileSize = (long)pbuf->pubseekoff(0, ios::end, ios::in);	//get the file size
	pbuf->pubseekpos(0, ios::in);	//seek the start location of the file

	buffer = new char[FileSize];	//distribution ram to it

	pbuf->sgetn(buffer, FileSize);	//get the file content

	file.close();

	return buffer;
}

void OpFile::GetAllData(char *FileBuf)
{
	long cnt;
	char *pBuf = FileBuf;

	for (cnt = 0; cnt < FileSize;)
	{
		//Get DateNum
		numinfor.DateNum.push_back(ASCII2String(pBuf, DateNumSize));
		pBuf += DateNumSize;
		++pBuf;	//add space
		cnt += DateNumSize + 1;

		//Get Date
		numinfor.Date.push_back(ASCII2String(pBuf, DateSize));
		pBuf += DateSize;
		++pBuf;	//add space
		cnt += DateSize + 1;

		//Get 7 RedBall Num on sequence
		for(U8 i = 0; i < 6; ++i)
		{
			numinfor.SequRed.push_back(ASCII2U8(pBuf, SequRedSize));
			pBuf += SequRedSize;
			++pBuf;	//add space
			cnt += SequRedSize + 1;
		}

		//Get 7 RedBall Num without sequence
		for(U8 i = 0; i < 6; ++i)
		{
			numinfor.UnseqRed.push_back(ASCII2U8(pBuf, UnseqRedSize));
			pBuf += UnseqRedSize;
			++pBuf;	//add space
			cnt += UnseqRedSize + 1;
		}

		//Get Blue Ball Num
		numinfor.BlueBall.push_back(ASCII2U8(pBuf, BlueBallSize));
		pBuf += BlueBallSize;
		pBuf += 2;	//add space
		cnt += BlueBallSize + 2;
	}
}

vector<U8>::iterator OpFile::GetInfor_it(int SerNum, U8 BallNum, U8 Type)
{
	vector<U8>::iterator it_BallNum;

	switch(Type)
	{
	case SE_RED_BALL:
		it_BallNum = numinfor.SequRed.begin();
		break;
	case US_RED_BALL:
		break;
	case BLUE_BALL:
		break;
	default:
		break;
	}

	return it_BallNum;
}

vector<string>::iterator OpFile::GetInfor_it(int SerNum, U8 Type)
{
	vector<string>::iterator it_Infor;
	
	switch(Type)
	{
	case DATA_NUM:
		it_Infor = numinfor.DateNum.begin();
		break;
		
	case DATA:
		it_Infor = numinfor.Date.begin();
		break;

	default:
		break;
	}

	it_Infor += SerNum;
	return it_Infor;
}

bool OpFile::Exit()
{
	delete(FileBuf);
	return true;
}
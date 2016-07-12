#pragma once
class Coordinate
{
public:
	Coordinate(void);
	~Coordinate(void);

	void DrawCoordinate(
		HDC hdcBuffer, 
		int xlocation, 
		int ylocation, 
		int start, 
		int len,
		int inter_len,
		int total_cnt, 
		unsigned char coortype,
		vector<string>::iterator TextString);
};


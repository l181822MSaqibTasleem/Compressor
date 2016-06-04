#include <iostream>
#include <fstream>
#include <iomanip>
#include <queue>
#include "HuffmanTree.h"

using namespace std;

#ifndef _Compressor_
#define _Compressor_

class Compressor{
private:
	BYTE leaf;
	BYTE attr;
	long length;
	Alpha alpha[_ARRAY_SIZE_];
	void LoadData(FILE *fp);
public:
	Compressor();
	void Compress(FILE *pIn, FILE *pOut);
	void Decompress(FILE *pIn, FILE *pOut);
};

#endif
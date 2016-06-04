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
	BYTE leaf;// Ҷ�ӽڵ���
	BYTE attr;// �����ֽ�����λ��
	long length;// Huffman������bit��Ŀ
	Alpha alpha[_ARRAY_SIZE_];// ͳ���ֽ�Ƶ����Ϣ
	void LoadData(FILE *fp);// ��ȡ�ļ���Ϣ
public:
	Compressor();
	void Compress(FILE *pIn, FILE *pOut);// ѹ���ļ�
	void Decompress(FILE *pIn, FILE *pOut);// ��ѹ�ļ�
};

#endif
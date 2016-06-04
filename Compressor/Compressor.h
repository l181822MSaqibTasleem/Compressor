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
	BYTE leaf;// 叶子节点数
	BYTE attr;// 结束字节填充的位数
	long length;// Huffman编码后的bit数目
	Alpha alpha[_ARRAY_SIZE_];// 统计字节频率信息
	void LoadData(FILE *fp);// 读取文件信息
public:
	Compressor();
	void Compress(FILE *pIn, FILE *pOut);// 压缩文件
	void Decompress(FILE *pIn, FILE *pOut);// 解压文件
};

#endif
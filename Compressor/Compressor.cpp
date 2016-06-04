#include "Compressor.h"
#include "HuffmanTree.h"
#include <string>
using namespace std;

long filesize(FILE *fp){
	fseek(fp, 0, SEEK_END);
	long size = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	return size;
}

template<class T>
void bubble_sort(T array[], int length)
{
	bool noswap;
	int i, j;
	for (i = 0; i < length - 1; i++)
	{
		noswap = true;
		for (j = length - 1; j>i; j--)
		{
			if (array[j] > array[j - 1]){
				swap(array[j], array[j - 1]); 
				noswap = false; 
			}
		}
		if (noswap)return;
	}
}

//压缩文件类构造函数
Compressor::Compressor()
{
	this->leaf = 0;
	this->attr = 0;
}

//压缩文件时获取文件字节的频率信息
void Compressor::LoadData(FILE *fp)
{
	//初始化alpha数组
	for (int index = 0; index < _ARRAY_SIZE_; index++){
		alpha[index].alpha = index;
		alpha[index].count = 0;
	}
	//读取字符频率信息
	size_t i, size;
	BYTE buf[_BLOCK_SIZE_];
	while ((size = fread(&buf, sizeof(BYTE), _BLOCK_SIZE_, fp))>0){
		i = 0;
		while (i < size)alpha[buf[i++]].count++;
	};
	// 对频率冒泡排序，高频在前
	bubble_sort<Alpha>(alpha, 256);
	leaf = 0;
	while (alpha[leaf].count>0){ leaf++; }
}

//压缩文件
void Compressor::Compress(FILE *pIn, FILE *pOut)
{
	printf("Load data\n");
	LoadData(pIn);
	printf("Load data finished\n");
	fwrite(&leaf, sizeof(BYTE), 1, pOut);
	fwrite(alpha, sizeof(Alpha), leaf, pOut);
	HuffmanTree tree(alpha, leaf);
	// 获取压缩数据
	int fs = filesize(pIn);
	length = 0;
	size_t i, size;
	BYTE c, buf[_BLOCK_SIZE_], byte = 0x00;
	printf("Compressing\n");
	fseek(pIn, 0, SEEK_SET);
	while ((size = fread(buf, sizeof(BYTE), _BLOCK_SIZE_, pIn)) > 0){
		i = 0;
		while (i < size){
			c = buf[i++];
			string str = tree.code(c);
			while (str.length() > 0){
				BYTE c = str.back();
				str.pop_back();
				length++;
				byte <<= 1;
				byte |= c == '1' ? 0x01 : 0x00;
				// 凑够一个字节
				if (length % 8 == 0){
					fwrite(&byte, sizeof(BYTE), 1, pOut);
					byte = 0x00;
				}
			}
		}
	}
	printf("Compressed: %ld bytes.\n", fs);
	// 不足一个字节的编码，通过填充低位
	// 以满足最后写入的是一个正确的字节
	attr = 8 - length % 8;
	while (attr-- > 0){
		byte <<= 1;
		byte |= 0x00;
	}
	fwrite(&byte, sizeof(BYTE), 1, pOut);
	fwrite(&attr, sizeof(BYTE), 1, pOut);
	fwrite(&length, sizeof(long), 1, pOut);
}

//解压文件
void Compressor::Decompress(FILE *pIn, FILE *pOut)
{
	fseek(pIn, -5, SEEK_END);
	fread(&attr, sizeof(BYTE), 1, pIn);
	fread(&length, sizeof(long), 1, pIn);
	fseek(pIn, 0, SEEK_SET);
	fread(&leaf, sizeof(BYTE), 1, pIn);
	fread(alpha, sizeof(Alpha), leaf, pIn);
	HuffmanTree tree(alpha, leaf);
	BYTE byte = 0x00, buf[_BLOCK_SIZE_], bit;
	WORD i;
	size_t s;
	Alpha data;
	long fs = 0;
	printf("Decompress\n");
	while ((s = fread(&buf, sizeof(BYTE), _BLOCK_SIZE_, pIn)) > 0){
		i = 0;
		while (i < s){
			byte = buf[i++];
			for (int j = 0x80; j != 0x00; j >>= 1){
				length--;
				bit = byte & 0x80;
				byte <<= 1;
				// 如果a->10010是a的压缩过程，这一步就是10010->a的解码过程
				if (tree.read(bit, data)){
					fwrite(&data.alpha, sizeof(BYTE), 1, pOut);
					fs++;
				}
				if (length == -1)break;// 如果已经解码了length个bit，就退出
			}
			if (length == -1)break;// 如果已经解码了length个bit，就退出
		}
		if (length == -1)break;// 如果已经解码了length个bit，就退出
	}
	printf("Decompressed: %ld bytes.\n", fs);
}
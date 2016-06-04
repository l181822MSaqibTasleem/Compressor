#include "Compressor.h"

void main(int argc, char *argv[])
{
	char c;
	if (argc < 4){
		printf("missing args\n");
		return;
	}
	printf("file:%s %s %s\n", argv[1], argv[2], argv[3]);
	Compressor comp;
	FILE  fIn, fOut;
	FILE *pIn = &fIn;
	FILE *pOut = &fOut;
	// 压缩文件
	fopen_s(&pIn, argv[1], "rb");
	fopen_s(&pOut, argv[2], "wb");
	comp.Compress(pIn, pOut);
	fclose(pIn);
	fclose(pOut);
	printf("Finished Compresse\n");
	// 解压文件
	fopen_s(&pIn, argv[2], "rb");
	fopen_s(&pOut, argv[3], "wb");
	comp.Decompress(pIn, pOut);
	fclose(pIn);
	fclose(pOut);
	printf("Finished decompress\n");
	printf("Press key to end:");
}
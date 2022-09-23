#include <stdio.h>
#include <stdlib.h>
#include <cwchar>
#include "zlib.h"
#include <iostream>
#include "BDO_encrypt.h"
#include "benchmark.h"
#include "UTF16Facet.h"
using namespace std;

int encrypt(char* srcFileName, char *outFileName)
{
	FILE* srcFile = NULL;
	fopen_s(&srcFile, srcFileName, "rb");
	if (srcFile == NULL){
		printf("File not found: %s", srcFileName);
		return -1;
	}
	FILE* tmpFile = NULL; 
	FILE* outFile = NULL;
	tmpfile_s(&tmpFile);
	if (!tmpFile)
	{
		printf("Temp file cannot be created");
		return -1;
	}
	fopen_s(&outFile, outFileName, "wb");
	if (!outFile)
	{
		printf("Output file cannot be created: %s", outFileName);
		return -1;
	}

	// start benchmark
	auto start = chrono::steady_clock::now();

	///Convert .txt source file into temporary .bss file
	unsigned long strSize;
	unsigned long strType;
	unsigned long strID1;
	unsigned long strID2; //for some reason "short" doesn't work well with fwscanf()
	unsigned long _strID3;
	unsigned long _strID4;
	unsigned char strID3;
	unsigned char strID4;
	int a;
	wchar_t strBuff[MAX_BUFF_SIZE];

	//Handle BOM (if present, skip it)
	fread(&strBuff[0], 2, 1, srcFile);
	if (strBuff[0] != BOM_UTF16LE) {
		rewind(srcFile);
	}

	printf("Reading phases...\r");
	while (1){
		wmemset(strBuff, CHAR_NULL, MAX_BUFF_SIZE);

		if (fwscanf_s(srcFile, L"%u\t%u\t%u\t%u\t%u\t", &strType, &strID1, &strID2, &_strID3, &_strID4) < 5) break; //this pattern "eats" leading white space from next string, so I had to enclose strings in double quotes (in bss -> txt conversion)

		strID3 = (unsigned char)_strID3;
		strID4 = (unsigned char)_strID4;

		fseek(srcFile, 2, SEEK_CUR); //skip leading double quotes

		for (a = 0; a < MAX_BUFF_SIZE; a++) {
			fread(&strBuff[a], 2, 1, srcFile);
			if (a > 0) {
				if (strBuff[a] == CHAR_LF || strBuff[a] == CHAR_CR) {
					if (strBuff[a-1] == L'"') {
						strBuff[a-1] = CHAR_NULL;
					}
					strBuff[a] = CHAR_NULL;
					break;
				}
				if (strBuff[a] == L'n' && strBuff[a-1] == L'\\') {
					a--;
					strBuff[a] = CHAR_LF;
				}
			}
		}

		strSize = wcslen(strBuff);

		fwrite(&strSize, 4, 1, tmpFile);
		fwrite(&strType, 4, 1, tmpFile);
		fwrite(&strID1,  4, 1, tmpFile);
		fwrite(&strID2,  2, 1, tmpFile);
		fwrite(&strID3,  1, 1, tmpFile);
		fwrite(&strID4,  1, 1, tmpFile);
		fwrite(&strBuff, 2, strSize, tmpFile);
		fputwc(CHAR_NULL, tmpFile);
		fputwc(CHAR_NULL, tmpFile);
	}
	printf("Encrypting...\r");
	strSize = strType = strID1 = strID2 = strID3 = strID4 = a = 0;
	wmemset(strBuff, CHAR_NULL, MAX_BUFF_SIZE);

	///Compress temporary .bss file to .loc
	unsigned long compressedSize = 0;
	unsigned long uncompressedSize = 0;

	fseek(tmpFile, 0, SEEK_END);
	uncompressedSize = ftell(tmpFile);
	rewind(tmpFile);

	compressedSize = compressBound(uncompressedSize);

	unsigned char *pCompressedData = (unsigned char *) calloc(compressedSize, sizeof(unsigned char));
	unsigned char *pUncompressedData = (unsigned char *) calloc(uncompressedSize, sizeof(unsigned char));

	fread(pUncompressedData, 1, uncompressedSize, tmpFile);

	int result = compress2(pCompressedData, &compressedSize, pUncompressedData, uncompressedSize, Z_BEST_SPEED);

	if (result == Z_OK) {
		fwrite(&uncompressedSize, 1, 4, outFile);
		fwrite(pCompressedData, 1, compressedSize, outFile);
		printf("Encryption completed");
	} else if (result == Z_MEM_ERROR) {
		printf("ERROR: Not enough memory.");
	} else if (result == Z_BUF_ERROR) {
		printf("ERROR: Output buffer is too small.");
	}

	fclose(srcFile);
	fclose(tmpFile);
	fclose(outFile);

	cout << endl << "Elapsed(ms)=" << since(start).count() << endl;

	srcFile = tmpFile = outFile = NULL;

	return 0;
}

#include <stdio.h>
#include <stdlib.h>
#include <cwchar>
#include "zlib.h"
#include <iostream>
#include "BDO_decrypt.h"
#include "benchmark.h"
#include "UTF16Facet.h"
using namespace std;

int decrypt(char* srcFileName, char* outFileName)
{

	FILE* srcFile = NULL;
	fopen_s(&srcFile, srcFileName, "rb");
	if (srcFile == NULL){
		printf("File not found: %s", srcFileName);
		return -1;
	}
	FILE* tmpFile = NULL;
	tmpfile_s(&tmpFile);
	if (!tmpFile)
	{
		printf("Temp file cannot be created");
		return -1;
	}
	FILE* outFile = NULL;
	fopen_s(&outFile, outFileName, "wb");
	if (!outFile)
	{
		printf("Output file cannot be created: %s", outFileName);
		return -1;
	}

	// start benchmark
	auto start = chrono::steady_clock::now();

	///Decompress source to temporary file
	unsigned long compressedSize = 0;
	unsigned long uncompressedSize = 0;

	fseek(srcFile, 0, SEEK_END);
	compressedSize = ftell(srcFile) - 4; // 1st 4 bytes holds information about uncompressed data size
	rewind(srcFile);

	fread(&uncompressedSize, 4, 1, srcFile);

	unsigned char *pCompressedData = (unsigned char *) calloc(compressedSize, sizeof(unsigned char));
	unsigned char *pUncompressedData = (unsigned char *) calloc(uncompressedSize, sizeof(unsigned char));

	fread(pCompressedData, 1, compressedSize, srcFile);

	int result = uncompress(pUncompressedData, &uncompressedSize, pCompressedData, compressedSize);

	if (result == Z_OK) {
		fwrite(pUncompressedData, 1, uncompressedSize, tmpFile);

		///Convert .bss data from temporary file to text file
		unsigned long strSize;
		unsigned long strType;
		unsigned long strID1;
		unsigned short strID2;
		unsigned char strID3;
		unsigned char strID4;
		int a, b;
		wchar_t tmp;
		wchar_t strBuff[MAX_BUFF_SIZE];

		rewind(tmpFile);

		//write BOM (byte order mark) 0xFEFF = UTF-16 little-endian
		fwrite(&BOM_UTF16LE, 2, 1, outFile);

		while (1){
			if (fread(&strSize, 4, 1, tmpFile) != 1) break;
			if (fread(&strType, 4, 1, tmpFile) != 1) break;
			if (fread(&strID1, 4, 1, tmpFile) != 1) break;
			if (fread(&strID2, 2, 1, tmpFile) != 1) break;
			if (fread(&strID3, 1, 1, tmpFile) != 1) break;
			if (fread(&strID4, 1, 1, tmpFile) != 1) break;
			b = 0;
			for (a = 0; a < strSize + 2; a++) {
				if (fread(&tmp, 2, 1, tmpFile) != 1) break;
				if (tmp == CHAR_LF) {
					strBuff[b] = L'\\';
					b++;
					strBuff[b] = L'n';
					b++;
				} else {
					strBuff[b] = tmp;
					b++;
				}
			}

			fwprintf(outFile, L"%u\t%u\t%u\t%u\t%u\t\"%s\"\r\n", strType, strID1, strID2, strID3, strID4, strBuff);
		}
	} else if (result == Z_MEM_ERROR) {
		printf("ERROR: Not enough memory.");
	} else if (result == Z_BUF_ERROR) {
		printf("ERROR: Output buffer is too small.");
	} else if (result == Z_DATA_ERROR) {
		printf("ERROR: Input data are corrupted or incomplete.");
	}

	fclose(srcFile);
	fclose(tmpFile);
	fclose(outFile);
	cout << "Decryption completed." << endl;
	cout << "Elapsed(ms)=" << since(start).count() << endl;

	return 0;
}

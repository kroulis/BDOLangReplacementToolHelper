#include <iostream>
#include <fstream>
#include "BDO_phrase.h"
#include "UTF16Facet.h"
using namespace std;

void print_progress_phrase(size_t current, size_t max, size_t phrases)
{
	printf("Reading %d phrases (%d/%d)\r", phrases, current, max);
}

int readPhrasesFromFile(const char* fileName, map<PhraseId, Phrase> &phraseMap, bool setExist)
{
	ifstream srcFile(fileName, ios::binary);
	if (srcFile.fail()) {
		printf("File not found: %s", fileName);
		return -1;
	}

	unsigned long strType;
	unsigned long strID1;
	unsigned long strID2;
	unsigned long strID3;
	unsigned long strID4;
	wstring localization;
	size_t strSize;
	int counter = 0;

	wchar_t strBuff[MAX_BUFF_SIZE];

	srcFile.seekg(0, ios::end);
	size_t size = (size_t)srcFile.tellg();

	//skip BOM
	srcFile.seekg(2, ios::beg);
	size -= 2;

	u16string u16((size / 2) + 1, '\0');
	srcFile.read((char*)&u16[0], size);
	srcFile.close();

	wstring input = u16ToWstring(u16);
	size_t inSize = input.size();
	size_t last = 0; size_t next = 0;
	while ((next = input.find('\n', last)) != wstring::npos || last < inSize)
	{
		wmemset(strBuff, CHAR_NULL, MAX_BUFF_SIZE);
		if (input.find('\n', last) == string::npos)
		{
			next = inSize;
		}
		wstring line = input.substr(last, next - last);
		if (swscanf_s(line.c_str(), L"%u\t%u\t%u\t%u\t%u\t %[^\r\n]s", &strType, &strID1, &strID2, &strID3, &strID4, strBuff, MAX_BUFF_SIZE) < 6)
		{
			break;
		}

		strSize = wcslen(strBuff);
		localization = wstring(strBuff);
		Phrase phrase(strType, strID1, strID2, strID3, strID4, localization);
		if (setExist)
			phrase.setExist();
		auto phraseFound = phraseMap.find(phrase.id);
		if (phraseFound == phraseMap.end())
		{
			phraseMap.insert(pair<PhraseId, Phrase>(phrase.id, phrase));
		}
		else
		{
			phraseMap[phrase.id] = phrase;
		}

		last = next + 1;
		counter++;

		print_progress_phrase(last, inSize, counter);
	}

	return counter;
}
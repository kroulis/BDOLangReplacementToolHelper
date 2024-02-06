#pragma once
#include <iostream>
#include <cstring>
#include <map>
#include <string>
using namespace std;

typedef struct PhraseId
{
	unsigned long strType;
	unsigned long strId1;
	unsigned long strId2;
	unsigned char strId3;
	unsigned char strId4;
	unsigned int wildMatch;

#define PHRASE_WILD_FLAG_STRTYPE  0b1
#define PHRASE_WILD_FLAG_STRID1   0b10
#define PHRASE_WILD_FLAG_STRID2   0b100
#define PHRASE_WILD_FLAG_STRID3   0b1000
#define PHRASE_WILD_FLAG_STRID4   0b10000
#define PHRASE_WILD_FLAG_MAX      0b100000

#define PHRASE_ISWILD(flag) ((wildMatch & flag) > 0)

	PhraseId(unsigned long _strType, unsigned long _strId1, unsigned long _strId2, unsigned char _strId3, unsigned char _strId4) :
		strType(_strType), strId1(_strId1), strId2(_strId2), strId3(_strId3), strId4(_strId4), wildMatch(0) {}

	PhraseId() : strType(0), strId1(0), strId2(0), strId3(0), strId4(0), wildMatch(0) {}

	string toHtml() const
	{
		return "<td>" + to_string(strType) + "</td><td>" + to_string(strId1) + "</td><td>" + to_string(strId2) + "</td><td>" + 
			to_string((unsigned int)strId3) + "</td><td>" + to_string((unsigned int)strId4) + "</td>";
	}

	bool hasWild() const
	{
		return wildMatch > 0;
	}

	bool isWild(int flag) const
	{
		return PHRASE_ISWILD(flag);
	}

	void setWild(unsigned int flag)
	{
		wildMatch |= flag;
	}

	bool operator == (const PhraseId& other) const
	{
		if (strType != other.strType && !isWild(PHRASE_WILD_FLAG_STRTYPE) && !other.isWild(PHRASE_WILD_FLAG_STRTYPE))
			return false;
		if (strId1 != other.strId1 && !isWild(PHRASE_WILD_FLAG_STRID1) && !other.isWild(PHRASE_WILD_FLAG_STRID1))
			return false;
		if (strId2 != other.strId2 && !isWild(PHRASE_WILD_FLAG_STRID2) && !other.isWild(PHRASE_WILD_FLAG_STRID2))
			return false;
		if (strId3 != other.strId3 && !isWild(PHRASE_WILD_FLAG_STRID3) && !other.isWild(PHRASE_WILD_FLAG_STRID3))
			return false;
		if (strId4 != other.strId4 && !isWild(PHRASE_WILD_FLAG_STRID4) && !other.isWild(PHRASE_WILD_FLAG_STRID4))
			return false;
		return true;
	}

	bool operator != (const PhraseId& other) const
	{
		return !(*this == other);
	}

	bool operator < (const PhraseId& other) const
	{
		if (strType != other.strType)
			return strType < other.strType;
		if (strId1 != other.strId1)
			return strId1 < other.strId1;
		if (strId2 != other.strId2)
			return strId2 < other.strId2;
		if (strId3 != other.strId3)
			return strId3 < other.strId3;
		if (strId4 != other.strId4)
			return strId4 < other.strId4;
		return false;
	}

	bool operator > (const PhraseId& other) const
	{
		if (strType != other.strType)
			return strType > other.strType;
		if (strId1 != other.strId1)
			return strId1 > other.strId1;
		if (strId2 != other.strId2)
			return strId2 > other.strId2;
		if (strId3 != other.strId3)
			return strId3 > other.strId3;
		if (strId4 != other.strId4)
			return strId4 > other.strId4;
		return false;
	}
} PhraseId;

typedef struct Phrase
{
	PhraseId id;
	wstring localization;
	bool exist;

	Phrase(unsigned long _strType, unsigned long _strId1, unsigned long _strId2, unsigned char _strId3, unsigned char _strId4, wstring _localization) :
		id(_strType, _strId1, _strId2, _strId3, _strId4), localization(_localization), exist(false) {}

	Phrase(): id(), localization(L""), exist(false) {}

	void setExist()
	{
		exist = true;
	}

	bool isExist()
	{
		return exist;
	}

	void setLocalization(wstring _loc)
	{
		localization = _loc;
	}

	wstring toString()
	{
#define BUFFER_MAX 4096
		wchar_t buff[BUFFER_MAX];
		swprintf_s(buff, BUFFER_MAX, L"%u\t%u\t%u\t%u\t%u\t%s\r\n", id.strType, id.strId1, id.strId2, id.strId3, id.strId4, localization.c_str());
		return wstring(buff);
	}
} Phrase;

int readPhrasesFromFile(const char* fileName, map<PhraseId, Phrase> &phraseMap, bool setExist);
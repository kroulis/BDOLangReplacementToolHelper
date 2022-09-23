#pragma once
#include <iostream>
#include <cstring>
#include <map>
using namespace std;

typedef struct PhaseId
{
	unsigned long strType;
	unsigned long strId1;
	unsigned long strId2;
	unsigned char strId3;
	unsigned char strId4;

	PhaseId(unsigned long _strType, unsigned long _strId1, unsigned long _strId2, unsigned char _strId3, unsigned char _strId4) :
		strType(_strType), strId1(_strId1), strId2(_strId2), strId3(_strId3), strId4(_strId4) {}

	PhaseId() : strType(0), strId1(0), strId2(0), strId3(0), strId4(0) {}

	bool operator == (const PhaseId& other) const
	{
		if (strType != other.strType)
			return false;
		if (strId1 != other.strId1)
			return false;
		if (strId2 != other.strId2)
			return false;
		if (strId3 != other.strId3)
			return false;
		if (strId4 != other.strId4)
			return false;
		return true;
	}

	bool operator != (const PhaseId& other) const
	{
		return !(*this == other);
	}

	bool operator < (const PhaseId& other) const
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

	bool operator > (const PhaseId& other) const
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
} PhaseId;

typedef struct Phase
{
	PhaseId id;
	wstring localization;
	bool exist;

	Phase(unsigned long _strType, unsigned long _strId1, unsigned long _strId2, unsigned char _strId3, unsigned char _strId4, wstring _localization) :
		id(_strType, _strId1, _strId2, _strId3, _strId4), localization(_localization), exist(false) {}

	Phase(): id(), localization(L""), exist(false) {}

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
} Phase;

int readPhasesFromFile(const char* fileName, map<PhaseId, Phase> &phaseMap, bool setExist);
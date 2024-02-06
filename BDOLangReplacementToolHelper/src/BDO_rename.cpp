#include <cstdio>
#include <iostream>
#include <fstream>
#include <codecvt>
#include <string>
#include <map>
#include "BDO_phrase.h"
#include "BDO_rename.h"
#include "UTF16Facet.h"
#include "benchmark.h"
using namespace std;

static bool strendswith(const char* str, const char* suffix)
{
	int len = strlen(str);
	int suffixlen = strlen(suffix);
	if (suffixlen > len)
	{
		return false;
	}

	str += (len - suffixlen);
	return strcmp(str, suffix) == 0;
}

static void print_progress_rename(size_t current, size_t max)
{
	double state = current * 1.0 / max * 100.0;
	string status = "";
	for (int i = 0; i < state - 1; i++)
	{
		status += "=";
	}
	status += ">";
	for (int i = (int)state + 1; i < 100; i++)
	{
		status += "*";
	}
	printf("(%d/%d) [%s]\r", current, max, status.c_str());
}

int rename(char* srcFileName, char* outFileName, vector<char*> additionalFiles)
{
	map<PhraseId, Phrase> phraseMap;
	map<PhraseId, PhraseId> replacement;

	// start benchmark
	auto start = chrono::steady_clock::now();
	int phrasesFound = 0;

	printf("Reading source phrases from: %s\r", srcFileName);
	if ((phrasesFound = readPhrasesFromFile(srcFileName, phraseMap, false)) == -1)
	{
		printf("File not found: %s", srcFileName);
		return -1;
	}
	else {
		printf("Found %d phrases from %s\n", phrasesFound, srcFileName);
	}

	for (size_t i = 0; i < additionalFiles.size(); i++)
	{
		if (!strendswith(additionalFiles[i], ".csv"))
		{
			printf("Ignored not supported file: %s\r", additionalFiles[i]);
			continue;
		}

		printf("Reading matching config file: %s\r", additionalFiles[i]);
		ifstream confFile(additionalFiles[i]);
		if (confFile.fail()) {
			printf("File not found: %s\n", additionalFiles[i]);
			continue;
		}

		int matchingCount = 0;
		string line;
		while (getline(confFile, line))
		{
			char* l = new char[line.size() + 1];
			strcpy_s(l, line.size() + 1, line.c_str());
			char* nxtTok = NULL;
			char* tok = strtok_s(l, ",", &nxtTok);
			int count = 0;
			PhraseId replaceFrom;
			PhraseId replaceTo;
			int index = 0;
			while (tok != NULL)
			{
				switch (count)
				{
				case 0:
					if (tok[0] == '*')
					{
						replaceFrom.strType = 0;
						replaceFrom.setWild(PHRASE_WILD_FLAG_STRTYPE);
					}
					else
					{
						replaceFrom.strType = atoi(tok);
					}
					break;
				case 1:
					if (tok[0] == '*')
					{
						replaceFrom.strId1 = 0;
						replaceFrom.setWild(PHRASE_WILD_FLAG_STRID1);
					}
					else
					{
						replaceFrom.strId1 = atoi(tok);
					}
					break;
				case 2:
					if (tok[0] == '*')
					{
						replaceFrom.strId2 = 0;
						replaceFrom.setWild(PHRASE_WILD_FLAG_STRID2);
					}
					else
					{
						replaceFrom.strId2 = atoi(tok);
					}
					break;
				case 3:
					if (tok[0] == '*')
					{
						replaceFrom.strId3 = 0;
						replaceFrom.setWild(PHRASE_WILD_FLAG_STRID3);
					}
					else
					{
						replaceFrom.strId3 = atoi(tok);
					}
					break;
				case 4:
					if (tok[0] == '*')
					{
						replaceFrom.strId4 = 0;
						replaceFrom.setWild(PHRASE_WILD_FLAG_STRID4);
					}
					else
					{
						replaceFrom.strId4 = atoi(tok);
					}
					break;
				case 6:
					replaceTo.strType = atoi(tok);
					break;
				case 7:
					replaceTo.strId1 = atoi(tok);
					break;
				case 8:
					replaceTo.strId2 = atoi(tok);
					break;
				case 9:
					replaceTo.strId3 = atoi(tok);
					break;
				case 10:
					replaceTo.strId4 = atoi(tok);
					break;
				}
				count++;
				tok = strtok_s(NULL, ",", &nxtTok);
			}
			delete[] l;
			if (count < 10)
				continue;

			replacement.insert(pair<PhraseId, PhraseId>(replaceFrom, replaceTo));
			matchingCount++;
			printf("Found %d renaming config\r", matchingCount);
		}

		confFile.close();
		printf("Read %d renaming config from %s\n", matchingCount, additionalFiles[i]);
	}

	for (auto itr = replacement.begin(); itr != replacement.end(); ++itr)
	{
		if (!itr->first.hasWild())
		{
			if (phraseMap.find(itr->first) != phraseMap.end())
			{
				Phrase p(phraseMap[itr->first]);
				phraseMap.erase(itr->first);
				p.id = itr->second;
				phraseMap.insert_or_assign(itr->second, p);
			}
		}
		else
		{
			map<PhraseId, Phrase> afterRename(phraseMap);
			bool changed = false;
			for (auto pp : phraseMap)
			{
				if (pp.first == itr->first)
				{
					Phrase p = pp.second;
					for (auto f = 1; f < PHRASE_WILD_FLAG_MAX; f <<= 1)
					{
						if (!itr->first.isWild(f))
						{
							switch (f)
							{
								case PHRASE_WILD_FLAG_STRTYPE:
									p.id.strType = itr->second.strType;
									break;
								case PHRASE_WILD_FLAG_STRID1:
									p.id.strId1 = itr->second.strId1;
									break;
								case PHRASE_WILD_FLAG_STRID2:
									p.id.strId2 = itr->second.strId2;
									break;
								case PHRASE_WILD_FLAG_STRID3:
									p.id.strId3 = itr->second.strId3;
									break;
								case PHRASE_WILD_FLAG_STRID4:
									p.id.strId4 = itr->second.strId4;
									break;
							}
						}
					}
					afterRename.erase(pp.first);
					afterRename.insert_or_assign(p.id, p);
					changed = true;
				}
			}

			if (changed)
			{
				phraseMap = afterRename;
			}
		}
	}

	int counter = 0;

	wofstream outFile;
	int total = phraseMap.size();
	locale utf16_locale(outFile.getloc(), new codecvt_utf16<wchar_t, MaxCode, UTF16_MODE>);
	outFile.imbue(utf16_locale);
	outFile.open(outFileName, ios::binary);
	wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> utf16Converter;

	if (outFile.fail()) {
		printf("File cannot be open: %s", outFileName);
		return -1;
	}

	for (auto pp : phraseMap)
	{
		Phrase p = pp.second;
		wstring phrase = p.toString();
		outFile << phrase;
		counter++;
		print_progress_rename(counter, total);
	}

	outFile.flush();
	outFile.close();

	cout << "Rename: " << counter << " phrases are written to " << outFileName << endl;
	cout << "Elapsed(ms)=" << since(start).count() << endl;
	return 0;
}
#include <iostream>
#include <fstream>
#include "BDO_match.h"
#include "BDO_phrase.h"
#include "benchmark.h"

int match(char* srcFileName, char* outFileName, vector<char*> additionalFiles)
{
	map<PhraseId, Phrase> phraseMap;
	map<PhraseId, Phrase> phraseMap2;

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

	for (auto i = 0; i < additionalFiles.size(); i++)
	{
		printf("Reading replacement phrases from: %s\r", additionalFiles[i]);
		if ((phrasesFound = readPhrasesFromFile(additionalFiles[i], phraseMap2, true)) == -1)
		{
			printf("Failed to read additional file: %s\n", additionalFiles[i]);
		}
		else
		{
			printf("Found %d phrases from %s\n", phrasesFound, additionalFiles[i]);
		}
	}

	ofstream outFile;
	outFile.open(outFileName);

	if (outFile.fail()) {
		printf("File cannot be open: %s", outFileName);
		return -1;
	}

	int totalMatch = 0;

	for (auto iter = phraseMap.begin(); iter != phraseMap.end(); ++iter)
	{
		PhraseId k = iter->first;
		if (k.strType <= 50 || k.strId3 != 12 || k.strId4 != 0)
			continue;
		PhraseId kAdjusted = k;
		kAdjusted.strId3 = 14;
		if (phraseMap2.find(kAdjusted) != phraseMap2.end())
		{
			int cnt = 0;
			for (int i = 1; i <= 4; i++)
			{
				kAdjusted.strId4 = i;
				if (phraseMap2.find(kAdjusted) != phraseMap2.end())
				{
					cnt = i;
				}
			}
			outFile << k.strType << "," << k.strId1 << "," << k.strId2 << "," << 12 << "," << 0 << ",=>," <<
				k.strType << "," << k.strId1 << "," << k.strId2 << "," << 14 << "," << 0 << ",=>," << cnt << endl;
		}
		else
		{
			int cnt = 0;
			for (int i = 1; i <= 4; i++)
			{
				k.strId4 = i;
				if (phraseMap.find(kAdjusted) != phraseMap.end())
				{
					cnt = i;
				}
			}
			outFile << k.strType << "," << k.strId1 << "," << k.strId2 << "," << 12 << "," << 0 << ",=>," <<
				k.strType << ",," << k.strId2 << "," << 14 << "," << 0 << ",=>," << cnt << endl;
		}
		totalMatch++;
		printf("Matches: %d\r", totalMatch);
	}

	outFile.flush();
	outFile.close();

	cout << "Match: " << totalMatch << " matches are written to " << outFileName << endl;
	cout << "Elapsed(ms)=" << since(start).count() << endl;
	return 0;
}
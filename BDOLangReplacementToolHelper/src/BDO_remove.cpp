#include <iostream>
#include <fstream>
#include <codecvt>
#include "BDO_remove.h"
#include "BDO_phrase.h"
#include "UTF16Facet.h"
#include "benchmark.h"
using namespace std;

void print_progress_remove(size_t current, size_t max)
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

int remove(char* srcFileName, char* outFileName, vector<char*> additionalFiles)
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
		printf("Reading removing phrases from: %s\r", additionalFiles[i]);
		if ((phrasesFound = readPhrasesFromFile(additionalFiles[i], phraseMap2, false)) == -1)
		{
			printf("Failed to read additional file: %s\n", additionalFiles[i]);
		}
		else
		{
			printf("Found %d phrases from %s\n", phrasesFound, additionalFiles[i]);
		}
	}

	for (auto pp : phraseMap2)
	{
		phraseMap.erase(pp.first);
	}

	int counter = 0;
	int total = phraseMap.size();

	wofstream outFile;
	locale utf16_locale(outFile.getloc(), new codecvt_utf16<wchar_t, MaxCode, UTF16_MODE>);
	outFile.imbue(utf16_locale);
	outFile.open(outFileName, ios::binary);
	wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> utf16Converter;

	if (outFile.fail()) {
		printf("File cannot be open: %s", outFileName);
		return -1;
	}

	for (auto itr = phraseMap.begin(); itr != phraseMap.end(); ++itr)
	{
		counter++;
		wstring phrase = itr->second.toString();
		outFile << phrase;
		print_progress_remove(counter, total);
	}

	outFile.flush();
	outFile.close();

	cout << "After Remove: " << counter << " phrases are written to " << outFileName << endl;
	cout << "Elapsed(ms)=" << since(start).count() << endl;
	return 0;
}
#include <iostream>
#include <fstream>
#include <codecvt>
#include "BDO_merge.h"
#include "BDO_phase.h"
#include "UTF16Facet.h"
#include "benchmark.h"
using namespace std;

void print_progress_merge(size_t current, size_t max)
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

int merge(char* srcFileName, char* outFileName, vector<char*> additionalFiles)
{
	map<PhaseId, Phase> phaseMap;

	// start benchmark
	auto start = chrono::steady_clock::now();
	int phasesFound = 0;

	printf("Reading source phases from: %s\r", srcFileName);
	if ((phasesFound = readPhasesFromFile(srcFileName, phaseMap, false)) == -1)
	{
		printf("File not found: %s", srcFileName);
		return -1;
	}
	else {
		printf("Found %d phases from %s\n", phasesFound, srcFileName);
	}

	for (int i = 0; i < additionalFiles.size(); i++)
	{
		printf("Reading replacement phases from: %s\r", additionalFiles[i]);
		if ((phasesFound = readPhasesFromFile(additionalFiles[i], phaseMap, true)) == -1)
		{
			printf("Failed to read additional file: %s\n", additionalFiles[i]);
		}
		else
		{
			printf("Found %d phases from %s\n", phasesFound, additionalFiles[i]);
		}
	}

	int counter = 0;
	int total = phaseMap.size();

	wofstream outFile;
	locale utf16_locale(outFile.getloc(), new codecvt_utf16<wchar_t, MaxCode, UTF16_MODE>);
	outFile.imbue(utf16_locale);
	outFile.open(outFileName, ios::binary);
	wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> utf16Converter;

	if (outFile.fail()) {
		printf("File cannot be open: %s", outFileName);
		return -1;
	}

	for (auto itr = phaseMap.begin(); itr != phaseMap.end(); ++itr)
	{
			counter++;
			wstring phase = itr->second.toString();
			outFile << phase;
			print_progress_merge(counter, total);
	}

	outFile.flush();
	outFile.close();

	cout << "Merged: " << counter << " phases are written to " << outFileName <<endl;
	cout << "Elapsed(ms)=" << since(start).count() << endl;
	return 0;
}
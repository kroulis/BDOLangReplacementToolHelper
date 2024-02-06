#include <iostream>
#include <fstream>
#include <codecvt>
#include "BDO_diff.h"
#include "BDO_phrase.h"
#include "UTF16Facet.h"
#include "benchmark.h"
using namespace std;

int diff(char* srcFileName, char* outFileName, vector<char*> additionalFiles)
{
	map<PhraseId, Phrase> phraseMap;

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
		printf("Reading juxtaposed phrases from: %s\r", additionalFiles[i]);
		if ((phrasesFound = readPhrasesFromFile(additionalFiles[i], phraseMap, true)) == -1)
		{
			printf("Failed to read additional file: %s\n", additionalFiles[i]);
		}
		else
		{
			printf("Found %d phrases from %s\n", phrasesFound, additionalFiles[i]);
		}
	}

	int counter = 0;

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
		if (!itr->second.isExist())
		{
			counter++;
			wstring phrase = itr->second.toString();
			outFile << phrase;
		}
	}

	outFile.flush();
	outFile.close();

	cout << "Diff: " << counter << " phrases are found" << endl;
	cout << "Elapsed(ms)=" << since(start).count() << endl;
	return 0;
}

int diff2(char* srcFileName, char* outFileName, vector<char*> additionalFiles)
{
	map<PhraseId, Phrase> phraseMap1;
	map<PhraseId, Phrase> phraseMap2;

	// start benchmark
	auto start = chrono::steady_clock::now();
	int phrasesFound = 0;

	printf("Reading source phrases from: %s\r", srcFileName);
	if ((phrasesFound = readPhrasesFromFile(srcFileName, phraseMap1, false)) == -1)
	{
		printf("File not found: %s", srcFileName);
		return -1;
	}
	else {
		printf("Found %d phrases from %s\n", phrasesFound, srcFileName);
	}

	for (size_t i = 0; i < additionalFiles.size(); i++)
	{
		printf("Reading juxtaposed phrases from: %s\r", additionalFiles[i]);
		if ((phrasesFound = readPhrasesFromFile(additionalFiles[i], phraseMap2, true)) == -1)
		{
			printf("Failed to read additional file: %s\n", additionalFiles[i]);
		}
		else
		{
			printf("Found %d phrases from %s\n", phrasesFound, additionalFiles[i]);
		}
	}

	int counter = 0;

	ofstream outFile(outFileName + to_string(counter / 30000) + ".html");

	//setup converter
	using convert_type = codecvt_utf8<wchar_t>;
	wstring_convert<convert_type, wchar_t> converter;

	if (outFile.fail()) {
		printf("File cannot be open: %s", outFileName);
		return -1;
	}

	outFile << "<link rel=\"stylesheet\" href=\"https://cdn.datatables.net/1.13.7/css/jquery.dataTables.css\" />" << endl <<
		"<script src = \"https://code.jquery.com/jquery-3.7.1.min.js\" integrity = \"sha256-/JqT3SQfawRcv/BIHPThkBvs0OEvtFFmqPF/lYI/Cxo=\""
		"crossorigin = \"anonymous\" > </script>" << endl <<
		"<script src = \"https://cdn.datatables.net/1.13.7/js/jquery.dataTables.js\"> </script>" << endl <<
		"<table id=\"myTable\" class=\"display\">" << endl<<
		"<thead><tr><th>strType</th><th>strId1</th><th>strId2</th><th>strId3</th><th>strId4</th><th>LOC1</th><th>LOC2</th></tr></thead>" << endl <<
		"<tbody>";

	for (auto itr = phraseMap1.begin(); itr != phraseMap1.end(); ++itr)
	{
		auto phrase2 = phraseMap2.find(itr->second.id);
		if (phrase2 == phraseMap2.end())
		{
			continue;
		}

		if (itr->second.localization.compare(phrase2->second.localization) != 0)
		{
			counter++;
			outFile << "<tr>" << itr->second.id.toHtml() << "<td>";
			string phrase = converter.to_bytes(itr->second.localization);
			outFile << phrase << "</td><td>";
			phrase = converter.to_bytes(phrase2->second.localization);
			outFile << phrase << "</td></tr>";

			if (counter % 30000 == 0)
			{
				outFile << "</tbody></table>" << endl << "<script> $(document).ready( function () {" << endl <<
					       "  $('#myTable').DataTable(); " << endl << " } ); </script>";
				outFile.flush();
				outFile.close();

				outFile = ofstream(outFileName + to_string(counter / 30000) + ".html");
				outFile << "<link rel=\"stylesheet\" href=\"https://cdn.datatables.net/1.13.7/css/jquery.dataTables.css\" />" << endl <<
					"<script src = \"https://code.jquery.com/jquery-3.7.1.min.js\" integrity = \"sha256-/JqT3SQfawRcv/BIHPThkBvs0OEvtFFmqPF/lYI/Cxo=\""
					"crossorigin = \"anonymous\" > </script>" << endl <<
					"<script src = \"https://cdn.datatables.net/1.13.7/js/jquery.dataTables.js\"> </script>" << endl <<
					"<table id=\"myTable\" class=\"display\">" << endl <<
					"<thead><tr><th>strType</th><th>strId1</th><th>strId2</th><th>strId3</th><th>strId4</th><th>LOC1</th><th>LOC2</th></tr></thead>" << endl <<
					"<tbody>";

			}
		}
		
	}

	outFile << "</tbody></table>" << endl << "<script> $(document).ready( function () {" << endl <<
		"  $('#myTable').DataTable(); " << endl << " } ); </script>";
	outFile.flush();
	outFile.close();

	cout << "Diff2: " << counter << " phrases are found" << endl;
	cout << "Elapsed(ms)=" << since(start).count() << endl;
	return 0;
}
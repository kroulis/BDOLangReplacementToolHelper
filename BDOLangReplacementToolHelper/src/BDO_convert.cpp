#include <iostream>
#include <cstdlib>
#include <fstream>
#include <codecvt>
#include "opencc/opencc.h"
#include "UTF16Facet.h"
#include "benchmark.h"
using namespace std;

#define MAX_LINE_LENGTH 8000


void print_progress_convert(size_t current, size_t max, size_t phases)
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
	printf("<%d> (%d/%d) [%s]\r", phases, current, max, status.c_str());
}

int convert_twcn2zhcn(char* srcFileName, char* outFileName)
{
	ifstream srcFile(srcFileName, ios::binary);
	if (srcFile.fail()) {
		printf("File not found: %s", srcFileName);
		return -1;
	}

	wofstream outFile;
	locale utf16_locale(outFile.getloc(), new codecvt_utf16<wchar_t, MaxCode, UTF16_MODE>);
	outFile.imbue(utf16_locale);
	outFile.open(outFileName, ios::binary);
	wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t> utf16Converter;

	if (outFile.fail()) {
		srcFile.close();
		printf("File cannot be open: %s", outFileName);
		return -1;
	}

	// start benchmark
	auto start = chrono::steady_clock::now();

	srcFile.seekg(0, ios::end);
	size_t size = (size_t)srcFile.tellg();

	//skip BOM
	srcFile.seekg(2, ios::beg);
	size -= 2;

	u16string u16((size / 2) + 1, '\0');
	srcFile.read((char*)&u16[0], size);
	srcFile.close();

	string utf8 = wstring_convert<codecvt_utf8_utf16<char16_t>, char16_t>{}.to_bytes(u16);

	opencc_t opencc = NULL;
	if (!opencc)
	{
		opencc = opencc_open("tw2sp.json");
		if ((int)opencc == 0xffffffff)
		{
			cout << "OpenCC error: " << opencc_error() << endl;
			return -1;
		}
	}

	int counter = 0;
	size_t inSize = utf8.size();

	size_t last = 0; size_t next = 0;
	while ((next = utf8.find('\n', last)) != string::npos || last < inSize)
	{
		if (utf8.find('\n', last) == string::npos)
		{
			next = inSize;
		}
		string input = utf8.substr(last, next - last -1);
		if (opencc)
		{
			input = opencc_convert_utf8(opencc, input.c_str(), input.size());
		}
		if (next != inSize)
		{
			input += "\r\n";
		}
		print_progress_convert(next, inSize, ++counter);
		outFile << u16ToWstring(utf16Converter.from_bytes(input));
		if (counter % 10000 == 0)
		{
			outFile.flush();
		}
		last = next + 1;
	}

	if (opencc)
	{
		opencc_close(opencc);
		opencc = NULL;
	}

	outFile.flush();
	outFile.close();
	printf("\nSuccessfully converted %d phases to simplified chinese\n", counter);
	std::cout << "Elapsed(ms)=" << since(start).count() << std::endl;
	return 0;
}
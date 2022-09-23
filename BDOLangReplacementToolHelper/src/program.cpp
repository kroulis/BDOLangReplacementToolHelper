#include <iostream>
#include <cstring>
#include <vector>
#include "BDO_decrypt.h"
#include "BDO_encrypt.h"
#include "BDO_convert.h"
#include "BDO_diff.h"
#include "BDO_merge.h"
#include "version.h"
using namespace std;

void licence()
{
	cout << "LICENCE FOR THIS TOOL:" << endl;
	cout << "MIT License\n\n"
			"Copyright(c) 2022 kroulis\n\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy\n"
			"of this softwareand associated documentation files(the \"Software\"), to deal\n"
			"in the Software without restriction, including without limitation the rights\n"
			"to use, copy, modify, merge, publish, distribute, sublicense, and /or sell\n"
			"copies of the Software, and to permit persons to whom the Software is\n"
			"furnished to do so, subject to the following conditions :\n\n"
			"The above copyright noticeand this permission notice shall be included in all\n"
			"copies or substantial portions of the Software.\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
			"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
			"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\n"
			"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
			"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
			"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
			"SOFTWARE." << endl;
	cout << "=========================================================================" << endl;
	cout << "END OF LICENCE" << endl << endl;
	cout << "Open Source Library LICENCES:" << endl;
	cout << "---------------------------------------------------------------------------" << endl;
	cout << "Library: Bevrc/BDOcrypt" << endl;
	cout << "MIT License\n\n"
			"Copyright(c) 2017 Garkin\n"
			"Permission is hereby granted, free of charge, to any person obtaining a copy\n"
			"of this softwareand associated documentation files(the \"Software\"), to deal\n"
			"in the Software without restriction, including without limitation the rights\n"
			"to use, copy, modify, merge, publish, distribute, sublicense, and /or sell\n"
			"copies of the Software, and to permit persons to whom the Software is\n"
			"furnished to do so, subject to the following conditions :\n\n"
			"The above copyright noticeand this permission notice shall be included in all\n"
			"copies or substantial portions of the Software.\n\n"
			"THE SOFTWARE IS PROVIDED \"AS IS\", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR\n"
			"IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,\n"
			"FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE\n"
			"AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER\n"
			"LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,\n"
			"OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE\n"
			"SOFTWARE."<< endl;
	cout << "=========================================================================" << endl;
	cout << "Library: zlib" << endl;
	cout << "zlib.h -- interface of the 'zlib' general purpose compression library\n"
			"version 1.2.12, March 27th, 2022\n\n"
			"Copyright(C) 1995 - 2022 Jean - loup Gailly and Mark Adler\n\n"
			"This software is provided 'as-is', without any express or implied\n"
			"warranty.In no event will the authors be held liable for any damages\n"
			"arising from the use of this software.\n\n"
			"Permission is granted to anyone to use this software for any purpose,\n"
			"including commercial applications, and to alter itand redistribute it\n"
			"freely, subject to the following restrictions :\n\n"
			"1. The origin of this software must not be misrepresented; you must not\n"
			"claim that you wrote the original software.If you use this software\n"
			"in a product, an acknowledgment in the product documentation would be\n"
			"appreciated but is not required.\n"
			"2. Altered source versions must be plainly marked as such, and must not be\n"
			"misrepresented as being the original software.\n"
			"3. This notice may not be removed or altered from any source distribution.\n\n"
			"Jean - loup Gailly        Mark Adler\n"
			"jloup@gzip.org          madler@alumni.caltech.edu" << endl;
	cout << "=========================================================================" << endl;
	cout << "Library: BYVoid / OpenCC" << endl;
	cout << "Apache License\n"
			"Version 2.0, January 2004\n"
			"http://www.apache.org/licenses/\n\n"
			"TERMS AND CONDITIONS FOR USE, REPRODUCTION, AND DISTRIBUTION\n"
			"1. Definitions.\n"
			"\"License\" shall mean the terms and conditions for use, reproduction, and distribution as defined by Sections 1 through 9 of this document.\n"
			"\"Licensor\" shall mean the copyright owner or entity authorized by the copyright owner that is granting the License.\n"
			"\"Legal Entity\" shall mean the union of the acting entity and all other entities that control, are controlled by, or are under common control with that entity.For the purposes of this definition, \"control\" means(i) the power, direct or indirect, to cause the direction or management of such entity, whether by contract or otherwise, or (ii)ownership of fifty percent(50 %) or more of the outstanding shares, or (iii)beneficial ownership of such entity.\n"
			"\"You\" (or \"Your\") shall mean an individual or Legal Entity exercising permissions granted by this License.\n"
			"\"Source\" form shall mean the preferred form for making modifications, including but not limited to software source code, documentation source, and configuration files.\n"
			"\"Object\" form shall mean any form resulting from mechanical transformation or translation of a Source form, including but not limited to compiled object code, generated documentation, and conversions to other media types.\n"
			"\"Work\" shall mean the work of authorship, whether in Source or Object form, made available under the License, as indicated by a copyright notice that is included in or attached to the work(an example is provided in the Appendix below).\n"
			"\"Derivative Works\" shall mean any work, whether in Source or Object form, that is based on(or derived from) the Workand for which the editorial revisions, annotations, elaborations, or other modifications represent, as a whole, an original work of authorship.For the purposes of this License, Derivative Works shall not include works that remain separable from, or merely link(or bind by name) to the interfaces of, the Workand Derivative Works thereof.\n"
			"\"Contribution\" shall mean any work of authorship, including the original version of the Workand any modifications or additions to that Work or Derivative Works thereof, that is intentionally submitted to Licensor for inclusion in the Work by the copyright owner or by an individual or Legal Entity authorized to submit on behalf of the copyright owner.For the purposes of this definition, \"submitted\" means any form of electronic, verbal, or written communication sent to the Licensor or its representatives, including but not limited to communication on electronic mailing lists, source code control systems, and issue tracking systems that are managed by, or on behalf of, the Licensor for the purpose of discussingand improving the Work, but excluding communication that is conspicuously marked or otherwise designated in writing by the copyright owner as \"Not a Contribution.\"\n"
			"\"Contributor\" shall mean Licensor and any individual or Legal Entity on behalf of whom a Contribution has been received by Licensor and subsequently incorporated within the Work.\n"
			"2. Grant of Copyright License.Subject to the terms and conditions of this License, each Contributor hereby grants to You a perpetual, worldwide, non - exclusive, no - charge, royalty - free, irrevocable copyright license to reproduce, prepare Derivative Works of, publicly display, publicly perform, sublicense, and distribute the Workand such Derivative Works in Source or Object form.\n"
			"3. Grant of Patent License.Subject to the terms and conditions of this License, each Contributor hereby grants to You a perpetual, worldwide, non - exclusive, no - charge, royalty - free, irrevocable(except as stated in this section) patent license to make, have made, use, offer to sell, sell, import, and otherwise transfer the Work, where such license applies only to those patent claims licensable by such Contributor that are necessarily infringed by their Contribution(s) alone or by combination of their Contribution(s) with the Work to which such Contribution(s) was submitted.If You institute patent litigation against any entity(including a cross - claim or counterclaim in a lawsuit) alleging that the Work or a Contribution incorporated within the Work constitutes direct or contributory patent infringement, then any patent licenses granted to You under this License for that Work shall terminate as of the date such litigation is filed.\n"
			"4. Redistribution.You may reproduce and distribute copies of the Work or Derivative Works thereof in any medium, with or without modifications, and in Source or Object form, provided that You meet the following conditions :\n"
			"	1. You must give any other recipients of the Work or Derivative Works a copy of this License; and\n"
			"	2. You must cause any modified files to carry prominent notices stating that You changed the files; and\n"
			"	3. You must retain, in the Source form of any Derivative Works that You distribute, all copyright, patent, trademark, and attribution notices from the Source form of the Work, excluding those notices that do not pertain to any part of the Derivative Works; and\n"
			"	4. If the Work includes a \"NOTICE\" text file as part of its distribution, then any Derivative Works that You distribute must include a readable copy of the attribution notices contained within such NOTICE file, excluding those notices that do not pertain to any part of the Derivative Works, in at least one of the following places : within a NOTICE text file distributed as part of the Derivative Works; within the Source form or documentation, if provided along with the Derivative Works; or , within a display generated by the Derivative Works, ifand wherever such third - party notices normally appear.The contents of the NOTICE file are for informational purposes onlyand do not modify the License.You may add Your own attribution notices within Derivative Works that You distribute, alongside or as an addendum to the NOTICE text from the Work, provided that such additional attribution notices cannot be construed as modifying the License.\n"
			"You may add Your own copyright statement to Your modificationsand may provide additional or different license terms and conditions for use, reproduction, or distribution of Your modifications, or for any such Derivative Works as a whole, provided Your use, reproduction, and distribution of the Work otherwise complies with the conditions stated in this License.\n"
			"5. Submission of Contributions.Unless You explicitly state otherwise, any Contribution intentionally submitted for inclusion in the Work by You to the Licensor shall be under the termsand conditions of this License, without any additional terms or conditions.Notwithstanding the above, nothing herein shall supersede or modify the terms of any separate license agreement you may have executed with Licensor regarding such Contributions.\n"
			"6. Trademarks.This License does not grant permission to use the trade names, trademarks, service marks, or product names of the Licensor, except as required for reasonableand customary use in describing the origin of the Workand reproducing the content of the NOTICE file.\n"
			"7. Disclaimer of Warranty.Unless required by applicable law or agreed to in writing, Licensor provides the Work(and each Contributor provides its Contributions) on an \"AS IS\" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied, including, without limitation, any warranties or conditions of TITLE, NON - INFRINGEMENT, MERCHANTABILITY, or FITNESS FOR A PARTICULAR PURPOSE.You are solely responsible for determining the appropriateness of using or redistributing the Work and assume any risks associated with Your exercise of permissions under this License.\n"
			"8. Limitation of Liability.In no event and under no legal theory, whether in tort(including negligence), contract, or otherwise, unless required by applicable law(such as deliberateand grossly negligent acts) or agreed to in writing, shall any Contributor be liable to You for damages, including any direct, indirect, special, incidental, or consequential damages of any character arising as a result of this License or out of the use or inability to use the Work(including but not limited to damages for loss of goodwill, work stoppage, computer failure or malfunction, or any and all other commercial damages or losses), even if such Contributor has been advised of the possibility of such damages.\n"
			"9. Accepting Warranty or Additional Liability.While redistributing the Work or Derivative Works thereof, You may choose to offer, and charge a fee for, acceptance of support, warranty, indemnity, or other liability obligations and /or rights consistent with this License.However, in accepting such obligations, You may act only on Your own behalfand on Your sole responsibility, not on behalf of any other Contributor, and only if You agree to indemnify, defend, and hold each Contributor harmless for any liability incurred by, or claims asserted against, such Contributor by reason of your accepting any such warranty or additional liability.\n"
			"END OF TERMS AND CONDITIONS" << endl;
	cout << "=========================================================================" << endl;
	cout << "END OF LIBRARIES" << endl;
}

void usage()
{
	cout << "BDOLangReplacementToolHelper Version: " << BDOLANGREPLTOOLHELPERVER << endl;
	cout << "Usage:" << endl;
	cout << "BDOLangReplacementToolHelper [-e|-d|-f|-m|-zh_cn|-lic] <srcFilePath> <outputFilePath> [additionalFiles]" << endl;
	cout << "Parameters:" << endl;
	cout << "-e     : Encrypt plaintext file to localization file" << endl;
	cout << "-d     : Decrypt localization file to plaintext file" << endl;
	cout << "-f     : Calculate difference between srcFile and additionalFiles. Output the extra phases in srcFile to outputFilePath" << endl;
	cout << "-m     : Merge multiple plaintext file together. The later in the additionalFile list, the higher the prioriry for replac translate localization files for Black Desert Online ement." << endl;
	cout << "-zh_cn : Convert TW_CN plain text to ZH_CN plain text" << endl;
	cout << "-lic   : List open source third party libraries' licences" << endl;
}

int main(int argc, char** argv)
{
	if (argc >= 4)
	{
		if (strcmp(argv[1], "-e") == 0)
		{
			return encrypt(argv[2], argv[3]);
		}
		else if (strcmp(argv[1], "-d") == 0)
		{
			return decrypt(argv[2], argv[3]);
		}
		else if (strcmp(argv[1], "-zh_cn") == 0)
		{
			return convert_twcn2zhcn(argv[2], argv[3]);
		}
		else if (strcmp(argv[1], "-f") == 0)
		{
			vector<char*> additionalFiles;
			for (int i = 4; i < argc; i++)
			{
				additionalFiles.push_back(argv[i]);
			}
			return diff(argv[2], argv[3], additionalFiles);
		}
		else if (strcmp(argv[1], "-m") == 0)
		{
			vector<char*> additionalFiles;
			for (int i = 4; i < argc; i++)
			{
				additionalFiles.push_back(argv[i]);
			}
			return merge(argv[2], argv[3], additionalFiles);
		}
		usage();
		return 1;
	}
	else if (argc == 2)
	{
		if (strcmp(argv[1], "-lic") == 0)
		{
			usage();
			licence();
			return 0;
		}
	}

	usage();
	return 1;
}
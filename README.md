# BDOLangReplacementToolHelper [![Build](https://github.com/kroulis/BDOLangReplacementToolHelper/actions/workflows/msbuild.yml/badge.svg)](https://github.com/kroulis/BDOLangReplacementToolHelper/actions/workflows/msbuild.yml)

Utilities can be used to decrypt / encrypt / merge / diff / translate tw_cn to zh_ch localization files for Black Desert Online  
This tool is supposed to be used along with BDOLanguageReplacementTool to achieve ease language file replacement. Check [kroulis/BDOLanguageReplacementTool](https://github.com/kroulis/BDOLanguageReplacementTool) for more information.

`BDOLangReplacementToolHelper [-e|-d|-f|-m|-r|-rm|-rn|-zh_cn|-lic] <srcFilePath> <outputFilePath> [additionalFiles]`

Parameters:
```
-d     : Decrypt localization file to plaintext file
-e     : Encrypt plaintext file to localization file
-f     : Calculate difference between srcFile and additionalFiles. Output the extra phrases in srcFile to outputFilePath
-m     : Merge multiple plaintext file together. The later in the additionalFile list, the higher the prioriry for replacement
-r     : Replace phrases from one localization file (srcFile) to another localization file (in additionalFiles) specified by a config csv file (in additionalFiles)
-rm    : Remove phrases in additional files from the source file
-rn    : Rename some phrases with another id specified by a config csv file (in additionalFiles)
-zh_cn : Convert TW_CN plain text to ZH_CN plain text
-lic   : List open source third party libraries' licences
```

Output file will be overwritten without asking.

Example of use:

- Decrypt localization file to plaintext file:

```
    BDOLangReplacementToolHelper.exe -d ads\languagedata_en.loc languagedata_en.txt
```
   
- Encrypt plaintext file to localization file:

```
    BDOLangReplacementToolHelper.exe -e languagedata_en.txt ads\languagedata_en.loc
```

- Diff the source plaintext file \(languagedata_en.txt\) with other files \(languagedata_tw.txt, languagedata_tw_extra.txt\), then output the differences to output file \(languagedata_en_diff.txt\)

```
    BDOLangReplacementToolHelper.exe -f languagedata_en.txt languagedata_en_diff.txt languagedata_tw.txt languagedata_tw_extra.txt
```

- Merge the source file \(languagedata_en.txt\) with other language files \(languagedata_tw.txt, languagedata_en_diff_tw.txt\), then output all phases to \(languagedata_tw_merge.txt\)

```
    BDOLangReplacementToolHelper.exe -m languagedata_en.txt languagedata_tw_merge.txt languagedata_tw.txt languagedata_en_diff_tw.txt
```

- Convert TW_CN language file \(languagedata_tw.txt\) to ZH_CN language file \(languagedata_zh_cn.txt\)

```
    BDOLangReplacementToolHelper.exe -zh_cn languagedata_tw.txt languagedata_zh_cn.txt
```

- Replace some of the source language file's phrases by a set rule with another language file. The set rule file is ending with .csv, and has the following format:

	```
	FSTRTYPE,FSTRID1,FSTRID2,FSTRID3,FSTRID4,_IGNORE_,TSTRTYPE,TSTRID1,TSTRID2,TSTRID3,TSTRID4,_IGNORE_,ENDING_STRID4
	```

	+ This replace phrases `<FSTRTYPE,FSTRID1,FSTRID2,FSTRID3,(FSTRID4 to ENDING_STRID4>`'s localization with `<TSTRTYPE,TSTRID1,TSTRID2,TSTRID3,(TSTRID4 to ENDING_STRID4)>`'s localization in additional files.
	
	+ For example, let's assume original language is in English, and replacement language is in Chinese:
		```
		50 123 0 12 0 "Cron Stone Bundle (100x)"`
		50 123 0 12 1 "Event"
		50 123 0 12 3 "Detailed Items: 100x Cron Stones"
		```
		```
		50 456 0 14 0 "100个克罗恩石包"`
		50 456 0 14 1 "活动"
		50 456 0 14 3 "包含物品: 100个克罗恩石"
		```
		We want to replace the `50 123 0 12 0->3` to `50 456 0 14 0->3`, by having a replacing config in the set rule file:
		
		```50,123,0,12,0,=>,50,456,0,14,0,3```
		
		You will generate a localization file with:
		```
		50 123 0 12 0 "100个克罗恩石包"`
		50 123 0 12 1 "活动"
		50 123 0 12 3 "包含物品: 100个克罗恩石"
		```
		
		To invoke this function, use:
```
BDOLangReplacementToolHelper.exe -r languagedata_en.txt languagedata_en_replace.txt languagedata_tw.txt en2tw.csv
```

- Remove phrases in additional files from the source file. This will remove all matching phrases from the source file with the same id.
```
BDOLangReplacementToolHelper.exe -rm languagedata_en.txt languagedata_en_rm.txt languagedata_en2rm.txt
```

- Rename some phrases with another id specified by a set rule csv file (in additionalFiles)

	Again we have the following set rule for the csv file:
	```
	FSTRTYPE,FSTRID1,FSTRID2,FSTRID3,FSTRID4,_IGNORE_,TSTRTYPE,TSTRID1,TSTRID2,TSTRID3,TSTRID4,_IGNORE_
	```
	+ You can use `*` to wild cast phrases (that has the F prefix id above) you want to change. Any place you used wild cast, you have to set `0` in the phrase id with T prefix at the same location.
	For example:
	`50,123,0,12,*,=>50,123,0,14,0` means rename any phrases with `<50, 123, 0, 12, (any id)>` to `<50, 123, 0, 14, <corresponding id>`.
	Using the replace example, set rule:
	`50,123,0,12,*,=>50,123,0,14,0` will generate the following phrases:
		```
		50 123 0 14 0 "Cron Stone Bundle (100x)"`
		50 123 0 14 1 "Event"
		50 123 0 14 3 "Detailed Items: 100x Cron Stones"
		```
	
	+ To invoke this function, please use:
```
BDOLangReplacementToolHelper.exe -rn languagedata_en.txt languagedata_en_rn.txt en2rn.csv
```
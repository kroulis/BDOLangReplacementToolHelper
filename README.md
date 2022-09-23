# BDOLangReplacementToolHelper [![Build](https://github.com/kroulis/BDOLangReplacementToolHelper/actions/workflows/msbuild.yml/badge.svg)](https://github.com/kroulis/BDOLangReplacementToolHelper/actions/workflows/msbuild.yml)

Utilities can be used to decrypt / encrypt / merge / diff / translate tw_cn to zh_ch localization files for Black Desert Online  
This tool is supposed to be used along with BDOLanguageReplacementTool to achieve ease language file replacement. Check [kroulis/BDOLanguageReplacementTool](https://github.com/kroulis/BDOLanguageReplacementTool) for more information.

`BDOLangReplacementToolHelper [-e|-d|-f|-m|-zh_cn|-lic] <srcFilePath> <outputFilePath> [additionalFiles]`

Parameters:
```
-d     : Decrypt localization file to plaintext file
-e     : Encrypt plaintext file to localization file
-f     : Calculate difference between srcFile and additionalFiles. Output the extra phases in srcFile to outputFilePath
-m     : Merge multiple plaintext file together. The later in the additionalFile list, the higher the prioriry for replacement
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

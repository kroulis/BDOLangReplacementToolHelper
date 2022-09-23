#include "UTF16Facet.h"

wstring Wdecode(const string& str, int codePage)
{
    if (str.empty()) return wstring();
    int size_needed = MultiByteToWideChar(codePage, 0, &str[0], (int)str.size(), NULL, 0);
    wstring wstrTo(size_needed, 0);
    MultiByteToWideChar(codePage, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
    return wstrTo;
}

wstring u16ToWstring(const std::u16string& str)
{
    wstring_convert< std::codecvt_utf16<wchar_t, 0x10ffff, std::little_endian>,
        wchar_t> conv;

    wstring wstr = conv.from_bytes(
        reinterpret_cast<const char*> (&str[0]),
        reinterpret_cast<const char*> (&str[0] + str.size())
    );

    return(wstr);
}
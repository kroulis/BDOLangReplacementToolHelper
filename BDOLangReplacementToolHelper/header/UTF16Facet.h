#include <iostream>
#include <locale>
#include <codecvt>
#include <Windows.h>
using namespace std;
#pragma once

const wchar_t CHAR_NULL = 0x0000;
const wchar_t CHAR_CR = 0x000D;
const wchar_t CHAR_LF = 0x000A;
const wchar_t BOM_UTF16LE = 0xFEFF;
#define MAX_BUFF_SIZE 16384
const unsigned long MaxCode = 0x10ffffUL;
#define UTF16_MODE (codecvt_mode)(generate_header | little_endian)

class UTF16Facet : public std::codecvt<wchar_t, char, std::char_traits<wchar_t>::state_type>
{
    typedef std::codecvt<wchar_t, char, std::char_traits<wchar_t>::state_type> MyType;
    typedef MyType::state_type          state_type;
    typedef MyType::result              result;


    /* This function deals with converting data from the input stream into the internal stream.*/
    /*
     * from, from_end:  Points to the beginning and end of the input that we are converting 'from'.
     * to,   to_limit:  Points to where we are writing the conversion 'to'
     * from_next:       When the function exits this should have been updated to point at the next location
     *                  to read from. (ie the first unconverted input character)
     * to_next:         When the function exits this should have been updated to point at the next location
     *                  to write to.
     *
     * status:          This indicates the status of the conversion.
     *                  possible values are:
     *                  error:      An error occurred the bad file bit will be set.
     *                  ok:         Everything went to plan
     *                  partial:    Not enough input data was supplied to complete any conversion.
     *                  nonconv:    no conversion was done.
     */
    virtual result  do_in(state_type& s,
        const char* from, const char* from_end, const char*& from_next,
        wchar_t* to, wchar_t* to_limit, wchar_t*& to_next) const
    {
        for (; from < from_end; from += 2, ++to)
        {
            if (to <= to_limit) {
                (*to) = L'\0';

                reinterpret_cast<char*>(to)[0] = from[0];
                reinterpret_cast<char*>(to)[1] = from[1];

                from_next = from;
                to_next = to;
            }
        }
        return((to != to_limit) ? partial : ok);
    }



    /* This function deals with converting data from the internal stream to a C/C++ file stream.*/
    /*
     * from, from_end:  Points to the beginning and end of the input that we are converting 'from'.
     * to,   to_limit:  Points to where we are writing the conversion 'to'
     * from_next:       When the function exits this should have been updated to point at the next location
     *                  to read from. (ie the first unconverted input character)
     * to_next:         When the function exits this should have been updated to point at the next location
     *                  to write to.
     *
     * status:          This indicates the status of the conversion.
     *                  possible values are:
     *                  error:      An error occurred the bad file bit will be set.
     *                  ok:         Everything went to plan
     *                  partial:    Not enough input data was supplied to complete any conversion.
     *                  nonconv:    no conversion was done.
     */
    virtual result do_out(state_type& state,
        const wchar_t* from, const wchar_t* from_end, const wchar_t*& from_next,
        char* to, char* to_limit, char*& to_next) const
    {
        for (; (from < from_end); ++from, to += 2)
        {
            if (to <= to_limit) {
                to[0] = reinterpret_cast<const char*>(from)[0];
                to[1] = reinterpret_cast<const char*>(from)[1];
                from_next = from;
                to_next = to;
            }
        }
        return((to != to_limit) ? partial : ok);
    }
};

wstring Wdecode(const string& str, int codePage = GetACP());

wstring u16ToWstring(const std::u16string& str);
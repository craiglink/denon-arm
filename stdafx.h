#ifndef _STDAFX_H
#define _STDAFX_H


#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

#define _STLP_USE_MALLOC
#define _STLP_USE_NO_IOSTREAMS
#define _STLP_SHORT_STRING_SZ 30
#include <string>

class TryParse
{
public:
    static bool Int( const char* str, int& outval )
    {
        outval = atoi(str);
        return isdigit(*str);
    }
    static bool Int( const std::string& str, int& outval ) 
    { return Int( str.c_str(), outval ); }
        
       
};

extern void Sleep (unsigned long time);
extern void log(const char*);

#endif
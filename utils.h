#ifndef _UTILS_H
#define _UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
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

#include "stdafx.h"
#include "avrcmd.h"


const AvrCmd::CMD AvrCmd::cmd_array[] =
{
    { AvrCmd::CMD_PW, "PW" },
    { AvrCmd::CMD_ZM, "ZM" },
    { AvrCmd::CMD_Z2, "Z2" },
    { AvrCmd::CMD_Z3, "Z3" },
    { AvrCmd::CMD_SR, "SR" },
    { AvrCmd::CMD_SI, "SI" },
    { AvrCmd::CMD_SD, "SD" },
    { AvrCmd::CMD_DC, "DC" },
    { AvrCmd::CMD_MV, "MV" },
    { AvrCmd::CMD_CV, "CV" },
    { AvrCmd::CMD_HD, "HD" },
    { AvrCmd::CMD_PS, "PS" },
    { AvrCmd::CMD_TF, "TF" },
    { AvrCmd::CMD_TP, "TP" },
    { AvrCmd::CMD_TM, "TM" }
};

const AvrCmd::PARAM AvrCmd::param_array[] =
{
    { AvrCmd::PARAM_VOL_00, "00" },
    { 1, "01" },
    { 2, "02" },
    { 3, "03" },
    { 4, "04" },
    { 5, "05" },
    { 6, "06" },
    { 7, "07" },
    { 8, "08" },
    { 9, "09" },

    { 10, "10" },
    { 11, "11" },
    { 12, "12" },
    { 13, "13" },
    { 14, "14" },
    { 15, "15" },
    { 16, "16" },
    { 17, "17" },
    { 18, "18" },
    { 19, "19" },

    { 20, "20" },
    { 21, "21" },
    { 22, "22" },
    { 23, "23" },
    { 24, "24" },
    { 25, "25" },
    { 26, "26" },
    { 27, "27" },
    { 28, "28" },
    { 29, "29" },

    { 30, "30" },
    { 31, "31" },
    { 32, "32" },
    { 33, "33" },
    { 34, "34" },
    { 35, "35" },
    { 36, "36" },
    { 37, "37" },
    { 38, "38" },
    { 39, "39" },

    { 40, "40" },
    { 41, "41" },
    { 42, "42" },
    { 43, "43" },
    { 44, "44" },
    { 45, "45" },
    { 46, "46" },
    { 47, "47" },
    { 48, "48" },
    { 49, "49" },

    { 50, "50" },
    { 51, "51" },
    { 52, "52" },
    { 53, "53" },
    { 54, "54" },
    { 55, "55" },
    { 56, "56" },
    { 57, "57" },
    { 58, "58" },
    { 59, "59" },

    { 60, "60" },
    { 61, "61" },
    { 62, "62" },
    { 63, "63" },
    { 64, "64" },
    { 65, "65" },
    { 66, "66" },
    { 67, "67" },
    { 68, "68" },
    { 69, "69" },

    { 70, "70" },
    { 71, "71" },
    { 72, "72" },
    { 73, "73" },
    { 74, "74" },
    { 75, "75" },
    { 76, "76" },
    { 77, "77" },
    { 78, "78" },
    { 79, "79" },

    { 80, "80" },
    { 81, "81" },
    { 82, "82" },
    { 83, "83" },
    { 84, "84" },
    { 85, "85" },
    { 86, "86" },
    { 87, "87" },
    { 88, "88" },
    { 89, "89" },

    { 90, "90" },
    { 91, "91" },
    { 92, "92" },
    { 93, "93" },
    { 94, "94" },
    { 95, "95" },
    { 96, "96" },
    { 97, "97" },
    { 98, "98" },
    { AvrCmd::PARAM_VOL_99, "99" },

    { AvrCmd::PARAM_OFF, "OFF" },
    { AvrCmd::PARAM_ON, "ON" },
    { AvrCmd::PARAM_QUERY, "?" },
    { AvrCmd::PARAM_STANDBY, "STANDBY" },
    { AvrCmd::PARAM_AUTO, "AUTO" },
    { AvrCmd::PARAM_SOURCE, "SOURCE" },
    { AvrCmd::PARAM_PHONO, "PHONE" },
    { AvrCmd::PARAM_CD, "CD" },
    { AvrCmd::PARAM_TUNER, "TUNER" },
    { AvrCmd::PARAM_DVD, "DVD" },
    { AvrCmd::PARAM_BD, "BD" },
    { AvrCmd::PARAM_TV, "TV" },
    { AvrCmd::PARAM_TV_CBL, "TV/CBL" },
    { AvrCmd::PARAM_SAT_CBL, "SAT/CBL" },
    { AvrCmd::PARAM_DVR, "DVR" },
    { AvrCmd::PARAM_GAME, "GAME" },
    { AvrCmd::PARAM_V_AUX, "V.AUX" },
    { AvrCmd::PARAM_DOCK, "DOCK" },
    { AvrCmd::PARAM_SIRIUS, "SIRIUS" },
    { AvrCmd::PARAM_HDRADIO, "HDRADIO" },
    { AvrCmd::PARAM_IPOD, "IPOD" },
    { AvrCmd::PARAM_NET_USB,  "NET/USB" },
    { AvrCmd::PARAM_RHAPSODY, "RHAPSODY" },
    { AvrCmd::PARAM_NAPSTER, "NAPSTER" },
    { AvrCmd::PARAM_LASTFM, "LASTFM" },
    { AvrCmd::PARAM_FLICKR, "FLICKR" },
    { AvrCmd::PARAM_FAVORITES, "FAVORITES" },
    { AvrCmd::PARAM_IRADIO, "IRADIO" },
    { AvrCmd::PARAM_SERVER, "SERVER" },
    { AvrCmd::PARAM_USB_IPOD, "USB/IPOD" }
};


unsigned char AvrCmd::toCmd( const char* str )
{
    if ( *str )
    {
        char c0 = toupper(str[0]);
        char c1 = toupper(str[1]);

        for ( int ndx = 0; ndx < sizeof(cmd_array)/sizeof(cmd_array[0]);ndx++ )
        {
            if ( c0 == cmd_array[ndx].str[0] && c1 == cmd_array[ndx].str[1] )
            {
                return cmd_array[ndx].id;
            }
        }
    }
    return CMD_INVALID;
}

unsigned char AvrCmd::toParam( const char* str )
{
    if ( *str )
    {
        if ( isdigit(*str) )
        {
            int vol = atoi(str);
            if ( vol < 100 )
            {
                return vol;
            }
            else
            {
                return vol / 10;
            }
        }
        else
        {
            char upper[30];
            if ( strlen(str) < sizeof(upper)/sizeof(upper[0]) )
            {
                char* ptr = upper;
                while(*str)
                {
                    *ptr++ = toupper(*str++);
                }
                *ptr = 0;
    
                for ( int ndx = PARAM_VOL_99+1; ndx < sizeof(param_array)/sizeof(param_array[0]);ndx++ )
                {
                    if ( strcmp( upper, param_array[ndx].str) == 0 )
                    {
                        return param_array[ndx].id;
                    }
                }
            }
        }
    }
    return PARAM_INVALID;
}


bool AvrCmd::write()
{
    if ( cmd != CMD_INVALID && param != PARAM_INVALID )
    {
        port->Write(cmd_array[cmd].str);
        port->Write(param_array[param].str);
        port->Write("\r");
        if (param == PARAM_ON)
        {
            Sleep(1000);
        }
        return true;
    }
    return false;
}

void AvrCmd::log()
{
    if (port != NULL)
    {
//            ::log(std::string("-> ") + port->PortName() + ": " + cmd + param + "\r\n");
    }
    else
    {
//            ::log(cmd + param + "\r\n");
    }
}

#ifndef _AVRCMD_H
#define _AVRCMD_H

#include "serialport.h"


class AvrCmd
{
	public:


    enum
    {
        CMD_INVALID = -1,
        CMD_PW,
        CMD_ZM,
        CMD_Z2,
        CMD_Z3,
        CMD_SR,
        CMD_SI,
        CMD_SD,
        CMD_DC,
        CMD_MV,
        CMD_CV,
        CMD_HD,
        CMD_PS,
        CMD_TF,
        CMD_TP,
        CMD_TM
    };
    enum
    {
        PARAM_INVALID = -1,
        PARAM_VOL_00 = 0,
        PARAM_VOL_99 = 99,
        PARAM_OFF,
        PARAM_ON,
        PARAM_QUERY, // ?
        PARAM_STANDBY,
        PARAM_AUTO,
        PARAM_SOURCE,
        PARAM_PHONO,
        PARAM_CD,
        PARAM_TUNER,
        PARAM_DVD,
        PARAM_BD,
        PARAM_TV,
        PARAM_TV_CBL,
        PARAM_SAT_CBL,
        PARAM_DVR,
        PARAM_GAME,
        PARAM_V_AUX,
        PARAM_DOCK,
        PARAM_SIRIUS,
        PARAM_HDRADIO,
        PARAM_IPOD,
        PARAM_NET_USB, 
        PARAM_RHAPSODY,
        PARAM_NAPSTER,
        PARAM_LASTFM,
        PARAM_FLICKR,
        PARAM_FAVORITES,
        PARAM_IRADIO,
        PARAM_SERVER,
        PARAM_USB_IPOD
        };

    SerialPort* port;
    unsigned char cmd;
    unsigned char param;

protected:
    struct CMD
    {
        unsigned char id;
        const char* const str;
    };

    static const CMD cmd_array[];

    static unsigned char toCmd( const char* str );

    struct PARAM
    {
        unsigned char id;
        const char* const str;
    };

    static const PARAM param_array[];
    static unsigned char toParam( const char* str );


public:

    AvrCmd(SerialPort& sp, unsigned char c, unsigned char p) 
        : port(&sp), cmd(c), param(p)
    {
    }

    AvrCmd(SerialPort& sp, const char* c, const char* p) 
    {
        port = &sp;
        cmd = toCmd(c);
        param = toParam(p);
    }

    AvrCmd(const char* message)
    {
        port = NULL;
        cmd = toCmd(message);
        if ( message[0] && message[1] )
        {
            param = toParam(message+2);
        }
        else
        {
            param = PARAM_INVALID;
        }
    }

	~AvrCmd()
	{
		port = NULL;
			
	}
    bool write();

    void log();


};

#endif
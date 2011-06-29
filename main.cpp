#include "stdafx.h"

#ifdef __cplusplus
extern "C" {
#endif

#include <board.h>
#include <pmc/pmc.h>
#include <pit/pit.h>
//#include <pit/pit.h>
//#include <pio/pio.h>
//#include <irq/irq.h>
//#include <dbgu/dbgu.h>
#include <utility/led.h>


void __putchar(int ch){}
void abort(void)
{
  while (1);
}



  
#ifdef __cplusplus
}
#endif

#define SLEEP_TIME 6*60*60 // seconds

#include "serialport.h"
#include "avrcmd.h"

#include <queue>

#define QUEUE_PUSH(avr) queue.push(avr)
//#define QUEUE_PUSH(avr) avr.write()

void Sleep (unsigned long time) {            /* Wait Function */
  volatile unsigned long tick;
  tick = time * 1000;
  while (--tick);          /* Wait for specified Time */
}
void log(const char*){}

int main (void) {

  PMC_EnablePeripheral(AT91C_ID_PIOA);

  LED_Configure(0);
  LED_Configure(1);

//    #define PIT_PERIOD          1000000 // 1000 ms
//    PIT_Init(PIT_PERIOD, BOARD_MCK / 1000000);
/*
    // Configure interrupt on PIT
    IRQ_DisableIT(AT91C_ID_SYS);
    IRQ_ConfigureIT(AT91C_ID_SYS, AT91C_AIC_PRIOR_LOWEST, ISR_Pit);
    IRQ_EnableIT(AT91C_ID_SYS);
    PIT_EnableIT();
*/
    // Enable the pit
//    PIT_Enable();

    SerialPort _master(0);
    if ( !_master.Open(true, true) )
    {
        log( "unable to open master port" );
        return -2;
    }

    SerialPort _slave(1);
    if ( !_slave.Open(false, true) )
    {
        log( "unable to open slave port" );
        return -3;
    }

    LED_Set(1);   


    std::queue<AvrCmd> queue;

    bool z2 = false;
    bool z3 = false;
    unsigned char zmInput = AvrCmd::PARAM_INVALID;
    unsigned char z2Input = AvrCmd::PARAM_INVALID;
    unsigned char z3Input = AvrCmd::PARAM_INVALID;
    bool z3PreOut = true;
    const unsigned char z3PreOutVol = 70;  // volume level for Zone3 PreOut

    // query the status of the master receiver
    QUEUE_PUSH(AvrCmd( _master,  AvrCmd::CMD_PW,  AvrCmd::PARAM_QUERY));

    std::string msg;
    bool loop = true;
    while (loop)
    {
//        if ( PIT_GetPIIR() > SLEEP_TIME )
//        {
//            QUEUE_PUSH(AvrCmd(_master, AvrCmd::CMD_PW, AvrCmd::PARAM_STANDBY)); 
//        }


        LED_Clear(0);
//        queue.empty()?LED_Clear(0):LED_Set(0);    

        _master.ReadTo('\r', msg);
        if ( !msg.empty() ) 
        {
//            PIT_GetPIVR(); // reset sleep timer

            LED_Set(0);
            const char* s = msg.c_str();
            AvrCmd avr = AvrCmd(msg.c_str());
                    
            //
            // Zone 1
            //
            switch ( avr.cmd )
            {
                case AvrCmd::CMD_PW:
                {
                    avr.log();
                    QUEUE_PUSH(AvrCmd(_master, AvrCmd::CMD_Z2, AvrCmd::PARAM_QUERY)); // query the state of zone 2 on the master
    
                    if (avr.param == AvrCmd::PARAM_ON )
                    {
                        // since we've queried the status of the Zone2, 
                        // turn if off before receiving the status to get it in sync
                        QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_Z2, AvrCmd::PARAM_OFF));  
                    }
                    else if (avr.param  ==  AvrCmd::PARAM_STANDBY )
                    {
                        // set everything to off and standby
                        QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_Z2, AvrCmd::PARAM_OFF));
                        QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_Z3, AvrCmd::PARAM_OFF));
                        QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_PW, AvrCmd::PARAM_STANDBY));
                    }
                }
                break;

                //
                // Main Zone
                //
                case AvrCmd::CMD_ZM:
                {
                    avr.log();
                    switch (avr.param)
                    {
                        case AvrCmd::PARAM_ON:
                            QUEUE_PUSH( AvrCmd(_master, AvrCmd::CMD_SI, AvrCmd::PARAM_QUERY)); // query the state of zone 3 on the master, now that we know the state of zone 2
                            if ( z2Input == AvrCmd::PARAM_INVALID )
                            {
                                QUEUE_PUSH( AvrCmd(_master, AvrCmd::CMD_Z2, AvrCmd::PARAM_QUERY)); // query the state of zone 3 on the master, now that we know the state of zone 2
                            }
                            if ( z3Input == AvrCmd::PARAM_INVALID )
                            {
                                QUEUE_PUSH( AvrCmd(_master, AvrCmd::CMD_Z3, AvrCmd::PARAM_QUERY)); // query the state of zone 3 on the master, now that we know the state of zone 2
                            }
                            break;

                        case AvrCmd::PARAM_OFF:
                        {
                            if ( z2Input == AvrCmd::PARAM_SOURCE )
                            {
                                QUEUE_PUSH( AvrCmd(_master, AvrCmd::CMD_Z2, AvrCmd::PARAM_OFF));
                            }
                            if ( z3Input == AvrCmd::PARAM_SOURCE )
                            {
                                QUEUE_PUSH( AvrCmd(_master, AvrCmd::CMD_Z3, AvrCmd::PARAM_OFF));
                            }
                        }
                        break;

                        default:
                            break;
                    }
                }
                break;

                //
                // Zone 2
                //
                case AvrCmd::CMD_Z2:
                {
                    avr.log();
                    switch (avr.param)
                    {
                        case AvrCmd::PARAM_ON:
                        {
                            QUEUE_PUSH( AvrCmd(_master, AvrCmd::CMD_Z3, AvrCmd::PARAM_QUERY)); // query the state of zone 3 on the master, now that we know the state of zone 2
                            if (!z2)
                            {
                                // initialized the settings for Zone2 playback
                                // by turning on the slave amp's main zone and input settings 
                                z2 = true;
                                QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_ZM, AvrCmd::PARAM_ON));
                                QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_SI, AvrCmd::PARAM_TV_CBL));
                                QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_SD, AvrCmd::PARAM_AUTO));
                                QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_DC, AvrCmd::PARAM_AUTO));
                            }
                        }
                        break;

                        case AvrCmd::PARAM_OFF:
                        {
                            // Zone2 playback is turned off, so turn of main zone on slave
                            z2 = false;
                            QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_ZM, AvrCmd::PARAM_OFF));
                        }
                        break;

                        case AvrCmd::PARAM_SOURCE:
                        case AvrCmd::PARAM_PHONO:
                        case AvrCmd::PARAM_CD:
                        case AvrCmd::PARAM_TUNER:
                        case AvrCmd::PARAM_DVD:
                        case AvrCmd::PARAM_BD:
                        case AvrCmd::PARAM_TV:
                        case AvrCmd::PARAM_SAT_CBL:
                        case AvrCmd::PARAM_DVR:
                        case AvrCmd::PARAM_GAME:
                        case AvrCmd::PARAM_V_AUX:
                        case AvrCmd::PARAM_DOCK:
                        case AvrCmd::PARAM_SIRIUS:
                        case AvrCmd::PARAM_HDRADIO:
                        //case AvrCmd::PARAM_IPOD:
                        case AvrCmd::PARAM_NET_USB:
                        case AvrCmd::PARAM_RHAPSODY:
                        case AvrCmd::PARAM_NAPSTER:
                        case AvrCmd::PARAM_LASTFM:
                        case AvrCmd::PARAM_FLICKR:
                        case AvrCmd::PARAM_FAVORITES:
                        case AvrCmd::PARAM_IRADIO:
                        case AvrCmd::PARAM_SERVER:
                        case AvrCmd::PARAM_USB_IPOD:
                        {
                            // Zone2's source has changed.  Change the source we're recording from on the master
                            // so that the slave receives the correct input
                            QUEUE_PUSH( AvrCmd(_master, AvrCmd::CMD_SR, avr.param));
        
                            // we don't need to change the input settings on the slave since we don't 
                            // change the source on the slave
                            //QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_SI, AvrCmd::PARAM_TV_CBL"));
                            //QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_SD, AvrCmd::PARAM_AUTO));
                            //QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_DC, AvrCmd::PARAM_AUTO));                
        
                            z2Input = avr.param; // save zone2 source for future use
                        }
                        break;

                        default:
                        {
                            if ( avr.param >= AvrCmd::PARAM_VOL_00 && avr.param <= AvrCmd::PARAM_VOL_99 )
                            {
                                QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_MV, avr.param));
                            }
                        }
                        break;
                    }
                }
                break;

                //
                // Zone 3
                //
                // assignable speakers output on the slave amp are assigned to the slave's Zone 3 output
                //
                case AvrCmd::CMD_Z3:
                {
                    avr.log();
                    switch (avr.param)
                    {
                        case AvrCmd::PARAM_ON:
                        {
                            if (!z3)
                            {
                                // initialized the settings for Zone3 playback
                                // by turning on the slave amp's zone 3 and set input settings 
                                // We're using the pre-out from the master, so we keep
                                // the volume of the slave's zone 3 fixed
        
                                z3 = true;
                                z3PreOut = true;
                                QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_Z3,  AvrCmd::PARAM_CD));
                                QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_Z3, z3PreOutVol));
                                QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_Z3,  AvrCmd::PARAM_ON));
                            }
                        }
                        break;

                        case AvrCmd::PARAM_OFF:
                        {
                            // Zone 3 playback is turned off on master, so turn off zone3 on the slave
                            z3 = false;
                            QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_Z3, AvrCmd::PARAM_OFF));
                        }
                        break;

                        case AvrCmd::PARAM_SOURCE:
                        case AvrCmd::PARAM_PHONO:
                        case AvrCmd::PARAM_CD:
                        case AvrCmd::PARAM_TUNER:
                        case AvrCmd::PARAM_DVD:
                        case AvrCmd::PARAM_BD:
                        case AvrCmd::PARAM_TV:
                        case AvrCmd::PARAM_SAT_CBL:
                        case AvrCmd::PARAM_DVR:
                        case AvrCmd::PARAM_GAME:
                        case AvrCmd::PARAM_V_AUX:
                        case AvrCmd::PARAM_DOCK:
                        case AvrCmd::PARAM_SIRIUS:
                        case AvrCmd::PARAM_HDRADIO:
                        //case AvrCmd::PARAM_IPOD:
                        case AvrCmd::PARAM_NET_USB:
                        case AvrCmd::PARAM_RHAPSODY:
                        case AvrCmd::PARAM_NAPSTER:
                        case AvrCmd::PARAM_LASTFM:
                        case AvrCmd::PARAM_FLICKR:
                        case AvrCmd::PARAM_FAVORITES:
                        case AvrCmd::PARAM_IRADIO:
                        case AvrCmd::PARAM_SERVER:
                        case AvrCmd::PARAM_USB_IPOD:
                        {
                            // Zone3's source has changed.  Save the source for future use.
                            // We don't need to change the slaves inputs since we're using the master's preout for Zone3
                            z3Input = avr.param;
                        }
                        break;

                        default:
                        {
                            if ( avr.param >= AvrCmd::PARAM_VOL_00 && avr.param <= AvrCmd::PARAM_VOL_99 )
                            {
                                if (z3PreOut)
                                {
                                    QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_Z3, z3PreOutVol));
                                }
                                else
                                {
                                    QUEUE_PUSH( AvrCmd(_slave, AvrCmd::CMD_Z3, avr.param));
                                }
                            }

                        }   
                        break;
                    }                  
                }
                break;

                //
                // Source Record
                //
                case AvrCmd::CMD_SR:  // Source Record - store input which is send optically to slave amp
                {
                    avr.log();
                    z2Input = avr.param;
                }
                break;

                case AvrCmd::CMD_SI:
                {
                    avr.log();
                    zmInput = avr.param;
                }
                break;

                //
                // Commands we ignore 
                //
                case AvrCmd::CMD_MV:
                case AvrCmd::CMD_CV:
                case AvrCmd::CMD_HD:
                case AvrCmd::CMD_PS:
                case AvrCmd::CMD_TF:
                case AvrCmd::CMD_TP:
                case AvrCmd::CMD_TM:
                {
                }
                break;

                default:
                {
                    avr.log(); // log unexpected commands
                }
                break;
            }
        }

        if (!queue.empty())
        {
            AvrCmd avr = queue.front();
            avr.log();
            if ( !avr.write() )
            {
              
                avr.port->Close();
                avr.port->Open(true,true);
            }
            queue.pop();
        }
    }

    _slave.Close();
    _master.Close();

    log( "exiting...\r\n" );

	return 0;
}
/*

//------------------------------------------------------------------------------
/// Configure the periodic interval timer to generate an interrupt every 10 ms
//------------------------------------------------------------------------------
static void ConfigurePit(void)
{
    // Initialize the PIT to the desired frequency
    PIT_Init(PIT_PERIOD, BOARD_MCK / 1000000);

    // Configure interrupt on PIT
    IRQ_DisableIT(AT91C_ID_SYS);
    IRQ_ConfigureIT(AT91C_ID_SYS, AT91C_AIC_PRIOR_LOWEST, ISR_Pit);
    IRQ_EnableIT(AT91C_ID_SYS);
    PIT_EnableIT();

    // Enable the pit
    PIT_Enable();
}


*/

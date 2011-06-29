#include "stdafx.h"
#include "serialport.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "pio/pio.h"
#include "pmc/pmc.h"
#include "usart/usart.h"

#ifdef __cplusplus
}
#endif

static const Pin pins_USART0[] = { PIN_USART0_RXD, PIN_USART0_TXD };
static const Pin pins_USART1[] = { PIN_USART1_RXD, PIN_USART1_TXD };

static AT91S_USART* pUSART[] = { AT91C_BASE_US0, AT91C_BASE_US1 };

    bool SerialPort::Open( bool read, bool write )
    {
        _hPort = _portName;

        PIO_Configure((_hPort==1)?pins_USART1:pins_USART0, 2);

        PMC_EnablePeripheral((_hPort==1)?AT91C_ID_US1:AT91C_ID_US0);

        const unsigned int mode = AT91C_US_USMODE_NORMAL
                          | AT91C_US_CLKS_CLOCK
                          | AT91C_US_CHRL_8_BITS
                          | AT91C_US_PAR_NONE
                          | AT91C_US_NBSTOP_1_BIT;

        USART_Configure(pUSART[_hPort], mode, _baudRate, BOARD_MCK);
        USART_SetTransmitterEnabled(pUSART[_hPort], write);
        USART_SetReceiverEnabled(pUSART[_hPort], read);

        return true;
    }

    void SerialPort::Close() 
    {
        if ( _hPort != -1 )
        {
          USART_SetTransmitterEnabled(pUSART[_hPort], 0);
          USART_SetReceiverEnabled(pUSART[_hPort], 0);
          PMC_DisablePeripheral((_hPort==1)?AT91C_ID_US1:AT91C_ID_US0);
  
          _read.clear();
          _hPort = -1;
         }
    }

    bool SerialPort::ReadTo(char toChar, std::string& outStr)
    {
        while(1)
        {
          if ( USART_IsRxReady(pUSART[_hPort]) )
          {
            char buf[1];
            *buf = USART_GetChar(pUSART[_hPort]);
            if ( *buf == toChar )
            {
                outStr = _read;
                _read.clear();
                return true;
            }
            else // no
            {
                _read.append(buf, 1);
            }
          }
          else
          {
            outStr.clear();
            return false;
          }
        }
    }

    bool SerialPort::Write(const char* str) 
    {
        while(*str)
        {
          USART_PutChar(pUSART[_hPort], *str++);
        }
        return true;
    }


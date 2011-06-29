#ifndef _SERIALPORT_H
#define _SERIALPORT_H

class SerialPort
{
	public:
    enum Parity 
    {
        Parity_None = 0,
        Parity_Odd = 1,
        Parity_Even = 2,
        Parity_Mark = 3,
        Parity_Space = 4
    };


    enum StopBits
    {
        StopBits_One = 0,
        StopBits_OnePointFive = 1,
        StopBits_Two = 2
    };

private:
    int _hPort;

    int _portName;
    int _baudRate;
    Parity _parity;
    int _dataBits;
    StopBits _stopBits;
    
    std::string _read;

public:
    SerialPort(int port ) :
        _hPort(-1), 
        _portName(port), _baudRate(9600), _parity(Parity_None), _dataBits(8), _stopBits(StopBits_One)
    {
    }
/*
    SerialPort(const std::string& str, int baudRate, Parity parity, int dataBits, StopBits stopBits ) :
        _hPort(-1), 
        _portName(str), _baudRate(baudRate), _parity(parity), _dataBits(dataBits), _stopBits(stopBits)
    {
    }
    SerialPort(const char* str, int baudRate, Parity parity, int dataBits, StopBits stopBits ) :
        _hPort(-1), 
        _portName(str), _baudRate(baudRate), _parity(parity), _dataBits(dataBits), _stopBits(stopBits)
    {
    }
*/
    int PortName() { return _portName; }
/*
    void SetReadTimeout(unsigned int ms)
    {
    }
    void SetWriteTimeout(unsigned int ms)
    {
    }
*/
    bool Open( bool read, bool write );
    void Close();
    bool ReadTo(char toChar, std::string& outStr);
    bool Write(const char* str);

};	

#endif

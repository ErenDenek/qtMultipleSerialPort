#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "stdint.h"
#include "QDebug"
#include "QSerialPort"
#include "QSerialPortInfo"

#define SERIALPORT_NB   7

class serialPort
{
public:
    serialPort();

    QSerialPort *serialPortObj[SERIALPORT_NB];

    void write(char * arr, uint8_t size, uint8_t serialPortIndex);

    void open(uint8_t serialPortIndex);

    void close(uint8_t serialPortIndex);

    void read(uint8_t serialPortIndex);

private:
    void dataCollector(uint8_t data, uint8_t serialPortIndex);

    QString serialPortName[SERIALPORT_NB];

    uint8_t UartRxDataBuffer[SERIALPORT_NB][10000];

    uint8_t packetReady;
};

#endif // SERIALPORT_H

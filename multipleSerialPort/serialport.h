#ifndef SERIALPORT_H
#define SERIALPORT_H

#include "stdint.h"
#include "QDebug"
#include "QSerialPort"
#include "QSerialPortInfo"

#define SERIALPORT_NB   2
extern bool pck_status;
extern bool pck_status2;
extern bool pck_status1;
extern bool pck_status3;

#pragma pack(push,1)
union test_pack_u{
    struct{
        uint16_t header; //paket başlangıcı 0x5555 (0101 0101 0101 0101)
        uint8_t pkt_direction; //paket iletim yönü 0x31
        uint8_t pkt_ID; //0xE1=225
        uint16_t pkt_size; //

        float test_value;
        uint16_t CRC;
        uint16_t pkt_end;
    }test;
    uint8_t testArray[sizeof(test)];
};
extern union test_pack_u test_pack;

union test_pack2_u{
    struct{
        uint16_t header; //paket başlangıcı 0x5555 (0101 0101 0101 0101)
        uint8_t pkt_direction; //paket iletim yönü 0x31
        uint8_t pkt_ID; //0xE2=226
        uint16_t pkt_size; //

        float test_value;
        uint16_t CRC;
        uint16_t pkt_end;
    }test2;
    uint8_t testArray[sizeof(test2)];
};
extern union test_pack2_u test_pack2;

#pragma pack(pop)

class serialPort
{
public:
    serialPort();

    QSerialPort *serialPortObj; //7 rakamı port sayısına göre değişebilir.

    void write(const char * arr, uint8_t size);

    void open(QString SerialPortName);

    void close(uint8_t serialPortIndex);

    void read();

    void pkt_evaluate(uint8_t serialPortIndex);

    bool receivedDatas;

    uint16_t qualityOfComm;

    uint8_t dataU8 = 0;

    //QString SerialPortName;

    uint8_t UartRxDataBuffer[10000] = {0};

    void dataCollector(uint8_t data);
    bool UartRxState;
    uint8_t packetReady;

private:

};

#endif // SERIALPORT_H

#ifndef __PARSER_H_
#define __PARSER_H_

#include "main.h"

#define PACKET_STX_1 0xF0
#define PACKET_STX_2 0xF1

#define PACKET_ETX_1 0xF3
#define PACKET_ETX_2 0xF4

#define MAX_SEND_DATA 50

enum
{
    // from PC to Transmitter
    PACKET_TO_TRANSMITTER_MIN = 0, /* 0x00 */
    // ----------------------
    PACKET_TO_TRANSMITTER_00, /* 0x01 : Version Request */
    PACKET_TO_TRANSMITTER_01, /* 0x02 : Type */
    PACKET_TO_TRANSMITTER_02, /* 0x03 : Period */
    PACKET_TO_TRANSMITTER_03, /* 0x04 : K Factor          - Requeset */
    PACKET_TO_TRANSMITTER_04, /* 0x05 : K Factor          - Send */
    PACKET_TO_TRANSMITTER_05, /* 0x06 */
    PACKET_TO_TRANSMITTER_06, /* 0x07 */
    PACKET_TO_TRANSMITTER_07, /* 0x08 */
    PACKET_TO_TRANSMITTER_08, /* 0x09 */
    PACKET_TO_TRANSMITTER_09, /* 0x0A */
    PACKET_TO_TRANSMITTER_10, /* 0x0B */
    PACKET_TO_TRANSMITTER_11, /* 0x0C */
    PACKET_TO_TRANSMITTER_12, /* 0x0D */
    PACKET_TO_TRANSMITTER_13, /* 0x0E : Network Info      - Request */
    PACKET_TO_TRANSMITTER_14, /* 0x0F : Network Info      - Set */

    PACKET_TO_TRANSMITTER_15, /* 0x10 : Device ID         - Request */
    PACKET_TO_TRANSMITTER_16, /* 0x11 : Datetime          - Set */
    PACKET_TO_TRANSMITTER_17, /* 0x12 : Sensor Data       - Request */
    PACKET_TO_TRANSMITTER_18, /* 0x13 */
    PACKET_TO_TRANSMITTER_19, /* 0x14 */

    // 4 Factory (RS485)
    PACKET_TO_TRANSMITTER_20, /* 0x15 : Device ID         - Request */
    PACKET_TO_TRANSMITTER_21, /* 0x16 : Product Serial    - Request */
    PACKET_TO_TRANSMITTER_22, /* 0x17 : Product Serial    - Set */
    PACKET_TO_TRANSMITTER_23, /* 0x18 : Calibration Co2   - Set */
    PACKET_TO_TRANSMITTER_24, /* 0x19 : Calibration Tvoc  - Set */
    PACKET_TO_TRANSMITTER_25, /* 0x1A : Version, Sensor   - Request */
    PACKET_TO_TRANSMITTER_26, /* 0x1B : */
    PACKET_TO_TRANSMITTER_27, /* 0x1C : */
    PACKET_TO_TRANSMITTER_28, /* 0x1D : */
    PACKET_TO_TRANSMITTER_29, /* 0x1E : */
    PACKET_TO_TRANSMITTER_30, /* 0x1F : */
    // ----------------------
    PACKET_TO_TRANSMITTER_MAX, /* 0x20 */
    // -------------------------------------------
    PACKET_TO_PC_MIN = 0x30, /* 0x30 */
    // ----------------------
    PACKET_TO_PC_00, /* 0x31 : System Info */
    PACKET_TO_PC_01, /* 0x32 : Version */
    PACKET_TO_PC_02, /* 0x33 : K Factor */
    PACKET_TO_PC_03, /* 0x34 : PM 2.5 */
    PACKET_TO_PC_04, /* 0x35 : PM(Current) */
    PACKET_TO_PC_05, /* 0x36 : PM(Current) + Temperature + Humidity */
    PACKET_TO_PC_06, /* 0x37 : PM (Current, Average) + Temperature + Humidity */
    PACKET_TO_PC_07, /* 0x38 : Particle */
    PACKET_TO_PC_08, /* 0x39 : �̼����� ���� */
    PACKET_TO_PC_09, /* 0x3A : KC ����, RS485 */
    PACKET_TO_PC_10, /* 0x3B */
    PACKET_TO_PC_11, /* 0x3C */
    PACKET_TO_PC_12, /* 0x3D */
    PACKET_TO_PC_13, /* 0x3E : Network Info */
    PACKET_TO_PC_14, /* 0x3F */

    PACKET_TO_PC_15, /* 0x40 : Device ID */
    PACKET_TO_PC_16, /* 0x41 : Datetime */
    PACKET_TO_PC_17, /* 0x42 : Sensor Data */
    PACKET_TO_PC_18, /* 0x43 */
    PACKET_TO_PC_19, /* 0x44 */

    // 4 Factory (RS485)
    PACKET_TO_PC_20, /* 0x45 : Device ID         - Request */
    PACKET_TO_PC_21, /* 0x46 : Product Serail    - Request */
    PACKET_TO_PC_22, /* 0x47 : Product Serial    - Set */
    PACKET_TO_PC_23, /* 0x48 : Calibration Co2   - Set */
    PACKET_TO_PC_24, /* 0x49 : Calibration Tvoc  - Set */
    PACKET_TO_PC_25, /* 0x4A : Version, Sensor   - Request */
    PACKET_TO_PC_26, /* 0x4B : */
    PACKET_TO_PC_27, /* 0x4C : */
    PACKET_TO_PC_28, /* 0x4D : */
    PACKET_TO_PC_29, /* 0x4E : */
    PACKET_TO_PC_30, /* 0x4F : Ack */
    // ----------------------
    PACKET_TO_PC_MAX, /* 0x50 */
};

enum
{
    P_STATE_SOM = 0, /* 00 : Search for start of message */
    // ---------------------------------------
    P_STATE_STX,       /* 01 : Start of Sequence */
    P_STATE_CMD,       /* 02 : Command */
    P_STATE_LENGTH_1,  /* 03 : Length */
    P_STATE_LENGTH_2,  /* 04 */
    P_STATE_DATA,      /* 05 : Data */
    P_STATE_CHECK_SUM, /* 06 : Check Sum */
    P_STATE_ETX,       /* 07 : End of Sequence */
    // ---------------------------------------
};

enum
{
    SEND_TYPE_00 = 0, /* 00 : PM 2.5 */
    SEND_TYPE_01,     /* 01 : PM */
    SEND_TYPE_02,     /* 02 : PM, Temperature, Humdity */
    SEND_TYPE_03,     /* 03 : PM, PM ���, Temperature, Humdity */
    SEND_TYPE_04,     /* 04 : PM5000S Particle Count */
    SEND_TYPE_05,     /* 05 : Reserved */
    SEND_TYPE_06,     /* 06 : KC���� ����, RS485 */
    SEND_TYPE_07,     /* 07 : Particle Serial ��� */
    SEND_TYPE_08,     /* 08 */
    SEND_TYPE_09,     /* 09 */
    SEND_TYPE_10,     /* 10 : �̼����� ���� */
    SEND_TYPE_MAX
};

enum
{
    SEND_PERIOD_SEC_01 = 0, /* 00 :  1 �� */
    SEND_PERIOD_SEC_02,     /* 01 :  2 �� */
    SEND_PERIOD_SEC_05,     /* 02 :  5 �� */
    SEND_PERIOD_SEC_10,     /* 03 : 10 �� */
    SEND_PERIOD_MIN_01,     /* 04 :  1 �� */
    SEND_PERIOD_MIN_02,     /* 05 :  2 �� */
    SEND_PERIOD_MIN_05,     /* 06 :  5 �� */
    SEND_PERIOD_MIN_60,     /* 07 : 60 �� */
    SEND_PERIOD_MAX
};

uint32_t _offset_dust(float data);

void parser(uint8_t data);

void parser_handle(void);
void parser_handle_rs485(void);
void parser_init(void);

void parser_modbus(uint8_t data);
void parser_handle_modbus(void);
void parser_init_modbus(void);

#endif

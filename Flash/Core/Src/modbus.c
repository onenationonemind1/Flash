#include "main.h"
#include <stdint.h>

uint8_t modbus_rx_buffer[256];
uint8_t g_modbus_state = MODBUS_SOM;

enum
{
    MODBUS_SOM = 0,              /* 00 : Search for start of message */
    MODBUS_FUNCTION_CODE,        /* 01 */
    MODBUS_ADDRESS,              /* 02 */
    MODBUS_QUANTITY_OF_REGISTER, /* 03 */
    MODBUS_LENGTH_OF_DATA,       /* 04 */
    MODBUS_DATA,                 /* 05 */
    MODBUS_CRC,                  /* 06 */
};

uint16_t ModBus_CRC16(uint8_t *data, uint16_t length);
void modbus_receive();
void parser_modbus(uint8_t data);

uint16_t ModBus_CRC16(uint8_t *data, uint16_t length)
{
    uint16_t crc = 0xFFFF; // Initial value
    uint16_t i, j;

    for (i = 0; i < length; i++)
    {
        crc ^= data[i]; // XOR byte with CRC
        for (j = 0; j < 8; j++)
        { // Process each bit
            if (crc & 0x0001)
            {                              // If LSB is 1
                crc = (crc >> 1) ^ 0xA001; // Shift right and XOR with polynomial
            }
            else
            {
                crc = crc >> 1; // Just shift right
            }
        }
    }

    return crc;
}

void modbus_receive()
{
    HAL_UART_Receive_IT(&huart3, (uint8_t *)modbus_rx_buffer, 1);
    parser_modbus(modbus_rx_buffer[0]);
}

void parser_modbus(uint8_t data)
{
    switch (g_modbus_state)
    {
        case
    }
}

uint8_t parser_modbus_function_code(uint8_t function_code)
{
    uint8_t result = MODBUS_NONE;

    switch (function_code)
    {

    case MODBUS_READ_COILS:             // 4 [01, 0x01] Read Coil
        case MODBUS_READ_DISCRETE_INPUT:    // 4 [02, 0x02] Read Discrete Input
        case MODBUS_READ_HOLDING_REGISTERS: // 4 [03, 0x03] Read Holding Registers
        case MODBUS_READ_INPUT_REGISTERS:   // 4 [04, 0x04] Read Input Registers
        {
            result = MODBUS_READ;
        }
        break;

        case MODBUS_WRITE_SINGLE_COIL:        // 4 [05, 0x05] Write Single Coil
        case MODBUS_WRITE_SINGLE_REGISTER:    // 4 [06, 0x06] Write Single Holding Register
        case MODBUS_WRITE_MULTIPLE_COILS:     // 4 [15, 0x0F] Write Multiple Coils
        case MODBUS_WRITE_MULTIPLE_REGISTERS: // 4 [16, 0x10] Write Multiple Holding Registers
        {
            result = MODBUS_WRITE;
        }
        break;
        }

        return result;
}
#include <stdbool.h> // bool 타입을 사용하기 위해 필요
#include <stdint.h>
#include "main.h"
#include "sensor.h"

#define AM1002_HEAD_SEND 0x11
#define AM1002_READ_MEASUREMENT_RESULT 0x16
#define AM1002_READ_SOFTWARE_VERSION_NUMBER 0x1E
#define AM1002_READ_SERIAL_NUMBER 0x1F
#define UART_TIMEOUT 0xFFFF

#define TEMP_OFFSET 500
#define SCALE_FACTOR 10
#define SERIAL_NUMBER_LENGTH 13
#define DATA_PACKET_LENGTH 22 // Total expected data length
#define AM1002_ERROR_INVALID_COMMAND 0xFF
#define UART_BUFFER_SIZE 256

t_sensor_data g_sensor_current;

typedef enum
{
    AM1002_SUCCESS = 0,
    AM1002_ERROR_NULL_POINTER,
    AM1002_ERROR_INVALID_LENGTH
} am1002_status_t;

void handle_am1002(void);
bool am1002_transmit(uint8_t command, uint8_t enable, uint8_t data);
uint8_t am1002_checksum(uint8_t *pdata, uint8_t length);
void send_packet_to_sensor(uint8_t *pdata, uint8_t length);
uint8_t am1002_receive(uint8_t command, uint8_t *p_data, t_sensor_data *sensor_data);
void uart_transmit(const char *data, uint16_t length);
void send_sensor_data(t_sensor_data *sensor);

uint8_t rx_data;
uint8_t cmd[] = {0x11, 0x01, 0x16, 0xD8};
static uint8_t rx_buffer_1[22];

uint8_t parser_buffer[50] = {
    0,
};

uint8_t g_am1002_data[32];

uint8_t request[] = {0x01, 0x03, 0x00, 0x00, 0x00, 0x06, 0xC5, 0xCD};

void handle_am1002(void)
{
    memset(rx_buffer_1, 0, sizeof(rx_buffer_1));
    am1002_transmit(AM1002_READ_MEASUREMENT_RESULT, 0, 0);
    HAL_UART_Receive(&huart2, rx_buffer_1, 22, 1000);
    if (rx_buffer_1[0] == 0x16)
    {
        am1002_receive(AM1002_READ_MEASUREMENT_RESULT, rx_buffer_1, &g_sensor_current);
    }
    send_sensor_data(&g_sensor_current);

    HAL_UART_Receive(&huart3, parser_buffer, 50, 1000);
    for (int i = 0; i < 50; i++)
    {
        parser(parser_buffer[i]);
        parser_modbus(parser_buffer[i]);
        }
}

uint8_t am1002_checksum(uint8_t *pdata, uint8_t length)
{
    uint16_t crc = 256;

    for (uint8_t i = 0; i < length; i++)
    {
        crc -= pdata[i];
    }

    return (uint8_t)(crc);
}
bool am1002_transmit(uint8_t command, uint8_t enable, uint8_t data)
{

    if (command != AM1002_READ_MEASUREMENT_RESULT &&
        command != AM1002_READ_SOFTWARE_VERSION_NUMBER &&
        command != AM1002_READ_SERIAL_NUMBER)
    {
        return 0;
    }

    uint8_t index = 0;
    uint8_t check_sum = 256;

    // Start Symbol
    // Build packet
    g_am1002_data[index++] = AM1002_HEAD_SEND;                      // Start Symbol
    g_am1002_data[index++] = 0x01;                                  // Length
    g_am1002_data[index++] = command;                               // Command
    g_am1002_data[index++] = am1002_checksum(g_am1002_data, index); // Checksum

    send_packet_to_sensor(g_am1002_data, index);
    return 1;
}

// HAL_StatusTypeDef am1002_read(void)
//{
//     HAL_StatusTypeDef status = HAL_UART_Receive(&huart2, &rx_buffer_1, 22, 1000);
//
//     return status;
// }

void send_packet_to_sensor(uint8_t *pdata, uint8_t length)
{
    if (pdata == NULL || length == 0)
    {
        return;
    }

#ifdef DEBUG_PRINT
    printf("-- send_packet_to_sensor() : [");
    for (int i = 0; i < length; i++)
    {
        printf("0x%02X ", pdata[i]);
    }
    printf("]\n");
#endif

    HAL_StatusTypeDef status = HAL_UART_Transmit(&huart2, pdata, length, UART_TIMEOUT);
    if (status != HAL_OK)
    {

        HAL_UART_Transmit(&huart2, "error", 5, UART_TIMEOUT);
        return;
    }

    while (__HAL_UART_GET_FLAG(&huart2, USART_ISR_BUSY) == SET)
        ;
}

uint8_t am1002_receive(uint8_t command, uint8_t *p_data, t_sensor_data *sensor_data)
{
    if (p_data == NULL || sensor_data == NULL)
    {
        return AM1002_ERROR_NULL_POINTER;
    }

    uint8_t index = 3;

    switch (command)
    {
    case AM1002_READ_MEASUREMENT_RESULT:
    {

        // TVOC concentration (ppb)
        sensor_data->tvoc.data = (p_data[index] << 8) | p_data[index + 1];
        index += 2;

        // Skip reserved bytes
        index += 2;

        // PM measurements (μg/m3)
        sensor_data->dust.pm_01_0 = (p_data[index] << 8) | p_data[index + 1];
        index += 2;

        sensor_data->dust.pm_02_5 = (p_data[index] << 8) | p_data[index + 1];
        index += 2;

        sensor_data->dust.pm_10_0 = (p_data[index] << 8) | p_data[index + 1];
        index += 2;

        // Temperature
        uint16_t raw_temp = (p_data[index] << 8) | p_data[index + 1];
        sensor_data->t_h.temperature = ((raw_temp - TEMP_OFFSET) * SCALE_FACTOR);
        index += 2;

        // Humidity
        uint16_t raw_humidity = (p_data[index] << 8) | p_data[index + 1];
        sensor_data->t_h.humidity = raw_humidity * SCALE_FACTOR;
        index += 2;

        // Skip remaining reserved bytes
        index += 4;

#ifdef DEBUG_SENSOR
        sensor_data_am1002_debug(sensor_data);
#else
// sensor_data_am1002(sensor_data);
#endif

        break;
    }

    case AM1002_READ_SOFTWARE_VERSION_NUMBER:
    {
        // Implement software version reading logic
        break;
    }

    case AM1002_READ_SERIAL_NUMBER:
    {
        if (index + SERIAL_NUMBER_LENGTH > DATA_PACKET_LENGTH)
        {
        }

#ifdef DEBUG_SENSOR
        printf("Serial Number: ");
        for (int i = 0; i < SERIAL_NUMBER_LENGTH; i++)
        {
            printf("0x%02X ", p_data[i]);
        }
        printf("\n");
#endif

        break;
    }

    default:
        return AM1002_ERROR_INVALID_COMMAND;
    }
}

// UART 송신 함수 (하드웨어에 맞게 구현 필요)
void uart_transmit(const char *data, uint16_t length)
{
    // 실제 UART 전송 구현
    HAL_UART_Transmit(&huart3, (uint8_t *)data, length, 100);
    // 예: HAL_UART_Transmit(&huart1, (uint8_t*)data, length, 100);
}

void send_sensor_data(t_sensor_data *sensor)
{
    char buffer[UART_BUFFER_SIZE];
    int length = 0;

    // 먼지 센서 데이터
    length = snprintf(buffer, UART_BUFFER_SIZE,
                      "Dust Data:\r\n"
                      "PM1.0: %lu ug/m3\r\n"
                      "PM2.5: %lu ug/m3\r\n"
                      "PM10.0: %lu ug/m3\r\n",
                      sensor->dust.pm_01_0,
                      sensor->dust.pm_02_5,
                      sensor->dust.pm_10_0);
    uart_transmit(buffer, length);

    // CO2, TVOC 데이터
    length = snprintf(buffer, UART_BUFFER_SIZE,
                      "\r\nGas Data:\r\n"

                      "TVOC: %lu ppb\r\n",

                      sensor->tvoc.data);
    uart_transmit(buffer, length);

    // 온습도 데이터
    // 온도는 실제값의 10배로 저장되어 있다고 가정
    uint32_t temp = sensor->t_h.temperature;
    length = snprintf(buffer, UART_BUFFER_SIZE,
                      "\r\nTemperature & Humidity:\r\n"
                      "Temperature: %lu \r\n"
                      "Humidity: %lu %%\r\n\r\n",
                      temp,
                      sensor->t_h.humidity);
    uart_transmit(buffer, length);
}

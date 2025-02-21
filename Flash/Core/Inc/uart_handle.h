#ifndef __UART_H_
#define __UART_H_

#define PACKET_STX_1 0xF0
#define PACKET_STX_2 0xF1

#define PACKET_ETX_1 0xF3
#define PACKET_ETX_2 0xF4

#define MAX_DATA 200

typedef struct
{
    uint8_t command;
    uint16_t length;
    uint8_t data[MAX_DATA];
    uint8_t check_sum;
} t_packet;

// uart 1 (ESP32)
void send_packet_to_ble(uint8_t *pdata, uint8_t length);

// uart 2 (Sensor)
void send_packet_to_sensor(uint8_t *pdata, uint8_t length);

// uart 4 (RS485)
void send_packet_to_rs485(t_packet *p_packet, uint8_t length);
void send_string_to_rs485(uint8_t *p_buffer, uint8_t length);

void uart1_transmit_string(const void *data);
void uart2_transmit_string(const void *data);
void uart3_transmit_string(const void *data);
void uart4_transmit_string(const void *data);

void uart_handle(void);
void uart_init(void);

#endif

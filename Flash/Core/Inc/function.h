#ifndef __FUNCTION_H_
#define __FUNCTION_H_

// - Flags ---------------------------------------
#define flag_NONE 0x00000000
//------------------------------------------------
#define flag_PARSER_HANDLE 0x00000004
#define flag_RS485 0x00000008
//------------------------------------------------
//------------------------------------------------
#define flag_UART_01 0x00000100
#define flag_UART_02 0x00000200
#define flag_UART_03 0x00000400
#define flag_UART_04 0x00000800
//------------------------------------------------
#define flag_MODE_00 0x00010000
#define flag_MODE_01 0x00020000
#define flag_MODE_02 0x00040000
#define flag_MODE_03 0x00080000
//------------------------------------------------

#define LIST_COUNT_05 5
#define LIST_COUNT_06 6
#define LIST_COUNT_10 10
#define LIST_COUNT_60 60

enum
{
    SWITCH_MODE_0 = 0, /* 00 */
    SWITCH_MODE_1,     /* 01 */
    SWITCH_MODE_2,     /* 02 */
    SWITCH_MODE_3,     /* 03 */
};

enum
{
    K_FACTOR_00 = 0, /* 00 */
    K_FACTOR_01,     /* 01 */
    K_FACTOR_02,     /* 02 */
    K_FACTOR_03,     /* 03 */
    K_FACTOR_04,     /* 04 */
    K_FACTOR_05,     /* 05 */
    K_FACTOR_06,     /* 06 */
    K_FACTOR_07,     /* 07 */
    K_FACTOR_08,     /* 08 */
    K_FACTOR_09,     /* 09 */
    K_FACTOR_MAX
};

enum
{
    FACTOR_LOW = 0,  /* 00 */
    FACTOR_HIGH,     /* 01 */
    FACTOR_MULTIPLE, /* 02 */
    FACTOR_PLUS,     /* 03 */
};

typedef struct
{
    // Spi Flash ���� Ȯ�ο� ������
    uint8_t init_data_1;
    uint8_t init_data_2;

    // RS485
    uint8_t device_id;
    uint8_t k_factor_index;

    // Sensor Data
    uint16_t tvoc_baseline;
    uint16_t eco2_baseline;
} t_setup;

typedef union
{
    float f;
    uint32_t u;
} u_factor;

extern t_setup g_setup;
extern u_factor g_factor[K_FACTOR_MAX][4];

void flag_set(uint32_t flag);
void flag_add(uint32_t flag);
void flag_del(uint32_t flag);
uint8_t is_flag(uint32_t flag);

void tick_start(void);
void tick_stop(void);
uint32_t tick_elapsed(void);

void setup_data_read(void);
void setup_data_write(void);
void setup_data_init(void);

void rs485_output(uint8_t enable);
void rs485_init(void);

void switch_handle(void);
void switch_init(void);

void function_init(void);

#endif

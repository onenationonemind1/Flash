#include "main.h"

#define RXBUFFERSIZE 10

UART_HandleTypeDef huart1; /* Uart (USB), 9600 */
UART_HandleTypeDef huart2; /* Sensor :   9600 */
UART_HandleTypeDef huart3; /* Debug  : 115200 */
UART_HandleTypeDef huart4; /* RS485  : 115200 */

DMA_HandleTypeDef hdma_usart1_rx;
DMA_HandleTypeDef hdma_usart2_rx;
DMA_HandleTypeDef hdma_usart3_rx;
DMA_HandleTypeDef hdma_usart4_rx;

uint8_t buffer_usart_1[RXBUFFERSIZE] = {
    0,
};
uint8_t buffer_usart_2[RXBUFFERSIZE] = {
    0,
};
uint8_t buffer_usart_3[RXBUFFERSIZE] = {
    0,
};
uint8_t buffer_usart_4[RXBUFFERSIZE] = {
    0,
};

uint8_t g_send_data[MAX_DATA];
uint8_t g_uart_04_index = 0;

#if 1
/* Private function prototypes -----------------------------------------------*/
#ifdef __GNUC__
/* With GCC, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

int fputc(int ch, FILE *f)
{
    /* Place your implementation of fputc here */
    /* e.g. write a character to the USART1 and Loop until the end of transmission */

    HAL_UART_Transmit(&huart3, (uint8_t *)&ch, 1, 0xFFFF);
    while (__HAL_UART_GET_FLAG(&huart3, USART_ISR_BUSY) == SET)
        ;

    return ch;
}
#endif

void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    if (huart->Instance == USART1)
    {
        // 4 Uart (USB)
        /* Peripheral clock enable */
        __HAL_RCC_USART1_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART1 GPIO Configuration
        PA09     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_9 | GPIO_PIN_10;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF1_USART1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART1 DMA Init */
        /* USART1_RX Init */
        hdma_usart1_rx.Instance = DMA1_Channel3;
        hdma_usart1_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart1_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart1_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart1_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart1_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart1_rx.Init.Mode = DMA_CIRCULAR;
        hdma_usart1_rx.Init.Priority = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_usart1_rx) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        __HAL_LINKDMA(huart, hdmarx, hdma_usart1_rx);
    }
    else if (huart->Instance == USART2)
    {
        // 4 Ethernet
        /* Peripheral clock enable */
        __HAL_RCC_USART2_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART2 GPIO Configuration
        PA02     ------> USART2_TX
        PA03     ------> USART2_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_2 | GPIO_PIN_3;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF1_USART2;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART2 DMA Init */
        /* USART2_RX Init */
        hdma_usart2_rx.Instance = DMA1_Channel5;
        hdma_usart2_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart2_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart2_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart2_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart2_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart2_rx.Init.Mode = DMA_CIRCULAR;
        hdma_usart2_rx.Init.Priority = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_usart2_rx) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        __HAL_LINKDMA(huart, hdmarx, hdma_usart2_rx);
    }
    else if (huart->Instance == USART3)
    {
        // 4 Debug
        /* Peripheral clock enable */
        __HAL_RCC_USART3_CLK_ENABLE();

        __HAL_RCC_GPIOB_CLK_ENABLE();
        /**USART3 GPIO Configuration
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_10 | GPIO_PIN_11;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_USART3;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        HAL_NVIC_SetPriority(USART3_4_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(USART3_4_IRQn);
    }
    else if (huart->Instance == USART4)
    {
        // 4 RS485
        /* Peripheral clock enable */
        __HAL_RCC_USART4_CLK_ENABLE();

        __HAL_RCC_GPIOA_CLK_ENABLE();
        /**USART4 GPIO Configuration
        PA00     ------> USART4_TX
        PA01     ------> USART4_RX
        */
        GPIO_InitStruct.Pin = GPIO_PIN_0 | GPIO_PIN_1;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF4_USART4;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* USART4 DMA Init */
        /* USART4_RX Init */
        hdma_usart4_rx.Instance = DMA1_Channel6;
        hdma_usart4_rx.Init.Direction = DMA_PERIPH_TO_MEMORY;
        hdma_usart4_rx.Init.PeriphInc = DMA_PINC_DISABLE;
        hdma_usart4_rx.Init.MemInc = DMA_MINC_ENABLE;
        hdma_usart4_rx.Init.PeriphDataAlignment = DMA_PDATAALIGN_BYTE;
        hdma_usart4_rx.Init.MemDataAlignment = DMA_MDATAALIGN_BYTE;
        hdma_usart4_rx.Init.Mode = DMA_CIRCULAR;
        hdma_usart4_rx.Init.Priority = DMA_PRIORITY_LOW;
        if (HAL_DMA_Init(&hdma_usart4_rx) != HAL_OK)
        {
            _Error_Handler(__FILE__, __LINE__);
        }

        __HAL_LINKDMA(huart, hdmarx, hdma_usart4_rx);
    }
}

void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART1_CLK_DISABLE();

        /**USART1 GPIO Configuration
        PA09     ------> USART1_TX
        PA10     ------> USART1_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9 | GPIO_PIN_10);

        /* USART1 DMA DeInit */
        HAL_DMA_DeInit(huart->hdmarx);
    }
    else if (huart->Instance == USART2)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART2_CLK_DISABLE();

        /**USART2 GPIO Configuration
        PA02     ------> USART2_TX
        PA03     ------> USART2_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_2 | GPIO_PIN_3);

        /* USART2 DMA DeInit */
        HAL_DMA_DeInit(huart->hdmarx);
    }
    else if (huart->Instance == USART3)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART3_CLK_DISABLE();

        /**USART3 GPIO Configuration
        PB10     ------> USART3_TX
        PB11     ------> USART3_RX
        */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10 | GPIO_PIN_11);

        /* USART3 DMA DeInit */
        HAL_DMA_DeInit(huart->hdmarx);
    }
    else if (huart->Instance == USART4)
    {
        /* Peripheral clock disable */
        __HAL_RCC_USART4_CLK_DISABLE();

        /**USART4 GPIO Configuration
        PA00     ------> USART4_TX
        PA01     ------> USART4_RX
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0 | GPIO_PIN_1);

        /* USART4 interrupt DeInit */
        HAL_NVIC_DisableIRQ(USART3_4_IRQn);
    }
}

void HAL_UART_TxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // pxMBFrameCBTransmitterEmpty();
    }
    else if (huart->Instance == USART2)
    {
    }
    else if (huart->Instance == USART3)
    {
    }
    else if (huart->Instance == USART4)
    {
    }
}

void HAL_UART_RxHalfCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
    }
    else if (huart->Instance == USART2)
    {
    }
    else if (huart->Instance == USART3)
    {
    }
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
{
    if (huart->Instance == USART1)
    {
        // Uart (USB), 9600
        // #if ((__USE_MODBUS_PARSER_ == 1) && (__USE_MODE_RS232_ == 1))
        //         parser(buffer_usart_1[0]);
        //         parser_modbus(buffer_usart_1[0]);
        // #endif
    }
    else if (huart->Instance == USART2)
    {
        // Sensor
        // sensor_parser(buffer_usart_2[0]);
    }
    else if (huart->Instance == USART3)
    {
#if ((__USE_MODBUS_PARSER_ == 1) && (__USE_MODE_RS232_ == 1))

#endif
        // Debug
        // parser(buffer_usart_3[0]);
        // HAL_UART_Receive_IT(&huart3, (uint8_t *)buffer_usart_3, 1);
    }
    else if (huart->Instance == USART4)
    {
// RS485, 9600
#if ((__USE_MODBUS_PARSER_ == 1) && (__USE_MODE_RS485_ == 1))
        parser(buffer_usart_4[0]);
        parser_modbus(buffer_usart_4[0]);
#endif
    }
}

void HAL_UART_ErrorCallback(UART_HandleTypeDef *huart)
{
}

void HAL_UARTEx_WakeupCallback(UART_HandleTypeDef *huart)
{
}

static void usart_puts(UART_HandleTypeDef *handle, volatile char *s)
{
    while (*s)
    {
        HAL_UART_Transmit(handle, (uint8_t *)s, 1, 0xFFFF);
        while (__HAL_UART_GET_FLAG(handle, USART_ISR_BUSY) == SET)
            ;

        *s++;
    }
}

void uart1_transmit_string(const void *data)
{
    usart_puts(&huart1, (char *)data);
}

void uart2_transmit_string(const void *data)
{
    usart_puts(&huart2, (char *)data);
}

void uart3_transmit_string(const void *data)
{
    usart_puts(&huart3, (char *)data);
}

void uart4_transmit_string(const void *data)
{
    usart_puts(&huart4, (char *)data);
}

// Uart (USB), 9600
static void uart_1_config(void)
{
    huart1.Instance = USART1;
    huart1.Init.BaudRate = 9600;
    huart1.Init.WordLength = UART_WORDLENGTH_8B;
    huart1.Init.StopBits = UART_STOPBITS_1;
    huart1.Init.Parity = UART_PARITY_NONE;
    huart1.Init.Mode = UART_MODE_TX_RX;
    huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart1.Init.OverSampling = UART_OVERSAMPLING_16;
    huart1.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart1.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart1) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

// Senaor
static void uart_2_config(void)
{
    huart2.Instance = USART2;
    huart2.Init.BaudRate = 9600;
    huart2.Init.WordLength = UART_WORDLENGTH_8B;
    huart2.Init.StopBits = UART_STOPBITS_1;
    huart2.Init.Parity = UART_PARITY_NONE;
    huart2.Init.Mode = UART_MODE_TX_RX;
    huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart2.Init.OverSampling = UART_OVERSAMPLING_16;
    huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;

    if (HAL_UART_Init(&huart2) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

// Debug
static void uart_3_config(void)
{
    huart3.Instance = USART3;
    huart3.Init.BaudRate = 115200;
    huart3.Init.WordLength = UART_WORDLENGTH_8B;
    huart3.Init.StopBits = UART_STOPBITS_1;
    huart3.Init.Parity = UART_PARITY_NONE;
    huart3.Init.Mode = UART_MODE_TX_RX;
    huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart3.Init.OverSampling = UART_OVERSAMPLING_16;
    huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart3) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

// RS485
static void uart_4_config(void)
{
    huart4.Instance = USART4;
    huart4.Init.BaudRate = 9600;
    huart4.Init.WordLength = UART_WORDLENGTH_8B;
    huart4.Init.StopBits = UART_STOPBITS_1;
    huart4.Init.Parity = UART_PARITY_NONE;
    huart4.Init.Mode = UART_MODE_TX_RX;
    huart4.Init.HwFlowCtl = UART_HWCONTROL_NONE;
    huart4.Init.OverSampling = UART_OVERSAMPLING_16;
    huart4.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
    huart4.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
    if (HAL_UART_Init(&huart4) != HAL_OK)
    {
        _Error_Handler(__FILE__, __LINE__);
    }
}

/*
    PA09     ------> USART1_TX
    PA10     ------> USART1_RX

    PA02     ------> USART2_TX
    PA03     ------> USART2_RX

    PB10     ------> USART3_TX
    PB11     ------> USART3_RX

    PA00     ------> USART4_TX
    PA01     ------> USART4_RX

*/

void uart_interrupt_init(void)
{
    if (HAL_UART_Receive_DMA(&huart1, (uint8_t *)buffer_usart_1, 1) != HAL_OK)
    {
        /* Transfer error in reception process */
        _Error_Handler(__FILE__, __LINE__);
    }

    if (HAL_UART_Receive_DMA(&huart2, (uint8_t *)buffer_usart_2, 1) != HAL_OK)
    {
        /* Transfer error in reception process */
        _Error_Handler(__FILE__, __LINE__);
    }

    HAL_UART_Receive_IT(&huart3, (uint8_t *)buffer_usart_3, 1);

    if (HAL_UART_Receive_DMA(&huart4, (uint8_t *)buffer_usart_4, 1) != HAL_OK)
    {
        /* Transfer error in reception process */
        _Error_Handler(__FILE__, __LINE__);
    }
    // HAL_UART_Receive_IT(&huart4, (uint8_t *)buffer_usart_4, 1);
}

// 1 ms
void uart_handle(void)
{
    static uint16_t period_uart = 0;

    if (is_flag(flag_UART_04) == ENABLE)
    {
        if (period_uart++ >= 2)
        {
            period_uart = 0;

            flag_del(flag_UART_04);

            HAL_UART_Transmit(&huart4, &g_send_data[0], g_uart_04_index, 0xFFFF);
            rs485_output(DISABLE);
        }
    }
}

void uart_init(void)
{
    // Uart(USB), 9600
    uart_1_config();

    // Sensor, 9600
    uart_2_config();

    // Debug, 115200
    uart_3_config();

    // RS485, 9600
    uart_4_config();

    uart_interrupt_init();
}

// UART 1 (Uart - USB)
void send_packet_to_ble(uint8_t *pdata, uint8_t length)
{
    uint8_t data;

    // printf("-- send_packet_to_ble() : [");
    for (int i = 0; i < length; i++)
    {
        data = pdata[i];

        // printf("0x%02X ", data);
        HAL_UART_Transmit(&huart1, &data, 1, 0xFFFF);
        while (__HAL_UART_GET_FLAG(&huart1, USART_ISR_BUSY) == SET)
            ;
    }
    // printf("]\n");
}

void send_string_to_uart_1(uint8_t *p_buffer)
{
    // printf("-- RS485 : %s\n", p_buffer);
    uart1_transmit_string(p_buffer);
}

// UART 2 (Sensor)
void send_packet_to_sensor(uint8_t *pdata, uint8_t length)
{
    uint8_t data;

#if 0
    printf("-- send_packet_to_sensor() : [");
    for (int i=0; i<length; i++)
    {
        printf("0x%02X ", pdata[i]);
    }
    printf("]\n");
#endif

    for (int i = 0; i < length; i++)
    {
        data = pdata[i];

        HAL_UART_Transmit(&huart2, &data, 1, 0xFFFF);
        while (__HAL_UART_GET_FLAG(&huart2, USART_ISR_BUSY) == SET)
            ;
    }
}

// UART 3 (DEBUG)

// UART 4 (RS485)
void send_packet_to_rs485(t_packet *p_packet, uint8_t length)
{
    uint8_t check_sum = 0;
    uint8_t Index = 0;

    memset(g_send_data, '\0', sizeof(g_send_data));

    // Start of Sequence
    g_send_data[Index++] = PACKET_STX_1;
    g_send_data[Index++] = PACKET_STX_2;

    // Command
    g_send_data[Index++] = p_packet->command;

    // Payload Length
    g_send_data[Index++] = p_packet->length >> 8;
    g_send_data[Index++] = p_packet->length & 0xFF;

    // Payload (Data)
    for (int i = 0; i < p_packet->length; i++)
    {
        g_send_data[Index++] = p_packet->data[i];
        check_sum = check_sum ^ p_packet->data[i];
    }

    g_send_data[Index++] = check_sum;

    // End of Sequence
    g_send_data[Index++] = PACKET_ETX_1;
    g_send_data[Index++] = PACKET_ETX_2;

#if 0
    for (int i=0; i<Index; i++)
    {
        printf("[0x%02X]", g_send_data[i]);
    }
    printf("\n");
#endif

#if (__USE_MODE_RS232_ == 1)
    for (int i = 0; i < Index; i++)
    {
        HAL_UART_Transmit(&huart1, (uint8_t *)&g_send_data[i], 1, 0xFFFF);
    }
#endif

#if (__USE_MODE_RS485_ == 1)
    rs485_output(ENABLE);
    _delay_us(100);

    for (int i = 0; i < Index; i++)
    {
        HAL_UART_Transmit(&huart4, (uint8_t *)&g_send_data[i], 1, 0xFFFF);
    }

    rs485_output(DISABLE);
#endif
}

// UART 4 (RS485)
void send_string_to_rs485(uint8_t *p_buffer, uint8_t length)
{
#if (__USE_MODE_RS232_ == 1)
    uart1_transmit_string(p_buffer);
#endif

#if (__USE_MODE_RS485_ == 1)
    rs485_output(ENABLE);
    uart4_transmit_string(p_buffer);
    rs485_output(DISABLE);
#endif

    // printf("-- RS485 : %s\n", p_buffer);
}

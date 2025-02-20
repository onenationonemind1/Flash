#include "main.h"

HAL_StatusTypeDef WriteMultiDataToFlash(uint16_t *data, uint16_t count)
{
    HAL_FLASH_Unlock();

    // 페이지 지우기
    FLASH_EraseInitTypeDef EraseInit;
    EraseInit.TypeErase = FLASH_TYPEERASE_PAGES;
    EraseInit.PageAddress = DATA_FLASH_PAGE_ADDR;
    EraseInit.NbPages = 1;

    uint32_t PageError;
    if (HAL_FLASHEx_Erase(&EraseInit, &PageError) != HAL_OK)
    {
        HAL_FLASH_Lock();
        return HAL_ERROR;
    }

    // 여러 데이터 쓰기
    for (uint16_t i = 0; i < count; i++)
    {
        if (HAL_FLASH_Program(FLASH_TYPEPROGRAM_HALFWORD,
                              DATA_FLASH_PAGE_ADDR + (i * 2), // 2바이트씩 증가
                              data[i]) != HAL_OK)
        {
            HAL_FLASH_Lock();
            return HAL_ERROR;
        }
    }
    HAL_FLASH_Lock();
    return HAL_OK;
}

uint16_t ReadDataFromFlashAt(uint16_t index)
{
    return *(__IO uint16_t *)(DATA_FLASH_PAGE_ADDR + (index * 2));
}

void multiDatawrite(AM1002_Data_t data)
{
    uint16_t multiData[6] = {data.tvoc, data.pm1_0, data.pm2_5, data.pm10, (uint16_t)data.temperature, data.humidity};
    WriteMultiDataToFlash(multiData, 6);
#ifdef terminal_mode
    if (WriteMultiDataToFlash(multiData, 6) == HAL_OK)
    {
        HAL_UART_Transmit(&huart3, (uint8_t *)"Data write!!\r\n", 20, HAL_MAX_DELAY);

        for (int i = 0; i < 6; i++)
        {
            uint16_t readData = ReadDataFromFlashAt(i);
            char debug_msg[50];
            sprintf(debug_msg, "Read[%d]: 0x%02X\r\n", i, readData);
            HAL_UART_Transmit(&huart3, (uint8_t *)debug_msg, strlen(debug_msg), HAL_MAX_DELAY);
        }
    }
    else
    {
        HAL_UART_Transmit(&huart3, (uint8_t *)"Data write fail!!\r\n", 20, HAL_MAX_DELAY);
    }
#endif
}

AM1002_Data_t multiDataread(void)
{
    AM1002_Data_t data;
    uint16_t readData[6];

    for (int i = 0; i < 6; i++)
    {
        readData[i] = ReadDataFromFlashAt(i);
#if 0 // terminal_mode가 정의되어 있으면
        HAL_UART_Transmit(&huart3, (uint8_t *)"Read Data: ", 11, HAL_MAX_DELAY);
        char debug_msg[50];
        sprintf(debug_msg, "[%d] 0x%04X ", i, readData[i]); // 수정된 부분
        HAL_UART_Transmit(&huart3, (uint8_t *)debug_msg, strlen(debug_msg), HAL_MAX_DELAY);
#endif
    }
    // HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);

    data.tvoc = readData[0];
    data.pm1_0 = readData[1];
    data.pm2_5 = readData[2];
    data.pm10 = readData[3];
    data.temperature = readData[4];
    data.humidity = readData[5];

    return data;
}

void PrintCalibrationData(AM1002_Data_t data)
{
    char buffer[50];

    // TVOC는 그대로 출력
    sprintf(buffer, "TVOC: %d ppb\r\n", data.tvoc);
    HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

    // PM 값들도 그대로 출력
    sprintf(buffer, "PM1.0: %d ug/m3\r\n", data.pm1_0);
    HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

    sprintf(buffer, "PM2.5: %d ug/m3\r\n", data.pm2_5);
    HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

    sprintf(buffer, "PM10: %d ug/m3\r\n", data.pm10);
    HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

    // 온도는 10으로 나누어 소수점 표시
    int16_t temp_whole = data.temperature / 10;
    uint16_t temp_decimal = abs(data.temperature % 10);
    sprintf(buffer, "Temperature: %d.%d C\r\n", temp_whole, temp_decimal);
    HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

    // 습도도 10으로 나누어 소수점 표시
    sprintf(buffer, "Humidity: %d.%d %%\r\n", data.humidity / 10, data.humidity % 10);
    HAL_UART_Transmit(&huart3, (uint8_t *)buffer, strlen(buffer), HAL_MAX_DELAY);

    HAL_UART_Transmit(&huart3, (uint8_t *)"\r\n", 2, HAL_MAX_DELAY);
}

#include "main.h"

uint8_t g_timer_ms_1000 = DISABLE;

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM2)
    {
        if(g_timer_ms_1000 == DISABLE)
        {
            g_timer_ms_1000 = ENABLE;
        }
    }
}
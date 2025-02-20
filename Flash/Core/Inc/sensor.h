#ifndef __SENSOR_H
#define __SENSOR_H

#include "main.h"
#include <stdbool.h> // bool 타입을 사용하기 위해 필요

typedef struct
{
    uint32_t pm_01_0;
    uint32_t pm_02_5;
    uint32_t pm_10_0;
} t_dust;

typedef struct
{
    uint32_t pc_00_3;
    uint32_t pc_00_5;
    uint32_t pc_01_0;
    uint32_t pc_02_5;
    uint32_t pc_05_0;
    uint32_t pc_10_0;
} t_particle;

typedef struct
{
    uint32_t data;
} t_co2, t_tvoc;

typedef struct
{
    int32_t temperature;
    uint32_t humidity;
} t_t_h;

typedef struct
{
    t_dust dust;
    t_particle particle;
    t_co2 co2;
    t_tvoc tvoc;
    t_t_h t_h;
} t_sensor_data;

extern t_sensor_data g_sensor_current;

#endif // __SENSOR_H

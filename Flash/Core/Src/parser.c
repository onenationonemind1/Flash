#include "main.h"

static uint8_t g_parser_state = P_STATE_SOM;

uint8_t g_send_type = SEND_TYPE_MAX;
uint8_t g_send_period = SEND_PERIOD_SEC_01;
uint16_t g_send_time = PERIOD_SEC_01;

uint8_t g_rs485_command = PACKET_TO_PC_MAX;

uint16_t g_period_parser = 0;
t_packet g_packet_receive = {
    0,
};
t_packet g_packet_transmit = {
    0,
};

uint32_t _offset(float data)
{
    float ret_data = 0;
    int i = 0;
    uint8_t found = DISABLE;

    if (g_setup.k_factor_index == 0)
    {
        ret_data = data;
    }
    else
    {
        for (i = 0; i < g_setup.k_factor_index; i++)
        {
            if ((g_factor[i][FACTOR_LOW].f <= data) && (data < g_factor[i][FACTOR_HIGH].f))
            {
                ret_data = (data * g_factor[i][FACTOR_MULTIPLE].f) + g_factor[i][FACTOR_PLUS].f;
#if 0
                printf("-- section [%.2f][%.2f]-[%.2f][%.2f]:[%.2f][%.2f]--[%d]\n", g_factor[i][FACTOR_LOW].f, g_factor[i][FACTOR_HIGH].f,
                                                                                    g_factor[i][FACTOR_MULTIPLE].f, g_factor[i][FACTOR_PLUS].f,
                                                                                    data, ret_data, i);
#endif

                found = ENABLE;
                break;
            }
        }

        if (found == DISABLE)
        {
            i = (g_setup.k_factor_index - 1);
            ret_data = (data * g_factor[i][FACTOR_MULTIPLE].f) + g_factor[i][FACTOR_PLUS].f;
        }
    }

    return (uint32_t)ret_data;
}

uint32_t _offset_2(float data)
{
    float ret_data = 0;
    int i = 0;
    uint8_t found = DISABLE;

    if (g_setup.k_factor_index == 0)
    {
        ret_data = data;
    }
    else
    {
        for (i = 0; i < g_setup.k_factor_index; i++)
        {
            if ((g_factor[i][FACTOR_LOW].f <= data) && (data < g_factor[i][FACTOR_HIGH].f))
            {
                ret_data = (data * g_factor[i][FACTOR_MULTIPLE].f) + g_factor[i][FACTOR_PLUS].f;
#if 0
                printf("-- section [%.2f][%.2f]-[%.2f][%.2f]:[%.2f][%.2f]--[%d]\n", g_factor[i][FACTOR_LOW].f, g_factor[i][FACTOR_HIGH].f,
                                                                                    g_factor[i][FACTOR_MULTIPLE].f, g_factor[i][FACTOR_PLUS].f,
                                                                                    data, ret_data, i);
#endif

                found = ENABLE;
                break;
            }
        }

        if (found == DISABLE)
        {
            i = (g_setup.k_factor_index - 1);
            ret_data = (data * g_factor[i][FACTOR_MULTIPLE].f) + g_factor[i][FACTOR_PLUS].f;
        }
    }

    return (uint32_t)ret_data;
}

// ��� 1, 2
#define __USE_SENSOR_1_ 1
#define __USE_SENSOR_2_ 0
#define __USE_SENSOR_3_ 0

uint32_t _offset_dust(float data)
{
    float ret_data = 0;

    /*
        ����	��	���ϱ�
             X < 10	    0.7	    4
        10 < X < 14	    0.7	    7
        15 < X < 18	    0.7	    11
        19 < X < 26	    0.75	7
        27 < X < 30	    0.7	    9
        31 < X	        0.75	12

        ����	��	���ϱ�
             X < 10	    0.7	    6
        10 < X < 14	    0.7	    8
        15 < X < 18	    0.7	    11
        19 < X < 26	    0.75	9
        27 < X < 30	    0.7	    11
        31 < X	        0.75	12
    */

#if (__USE_SENSOR_1_ == 1)
    if ((0.0 <= data) && (data <= 4.0))
    {
        ret_data = (data * 1.00) + 0.0;
    }
    if ((4.0 < data) && (data <= 9.0))
    {
        ret_data = (data * 0.70) + 3.0;
    }
    else if ((9.0 < data) && (data <= 14.0))
    {
        ret_data = (data * 0.70) + 7.0;
    }
    else if ((14.0 < data) && (data <= 18.0))
    {
        ret_data = (data * 0.70) + 11.0;
    }
    else if ((18.0 < data) && (data <= 26.0))
    {
        ret_data = (data * 0.75) + 7.0;
    }
    else if ((26.0 < data) && (data <= 30.0))
    {
        ret_data = (data * 0.70) + 9.0;
    }
    else if ((30.0 < data))
    {
        ret_data = (data * 0.75) + 12.0;
    }
#endif

#if (__USE_SENSOR_2_ == 1)
    if ((0.0 <= data) && (data <= 9.0))
    {
        ret_data = (data * 0.70) + 4.0;
    }
    else if ((9.0 < data) && (data <= 14.0))
    {
        ret_data = (data * 0.70) + 7.0;
    }
    else if ((14.0 < data) && (data <= 18.0))
    {
        ret_data = (data * 0.70) + 11.0;
    }
    else if ((18.0 < data) && (data <= 26.0))
    {
        ret_data = (data * 0.75) + 7.0;
    }
    else if ((26.0 < data) && (data <= 30.0))
    {
        ret_data = (data * 0.70) + 9.0;
    }
    else if ((30.0 < data))
    {
        ret_data = (data * 0.75) + 12.0;
    }
#endif

#if (__USE_SENSOR_3_ == 1)
    if ((0.0 <= data) && (data <= 9.0))
    {
        ret_data = (data * 0.70) + 6.0;
    }
    else if ((9.0 < data) && (data <= 14.0))
    {
        ret_data = (data * 0.70) + 8.0;
    }
    else if ((14.0 < data) && (data <= 18.0))
    {
        ret_data = (data * 0.70) + 11.0;
    }
    else if ((18.0 < data) && (data <= 26.0))
    {
        ret_data = (data * 0.75) + 9.0;
    }
    else if ((26.0 < data) && (data <= 30.0))
    {
        ret_data = (data * 0.70) + 11.0;
    }
    else if ((30.0 < data))
    {
        ret_data = (data * 0.75) + 12.0;
    }
#endif

    return (uint32_t)ret_data;
}

uint32_t cal_temperature(int32_t temperature)
{
    uint32_t data_32 = DISABLE;

    if (temperature < 0)
    {
        data_32 = (uint32_t)((temperature * -1) * 10);
        data_32 |= 0x80000000;
    }
    else
    {
        data_32 = (uint32_t)(temperature * 10);
    }

    return data_32;
}

void parser_transmit(uint8_t command, uint8_t parameter)
{
    uint8_t index = 0;
    uint8_t rs485_string = DISABLE;
    uint32_t data_32 = 0;

    switch (command)
    {
    case PACKET_TO_PC_00:
    {
        /* 0x31 : System Info */

        // version
        g_packet_transmit.data[index++] = VERSION_MAJOR;
        g_packet_transmit.data[index++] = VERSION_MINOR;
        g_packet_transmit.data[index++] = VERSION_REVISION;

        // type
        g_packet_transmit.data[index++] = g_send_type;

        // period
        g_packet_transmit.data[index++] = g_send_period;
    }
    break;

    case PACKET_TO_PC_01:
    {
        /* 0x32 : Version */

        // Version
        g_packet_transmit.data[index++] = VERSION_MAJOR;
        g_packet_transmit.data[index++] = VERSION_MINOR;
        g_packet_transmit.data[index++] = VERSION_REVISION;
    }
    break;

    case PACKET_TO_PC_02:
    {
        /* 0x33 : K Factor */

        // K Factor Index
        g_packet_transmit.data[index++] = g_setup.k_factor_index;

        // K Factor
        for (int i = 0; i < K_FACTOR_MAX; i++)
        {
            g_packet_transmit.data[index++] = (g_factor[i][0].u & 0xFF000000) >> 24;
            g_packet_transmit.data[index++] = (g_factor[i][0].u & 0x00FF0000) >> 16;
            g_packet_transmit.data[index++] = (g_factor[i][0].u & 0x0000FF00) >> 8;
            g_packet_transmit.data[index++] = (g_factor[i][0].u & 0x000000FF);

            g_packet_transmit.data[index++] = (g_factor[i][1].u & 0xFF000000) >> 24;
            g_packet_transmit.data[index++] = (g_factor[i][1].u & 0x00FF0000) >> 16;
            g_packet_transmit.data[index++] = (g_factor[i][1].u & 0x0000FF00) >> 8;
            g_packet_transmit.data[index++] = (g_factor[i][1].u & 0x000000FF);

            g_packet_transmit.data[index++] = (g_factor[i][2].u & 0xFF000000) >> 24;
            g_packet_transmit.data[index++] = (g_factor[i][2].u & 0x00FF0000) >> 16;
            g_packet_transmit.data[index++] = (g_factor[i][2].u & 0x0000FF00) >> 8;
            g_packet_transmit.data[index++] = (g_factor[i][2].u & 0x000000FF);

            g_packet_transmit.data[index++] = (g_factor[i][3].u & 0xFF000000) >> 24;
            g_packet_transmit.data[index++] = (g_factor[i][3].u & 0x00FF0000) >> 16;
            g_packet_transmit.data[index++] = (g_factor[i][3].u & 0x0000FF00) >> 8;
            g_packet_transmit.data[index++] = (g_factor[i][3].u & 0x000000FF);
        }
    }
    break;

    case PACKET_TO_PC_03:
    {
        /* 0x34 : PM 2.5 */

        // PM 2.5
        data_32 = g_sensor_current.dust.pm_02_5;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);
    }
    break;

    case PACKET_TO_PC_04:
    {
        /* 0x35 : PM(Current) */

        // PM 1.0
        data_32 = g_sensor_current.dust.pm_01_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // PM 2.5
        data_32 = g_sensor_current.dust.pm_02_5;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // PM 10
        data_32 = g_sensor_current.dust.pm_10_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);
    }
    break;

    case PACKET_TO_PC_05:
    {
        /* 0x36 : PM(Current) + Temperature + Humidity */

        // PM 1.0
        data_32 = g_sensor_current.dust.pm_01_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // PM 2.5
        data_32 = g_sensor_current.dust.pm_02_5;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // PM 10
        data_32 = g_sensor_current.dust.pm_10_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // TVOC
        data_32 = g_sensor_current.tvoc.data;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // Temperature
        data_32 = cal_temperature(g_sensor_current.t_h.temperature);
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // Humidity
        data_32 = (g_sensor_current.t_h.humidity * 10);
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);
    }
    break;

    case PACKET_TO_PC_06:
    {
        /* 0x37 : PM (Current, Average) + Temperature + Humidity */

        // Current
        data_32 = g_sensor_current.dust.pm_01_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        data_32 = g_sensor_current.dust.pm_02_5;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        data_32 = g_sensor_current.dust.pm_10_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // 10�� ���
        data_32 = g_sensor_average_sec_10.dust.pm_01_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        data_32 = g_sensor_average_sec_10.dust.pm_02_5;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        data_32 = g_sensor_average_sec_10.dust.pm_10_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // 5�� ���
        data_32 = g_sensor_average_min_05.dust.pm_01_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        data_32 = g_sensor_average_min_05.dust.pm_02_5;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        data_32 = g_sensor_average_min_05.dust.pm_10_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // 60�� ���
        data_32 = g_sensor_average_min_60.dust.pm_01_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        data_32 = g_sensor_average_min_60.dust.pm_02_5;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        data_32 = g_sensor_average_min_60.dust.pm_10_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // Temperature
        data_32 = cal_temperature(g_sensor_current.t_h.temperature);
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // Humidity
        data_32 = (g_sensor_current.t_h.humidity * 10);
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);
    }
    break;

    case PACKET_TO_PC_07:
    {
        /* 0x38 : Particle */

        // > 0.3 um particle quantity, unit: pcs/L
        data_32 = g_sensor_current.particle.pc_00_3;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // > 0.5 um particle quantity, unit: pcs/L
        data_32 = g_sensor_current.particle.pc_00_5;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // > 1.0 um particle quantity, unit: pcs/L
        data_32 = g_sensor_current.particle.pc_01_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // > 2.5 um particle quantity, unit: pcs/L
        data_32 = g_sensor_current.particle.pc_02_5;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // > 5.0 um particle quantity, unit: pcs/L
        data_32 = g_sensor_current.particle.pc_05_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // >10.0 um particle quantity, unit: pcs/L
        data_32 = g_sensor_current.particle.pc_10_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // Temperature
        data_32 = cal_temperature(g_sensor_current.t_h.temperature);
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // Humidity
        data_32 = (g_sensor_current.t_h.humidity * 10);
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);
    }
    break;

    case PACKET_TO_PC_08:
    {
        /* 0x39 : �̼����� ���� */

        // Type
        g_packet_transmit.data[index++] = parameter;

        // Current
        data_32 = _offset_dust(g_sensor_current.dust.pm_02_5);
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // 10�� ���
        data_32 = _offset_dust(g_sensor_average_sec_10.dust.pm_02_5);
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // 5�� ���
        data_32 = _offset_dust(g_sensor_average_min_05.dust.pm_02_5);
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // 1�ð� ���
        data_32 = _offset_dust(g_sensor_average_min_60.dust.pm_02_5);
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);
    }
    break;

    case PACKET_TO_PC_09:
    {
        /* 0x40 : KC ����, RS485 */

        sprintf((char *)g_packet_transmit.data, "PM 1.0 : %5d, PM 2.5 : %5d, PM 10 : %5d, Temperature : %3d.%d, Humidity : %3d.%d",
                g_sensor_current.dust.pm_01_0,
                g_sensor_current.dust.pm_02_5,
                g_sensor_current.dust.pm_10_0,
                g_sensor_current.t_h.temperature / 10,
                g_sensor_current.t_h.temperature % 10,
                g_sensor_current.t_h.humidity / 10,
                g_sensor_current.t_h.humidity % 10);

        rs485_string = ENABLE;
    }
    break;

    case PACKET_TO_PC_10:
    {
    }
    break;

    case PACKET_TO_PC_11:
    {
    }
    break;

    case PACKET_TO_PC_12:
    {
    }
    break;

    case PACKET_TO_PC_13:
    {
#if 0
            g_packet_transmit.data[index++] = parameter;

            if (parameter == NETWORK_IP)
            {
                // IP
                g_packet_transmit.data[index++] = g_network.ip.addr[0][0];
                g_packet_transmit.data[index++] = g_network.ip.addr[0][1];
                g_packet_transmit.data[index++] = g_network.ip.addr[0][2];
                g_packet_transmit.data[index++] = g_network.ip.addr[0][3];

                g_packet_transmit.data[index++] = g_network.ip.addr[1][0];
                g_packet_transmit.data[index++] = g_network.ip.addr[1][1];
                g_packet_transmit.data[index++] = g_network.ip.addr[1][2];
                g_packet_transmit.data[index++] = g_network.ip.addr[1][3];

                g_packet_transmit.data[index++] = g_network.ip.addr[2][0];
                g_packet_transmit.data[index++] = g_network.ip.addr[2][1];
                g_packet_transmit.data[index++] = g_network.ip.addr[2][2];
                g_packet_transmit.data[index++] = g_network.ip.addr[2][3];

                g_packet_transmit.data[index++] = g_network.ip.addr[3][0];
                g_packet_transmit.data[index++] = g_network.ip.addr[3][1];
                g_packet_transmit.data[index++] = g_network.ip.addr[3][2];
                g_packet_transmit.data[index++] = g_network.ip.addr[3][3];

                // Gateway
                g_packet_transmit.data[index++] = g_network.gateway.addr[0][0];
                g_packet_transmit.data[index++] = g_network.gateway.addr[0][1];
                g_packet_transmit.data[index++] = g_network.gateway.addr[0][2];
                g_packet_transmit.data[index++] = g_network.gateway.addr[0][3];

                g_packet_transmit.data[index++] = g_network.gateway.addr[1][0];
                g_packet_transmit.data[index++] = g_network.gateway.addr[1][1];
                g_packet_transmit.data[index++] = g_network.gateway.addr[1][2];
                g_packet_transmit.data[index++] = g_network.gateway.addr[1][3];

                g_packet_transmit.data[index++] = g_network.gateway.addr[2][0];
                g_packet_transmit.data[index++] = g_network.gateway.addr[2][1];
                g_packet_transmit.data[index++] = g_network.gateway.addr[2][2];
                g_packet_transmit.data[index++] = g_network.gateway.addr[2][3];

                g_packet_transmit.data[index++] = g_network.gateway.addr[3][0];
                g_packet_transmit.data[index++] = g_network.gateway.addr[3][1];
                g_packet_transmit.data[index++] = g_network.gateway.addr[3][2];
                g_packet_transmit.data[index++] = g_network.gateway.addr[3][3];

                // Netmask
                g_packet_transmit.data[index++] = g_network.netmask.addr[0][0];
                g_packet_transmit.data[index++] = g_network.netmask.addr[0][1];
                g_packet_transmit.data[index++] = g_network.netmask.addr[0][2];
                g_packet_transmit.data[index++] = g_network.netmask.addr[0][3];

                g_packet_transmit.data[index++] = g_network.netmask.addr[1][0];
                g_packet_transmit.data[index++] = g_network.netmask.addr[1][1];
                g_packet_transmit.data[index++] = g_network.netmask.addr[1][2];
                g_packet_transmit.data[index++] = g_network.netmask.addr[1][3];

                g_packet_transmit.data[index++] = g_network.netmask.addr[2][0];
                g_packet_transmit.data[index++] = g_network.netmask.addr[2][1];
                g_packet_transmit.data[index++] = g_network.netmask.addr[2][2];
                g_packet_transmit.data[index++] = g_network.netmask.addr[2][3];

                g_packet_transmit.data[index++] = g_network.netmask.addr[3][0];
                g_packet_transmit.data[index++] = g_network.netmask.addr[3][1];
                g_packet_transmit.data[index++] = g_network.netmask.addr[3][2];
                g_packet_transmit.data[index++] = g_network.netmask.addr[3][3];
            }
            else if (parameter == NETWORK_TCP)
            {
                // IP
                g_packet_transmit.data[index++] = g_network.tcp.addr[0][0];
                g_packet_transmit.data[index++] = g_network.tcp.addr[0][1];
                g_packet_transmit.data[index++] = g_network.tcp.addr[0][2];
                g_packet_transmit.data[index++] = g_network.tcp.addr[0][3];

                g_packet_transmit.data[index++] = g_network.tcp.addr[1][0];
                g_packet_transmit.data[index++] = g_network.tcp.addr[1][1];
                g_packet_transmit.data[index++] = g_network.tcp.addr[1][2];
                g_packet_transmit.data[index++] = g_network.tcp.addr[1][3];

                g_packet_transmit.data[index++] = g_network.tcp.addr[2][0];
                g_packet_transmit.data[index++] = g_network.tcp.addr[2][1];
                g_packet_transmit.data[index++] = g_network.tcp.addr[2][2];
                g_packet_transmit.data[index++] = g_network.tcp.addr[2][3];

                g_packet_transmit.data[index++] = g_network.tcp.addr[3][0];
                g_packet_transmit.data[index++] = g_network.tcp.addr[3][1];
                g_packet_transmit.data[index++] = g_network.tcp.addr[3][2];
                g_packet_transmit.data[index++] = g_network.tcp.addr[3][3];

                // Domain

                // Port
                g_packet_transmit.data[index++] = g_network.port[0];
                g_packet_transmit.data[index++] = g_network.port[1];
                g_packet_transmit.data[index++] = g_network.port[2];
                g_packet_transmit.data[index++] = g_network.port[3];
                g_packet_transmit.data[index++] = g_network.port[4];
                g_packet_transmit.data[index++] = g_network.port[5];
            }
            else if (parameter == NETWORK_ROUTER)
            {
                // IP
                g_packet_transmit.data[index++] = g_router.ip.addr[0][0];
                g_packet_transmit.data[index++] = g_router.ip.addr[0][1];
                g_packet_transmit.data[index++] = g_router.ip.addr[0][2];
                g_packet_transmit.data[index++] = g_router.ip.addr[0][3];

                g_packet_transmit.data[index++] = g_router.ip.addr[1][0];
                g_packet_transmit.data[index++] = g_router.ip.addr[1][1];
                g_packet_transmit.data[index++] = g_router.ip.addr[1][2];
                g_packet_transmit.data[index++] = g_router.ip.addr[1][3];

                g_packet_transmit.data[index++] = g_router.ip.addr[2][0];
                g_packet_transmit.data[index++] = g_router.ip.addr[2][1];
                g_packet_transmit.data[index++] = g_router.ip.addr[2][2];
                g_packet_transmit.data[index++] = g_router.ip.addr[2][3];

                g_packet_transmit.data[index++] = g_router.ip.addr[3][0];
                g_packet_transmit.data[index++] = g_router.ip.addr[3][1];
                g_packet_transmit.data[index++] = g_router.ip.addr[3][2];
                g_packet_transmit.data[index++] = g_router.ip.addr[3][3];

                // SSID
                for (int i=0; i<MAX_ROUTER_SSID; i++)
                {
                    g_packet_transmit.data[index++] = g_router.ssid[i];
                }

                // Password
                for (int i=0; i<MAX_ROUTER_PASSWORD; i++)
                {
                    g_packet_transmit.data[index++] = g_router.password[i];
                }
            }
            else
            {
            }
#endif
    }
    break;

    case PACKET_TO_PC_14:
    {
    }
    break;

    case PACKET_TO_PC_15:
    {
        /* 0x40 : Device ID */

        // Target ID
        g_packet_transmit.data[index++] = parameter;

        // Sender ID
        g_packet_transmit.data[index++] = g_setup.device_id;
    }
    break;

    case PACKET_TO_PC_16:
    {
        /* 0x41 : Datetime */

        // Target ID
        g_packet_transmit.data[index++] = parameter;

        // Sender ID
        g_packet_transmit.data[index++] = g_setup.device_id;
    }
    break;

    case PACKET_TO_PC_17:
    {
        /* 0x42 : Sensor Data */

        // Dust
        data_32 = g_sensor_output.dust.pm_01_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        data_32 = g_sensor_output.dust.pm_02_5;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        data_32 = g_sensor_output.dust.pm_10_0;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // Tvoc
        data_32 = g_sensor_output.tvoc.data;
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // Temperature
        data_32 = cal_temperature(g_sensor_output.t_h.temperature);
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        // Humidity
        data_32 = (g_sensor_output.t_h.humidity * 10);
        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
        g_packet_transmit.data[index++] = (data_32 & 0x00FF0000) >> 16;
        g_packet_transmit.data[index++] = (data_32 & 0x0000FF00) >> 8;
        g_packet_transmit.data[index++] = (data_32 & 0x000000FF);

        printf("----------------------\n");
        printf("-- [%5d, %5d, %5d], [%5d], [%5d][%5d]\n", g_sensor_current.dust.pm_01_0,
               g_sensor_current.dust.pm_02_5,
               g_sensor_current.dust.pm_10_0,
               g_sensor_current.tvoc.data,
               g_sensor_current.t_h.temperature,
               g_sensor_current.t_h.humidity);

        printf("-- [%5d, %5d, %5d], [%5d], [%5d][%5d]\n", g_sensor_output.dust.pm_01_0,
               g_sensor_output.dust.pm_02_5,
               g_sensor_output.dust.pm_10_0,
               g_sensor_output.tvoc.data,
               g_sensor_output.t_h.temperature,
               g_sensor_output.t_h.humidity);
    }
    break;

    case PACKET_TO_PC_24:
    {
        // PM 01.0
        // PM 02.5
        // PM 10.0
        // TVOC
        // Temperature
        // Humidity

        g_packet_transmit.data[index++] = (data_32 & 0xFF000000) >> 24;
    }
    break;
    }

    // command
    g_packet_transmit.command = command;

    // length
    g_packet_transmit.length = index;

    if (rs485_string == DISABLE)
    {
        send_packet_to_rs485(&g_packet_transmit, index);
    }
    else
    {
        send_string_to_rs485(g_packet_transmit.data, strlen((char *)g_packet_transmit.data));
    }
}

void parser_receive(uint8_t payload_length, uint8_t command, uint8_t *p_data)
{
    uint8_t index = 0;

    switch (command)
    {
    case PACKET_TO_TRANSMITTER_00:
    {
        parser_transmit(PACKET_TO_PC_01, 0);
    }
    break;

    case PACKET_TO_TRANSMITTER_01:
    {
        // Type
        g_send_type = p_data[index++];
        g_period_parser = 0;

        printf("-- type : [%d]\n", g_send_type);
    }
    break;

    case PACKET_TO_TRANSMITTER_02:
    {
        // Period
        g_send_period = p_data[index++];
        g_period_parser = 0;

        printf("-- Period [%d]\n", g_send_period);

        if (g_send_period == SEND_PERIOD_SEC_01)
        {
            g_send_time = PERIOD_SEC_01;
            printf("-- period : 01 Sec\n");
        }
        else if (g_send_period == SEND_PERIOD_SEC_02)
        {
            g_send_time = PERIOD_SEC_02;
            printf("-- period : 02 Sec\n");
        }
        else if (g_send_period == SEND_PERIOD_SEC_05)
        {
            g_send_time = PERIOD_SEC_05;
            printf("-- period : 05 Sec\n");
        }
        else if (g_send_period == SEND_PERIOD_SEC_10)
        {
            g_send_time = PERIOD_SEC_10;
            printf("-- period : 10 Sec\n");
        }
        else if (g_send_period == SEND_PERIOD_MIN_01)
        {
            g_send_time = PERIOD_MIN_01;
            printf("-- period : 01 Min\n");
        }
        else if (g_send_period == SEND_PERIOD_MIN_02)
        {
            g_send_time = PERIOD_MIN_02;
            printf("-- period : 02 Min\n");
        }
        else if (g_send_period == SEND_PERIOD_MIN_05)
        {
            g_send_time = PERIOD_MIN_05;
            printf("-- period : 05 Min\n");
        }
        else if (g_send_period == SEND_PERIOD_MIN_60)
        {
            g_send_time = PERIOD_MIN_60;
            printf("-- period : 60 Min\n");
        }
    }
    break;

    case PACKET_TO_TRANSMITTER_03:
    {
        parser_transmit(PACKET_TO_PC_02, 0);
    }
    break;

    case PACKET_TO_TRANSMITTER_04:
    {
        g_setup.k_factor_index = p_data[index++];

        for (int i = 0; i < K_FACTOR_MAX; i++)
        {
            g_factor[i][0].u = p_data[index++] << 24;
            g_factor[i][0].u += p_data[index++] << 16;
            g_factor[i][0].u += p_data[index++] << 8;
            g_factor[i][0].u += p_data[index++] << 0;

            g_factor[i][1].u = p_data[index++] << 24;
            g_factor[i][1].u += p_data[index++] << 16;
            g_factor[i][1].u += p_data[index++] << 8;
            g_factor[i][1].u += p_data[index++] << 0;

            g_factor[i][2].u = p_data[index++] << 24;
            g_factor[i][2].u += p_data[index++] << 16;
            g_factor[i][2].u += p_data[index++] << 8;
            g_factor[i][2].u += p_data[index++] << 0;

            g_factor[i][3].u = p_data[index++] << 24;
            g_factor[i][3].u += p_data[index++] << 16;
            g_factor[i][3].u += p_data[index++] << 8;
            g_factor[i][3].u += p_data[index++] << 0;
        }

        // setup_data_write();
        // setup_data_read();
    }
    break;

    case PACKET_TO_TRANSMITTER_05:
    {
    }
    break;

    case PACKET_TO_TRANSMITTER_17:
    {
        // Calibration TVOC  - Set
        g_rs485_command = PACKET_TO_PC_17;
    }
    break;
    }
}

uint8_t parser_command(uint8_t command)
{
    if ((PACKET_TO_TRANSMITTER_MIN < command) && (command < PACKET_TO_TRANSMITTER_MAX))
    {
        return ENABLE;
    }

    return DISABLE;
}

void parser(uint8_t data)
{
    static uint8_t index = 0;

    switch (g_parser_state)
    {
    case P_STATE_SOM:
    {
        if (data == PACKET_STX_1)
        {
            g_packet_receive.length = 0;
            g_packet_receive.command = 0;
            g_packet_receive.check_sum = 0;

            index = 0;
            g_parser_state = P_STATE_STX;
        }
        else
        {
            g_parser_state = P_STATE_SOM;
        }
    }
    break;

    case P_STATE_STX:
    {
        if (data == PACKET_STX_2)
        {
            g_parser_state = P_STATE_CMD;
        }
        else
        {
            g_parser_state = P_STATE_SOM;
        }
    }
    break;

    case P_STATE_CMD:
    {
        if (parser_command(data) == ENABLE)
        {
            g_packet_receive.command = data;
            g_parser_state = P_STATE_LENGTH_1;
        }
        else
        {
            g_parser_state = P_STATE_SOM;
        }
    }
    break;

    case P_STATE_LENGTH_1:
    {
        g_packet_receive.length = (data << 8);
        g_parser_state = P_STATE_LENGTH_2;
    }
    break;

    case P_STATE_LENGTH_2:
    {
        g_packet_receive.length += data;
        g_parser_state = P_STATE_DATA;
    }
    break;

    case P_STATE_DATA:
    {
        if (index < g_packet_receive.length)
        {
            g_packet_receive.data[index++] = data;
            g_packet_receive.check_sum ^= data;
        }
        else
        {
            if (g_packet_receive.check_sum == data)
            {
                g_parser_state = P_STATE_CHECK_SUM;
            }
            else
            {
                g_parser_state = P_STATE_SOM;
            }
        }
    }
    break;

    case P_STATE_CHECK_SUM:
    {
        if (data == PACKET_ETX_1)
        {
            g_parser_state = P_STATE_ETX;
        }
        else
        {
            g_parser_state = P_STATE_SOM;
        }
    }
    break;

    case P_STATE_ETX:
    {
        if (data == PACKET_ETX_2)
        {
            parser_receive(g_packet_receive.length, g_packet_receive.command, (uint8_t *)g_packet_receive.data);
        }

        g_parser_state = P_STATE_SOM;
    }
    break;

    default:
    {
        g_parser_state = P_STATE_SOM;
    }
    break;
    }
}

// 1 sec
void parser_handle(void)
{
    if (is_flag(flag_PARSER_HANDLE) == DISABLE)
    {
        flag_add(flag_PARSER_HANDLE);

        parser_transmit(PACKET_TO_PC_00, 0);
    }
    else
    {
        if (g_send_type < SEND_TYPE_10)
        {
            g_period_parser++;
            if (g_period_parser == g_send_time)
            {
                g_period_parser = 0;

                switch (g_send_type)
                {
                case SEND_TYPE_00:
                {
                    parser_transmit(PACKET_TO_PC_03, 0);
                }
                break;
                case SEND_TYPE_01:
                {
                    parser_transmit(PACKET_TO_PC_04, 0);
                }
                break;
                case SEND_TYPE_02:
                {
                    parser_transmit(PACKET_TO_PC_05, 0);
                }
                break;
                case SEND_TYPE_03:
                {
                    parser_transmit(PACKET_TO_PC_06, 0);
                }
                break;
                case SEND_TYPE_04: /* Reserved */
                case SEND_TYPE_05: /* Reserved */
                case SEND_TYPE_06:
                {
                    parser_transmit(PACKET_TO_PC_09, 0);
                }
                break;
                case SEND_TYPE_07:
                {
                    parser_transmit(PACKET_TO_PC_07, 0); /*parser_transmit(PACKET_TO_PC_10, 0);*/
                }
                break;
                    // case SEND_TYPE_06:  { send_data_particle(); } break;
                }
            }
        }
        else if (g_send_type == SEND_TYPE_10)
        {
            g_period_parser++;

            if ((g_period_parser % TIME_CETIFICATION_MIN_60) == 0)
            {
                // 60��
                parser_transmit(PACKET_TO_PC_08, SEND_PERIOD_MIN_60);

                g_period_parser = 0;
            }
            else if ((g_period_parser % TIME_CETIFICATION_MIN_05) == 0)
            {
                // 5 ��
                parser_transmit(PACKET_TO_PC_08, SEND_PERIOD_MIN_05);
            }
            else if ((g_period_parser % TIME_CETIFICATION_MIN_01) == 0)
            {
                // 1 ��
                parser_transmit(PACKET_TO_PC_08, SEND_PERIOD_MIN_01);
            }
        }
    }
}

// 1 ms
void parser_handle_rs485(void)
{
    static uint16_t period_parser_rs485 = 0;

    if (g_rs485_command != PACKET_TO_PC_MAX)
    {
        if (period_parser_rs485++ >= 50)
        {
            period_parser_rs485 = 0;

            parser_transmit(g_rs485_command, 0);
            g_rs485_command = PACKET_TO_PC_MAX;
        }
    }
}

void parser_init(void)
{
    printf("-- parser_init()\n");

#if 0
    for (int i=0; i<100; i+=10)
    {
        printf("-- [%d]-[%d]\n", i, _offset((float)i));
    }
#endif
}

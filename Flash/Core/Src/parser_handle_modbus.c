void parser_handle_modbus(void)
{
#if 0
    static uint16_t period_parser_id = 0;
    static uint16_t period_parser_modbus = 0;
    static uint16_t period_parser_send = 0;

    uint32_t data_32 = 0;
    if (g_parser_modbus_send_enable == ENABLE)
    {
        if (period_parser_send++ >= 10)
        {
            period_parser_send = 0;

            g_parser_modbus_send_enable = DISABLE;
            parser_modbus_transmit(g_modbus_data, g_modbus_index);
        }
    }

    if (++period_parser_id >= 100)
    {
        period_parser_id = 0;

        if (g_modbus_slave_address != g_holding_register_device[0])
        {
            g_modbus_slave_address = g_setup.device_id = g_holding_register_device[0];
            setup_data_write();
        }
    }

    if (++period_parser_modbus >= 1000)
    {
        period_parser_modbus = 0;

        // Dust
        g_holding_register_sensor[MODBUS_PM_01_0] = (uint16_t)g_sensor_output.dust.pm_01_0;
        g_holding_register_sensor[MODBUS_PM_02_5] = (uint16_t)g_sensor_output.dust.pm_02_5;
        g_holding_register_sensor[MODBUS_PM_10_0] = (uint16_t)g_sensor_output.dust.pm_10_0;

        // Co2
        g_holding_register_sensor[MODBUS_CO2] = (uint16_t)g_sensor_output.co2.data;

        // Tvoc
        g_holding_register_sensor[MODBUS_TVOC] = (uint16_t)g_sensor_output.tvoc.data;

        // Temperature
        g_holding_register_sensor[MODBUS_TEMPERATURE] = (uint16_t)(g_sensor_output.t_h.temperature / 10);
        if (g_temperature_minus == ENABLE)
            g_holding_register_sensor[MODBUS_TEMPERATURE] |= 0x8000;

        // Humidity
        g_holding_register_sensor[MODBUS_HUMIDITY] = (uint16_t)(g_sensor_output.t_h.humidity / 10);

        // Particle
        data_32 = (uint32_t)g_sensor_output.particle.pc_00_3;
        g_holding_register_sensor[MODBUS_PC_00_3_MSB] = (uint16_t)((data_32 & 0xFFFF0000) >> 16);
        g_holding_register_sensor[MODBUS_PC_00_3_LSB] = (uint16_t)((data_32 & 0x0000FFFF) >> 0);

        data_32 = (uint32_t)g_sensor_output.particle.pc_00_5;
        g_holding_register_sensor[MODBUS_PC_00_5_MSB] = (uint16_t)((data_32 & 0xFFFF0000) >> 16);
        g_holding_register_sensor[MODBUS_PC_00_5_LSB] = (uint16_t)((data_32 & 0x0000FFFF) >> 0);

        data_32 = (uint32_t)g_sensor_output.particle.pc_01_0;
        g_holding_register_sensor[MODBUS_PC_01_0_MSB] = (uint16_t)((data_32 & 0xFFFF0000) >> 16);
        g_holding_register_sensor[MODBUS_PC_01_0_LSB] = (uint16_t)((data_32 & 0x0000FFFF) >> 0);

        data_32 = (uint32_t)g_sensor_output.particle.pc_02_5;
        g_holding_register_sensor[MODBUS_PC_02_5_MSB] = (uint16_t)((data_32 & 0xFFFF0000) >> 16);
        g_holding_register_sensor[MODBUS_PC_02_5_LSB] = (uint16_t)((data_32 & 0x0000FFFF) >> 0);

        data_32 = (uint32_t)g_sensor_output.particle.pc_05_0;
        g_holding_register_sensor[MODBUS_PC_05_0_MSB] = (uint16_t)((data_32 & 0xFFFF0000) >> 16);
        g_holding_register_sensor[MODBUS_PC_05_0_LSB] = (uint16_t)((data_32 & 0x0000FFFF) >> 0);

        data_32 = (uint32_t)g_sensor_output.particle.pc_10_0;
        g_holding_register_sensor[MODBUS_PC_10_0_MSB] = (uint16_t)((data_32 & 0xFFFF0000) >> 16);
        g_holding_register_sensor[MODBUS_PC_10_0_LSB] = (uint16_t)((data_32 & 0x0000FFFF) >> 0);

#if 0
        printf("-- Device ID [%d]-----------------------------------\n", g_modbus_slave_address);
        printf("-- Sensor      : ");
        for (int i=0; i<HOLDING_REGISTER_QUANTITY_SENSOR; i++)
        {
            printf("%02X ", g_holding_register_sensor[i]);
        }
        printf("\n");

        printf("-- Device      : ");
        for (int i=0; i<HOLDING_REGISTER_QUANTITY_DEVICE; i++)
        {
            printf("%02X ", g_holding_register_device[i]);
        }
        printf("\n");

        printf("-- Information : ");
        for (int i=0; i<HOLDING_REGISTER_QUANTITY_INFORMATION; i++)
        {
            printf("%02X ", g_holding_register_information[i]);
        }
        printf("\n");
#endif
    }
#endif
}

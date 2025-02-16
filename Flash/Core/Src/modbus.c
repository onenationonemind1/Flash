#include "main.h"

uint16_t ModBus_CRC16(uint8_t *data, uint16_t length) {
  uint16_t crc = 0xFFFF;    // Initial value
  uint16_t i, j;
  
  for(i = 0; i < length; i++) {
      crc ^= data[i];       // XOR byte with CRC
      for(j = 0; j < 8; j++) {   // Process each bit
          if(crc & 0x0001) {     // If LSB is 1
              crc = (crc >> 1) ^ 0xA001;  // Shift right and XOR with polynomial
          } else {
              crc = crc >> 1;    // Just shift right
          }
      }
  }
  
  return crc;
}
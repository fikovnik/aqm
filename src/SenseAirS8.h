#ifndef SENSEAIRS8_H
#define SENSEAIRS8_H

#include <SoftwareSerial.h>
#include <cstdint>

class SenseAirS8 {
private:
  static byte const CMD_READ_CO2[];
  static byte const CMD_READ_CO2_W_STATUS[];

  SoftwareSerial comm;

  static void crc(byte buf[], int8_t len, byte crc[]);
  int8_t read_ir(int8_t reg, int8_t quantity, byte res[]);

public:
  SenseAirS8(int8_t rx_pin, int8_t tx_pin) : comm(rx_pin, tx_pin) {}
  void begin();
  int read_co2();
};

#endif // SENSEAIRS8_H

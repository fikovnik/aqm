#ifndef SENSEAIRS8_H
#define SENSEAIRS8_H

#include <SoftwareSerial.h>
#include <cstdint>

enum RegisterType {
  IR, // input register
  HR  // holding register
};

class SenseAirS8 {
private:
  SoftwareSerial comm;

  static void crc(byte buf[], int8_t len, byte crc[]);
  void send(byte cmd[], int8_t size);
  int8_t recv(byte res[], int8_t size);

  int8_t write(byte addr, byte val_low, byte val_high);
  int8_t read(RegisterType type, byte addr, byte quantity, byte res[]);

  int8_t read_hr(byte reg, byte quantity, byte res[]) {
    return read(HR, reg, quantity, res);
  }
  int8_t read_ir(byte reg, byte quantity, byte res[]) {
    return read(IR, reg, quantity, res);
  }

public:
  SenseAirS8(int8_t rx_pin, int8_t tx_pin) : comm(rx_pin, tx_pin) {}
  void begin();
  int read_status();
  int read_co2();
  int read_ABS();
  int calibrate();
};

#endif // SENSEAIRS8_H

#include <SenseAirS8.h>
#include <cstdint>
#include <cstring>
#include <debug.h>

byte const SenseAirS8::CMD_READ_CO2[] = {
  0xFE,       // any address
  0x04,       // read IR
  0x00, 0x03, // IR4 (3+1)
  0x00, 0x01, // 1 register
  0xD5, 0xC5  // CRC
};

byte const SenseAirS8::CMD_READ_CO2_W_STATUS[] = {
  0xFE,       // any address
  0x04,       // read IR
  0x00, 0x00, // IR1 (0+1)
  0x00, 0x04, // 4 registers
  0xE5, 0xC6  // CRC
};

void SenseAirS8::crc(byte buf[], int8_t len, byte crc[]) {
  unsigned int x = 0xFFFF;

  for (int pos = 0; pos < len; pos++) {
    x ^= (unsigned int)buf[pos];

    for (int i = 8; i != 0; i--) {
      // if the LSB is set
      if ((x & 0x0001) != 0) {
        x >>= 1;
        x ^= 0xA001;
      } else
        x >>= 1;
    }
  }

  // the CRC in x has low and high bytes swapped
  crc[0] = (byte)(x % 256);
  crc[1] = (byte)(x / 256);
}

void SenseAirS8::begin() {
  // 9600 is the max from the data sheet
  comm.begin(9600);
}

int8_t SenseAirS8::read_ir(int8_t reg, int8_t quantity, byte res[]) {
  // TODO check register number 0 -- 31
  // TODO check quantity accordingly
  byte cmd[8];
  cmd[0] = 0xFE; // any address
  cmd[1] = 0x04; // read IR
  cmd[2] = 0;
  cmd[3] = reg;
  cmd[4] = 0;
  cmd[5] = quantity;
  crc(cmd, 6, cmd+6);

  DEBUG("- sending: ");
  for (int j=0; j<8; j++) DEBUG("%x,", cmd[j]);
  DEBUG("\n");

  comm.write(cmd, 8);

  delay(250);

  while (!comm.available()) {
    DEBUG("- waiting...\n");
    delay(100);
  }

  DEBUG("- reading:\n");

  byte i = 0;
  // 1 (address), 1 (function code), 1 (size), 2*quantity (data), 2 (CRC)
  byte size = 1 + 1 + 1 + 2 * quantity + 2;
  byte *buf = new byte[size];

  while (comm.available()) {
    buf[i] = comm.read();
    i++;

    if (i > size) {
      DEBUG("!!! Too much data\n");
      delete[] buf;
      return 1;
    }
  }

  if (i != size) {
    DEBUG("!!! Expected %d bytes, got %d\n", size, i);
    delete[] buf;
    return 2;
  }

  DEBUG("\t- address: %x\n", buf[0]);
  DEBUG("\t- function: %x\n", buf[1]);
  DEBUG("\t- size: %x\n", buf[2]);
  DEBUG("\t- data: ");
  for (byte j=0; j<buf[2]; j++) DEBUG("%x,",buf[3+j]);
  DEBUG("\n");

  byte exp_crc[2];
  byte *act_crc = buf+size-2;
  crc(buf, size-2, exp_crc);

  DEBUG("\t- CRC %x,%x\n", act_crc[0], act_crc[1]);
  DEBUG("\t- expected CRC %x,%x\n", exp_crc[0], exp_crc[1]);

  if (memcmp(exp_crc, act_crc, 2)) {
    DEBUG("!!! CRC do not match\n");
    delete[] buf;
    return 3;
  }

  memcpy(res, buf+3, buf[2]);

  delete[] buf;
  return 0;
}

int SenseAirS8::read_co2() {
  byte res[2];

  int8_t status = read_ir(3, 1, res);
  if (status == 0) {
    int co2 = res[0] * 256 + res[1];
    DEBUG("co2: %5d\n", co2);

    return co2;
  } else {
    return -status;
  }

  // int status = result[3] * 256 + result[4];
  // int co2 = result[9] * 256 + result[10];
  // DEBUG("co2: %5d status: %d\n", co2, status);
}

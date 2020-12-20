#include <SenseAirS8.h>
#include <cstdint>
#include <cstring>
#include <debug.h>

/* Example commands:
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
*/

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

void SenseAirS8::send(byte cmd[], int8_t size) {
  int8_t payload_size = 1 + size + 2; // sensor address + size + CRC
  byte *payload = new byte[payload_size];

  payload[0] = 0xFE; // any address
  memcpy(payload + 1, cmd, size);
  crc(payload, size + 1, payload + size + 1);

  DEBUG("- sending: ");
  for (int j=0; j < payload_size; j++) {
    comm.write(payload[j]);
    DEBUG("%x,", payload[j]);
  }
  DEBUG("\n");

  delete[] payload;
  delay(250);
}

int8_t SenseAirS8::recv(byte res[], int8_t size) {
  while (!comm.available()) {
    DEBUG("- waiting...\n");
    delay(100);
  }

  DEBUG("- reading: ");

  byte i = 0;
  byte payload_size = 1 + size + 2; // sensor address + size + CRC
  byte *payload = new byte[payload_size];

  while (comm.available()) {
    payload[i] = comm.read();
    DEBUG("%x,", payload[i]);
    i++;

    if (i > payload_size) {
      DEBUG("\n!!! Too much data\n");
      delete[] payload;
      return -1;
    }
  }
  DEBUG("\n");

  if (i != payload_size) {
    DEBUG("!!! Expected %d bytes, got %d\n", payload_size, i);
    delete[] payload;
    return -1;
  }

  DEBUG("\t- address: %x\n", payload[0]);
  DEBUG("\t- data: ");
  for (byte j=0; j < size; j++) DEBUG("%x,", payload[1 + j]);
  DEBUG("\n");

  byte exp_crc[2];
  byte *act_crc = payload + payload_size - 2;
  crc(payload, payload_size - 2, exp_crc);

  DEBUG("\t- CRC %x,%x\n", act_crc[0], act_crc[1]);
  DEBUG("\t- expected CRC %x,%x\n", exp_crc[0], exp_crc[1]);

  if (memcmp(exp_crc, act_crc, 2)) {
    DEBUG("!!! CRC do not match\n");
    delete[] payload;
    return -2;
  }

  memcpy(res, payload + 1, size);

  delete[] payload;
  return 0;
}

int8_t SenseAirS8::write(byte addr, byte val_low, byte val_high) {
  byte cmd[5];
  cmd[0] = 0x06; // write single register
  cmd[1] = 0x00; // low address
  cmd[2] = addr;
  cmd[3] = val_low;
  cmd[4] = val_high;

  byte res[5];

  send(cmd, 5);

  int status = recv(res, 5);
  if (status) {
    return status;
  }

  // the response whould be the same
  if (!memcpy(res, cmd, 5)) {
    DEBUG("!!! Unexpected command reply\n");
    return status;
  }

  return 0;
}

int8_t SenseAirS8::read(RegisterType type, byte addr, byte quantity, byte res[]) {
  // TODO check register number 0 -- 31
  // TODO check quantity accordingly
  byte cmd[5];

  switch(type) {
    case IR: cmd[0] = 0x04; break;
    case HR: cmd[0] = 0x03; break;
  }
  cmd[1] = 0;
  cmd[2] = addr;
  cmd[3] = 0;
  cmd[4] = quantity;

  send(cmd, 5);

  byte size = 1 + 1 + 2 * quantity; // function code + size + data
  byte *buf = new byte[size];

  int status = recv(buf, size);
  if (status) {
    delete[] buf;
    return status;
  }

  DEBUG("\t- function: %x\n", buf[0]);
  DEBUG("\t- size: %x\n", buf[1]);

  memcpy(res, buf + 2, size - 2);
  delete[] buf;

  return 0;
}

int SenseAirS8::read_co2() {
  byte res[2];

  int8_t status = read_ir(3, 1, res);
  if (!status) {
    int co2 = res[0] * 256 + res[1];

    return co2;
  } else {
    return status;
  }

  // int status = result[3] * 256 + result[4];
  // int co2 = result[9] * 256 + result[10];
  // DEBUG("co2: %5d status: %d\n", co2, status);
}

int SenseAirS8::read_status() {
  byte res[2];

  int8_t status = read_ir(0, 1, res);
  if (!status) {
    return res[0] * 256 + res[1];
  } else {
    return status;
  }
}

int SenseAirS8::read_ABS() {
  byte res[2];

  int8_t status = read_hr(0x1F, 1, res);
  if (!status) {
    return res[0] * 256 + res[1];
  } else {
    return status;
  }
}

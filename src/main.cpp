#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>
#include <U8x8lib.h>
#include <cstdio>


SoftwareSerial s8_comm(D7, D8);
// byte read_co2[] = {0xFE, 0x04, 0x00, 0x03, 0x00, 0x01, 0xD5, 0xC5};
byte read_co2[] = {0xFE, 0x04, 0x00, 0x00, 0x00, 0x04, 0xE5, 0xC6};
void modbus_CRC(byte buf[], int len, byte crc[]);

int s8_read_co2();

// void s8_init() {
//   //byte ch, cmd[] = {0xFE, 0x6, 0x0, 0x1F, 0x0, 0xB4, 0xAC, 0x74}; // Set ABC Period to be 180 hours
//   //byte ch, cmd[] = {0xFE, 0x6, 0x0, 0x1F, 0x0, 0x30, 0xAC, 0x17}; // Set ABC Period to be 48 hours
//   byte ch, cmd[] = {0xFE, 0x6, 0x0, 0x1F, 0x0, 0x18, 0xAC, 0x09}; // Set ABC Period to be 24 hours
//   //s8_comm.enableTx(true);
//   s8_comm.write(cmd, 8);
//   //s8_comm.enableTx(false);
//   delay(250);

//   if (s8_comm.available()) {
//     Serial.print("SenseAir Response:");
//     while (s8_comm.available()) {
//       ch = (byte) s8_comm.read();
//       Serial.print(ch < 0x01 ? " 0" : " ");
//       Serial.print(ch, HEX);
//     }
//     Serial.println();
//   }
// }

U8X8_SSD1306_128X64_NONAME_SW_I2C u8x8(/* clock=*/ D5, /* data=*/ D2, /* reset=*/ U8X8_PIN_NONE);   // OLEDs without Reset of the Display

void setup(){
  Serial.begin(9600);

  s8_comm.begin(9600);
  u8x8.begin();
  u8x8.clearDisplay();
//  s8_init();
//

  // char time_output[30];

  //u8x8.setFont(u8x8_font_courB18_2x3_f);
  //u8x8.print("CO2: ");
  // sprintf(time_output, "%02d:%02d:%02d", localTime->tm_hour, localTime->tm_min, localTime->tm_sec);
  // u8x8.print(time_output);
}

#define SIZE 8
unsigned int m = 0;
unsigned int co2s[SIZE];

void loop(){
  u8x8.setFont(u8x8_font_chroma48medium8_r);

  co2s[m] = s8_read_co2();

  if (m == 0) {
    u8x8.clear();
  }

  u8x8.printf("%d: %d\n", m, co2s[m]);

  if (m + 1 == SIZE) {
    m = 0;
  } else {
    m++;
  }

//   //u8x8.drawString(1, 1, "CO2");
//   u8x8.printf("1\n2\n3\n4\n5\n6\n7\n8");
// //  u8x8.drawString(0,1,"");

//   byte crc[2] = {0,0};
//   modbus_CRC(read_co2, 6, crc);
//   Serial.printf("CRC: %x,%x\n", crc[0], crc[1]);
//   Serial.print("CO2 ppm = ");
//   Serial.println(co2);

  delay(15000);
}

int s8_read_co2() {
  Serial.println("- request");
  s8_comm.write(read_co2, 8);
  delay(250);
  
  while (!s8_comm.available()) {
    Serial.print("- waiting...");
    delay(100);
  }
  
  Serial.print("- response: ");

  byte result[20];
  int i = 0;
  while (s8_comm.available()) {
    byte ch = s8_comm.read();
    Serial.print(ch, HEX);
    Serial.print(",");
    
    result[i] = ch;
    i++;
  }
  Serial.println();

  int status = result[3] * 256 + result[4];
  int co2 = result[9] * 256 + result[10];
  Serial.printf("co2: %5d status: %d\n", co2, status);

  return co2;
}

void modbus_CRC(byte buf[], int len, byte crc[]) {
  unsigned int x = 0xFFFF;

  for (int pos = 0; pos < len; pos++) {
    x ^= (unsigned int) buf[pos];          // XOR byte into least sig. byte of crc

    for (int i = 8; i != 0; i--) {    // Loop over each bit
      if ((x & 0x0001) != 0) {      // If the LSB is set
        x >>= 1;                    // Shift right and XOR 0xA001
        x ^= 0xA001;
      }
      else                            // Else LSB is not set
        x >>= 1;                    // Just shift right
    }
  }

  // Note, this number has low and high bytes swapped, so use it accordingly (or swap bytes)
  crc[0] = (byte) (x % 256);
  crc[1] = (byte) (x / 256);
}

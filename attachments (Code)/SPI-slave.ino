#include <SPI.h>
#include <AES.h>

const int SS_PIN = 10;
#define PULSE_PIN 3

volatile unsigned long pulseTime = 0;
#define AES_BLOCK_SIZE 16

byte key[32] = {
  0x00, 0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07,
  0x08, 0x09, 0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F,
  0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17,
  0x18, 0x19, 0x1A, 0x1B, 0x1C, 0x1D, 0x1E, 0x1F
};

byte iv[16] = {
  0x20, 0x21, 0x22, 0x23, 0x24, 0x25, 0x26, 0x27,
  0x28, 0x29, 0x2A, 0x2B, 0x2C, 0x2D, 0x2E, 0x2F
};

byte receivedEncrypted[128];
volatile bool dataReceived = false;
AES aes;

void pulseISR() {
  pulseTime = micros();
}

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(PULSE_PIN, INPUT);
  attachInterrupt(digitalPinToInterrupt(PULSE_PIN), pulseISR, RISING);

  Serial.println("SPI Receiver with Decryption");

  SPI.begin();
  pinMode(SS_PIN, INPUT);
  SPCR |= _BV(SPE);
}

ISR(SPI_STC_vect) {
  static int byteIndex = 0;
  receivedEncrypted[byteIndex] = SPDR;
  byteIndex++;

  if (byteIndex >= 128) {
    byteIndex = 0;
    dataReceived = true;
  }

}

void loop() {

  for (int i = 0; i < 128; i++) {
    while(!(SPSR | _BV(SPIF)));
    receivedEncrypted[i] = SPI.transfer(0x00);
  }

  byte decrypted[156];
  int decryptedLength = decryptMessage(receivedEncrypted, 128, decrypted);

  unsigned long receiveTime = micros();
  unsigned long transmissionTime = receiveTime - pulseTime;
  Serial.print("Transmission Time: ");
  Serial.println(transmissionTime);

  if (decryptedLength > 0) {
    Serial.print("Decrypted message: ");
    Serial.println((char*)decrypted);
  } else {
    Serial.println("Decryption failed!");
  }
}

int decryptMessage(byte* input, int inputLength, byte* output) {
  byte local_iv[16];
  memcpy(local_iv, iv, 16);

  aes.set_key(key, sizeof(key));
  aes.cbc_decrypt(input, output, inputLength / AES_BLOCK_SIZE, local_iv);

  output[128] = 0;

  return 128;
}


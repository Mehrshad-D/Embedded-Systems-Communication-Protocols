#include <CAN.h>
#include <AES.h>

#define PULSE_PIN 3

// AES-256-CBC-PAD encryption
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

char message[128] = "This is a 128-character message. It needs to be exactly 128 characters long to fit the requirement. salam salam hamegi salam. b";

byte encrypted[256];
AES aes;

void setup() {
  Serial.begin(9600);
  while (!Serial);

  pinMode(PULSE_PIN, OUTPUT);
  digitalWrite(PULSE_PIN, LOW);

  Serial.println("CAN Sender with Encryption");

  if (!CAN.begin(500E3)) {
    Serial.println("Starting CAN failed!");
    while (1);
  }
}

void loop() {
  // Encrypt the message
  int encryptedLength = encryptMessage((byte*)message, strlen(message), encrypted);

  // Split the encrypted message into 8-byte chunks and send them
  digitalWrite(PULSE_PIN, HIGH);
  delayMicroseconds(100);
  digitalWrite(PULSE_PIN, LOW);

  Serial.println("Sending encrypted message in 8-byte chunks...");

  for (int i = 0; i < encryptedLength; i += 8) {
    CAN.beginPacket(0x12); // Use the same CAN ID for all chunks
    int chunkSize = min(8, encryptedLength - i);
    for (int j = 0; j < chunkSize; j++) {
      CAN.write(encrypted[i + j]);
    }
    CAN.endPacket();

    delay(10); // Small delay between frames
  }

  Serial.println("Message sent.");
  delay(1000);
}

int encryptMessage(byte* input, int inputLength, byte* output) {
  byte local_iv[16];
  memcpy(local_iv, iv, 16);

  int paddingLength = AES_BLOCK_SIZE - (inputLength % AES_BLOCK_SIZE);
  int paddedLength = inputLength + paddingLength;

  byte paddedMessage[paddedLength];
  memcpy(paddedMessage, input, inputLength);

  for (int i = inputLength; i < paddedLength; i++) {
    paddedMessage[i] = paddingLength;
  }

  aes.set_key(key, sizeof(key));
  aes.cbc_encrypt(paddedMessage, output, paddedLength / AES_BLOCK_SIZE, local_iv);

  return paddedLength;
}

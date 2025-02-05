#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <AES.h>

#define PULSE_PIN 3

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";
const char* serverIP = "192.168.1.100"; // Receiver's IP
const int serverPort = 8080;

WiFiClient client;

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

byte encrypted[128];
AES aes;

void setup() {
  Serial.begin(115200);

  pinMode(PULSE_PIN, OUTPUT);
  digitalWrite(PULSE_PIN, LOW);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
}

void loop() {
  if (client.connect(serverIP, serverPort)) {
    Serial.println("Connected to receiver!");

    digitalWrite(PULSE_PIN, HIGH);
    delayMicroseconds(100);
    digitalWrite(PULSE_PIN, LOW);
    
    int encryptedLength = encryptMessage((byte*)message, strlen(message), encrypted);
    unsigned long startTime = millis();
    
    client.write(encrypted, encryptedLength);
    client.stop();

    unsigned long endTime = millis();
    Serial.print("Transmission Time: ");
    Serial.println(endTime - startTime);

    Serial.println("Encrypted message sent!");
  } else {
    Serial.println("Connection failed!");
  }

  delay(5000);
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

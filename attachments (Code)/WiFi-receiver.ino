#include <ESP8266WiFi.h>
#include <WiFiServer.h>
#include <AES.h>

const char* ssid = "Your_SSID";
const char* password = "Your_PASSWORD";

WiFiServer server(8080);

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
AES aes;

void setup() {
  Serial.begin(115200);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }

  Serial.println("Connected to WiFi!");
  server.begin();
  Serial.println("Server started!");
}

void loop() {
  WiFiClient client = server.available();

  if (client) {
    Serial.println("Client connected!");
    unsigned long startTime = millis();
    int index = 0;

    while (client.connected()) {
      while (client.available()) {
        receivedEncrypted[index++] = client.read();
      }
    }

    client.stop();
    unsigned long endTime = millis();
    
    byte decrypted[128];
    int decryptedLength = decryptMessage(receivedEncrypted, index, decrypted);

    Serial.print("Transmission Time: ");
    Serial.println(endTime - startTime);

    if (decryptedLength > 0) {
      Serial.print("Decrypted message: ");
      Serial.println((char*)decrypted);
    } else {
      Serial.println("Decryption failed!");
    }
  }
}

int decryptMessage(byte* input, int inputLength, byte* output) {
  byte local_iv[16];
  memcpy(local_iv, iv, 16);

  aes.set_key(key, sizeof(key));
  aes.cbc_decrypt(input, output, inputLength / AES_BLOCK_SIZE, local_iv);

  int paddingLength = output[inputLength - 1];
  int decryptedLength = inputLength - paddingLength;

  output[decryptedLength] = 0;

  return decryptedLength;
}

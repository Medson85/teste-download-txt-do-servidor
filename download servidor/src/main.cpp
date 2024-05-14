#include <Arduino.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <SPIFFS.h>

const char *ssid = "SE COLABORADORES";
const char *password = "#novaenergia@";

// Declaração da função downloadFile
void downloadFile(const char *url, const char *fileName);

void setup() {
  Serial.begin(115200);
  pinMode(2, OUTPUT);

  // Inicializa o sistema de arquivos SPIFFS
  if (!SPIFFS.begin(true)) {
    Serial.println("SPIFFS Mount Failed");
    return;
  }

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi..");
  }

  Serial.println("Connected to WiFi");

  // Faz o download do arquivo
  downloadFile("http://172.16.1.99:3000/download", "/teste-uart.txt");
}

void loop() {
  if (WiFi.status() != WL_CONNECTED){
    digitalWrite(2, LOW);
  } else {
    digitalWrite(2, HIGH);
  }
}

/*void downloadFile(const char *url, const char *fileName) {
  HTTPClient http;

  // Inicia a conexão HTTP
  http.begin(url);

  int httpResponseCode = http.GET();

  if (httpResponseCode == HTTP_CODE_OK) {
    // Abre o arquivo para escrita
    File file = SPIFFS.open(fileName, FILE_WRITE);

    if (!file) {
      Serial.println("Failed to open file for writing");
      return;
    }

    // Lê o corpo da resposta e escreve no arquivo
    Stream *responseStream = http.getStreamPtr();
    while(responseStream->available()){
      file.write(responseStream->read());
    }

    // Fecha o arquivo
    file.close();

    Serial.println("File downloaded successfully");
  } else {
    Serial.print("HTTP request failed with error code: ");
    Serial.println(httpResponseCode);
  }

  // Fecha a conexão HTTP
  http.end();
}*/

void downloadFile(const char *url, const char *fileName) {
  HTTPClient http;

  // Inicia a conexão HTTP
  http.begin(url);

  int httpResponseCode = http.GET();

  if (httpResponseCode == HTTP_CODE_OK) {
    // Abre o arquivo para escrita
    File file = SPIFFS.open(fileName, FILE_WRITE);

    if (!file) {
      Serial.println("Failed to open file for writing");
      return;
    }

    // Lê o corpo da resposta e escreve no arquivo
    Stream *responseStream = http.getStreamPtr();
    while(responseStream->available()){
      file.write(responseStream->read());
    }

    // Fecha o arquivo
    file.close();

    // Abre o arquivo para leitura
    File readFile = SPIFFS.open(fileName, FILE_READ);

    if (!readFile) {
      Serial.println("Failed to open file for reading");
      return;
    }

    // Lê e envia o conteúdo do arquivo para a porta serial
    while(readFile.available()){
      Serial.write(readFile.read());
    }

    // Fecha o arquivo
    readFile.close();

    Serial.println("\nFile content displayed successfully");
  } else {
    Serial.print("HTTP request failed with error code: ");
    Serial.println(httpResponseCode);
  }

  // Fecha a conexão HTTP
  http.end();
}

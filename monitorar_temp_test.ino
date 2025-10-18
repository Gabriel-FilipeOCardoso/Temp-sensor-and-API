// Arduino - Sensor DHT - Umidade e Temperatura
// https://blog.eletrogate.com/
// Adafruit Unified Sensor Library: https://github.com/adafruit/Adafruit_Sensor

#include <Adafruit_Sensor.h>                       // Biblioteca DHT Sensor Adafruit 
#include <DHT.h>
#include <DHT_U.h>
//#include <iostream>
#include <esp_wifi.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <time.h>
//#include <string>
#include <ESP32Ping.h>

// selecione um sensor, retirando o comentário - duas barras
#define DHTTYPE    DHT11                    // Sensor DHT11

#define DHTPIN 21                           // Pino do Arduino conectado no Sensor(Data) 
DHT dht(DHTPIN, DHTTYPE);                   // configurando o Sensor DHT - pino e tipo

void setup()
{
  Serial.begin(9600);                             // monitor serial 9600 bps
  dht.begin();                                    // inicializa a função
  Serial.println("Usando o Sensor DHT");


  const char* ssid = "rstaex";
  const char* password = "rstaex123";

  // Conecta ao Wi-Fi
  WiFi.begin(ssid, password);
  Serial.println("Conectando ao Wi-Fi...");

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("\nConectado!");
  Serial.println(WiFi.localIP());


   //NTP
  const char* ntpServer = "pool.ntp.org";
  const long gmtOffset_sec = -10800;
  const int daylightOffset_sec = 0;

  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
  Serial.println("Sincronizando horário...");
  delay(2000);

}



void loop()
{
  
  int temp = dht.readTemperature();

  int cont = 0;

  time_t now;
  time(&now);

  if (isnan(temp) || (temp > 100)) {
    Serial.println("Falha na leitura do sensor!");
  }
  
  else {
    //Serial.print("Temperatura: ");
    //Serial.print(temp);
    //Serial.println(" °C");
    //Serial.println(now);


    HTTPClient http;
    http.begin("http://172.16.9.101:5000/dados");
    http.addHeader("Content-Type", "application/json");


    String payload = "{" + String("\"type\":") + String(1) + "," + String("\"value\":") + String(temp) + "," + String("\"created_at\":") + String(now) + "}";
    Serial.println(payload);

    int code = http.POST(payload);

    if (cont == 5){
      Ping.ping("172.16.9.101");
      cont = 0;
    }
    
    if (code > 0) {
        Serial.printf("Código da resposta: %d\n", code);  // Mostra o código de resposta HTTP
        String response = http.getString();               // Lê o corpo da resposta da API
        Serial.println("Resposta da API: " + response);
      } else {
        Serial.printf("Erro ao enviar dados: %d\n", code);  // Mostra erro se falhar
      }

      http.end();  // Finaliza a conexão HTTP

  }

 cont+=1;
 delay(60000);  // Espera 1 min para nova leitura

}
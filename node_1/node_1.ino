#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define LM35_PIN A0
#define VIB_PIN A1
#define LED_PIN 3
#define SYS_LED_PIN 4

DHT dht(DHTPIN, DHTTYPE);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
char server[] = "thingsboard.cloud";
String token = "PA6VCrsOb1RfGJF8cvEu";

EthernetClient client;

float readLM35Temp() {
  int adc = analogRead(LM35_PIN);
  float voltage = adc * (5.0 / 1023.0);
  float tempC = voltage * 100.0;
  return tempC;
}

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(LED_PIN, OUTPUT);
  pinMode(SYS_LED_PIN, OUTPUT);
  pinMode(10, OUTPUT);   // W5500 CS safety

  digitalWrite(LED_PIN, LOW);
  digitalWrite(SYS_LED_PIN, LOW);

  Serial.println("Starting Ethernet...");
  if (Ethernet.begin(mac) == 0) {
    Serial.println("Ethernet DHCP failed");
    while (true) {
      digitalWrite(SYS_LED_PIN, LOW);
    }
  }

  delay(1000);
  Serial.print("Local IP: ");
  Serial.println(Ethernet.localIP());

  // Ethernet started successfully
  digitalWrite(SYS_LED_PIN, HIGH);
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  float lm35Temp = readLM35Temp();
  int vibration = analogRead(VIB_PIN);

  int ledStatus = 0;
  int systemStatus = 1;

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT22 read failed");
    systemStatus = 0;
    digitalWrite(SYS_LED_PIN, LOW);

    // send failure state also if you want
    sendToThingsBoard(0, 0, 0, 0, 0, systemStatus);

    delay(2000);
    return;
  }

  // system healthy
  systemStatus = 1;
  digitalWrite(SYS_LED_PIN, HIGH);

  // temperature alert LED
  if (temperature > 35.0) {
    digitalWrite(LED_PIN, HIGH);
    ledStatus = 1;
  } else {
    digitalWrite(LED_PIN, LOW);
    ledStatus = 0;
  }

  Serial.println("------ NODE 1 DATA ------");
  Serial.print("DHT22 Temp: ");
  Serial.println(temperature);
  Serial.print("Humidity: ");
  Serial.println(humidity);
  Serial.print("LM35 Temp: ");
  Serial.println(lm35Temp);
  Serial.print("Vibration: ");
  Serial.println(vibration);
  Serial.print("LED Status: ");
  Serial.println(ledStatus);
  Serial.print("System Status: ");
  Serial.println(systemStatus);

  sendToThingsBoard(temperature, humidity, lm35Temp, vibration, ledStatus, systemStatus);

  delay(5000);
}

void sendToThingsBoard(float temperature, float humidity, float lm35Temp, int vibration, int ledStatus, int systemStatus) {
  String payload = "{";
  payload += "\"temperature\":" + String(temperature, 2) + ",";
  payload += "\"humidity\":" + String(humidity, 2) + ",";
  payload += "\"lm35Temp\":" + String(lm35Temp, 2) + ",";
  payload += "\"vibration\":" + String(vibration) + ",";
  payload += "\"ledStatus\":" + String(ledStatus) + ",";
  payload += "\"systemStatus\":" + String(systemStatus);
  payload += "}";

  Serial.println("Connecting to ThingsBoard...");

  if (!client.connect(server, 80)) {
    Serial.println("Connection to server failed");
    digitalWrite(SYS_LED_PIN, LOW);
    client.stop();
    return;
  }

  client.println("POST /api/v1/" + token + "/telemetry HTTP/1.1");
  client.println("Host: thingsboard.cloud");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(payload.length());
  client.println();
  client.print(payload);

  unsigned long timeout = millis();
  while (!client.available() && millis() - timeout < 5000) {
    delay(10);
  }

  Serial.println("----- HTTP RESPONSE -----");
  while (client.available()) {
    Serial.write(client.read());
  }
  Serial.println();
  Serial.println("----- END RESPONSE -----");

  client.stop();
}
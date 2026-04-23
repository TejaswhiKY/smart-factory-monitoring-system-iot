#include <SPI.h>
#include <Ethernet.h>
#include <DHT.h>

#define DHTPIN 2
#define DHTTYPE DHT22
#define BUTTON_PIN 3
#define LDR_PIN A0
#define SYS_LED_PIN 7
#define ETH_CS 10

DHT dht(DHTPIN, DHTTYPE);

byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xEE };

// Static network settings
IPAddress ip(192, 168, 1, 177);
IPAddress dns(8, 8, 8, 8);
IPAddress gateway(192, 168, 1, 1);
IPAddress subnet(255, 255, 255, 0);

// ThingsBoard settings
char server[] = "thingsboard.cloud";
String token = "0yDw2HKSuS5UXPkuxovQ";

EthernetClient client;

int motionDetected = 0;
int lastButtonState = HIGH;
unsigned long lastSendTime = 0;
const unsigned long sendInterval = 5000;

void setup() {
  Serial.begin(9600);
  dht.begin();

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(SYS_LED_PIN, OUTPUT);
  pinMode(ETH_CS, OUTPUT);

  digitalWrite(SYS_LED_PIN, LOW);
  digitalWrite(ETH_CS, HIGH);

  Serial.println("Starting system...");
  startEthernet();
}

void loop() {
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();
  int ldrValue = analogRead(LDR_PIN);

  handleButton();

  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("DHT22 read failed");
    digitalWrite(SYS_LED_PIN, LOW);
    delay(2000);
    return;
  }

  // systemStatus follows the button toggle
  int systemStatus = motionDetected;

  // PICSimLab LED on D7
  digitalWrite(SYS_LED_PIN, systemStatus ? HIGH : LOW);

  if (millis() - lastSendTime >= sendInterval) {
    lastSendTime = millis();

    printSensorData(temperature, humidity, ldrValue, motionDetected, systemStatus);
    sendToThingsBoard(temperature, humidity, ldrValue, motionDetected, systemStatus);
  }
}

void startEthernet() {
  Serial.println("Starting Ethernet...");

  Ethernet.init(ETH_CS);
  Ethernet.begin(mac, ip, dns, gateway, subnet);
  delay(1000);

  if (Ethernet.hardwareStatus() == EthernetNoHardware) {
    Serial.println("Ethernet shield not found");
    while (true) {}
  }

  Serial.print("Hardware status: ");
  switch (Ethernet.hardwareStatus()) {
    case EthernetW5100:
      Serial.println("W5100");
      break;
    case EthernetW5200:
      Serial.println("W5200");
      break;
    case EthernetW5500:
      Serial.println("W5500");
      break;
    default:
      Serial.println("Unknown");
      break;
  }

  Serial.print("Link status: ");
  switch (Ethernet.linkStatus()) {
    case Unknown:
      Serial.println("Unknown");
      break;
    case LinkON:
      Serial.println("ON");
      break;
    case LinkOFF:
      Serial.println("OFF");
      break;
  }

  Serial.print("Local IP: ");
  Serial.println(Ethernet.localIP());
}

void handleButton() {
  int currentButtonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && currentButtonState == LOW) {
    motionDetected = !motionDetected;
    delay(200);
  }

  lastButtonState = currentButtonState;
}

void printSensorData(float temperature, float humidity, int ldrValue, int motionDetected, int systemStatus) {
  Serial.println("------ NODE 2 DATA ------");
  Serial.print("Temperature: ");
  Serial.println(temperature, 2);
  Serial.print("Humidity: ");
  Serial.println(humidity, 2);
  Serial.print("LDR: ");
  Serial.println(ldrValue);
  Serial.print("Motion: ");
  Serial.println(motionDetected);
  Serial.print("System Status: ");
  Serial.println(systemStatus);
}

void sendToThingsBoard(float temperature, float humidity, int ldrValue, int motionDetected, int systemStatus) {
  String payload = "{";
  payload += "\"temperature\":" + String(temperature, 2) + ",";
  payload += "\"humidity\":" + String(humidity, 2) + ",";
  payload += "\"ldrValue\":" + String(ldrValue) + ",";
  payload += "\"motionDetected\":" + String(motionDetected) + ",";
  payload += "\"systemStatus\":" + String(systemStatus);
  payload += "}";

  if (Ethernet.linkStatus() == LinkOFF) {
    Serial.println("Ethernet link is OFF. Skipping ThingsBoard send.");
    return;
  }

  Serial.println("Connecting to ThingsBoard...");

  if (!client.connect(server, 80)) {
    Serial.println("Connection to server failed");
    client.stop();
    return;
  }

  client.println("POST /api/v1/" + token + "/telemetry HTTP/1.1");
  client.println("Host: thingsboard.cloud");
  client.println("User-Agent: Arduino");
  client.println("Content-Type: application/json");
  client.println("Connection: close");
  client.print("Content-Length: ");
  client.println(payload.length());
  client.println();
  client.print(payload);

  unsigned long timeout = millis();
  while (!client.available() && (millis() - timeout < 5000)) {
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
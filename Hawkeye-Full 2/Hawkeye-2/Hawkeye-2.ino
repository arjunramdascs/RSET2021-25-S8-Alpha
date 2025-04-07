#include <arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>

#define BULB1_PIN 32
#define BULB2_PIN 33
#define FAN_PIN 27

const char* ssid = "Jio 4g";
const char* password = "20012001";
const char* mqtt_server = "test.mosquitto.org";
bool bulb1State = false;
bool bulb2State = false;
bool fanState = false;

WiFiClient esp32wifi;
PubSubClient MQTT_client(esp32wifi);

void callback(char* topic, byte* payload, unsigned int length) 
{
  String receivedMessage;
  for (int i = 0; i < length; i++) 
  {
    receivedMessage += (char)payload[i];
  }
  Serial.print("Message received on topic: ");
  Serial.println(topic);
  Serial.print("Message: ");
  Serial.println(receivedMessage);

  if (receivedMessage == "bulb1") 
  {
    bulb1State = !bulb1State; 
    digitalWrite(BULB1_PIN, bulb1State ? LOW : HIGH);
    Serial.println("Switch bulb2");
    
  }
  if (receivedMessage == "bulb2") 
  {
    bulb2State = !bulb2State; 
    digitalWrite(BULB2_PIN, bulb2State ? HIGH : LOW);
    Serial.println("Switch bulb2");
  }
  if (receivedMessage == "fan")
  {
    fanState = !fanState;
    digitalWrite(FAN_PIN, fanState ? HIGH : LOW);
    Serial.println("Switch fan");
  }

}

void setup() 
{
  Serial.begin(9600);
  wifi_configure();
  MQTT_client.setServer(mqtt_server, 1883);
  MQTT_client.setCallback(callback);
  pinMode(BULB1_PIN, OUTPUT);
  digitalWrite(BULB1_PIN, HIGH); // Initialize relay in OFF state
  pinMode(BULB2_PIN, OUTPUT);
  digitalWrite(BULB2_PIN, HIGH); // Initialize relay in OFF state
  pinMode(FAN_PIN, OUTPUT);
  digitalWrite(FAN_PIN, HIGH);
}

void loop() 
{
  if (!MQTT_client.connected()) 
  {
    reconnect();
  }
  MQTT_client.loop();
// {
//   digitalWrite(BULB1_PIN, LOW);
//   digitalWrite(BULB2_PIN, LOW);
//   digitalWrite(FAN_PIN, LOW);
//   delay(2000);
//   digitalWrite(BULB1_PIN, HIGH);
//   digitalWrite(BULB2_PIN, HIGH);
//   digitalWrite(FAN_PIN, HIGH);
//   delay(2000);
}

void wifi_configure() 
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void reconnect() 
{
  while (!MQTT_client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-Receiver-";
    clientId += String(random(0xffff), HEX);
    if (MQTT_client.connect(clientId.c_str())) 
    {
      Serial.println("connected");
      MQTT_client.subscribe("hawkeye"); // Subscribe to the same topic
    }
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(MQTT_client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}


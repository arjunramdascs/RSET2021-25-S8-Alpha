#include <WiFi.h>
#include <PubSubClient.h>
#include <Wire.h>
#include <U8g2lib.h>

#define TOUCH_SENSOR_PIN 32                                           // ESP32 Touch Pin (Change based on your connection)
#define MQTT_SERVER "test.mosquitto.org"                               // Replace with your MQTT broker
#define MQTT_PORT 1883
#define MQTT_TOPIC_SUB "Automation001"
#define MQTT_TOPIC_PUB "hawkeye"

const char* ssid = "PHOTON-2";
const char* password = "btech@photon";

WiFiClient espClient;
PubSubClient client(espClient);
U8G2_SSD1309_128X64_NONAME2_F_4W_HW_SPI u8g2(U8G2_R0, /* cs=*/ 5, /* dc=*/ 16, /* reset=*/ 17);

unsigned long ReceivedTime = 0;
bool light1Received = false;
bool light2Received = false;
bool fanReceived = false;

void callback(char* topic, byte* payload, unsigned int length) 
{
    String message = "";
    for (int i = 0; i < length; i++) {
        message += (char)payload[i];
    }
    message.trim();

    if (message == "bulb1") 
    {
        light1Received = true;
        ReceivedTime = millis();                                  // Store the timestamp when "light" is received
        display_oled("Bulb 1");
    }

    if (message == "bulb2") 
    {
        light2Received = true;
        ReceivedTime = millis();                                  // Store the timestamp when "light" is received
        display_oled("Bulb 2");
    }

    if (message == "fan")
    {
        fanReceived = true;
        ReceivedTime = millis();
        display_oled("Fan");

    }
}

void setup() 
{
    Serial.begin(9600);
    pinMode(TOUCH_SENSOR_PIN, INPUT);
    wifi_configure();
    u8g2.begin();
    client.setServer(MQTT_SERVER, 1883);
    client.setCallback(callback);
 
}

void loop() 
{
   if (!client.connected()) 
  {
    reconnect();
  }
  client.loop();

    if (light1Received) 
    {
        unsigned long currentTime = millis();
        
        if (currentTime - ReceivedTime <= 3000) 
        {
            int touchValue = digitalRead(TOUCH_SENSOR_PIN);
            Serial.print("Touch Sensor Value: ");
            Serial.println(touchValue);

            if (touchValue) 
            {
                Serial.println("Touch detected! Publishing 'bulb1' to hawkeye.");
                client.publish(MQTT_TOPIC_PUB, "bulb1");
                light1Received = false;  // Reset flag
            }
        }
        else 
        {
            light1Received = false;                                             // Reset if 3 seconds passed
        }
    }
    if (light2Received) 
    {
        unsigned long currentTime = millis();
        
        if (currentTime - ReceivedTime <= 3000) 
        {
            int touchValue = digitalRead(TOUCH_SENSOR_PIN);
            Serial.print("Touch Sensor Value: ");
            Serial.println(touchValue);

            if (touchValue) 
            {
                Serial.println("Touch detected! Publishing 'bulb2' to hawkeye.");
                client.publish(MQTT_TOPIC_PUB, "bulb2");
                light2Received = false;  // Reset flag
            }
        }
        else 
        {
            light2Received = false;                                             // Reset if 3 seconds passed
        }
    }
    if (fanReceived) 
    {
        unsigned long currentTime = millis();
        
        if (currentTime - ReceivedTime <= 3000) 
        {
            int touchValue = digitalRead(TOUCH_SENSOR_PIN);
            Serial.print("Touch Sensor Value: ");
            Serial.println(touchValue);

            if (touchValue) 
            {
                Serial.println("Touch detected! Publishing 'fan' to hawkeye.");
                client.publish(MQTT_TOPIC_PUB, "fan");
                fanReceived = false;  // Reset flag
            }
        }
        else 
        {
            fanReceived = false;                                             // Reset if 3 seconds passed
        }
    }
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
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    String clientId = "ESP32Client-Receiver-";
    clientId += String(random(0xffff), HEX);
    if (client.connect(clientId.c_str())) 
    {
      Serial.println("connected");
      client.subscribe("Automation001"); // Subscribe to the same topic
    }
    else 
    {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
}

void display_oled(String object) 
{
  u8g2.clearBuffer();  
  u8g2.setFont(u8g2_font_6x10_tr);  // Set font
  u8g2.drawStr(10, 20, object.c_str()); // Convert String to const char*
  u8g2.sendBuffer();  // Refresh display
}




#include <Arduino.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include <DHT.h>

#define DHTPIN 14
#define DHTTYPE DHT22

DHT dht(DHTPIN, DHTTYPE);

// put your WiFi credentials here:
#define WIFI_NAME "Wokwi-GUEST"
#define WIFI_PASS ""

// put your MQTT credentials here:
#define MQTT_SERVER "broker.emqx.io"
#define MQTT_PORT 1883
#define MQTT_CLIENT_ID "62051916"

#define MQTT_TOPIC_RELAY_1 "62051916/relay1"
#define MQTT_TOPIC_RELAY_2 "62051916/relay2"
#define MQTT_TOPIC_RELAY_3 "62051916/relay3"
#define MQTT_TOPIC_RELAY_4 "62051916/relay4"

// RGB PIN
#define RED_PIN 25
#define GREEN_PIN 26
#define BLUE_PIN 27

// Relay PIN
#define RELAY_PIN1 19
#define RELAY_PIN2 18
#define RELAY_PIN3 17
#define RELAY_PIN4 16

#define MQTT_TEST_TOPIC "62051919/test"

WiFiClient wifiClient;
PubSubClient mqtt(wifiClient);

// function to connect and reconnect as necessary to the MQTT server
void connectWiFi();
void connectMQTT();
void callback(char *topic, byte *payload, unsigned int length);
float getTemperature();
float getHumidity();
void setRGB(int red, int green, int blue);
void turnOnRelay(int relay);
void turnOffRelay(int relay);
void setPin();

void setup()
{
  Serial.begin(115200);
  setPin();

  // connect to MQTT
  mqtt.setServer("broker.emqx.io", 1883);
  mqtt.setCallback(callback);

  // intialize DHT sensor
  dht.begin();
}

void loop()
{
  connectMQTT();
  delay(500);
}

void connectMQTT()
{
  while (!mqtt.connected())
  {
    Serial.print("Attempting MQTT connection...");
    if (mqtt.connect(MQTT_CLIENT_ID))
    {
      Serial.println("connected");
      mqtt.publish(MQTT_TEST_TOPIC, "hello world");

      // subscribe to topics
      mqtt.subscribe(MQTT_TOPIC_RELAY_1);
      mqtt.subscribe(MQTT_TOPIC_RELAY_2);
      mqtt.subscribe(MQTT_TOPIC_RELAY_3);
      mqtt.subscribe(MQTT_TOPIC_RELAY_4);
    }
    else
    {
      Serial.print("MQTT Connection Fail, rc= ");
      Serial.print(mqtt.state());
      Serial.println(" try again in 5 seconds");
      delay(5000);
    }
  }
  mqtt.loop();
}

void callback(char *topic, byte *payload, unsigned int length)
{
  payload[length] = '\0';
  String top_str, payload_str;
  top_str = topic;
  payload_str = (char *)payload;
  Serial.print("Message arrived in topic: ");
  Serial.println(topic);
  Serial.print("Message:");
  Serial.println(payload_str);

  if (top_str == MQTT_TOPIC_RELAY_1)
  {
    if (payload_str == "ON")
    {
      turnOnRelay(1);
    }
    else
    {
      turnOffRelay(1);
    }
  }
  else if (top_str == MQTT_TOPIC_RELAY_2)
  {
    if (payload_str == "ON")
    {
      turnOnRelay(2);
    }
    else
    {
      turnOffRelay(2);
    }
  }
  else if (top_str == MQTT_TOPIC_RELAY_3)
  {
    if (payload_str == "ON")
    {
      turnOnRelay(3);
    }
    else
    {
      turnOffRelay(3);
    }
  }
  else if (top_str == MQTT_TOPIC_RELAY_4)
  {
    if (payload_str == "ON")
    {
      turnOnRelay(4);
    }
    else
    {
      turnOffRelay(4);
    }
  }
}

float getTemperature()
{
  return dht.readTemperature();
}

float getHumidity()
{
  return dht.readHumidity();
}

void setRGB(int red, int green, int blue)
{
  // active LOW
  digitalWrite(RED_PIN, !red);
  digitalWrite(GREEN_PIN, !green);
  digitalWrite(BLUE_PIN, !blue);
}

void turnOnRelay(int relay)
{
  switch (relay)
  {
  case 1:
    digitalWrite(RELAY_PIN1, HIGH);
    break;
  case 2:
    digitalWrite(RELAY_PIN2, HIGH);
    break;
  case 3:
    digitalWrite(RELAY_PIN3, HIGH);
    break;
  case 4:
    digitalWrite(RELAY_PIN4, HIGH);
    break;
  default:
    break;
  }
}

void turnOffRelay(int relay)
{
  switch (relay)
  {
  case 1:
    digitalWrite(RELAY_PIN1, LOW);
    break;
  case 2:
    digitalWrite(RELAY_PIN2, LOW);
    break;
  case 3:
    digitalWrite(RELAY_PIN3, LOW);
    break;
  case 4:
    digitalWrite(RELAY_PIN4, LOW);
    break;
  default:
    break;
  }
}

void setPin()
{
  // RGB LED
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);

  // turn off all LED
  digitalWrite(RED_PIN, HIGH);
  digitalWrite(GREEN_PIN, HIGH);
  digitalWrite(BLUE_PIN, HIGH);

  // Relay
  pinMode(RELAY_PIN1, OUTPUT);
  pinMode(RELAY_PIN2, OUTPUT);
  pinMode(RELAY_PIN3, OUTPUT);
  pinMode(RELAY_PIN4, OUTPUT);
  // turn off all relay
  digitalWrite(RELAY_PIN1, LOW);
  digitalWrite(RELAY_PIN2, LOW);
  digitalWrite(RELAY_PIN3, LOW);
  digitalWrite(RELAY_PIN4, LOW);
}

void connectWiFi()
{
  // connect to WiFi
  Serial.println("Connecting to WiFi: " + String(WIFI_NAME));
  WiFi.begin(WIFI_NAME, WIFI_PASS);
  while (WiFi.status() != WL_CONNECTED)
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to WiFi");
  Serial.println("IP address: " + WiFi.localIP().toString());
}
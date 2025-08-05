#include <WiFi.h>
#include <PubSubClient.h>
#include <Firebase_ESP_Client.h>
#include <AceButton.h>
#include <map>
using namespace ace_button;

const char* ssid = "****";
const char* password = "****";

const char* mqtt_server = "*****";
const char* mqtt_user = "*******";
const char* mqtt_password = "********";
#define mqtt_port 1883
#define mqtt_client_id "ESP32_RELAY"

#define API_KEY "************************"
#define DATABASE_URL "*************************************"
#define USER_EMAIL "******************"
#define USER_PASSWORD "*******************"

WiFiClient espClient;
PubSubClient client(espClient);
FirebaseData fbdo;
FirebaseData stream;
FirebaseAuth auth;
FirebaseConfig config;

const int relayPins[] = { 23, 19, 18, 5 };
const int buttonPins[] = { 13, 12, 14, 27 };
const int NUM_RELAYS = 4;
bool relayStates[NUM_RELAYS] = { false, false, false, false };
bool relayLoaded[NUM_RELAYS] = { false, false, false, false };
AceButton* buttons[NUM_RELAYS];
ButtonConfig buttonConfig;
std::map<AceButton*, int> buttonIdMap;

String mqttSetTopics[NUM_RELAYS] = {
  "home/relay1/set", "home/relay2/set", "home/relay3/set", "home/relay4/set"
};
String mqttStateTopics[NUM_RELAYS] = {
  "home/relay1/state", "home/relay2/state", "home/relay3/state", "home/relay4/state"
};

// الزرار الفيزيائي
void handleEvent(AceButton* button, uint8_t eventType, uint8_t buttonState) {
  int id = buttonIdMap[button];
  if (eventType == AceButton::kEventPressed) {
    relayStates[id] = !relayStates[id];
    digitalWrite(relayPins[id], relayStates[id] ? LOW : HIGH);
    if (client.connected()) {
      client.publish(mqttStateTopics[id].c_str(), relayStates[id] ? "on" : "off", true);
    }
    if (Firebase.ready()) {
      Firebase.RTDB.setBool(&fbdo, ("/relay" + String(id + 1)).c_str(), relayStates[id]);
    }
  }
}

// Firebase Listener
void streamCallback(FirebaseStream data) {
  String path = data.dataPath();
  if (path.startsWith("/relay")) {
    int index = path.substring(6).toInt() - 1;
    if (index >= 0 && index < NUM_RELAYS) {
      bool newState = data.boolData();
      relayStates[index] = newState;
      digitalWrite(relayPins[index], newState ? LOW : HIGH);
      if (client.connected()) {
        client.publish(mqttStateTopics[index].c_str(), newState ? "on" : "off", true);
      }
    }
  }
}

void streamTimeoutCallback(bool timeout) {
  if (timeout) Serial.println("🔥 Firebase stream timed out");
}

// MQTT
void callback(char* topic, byte* payload, unsigned int length) {
  String msg;
  for (unsigned int i = 0; i < length; i++) msg += (char)payload[i];
  for (int i = 0; i < NUM_RELAYS; i++) {
    if (String(topic) == mqttSetTopics[i]) {
      bool newState = (msg == "on");
      if (relayStates[i] != newState) {
        relayStates[i] = newState;
        digitalWrite(relayPins[i], newState ? LOW : HIGH);
        if (Firebase.ready()) {
          Firebase.RTDB.setBool(&fbdo, ("/relay" + String(i + 1)).c_str(), newState);
        }
      }
    }
  }
}

void reconnectMQTT() {
  static unsigned long lastAttempt = 0;
  if (millis() - lastAttempt < 5000) return;  // حاول كل 5 ثواني بس

  Serial.print("🔁 Trying to connect to MQTT... ");
  if (client.connect(mqtt_client_id, mqtt_user, mqtt_password)) {
    Serial.println("✅ connected!");
    
    // الاشتراك في المواضيع
    for (int i = 0; i < NUM_RELAYS; i++) {
      client.subscribe(mqttSetTopics[i].c_str());
    }

    publishDiscovery();  // نشر MQTT Discovery

    // 🟢 نشر الحالة الحالية للريلايات مباشرة بعد Discovery
    for (int i = 0; i < NUM_RELAYS; i++) {
      client.publish(mqttStateTopics[i].c_str(), relayStates[i] ? "on" : "off", true);
    }

  } else {
    Serial.print("❌ failed, rc=");
    Serial.println(client.state());
  }

  lastAttempt = millis();
}

void publishDiscovery() {
  for (int i = 0; i < NUM_RELAYS; i++) {
    String topic = "homeassistant/switch/esp32_relay" + String(i + 1) + "/config";

    String payload = "{";
    payload += "\"name\": \"Relay " + String(i + 1) + "\",";
    payload += "\"command_topic\": \"" + mqttSetTopics[i] + "\",";
    payload += "\"state_topic\": \"" + mqttStateTopics[i] + "\",";
    payload += "\"payload_on\": \"on\",";
    payload += "\"payload_off\": \"off\",";
    payload += "\"retain\": true,";
    payload += "\"unique_id\": \"esp32_relay_" + String(i + 1) + "\",";
    payload += "\"device_class\": \"switch\",";
    payload += "\"device\": {";
    payload += "\"identifiers\": \"esp32-relay-device\",";
    payload += "\"name\": \"ESP32 Relay Board\",";
    payload += "\"manufacturer\": \"Mohamed Eid\",";
    payload += "\"model\": \"ESP32-4Relay\"";
    payload += "}}";

    client.publish(topic.c_str(), payload.c_str(), true);
  }
}

void setup() {
  Serial.begin(115200);

  // WiFi connect
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\n✅ WiFi connected");

  // MQTT setup
  client.setServer(mqtt_server, mqtt_port);
  client.setCallback(callback);

  // Firebase config
  config.api_key = API_KEY;
  config.database_url = DATABASE_URL;
  auth.user.email = USER_EMAIL;
  auth.user.password = USER_PASSWORD;
  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);

  // تحميل الحالة من Firebase
  for (int i = 0; i < NUM_RELAYS; i++) {
    pinMode(relayPins[i], OUTPUT);
    if (Firebase.RTDB.getBool(&fbdo, "/relay" + String(i + 1))) {
      relayStates[i] = fbdo.boolData();
      digitalWrite(relayPins[i], relayStates[i] ? LOW : HIGH);
      relayLoaded[i] = true;
      Serial.printf("✅ Relay %d loaded from Firebase: %s\n", i + 1, relayStates[i] ? "ON" : "OFF");
    } else {
      Serial.printf("❌ Failed to load relay %d from Firebase\n", i + 1);
      relayStates[i] = false;
      digitalWrite(relayPins[i], HIGH); // OFF حسب اللوجيك العكسي
    }
  }

  // Start Firebase stream
  Firebase.RTDB.beginStream(&stream, "/");
  Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);

  // تهيئة الأزرار
  for (int i = 0; i < NUM_RELAYS; i++) {
    pinMode(buttonPins[i], INPUT_PULLUP);
    buttons[i] = new AceButton(&buttonConfig);
    buttons[i]->init(buttonPins[i]);
    buttonIdMap[buttons[i]] = i;
  }
  buttonConfig.setEventHandler(handleEvent);

  // اتصال MQTT (بيشمل نشر الحالة بعد الاتصال)
  reconnectMQTT();
}


void loop() {
  client.loop();
  for (int i = 0; i < NUM_RELAYS; i++) buttons[i]->check();

  if (!client.connected()) {
    reconnectMQTT();
  }

  // 🔁 محاولة تحميل الحالات من Firebase بعد البوت
  static unsigned long lastFirebaseRetry = 0;
  unsigned long now = millis();

  for (int i = 0; i < NUM_RELAYS; i++) {
    if (!relayLoaded[i] && now - lastFirebaseRetry > 30000) {
      if (Firebase.RTDB.getBool(&fbdo, "/relay" + String(i + 1))) {
        relayStates[i] = fbdo.boolData();
        digitalWrite(relayPins[i], relayStates[i] ? LOW : HIGH);
        relayLoaded[i] = true;
        Serial.printf("✅ Relay %d synced from Firebase\n", i + 1);
      } else {
        Serial.printf("🔁 Retry failed for relay %d\n", i + 1);
      }
    }
  }
  if (now - lastFirebaseRetry > 30000) lastFirebaseRetry = now;

  // 🔁 إعادة تفعيل الـ Stream لو فصل
  static unsigned long lastReconnectAttempt = 0;
  if (!stream.httpConnected()) {
    if (now - lastReconnectAttempt > 3000) {
      Serial.println("🚨 Firebase Stream disconnected, reconnecting...");
      Firebase.RTDB.endStream(&stream);
      if (Firebase.RTDB.beginStream(&stream, "/")) {
        Firebase.RTDB.setStreamCallback(&stream, streamCallback, streamTimeoutCallback);
        Serial.println("✅ Stream reconnected!");
      } else {
        Serial.println("❌ Failed to reconnect Firebase Stream");
      }
      lastReconnectAttempt = now;
    }
  }
}

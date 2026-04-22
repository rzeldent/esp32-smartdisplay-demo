#include <Arduino.h>
#include <esp32_smartdisplay.h>
#include <ui/ui.h>
#include <ModbusMaster.h>
#include <WiFi.h>
#include <Preferences.h>
#include <time.h>

#include "secrets.h"
#include <WiFiClientSecure.h>
#define MQTT_MAX_PACKET_SIZE 100000
#define MQTT_KEEPALIVE 200
#define MQTT_SOCKET_TIMEOUT 200
#define ARDUINOJSON_USE_LONG_LONG 1
#include <PubSubClient.h>
#include <ArduinoJson.h>

ModbusMaster node;

// The MQTT topics that this device should publish/subscribe
#define AWS_IOT_PUBLISH_TOPIC "iot/firealarm/" THINGNAME
#define AWS_IOT_CONFIG_TOPIC "iot/firealarm/" THINGNAME "/config"
#define PUBLISH_INTERVAL 30000
#define LINE_TOKEN "jlEuC15y3q0H9amIaRrr25AEvVFnnzS1U78B2mhLPfwTgBoIohh023LEAC+coo80dIgSmWYWhOGII0JLRJHvadyotfinQo1vHzzKYwDx5d7Yy4eb++9OULo1Uco4vH6TjAqMulSiGe068San6VK2AgdB04t89/1O/w1cDnyilFU="
#define RESTART_INTERVAL 10800000UL

WiFiClientSecure net = WiFiClientSecure();
PubSubClient client(net);
Preferences preferences;

static constexpr char CONFIG_NAMESPACE[] = "runtimecfg";
static constexpr char PREF_TEMP_OFFSET[] = "temp_offset";
static constexpr char PREF_HUMID_OFFSET[] = "humid_offset";
static constexpr char PREF_LABEL4_TEXT[] = "label4_text";
static constexpr char DEFAULT_LABEL4_TEXT[] = "Finishing Goods";
static constexpr size_t LABEL4_TEXT_MAX_LENGTH = 63;

float temperature = 0.0f;
float humidity = 0.0f;
float temp_offset = 0.0f;
float humid_offset = 0.0f;
String label4_text = DEFAULT_LABEL4_TEXT;
bool preferences_ready = false;

extern "C" void ui_adjust_temp_offset(float delta);
extern "C" void ui_adjust_humid_offset(float delta);

static void normalize_label4_text()
{
    if (label4_text.length() == 0) {
        label4_text = DEFAULT_LABEL4_TEXT;
    }
    if (label4_text.length() > LABEL4_TEXT_MAX_LENGTH) {
        label4_text.remove(LABEL4_TEXT_MAX_LENGTH);
    }
}

static void update_offset_labels()
{
    if (ui_tempOffsetValue) {
        lv_label_set_text_fmt(ui_tempOffsetValue, "%.1f", temp_offset);
    }
    if (ui_humidOffsetValue) {
        lv_label_set_text_fmt(ui_humidOffsetValue, "%.1f", humid_offset);
    }
}

static void update_label4_text()
{
    normalize_label4_text();
    if (ui_Label4) {
        lv_label_set_text(ui_Label4, label4_text.c_str());
    }
}

static void apply_runtime_config_to_ui()
{
    update_offset_labels();
    update_label4_text();
}

static void save_runtime_config()
{
    if (!preferences_ready) {
        return;
    }

    normalize_label4_text();
    preferences.putFloat(PREF_TEMP_OFFSET, temp_offset);
    preferences.putFloat(PREF_HUMID_OFFSET, humid_offset);
    preferences.putString(PREF_LABEL4_TEXT, label4_text);
}

static void load_runtime_config()
{
    if (!preferences_ready) {
        return;
    }

    temp_offset = preferences.getFloat(PREF_TEMP_OFFSET, 0.0f);
    humid_offset = preferences.getFloat(PREF_HUMID_OFFSET, 0.0f);
    label4_text = preferences.getString(PREF_LABEL4_TEXT, DEFAULT_LABEL4_TEXT);
    normalize_label4_text();
}

extern "C" void ui_adjust_temp_offset(float delta)
{
    temp_offset += delta;
    update_offset_labels();
    save_runtime_config();
}

extern "C" void ui_adjust_humid_offset(float delta)
{
    humid_offset += delta;
    update_offset_labels();
    save_runtime_config();
}

const char* getModbusErrorDescription(uint8_t code)
{
    switch (code) {
        case 0x00: return "None";
        case 0xE0: return "Timeout";
        case 0xE1: return "Invalid Response";
        case 0xE2: return "CRC Error | Wiring Error";
        case 0xE3: return "Modbus Exception";
        case 0x01: return "Illegal Function";
        case 0x02: return "Illegal Data Address";
        case 0x03: return "Illegal Data Value";
        case 0x04: return "Slave Device Failure";
        case 0x05: return "Acknowledge";
        case 0x06: return "Slave Device Busy";
        case 0x07: return "Negative Acknowledge";
        case 0x08: return "Memory Parity Error";
        case 0x10: return "Gateway Path Unavailable";
        default: return "Unknown Modbus Error";
    }
}

const char* getWiFiStatusDescription()
{
    switch (WiFi.status()) {
        case WL_IDLE_STATUS: return "Idle, not connected";
        case WL_NO_SSID_AVAIL: return "No SSID available";
        case WL_CONNECT_FAILED: return "Wi-Fi connection failed";
        case WL_CONNECTION_LOST: return "Wi-Fi connection lost";
        case WL_CONNECTED: return "Connected to Wi-Fi";
        case WL_DISCONNECTED: return "Disconnected";
        default: return "Unknown Wi-Fi status";
    }
}

const char* getMqttErrorDescription(int state)
{
    switch (state) {
        case -4: return "Connection Timeout";
        case -3: return "Connection Lost";
        case -2: return "Connect Failed";
        case -1: return "Disconnected";
        case 0: return "Connected";
        case 1: return "Bad Protocol";
        case 2: return "Bad Client ID";
        case 3: return "Unavailable";
        case 4: return "Bad Credentials";
        case 5: return "Unauthorized";
        default: return "Unknown Error";
    }
}

StaticJsonDocument<200> publish_doc;

void mqttCallback(char* topic, byte* payload, unsigned int length)
{
    if (strcmp(topic, AWS_IOT_CONFIG_TOPIC) != 0) {
        return;
    }

    StaticJsonDocument<256> config_doc;
    DeserializationError error = deserializeJson(config_doc, payload, length);
    if (error) {
        if (ui_awsValue) {
            lv_label_set_text(ui_awsValue, "Config JSON error");
        }
        return;
    }

    bool updated = false;

    if (config_doc.containsKey("temp_offset")) {
        temp_offset = config_doc["temp_offset"].as<float>();
        updated = true;
    }

    if (config_doc.containsKey("humid_offset")) {
        humid_offset = config_doc["humid_offset"].as<float>();
        updated = true;
    }

    const char* incoming_label4 = nullptr;
    if (config_doc["label4_text"].is<const char*>()) {
        incoming_label4 = config_doc["label4_text"];
    } else if (config_doc["ui_Label4"].is<const char*>()) {
        incoming_label4 = config_doc["ui_Label4"];
    } else if (config_doc["label4"].is<const char*>()) {
        incoming_label4 = config_doc["label4"];
    }

    if (incoming_label4) {
        label4_text = incoming_label4;
        normalize_label4_text();
        updated = true;
    }

    if (!updated) {
        if (ui_awsValue) {
            lv_label_set_text(ui_awsValue, "No config fields");
        }
        return;
    }

    save_runtime_config();
    apply_runtime_config_to_ui();

    if (ui_awsValue) {
        lv_label_set_text(ui_awsValue, "Config updated");
    }
}

static bool ensure_mqtt_connection()
{
    if (client.connected()) {
        return true;
    }

    if (!client.connect(THINGNAME)) {
        return false;
    }

    if (!client.subscribe(AWS_IOT_CONFIG_TOPIC) && ui_awsValue) {
        lv_label_set_text(ui_awsValue, "Subscribe failed");
    }

    return true;
}

static void update_wifi_and_time_labels()
{
    String wifiInfo = "Connect -->" + String(WiFi.SSID()) + " (" + String(WiFi.RSSI()) + " dBm)";
    lv_label_set_text(ui_inteneterrorValue, wifiInfo.c_str());

    time_t rawtime;
    struct tm timeinfo;
    time(&rawtime);
    localtime_r(&rawtime, &timeinfo);

    char dateStr[16];
    char timeStr[16];
    strftime(dateStr, sizeof(dateStr), "%Y-%m-%d", &timeinfo);
    strftime(timeStr, sizeof(timeStr), "%H:%M:%S", &timeinfo);

    lv_label_set_text(ui_dateValue, dateStr);
    lv_label_set_text(ui_timeValue, timeStr);
}

static void read_sensor_and_refresh_ui()
{
    Serial.flush();
    node.begin(1, Serial);
    uint8_t read_result = node.readHoldingRegisters(0, 2);
    delay(100);

    String msgerror = String(read_result, HEX) + " (" + getModbusErrorDescription(read_result) + ")";

    if (msgerror != "0 (None)") {
#ifdef BOARD_HAS_RGB_LED
        smartdisplay_led_set_rgb(255, 0, 0);
#endif
        lv_label_set_text(ui_sensorerrorValue, msgerror.c_str());
        delay(1000);
    } else {
#ifdef BOARD_HAS_RGB_LED
        smartdisplay_led_set_rgb(0, 255, 0);
#endif
        lv_label_set_text(ui_sensorerrorValue, "Connected");
    }

    humidity = node.getResponseBuffer(0) / 10.0f;
    temperature = node.getResponseBuffer(1) / 10.0f;

    float adjusted_humidity = humidity + humid_offset;
    float adjusted_temp = temperature + temp_offset;

    String humidity_str = String(adjusted_humidity, 2);
    if (adjusted_humidity > 60.0f) {
        lv_label_set_text(ui_humidValue, "");
        lv_label_set_text(ui_humidValue1, humidity_str.c_str());
    } else {
        lv_label_set_text(ui_humidValue1, "");
        lv_label_set_text(ui_humidValue, humidity_str.c_str());
    }

    String temp_str = String(adjusted_temp, 2);
    lv_label_set_text(ui_tempValue, temp_str.c_str());

    node.clearResponseBuffer();
}

static void publish_sensor_values()
{
    float adjusted_humidity = humidity + humid_offset;
    float adjusted_temp = temperature + temp_offset;

    publish_doc["humidity"] = String(adjusted_humidity, 2);
    publish_doc["temperature"] = String(adjusted_temp, 2);

    char jsonBuffer[512];
    size_t len = serializeJson(publish_doc, jsonBuffer);
    bool published = client.publish(AWS_IOT_PUBLISH_TOPIC, reinterpret_cast<const uint8_t*>(jsonBuffer), len);

    if (published) {
        char statusMsg[64];
        snprintf(statusMsg, sizeof(statusMsg), "Published (Size: %u)", static_cast<unsigned>(len));
        lv_label_set_text(ui_awsValue, statusMsg);
    } else {
        char statusMsg[64];
        snprintf(statusMsg, sizeof(statusMsg), "Pub Failed (State: %d)", client.state());
        lv_label_set_text(ui_awsValue, statusMsg);
    }

    publish_doc.clear();
}

void setup()
{
#ifdef ARDUINO_USB_CDC_ON_BOOT
    delay(5000);
#endif
    Serial.begin(4800, SERIAL_8N1, 1, 3);
    Serial.flush();

    preferences_ready = preferences.begin(CONFIG_NAMESPACE, false);
    load_runtime_config();

    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

    net.setCACert(AWS_CERT_CA);
    net.setCertificate(AWS_CERT_CRT);
    net.setPrivateKey(AWS_CERT_PRIVATE);

    client.setServer(AWS_IOT_ENDPOINT, 8883);
    client.setCallback(mqttCallback);
    client.setBufferSize(1024);

    configTzTime("ICT-7", "pool.ntp.org", "time.nist.gov");

    smartdisplay_init();

    __attribute__((unused)) auto disp = lv_disp_get_default();
    ui_init();
    apply_runtime_config_to_ui();
}

ulong next_millis = 0;
ulong lv_last_tick = 0;
ulong lastPublishTime = 0;
ulong systemStartTime = 0;

void loop()
{
    unsigned long now = millis();

    if (lv_last_tick == 0) {
        lv_last_tick = now;
    }
    if (systemStartTime == 0) {
        systemStartTime = now;
    }

    if (now - systemStartTime >= RESTART_INTERVAL) {
        ESP.restart();
    }

    if (now > next_millis) {
        next_millis = now + 100;

        read_sensor_and_refresh_ui();

        if (WiFi.status() != WL_CONNECTED) {
            WiFi.disconnect();
            WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

            String msgerror = String(WiFi.status(), HEX) + " (" + getWiFiStatusDescription() + ")";
#ifdef BOARD_HAS_RGB_LED
            smartdisplay_led_set_rgb(0, 0, 255);
#endif
            lv_label_set_text(ui_inteneterrorValue, msgerror.c_str());
            lv_label_set_text(ui_dateValue, "Loss");
            lv_label_set_text(ui_timeValue, "Loss");
            delay(500);
        } else {
#ifdef BOARD_HAS_RGB_LED
            smartdisplay_led_set_rgb(0, 255, 0);
#endif
            update_wifi_and_time_labels();

            if (!ensure_mqtt_connection()) {
                char msgerror[100];
                snprintf(msgerror, sizeof(msgerror), "%X (%s)", client.state(), getMqttErrorDescription(client.state()));
                lv_label_set_text(ui_awsValue, msgerror);
            } else {
                client.loop();

                if (now - lastPublishTime > PUBLISH_INTERVAL) {
                    publish_sensor_values();
                    lastPublishTime = now;
                    delay(1000);
#ifdef BOARD_HAS_RGB_LED
                    smartdisplay_led_set_rgb(255, 255, 0);
#endif
                }
            }
        }

#ifdef BOARD_HAS_RGB_LED
        smartdisplay_led_set_rgb(0, 255, 0);
#endif
    }

    if (client.connected()) {
        client.loop();
    }

    unsigned long tick_now = millis();
    lv_tick_inc(tick_now - lv_last_tick);
    lv_last_tick = tick_now;
    lv_timer_handler();
}

#include <Wire.h>
#include <BH1750FVI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>

//  Configuration Namespace
namespace Config
{
    //  WiFi Credentials
    constexpr char WIFI_SSID[]{"wifi_name"};
    constexpr char WIFI_PASSWORD[]{"wifi_password"};

    //  MQTT Broker Details
    constexpr char MQTT_SERVER[]{"5777d9d7fc374637a07f314993f0d3bf.s1.eu.hivemq.cloud"};
    constexpr int MQTT_PORT{8883};
    constexpr char MQTT_TOPIC[]{"terrarium/sunlight"};
    constexpr char MQTT_USER[]{"hivemq.webclient.1742269150104"};
    constexpr char MQTT_PASSWORD[]{"mqtt_password"};

    //  Light Sensor Configuration
    BH1750FVI lightSensor{BH1750FVI::k_DevModeContHighRes};

    //  WiFi & MQTT Clients
    WiFiSSLClient wifiSSLClient{};
    PubSubClient mqttClient{wifiSSLClient};

    //  State Variable (Tracks Sunlight Detection State)
    bool sunlightOn{false};

    //  Sunlight Detection Threshold (Adjust based on environment)
    constexpr int SUNLIGHT_THRESHOLD{1000};
}

/**
 * @brief Connects the device to a WiFi network.
 */
void connectWiFi()
{
    WiFi.begin(Config::WIFI_SSID, Config::WIFI_PASSWORD);

    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
        Serial.println("Connecting to WiFi...");
    }

    Serial.println(" WiFi Connected!");
}

/**
 * @brief Establishes a connection to the MQTT broker.
 */
void connectMQTT()
{
    while (!Config::mqttClient.connected())
    {
        Serial.println("Connecting to MQTT...");

        if (Config::mqttClient.connect("TerrariumMonitor", Config::MQTT_USER, Config::MQTT_PASSWORD))
        {
            Serial.println(" Connected to MQTT Broker!");
        }
        else
        {
            Serial.print(" MQTT Connection Failed, retrying in 5 seconds...");
            delay(5000);
        }
    }
}

/**
 * @brief Initializes the sensor, WiFi, and MQTT connections.
 */
void setup()
{
    Serial.begin(115200);
    Config::lightSensor.begin(); // Initialize the BH1750 light sensor

    connectWiFi(); // Connect to WiFi
    Config::mqttClient.setServer(Config::MQTT_SERVER, Config::MQTT_PORT);
    connectMQTT(); // Connect to MQTT broker
}

/**
 * @brief Reads light intensity and publishes sunlight status to MQTT broker.
 */
void loop()
{
    if (!Config::mqttClient.connected())
    {
        connectMQTT();
    }

    Config::mqttClient.loop(); // Maintain MQTT connection

    uint16_t lux{Config::lightSensor.GetLightIntensity()};
    Serial.print("🌞 Light Intensity: ");
    Serial.print(lux);
    Serial.println(" lx");

    // Detect sunlight changes and send MQTT notifications
    if (lux >= Config::SUNLIGHT_THRESHOLD && !Config::sunlightOn)
    {
        Config::sunlightOn = true;
        Config::mqttClient.publish(Config::MQTT_TOPIC, "Sunlight ON");
        Serial.println("🔆 Sunlight detected.");
    }
    else if (lux < Config::SUNLIGHT_THRESHOLD && Config::sunlightOn)
    {
        Config::sunlightOn = false;
        Config::mqttClient.publish(Config::MQTT_TOPIC, "Sunlight OFF");
        Serial.println("🌑 Sunlight stopped.");
    }

    delay(5000); // Wait 5 seconds before next reading
}

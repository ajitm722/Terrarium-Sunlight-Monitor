
# 🌞 Terrarium Sunlight Monitor using Arduino Nano 33 IoT, BH1750, MQTT & Node-RED

## 📌 Overview

This project monitors the **sunlight exposure** of a terrarium using an **Arduino Nano 33 IoT** and a **BH1750 light sensor**. The system:

- Measures **light intensity** in **lux (lx)**.
- **Publishes data** to an **MQTT broker** (HiveMQ Cloud).
- Uses **Node-RED** to process the data and **send email alerts** when sunlight starts/stops.

---

## 🛠️ Hardware Requirements

- **Arduino Nano 33 IoT**  
- **BH1750 Light Sensor**  
- **Breadboard & Jumper Wires**  
- **Wi-Fi Connection**

---

## 🔌 Wiring (Pin Connections)

| **BH1750 Sensor** | **Arduino Nano 33 IoT** |
|-------------------|----------------------|
| VCC              | 3.3V                 |
| GND              | GND                   |
| SDA              | A4 (SDA)              |
| SCL              | A5 (SCL)              |

---

## 🔧 Software Requirements

- **Arduino IDE** (Install `WiFiNINA`, `PubSubClient`, and `BH1750FVI` libraries)
- **Node-RED** (For processing and sending email alerts)
- **MQTT Broker** (HiveMQ Cloud)

---

## 🚀 Setup Instructions

### 1️⃣ **Arduino Nano 33 IoT Configuration**

1. Install the required **Arduino libraries**:

   ```bash
   Arduino IDE -> Sketch -> Include Library -> Manage Libraries
   ```

   Search for and install:
   - `WiFiNINA`
   - `PubSubClient`
   - `BH1750FVI`
2. Upload the provided Arduino sketch to your **Nano 33 IoT**.

---

### 2️⃣ **Setup MQTT Broker (HiveMQ Cloud)**

1. **Create an account** on [HiveMQ Cloud](https://www.hivemq.com/company/get-hivemq/).
2. **Generate MQTT credentials** (server URL, port, username, password).
3. **Update the Arduino code** with your MQTT credentials.

---

### 3️⃣ **Setup Node-RED**

1. **Install Node-RED** (follow [installation guide](https://nodered.org/docs/getting-started/)).
2. Install required **Node-RED nodes**:

   ```bash
   npm install node-red-node-mqtt node-red-node-email
   ```

3. **Create a Node-RED Flow**:
   - Add **MQTT In** node (subscribe to `terrarium/sunlight`).
   - Add **Function** node (process message).
   - Add **Email Out** node (send email notification).
4. **Configure Email Node**:
   - SMTP Server: `smtp.gmail.com`
   - Port: `465`
   - **Enable App Passwords** for Gmail ([Guide](https://myaccount.google.com/security)).
  
### Node Red Setup -

![node-red email](assets/node-red%20setup.png)

---

## 📜 Code Explanation

### **Arduino Code**

- Connects to **Wi-Fi**.
- Reads **light intensity** from **BH1750 sensor**.
- Publishes **"Sunlight ON" / "Sunlight OFF"** messages as and when threshold is reached, to MQTT.
- Uses **5-second delay** to avoid spamming the broker.

### **Node-RED Flow**

- Listens to **MQTT messages**.
- Sends **email alerts** when sunlight **starts/stops**.

---

## 📝 Example Output

```bash
🌞 Light Intensity: 163 lx
🌞 Light Intensity: 3598 lx
🔆 Sunlight detected.
🌞 Light Intensity: 162 lx
🌑 Sunlight stopped.
```

✅ **Email Notifications Received!**

![node-red email](assets/node-red%20email.png)

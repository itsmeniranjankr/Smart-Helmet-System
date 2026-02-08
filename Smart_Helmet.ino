/* * SMART HELMET PRO | CYBER INTERFACE (CUSAT_ECE 2026)
 * SOS: 8848825350 | LEAD: NIRANJAN K R
 */

#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include <TinyGPS++.h>

// PINOUTS
const int PIN_MQ3     = 34;
const int PIN_BUCKLE  = 32;
const int PIN_RELAY   = 25;
const int PIN_PIEZO   = 26;
const int PIN_GPS_RX  = 16;
const int PIN_GPS_TX  = 17;
const int PIN_GSM_RX  = 18;
const int PIN_GSM_TX  = 19;

// CONFIG & THRESHOLDS
const char* TARGET_NUM  = "+918848825350";
const float G_LIMIT     = 8.0;      // Impact threshold
const int ALCOHOL_LIMIT = 1800;     // ADC Threshold
const int WARMUP_TIME   = 20000;    // 20s MQ-3 Warmup
const int AVG_SAMPLES   = 10;       // For signal smoothing

// STATE VARIABLES
unsigned long lastGpsUpdate = 0;
unsigned long startTime     = 0;
float alcAverage            = 0;
bool sosSent                = false;

Adafruit_MPU6050 mpu;
TinyGPSPlus gps;
HardwareSerial SerialGPS(2); 
HardwareSerial SerialGSM(1);

void setup() {
    Serial.begin(115200);
    SerialGPS.begin(9600, SERIAL_8N1, PIN_GPS_RX, PIN_GPS_TX);
    SerialGSM.begin(9600, SERIAL_8N1, PIN_GSM_RX, PIN_GSM_TX);
    
    pinMode(PIN_MQ3, INPUT);
    pinMode(PIN_BUCKLE, INPUT_PULLUP);
    pinMode(PIN_RELAY, OUTPUT);
    pinMode(PIN_PIEZO, OUTPUT);
    
    digitalWrite(PIN_RELAY, LOW); // Fail-secure: Locked at start
    
    if (!mpu.begin()) {
        Serial.println("Critial Error: MPU6050 Not Found");
        while(1);
    }
    mpu.setAccelerometerRange(MPU6050_RANGE_16_G);
    startTime = millis();
}

void loop() {
    handleSensors();
    handleGps();
    handleSafetyLogic();
}

void handleSensors() {
    // 1. Moving Average for MQ-3 to prevent noise spikes
    int raw = analogRead(PIN_MQ3);
    alcAverage = (alcAverage * (AVG_SAMPLES - 1) + raw) / AVG_SAMPLES;

    // 2. High-Frequency Impact Monitoring
    sensors_event_t a, g, temp;
    mpu.getEvent(&a, &g, &temp);
    
    // Resultant G calculation: sqrt(x² + y² + z²) / 9.8
    float totalG = sqrt(sq(a.acceleration.x) + sq(a.acceleration.y) + sq(a.acceleration.z)) / 9.81;

    if (totalG > G_LIMIT && !sosSent) {
        triggerAsynchronousSOS(totalG);
        sosSent = true; // Prevent SMS spamming
    }
}

void handleSafetyLogic() {
    bool buckled = (digitalRead(PIN_BUCKLE) == LOW);
    bool warmedUp = (millis() - startTime > WARMUP_TIME);
    
    // Safety Logic Alpha
    if (!buckled) {
        updateInterlock(false, "BUCKLE_OPEN");
        digitalWrite(PIN_PIEZO, (millis() % 1000 < 150)); // Fast chirp
    } 
    else if (warmedUp && alcAverage > ALCOHOL_LIMIT) {
        updateInterlock(false, "ALCOHOL_DETECTED");
        digitalWrite(PIN_PIEZO, HIGH);
    } 
    else if (!warmedUp) {
        updateInterlock(false, "SENSOR_WARMING"); // Don't allow start until sensor stable
    }
    else {
        updateInterlock(true, "READY");
        digitalWrite(PIN_PIEZO, LOW);
    }
}

void updateInterlock(bool state, String reason) {
    static String lastReason = "";
    digitalWrite(PIN_RELAY, state ? HIGH : LOW);
    if (reason != lastReason) {
        Serial.println("System Status: " + reason);
        lastReason = reason;
    }
}

void handleGps() {
    while (SerialGPS.available() > 0) {
        gps.encode(SerialGPS.read());
    }
}

void triggerAsynchronousSOS(float gValue) {
    Serial.println("!! CRITICAL IMPACT !!");
    
    // Prepare SMS string with coordinates if available
    String googleMapsUrl = "No GPS Lock";
    if (gps.location.isValid()) {
        googleMapsUrl = "https://www.google.com/maps?q=" + String(gps.location.lat(), 6) + "," + String(gps.location.lng(), 6);
    }

    String message = "HELMET SOS: Impact of " + String(gValue, 1) + "G. Loc: " + googleMapsUrl;

    // Non-blocking GSM send (Simplified)
    SerialGSM.println("AT+CMGF=1"); 
    delay(200); // Small delays for GSM buffer
    SerialGSM.print("AT+CMGS=\"");
    SerialGSM.print(TARGET_NUM);
    SerialGSM.println("\"");
    delay(200);
    SerialGSM.print(message);
    SerialGSM.write(26); // ASCII for Ctrl+Z
    
    Serial.println("SMS Sent to " + String(TARGET_NUM));
}

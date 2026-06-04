/*
 * =======================================================
 * 🛡️ SMART BAG - FINAL MASTER SKETCH (V3.0 - Stealth Mode) 🛡️
 * =======================================================
 * Board: ESP32 WROOM
 * Features:
 * - 📬 ALERTS for Water & Failed Attempts ONLY
 * - 💧 Water Sensor Monitoring
 * - 🏷️ RFID 2-Factor Authentication
 * - ⌨️ Keypad Override
 * - 🦾 Servo Lock/Unlock
 * - 📍 GPS Location on Alerts
 *
 * --- PIN CONFIGURATION ---
 * - Servo: GPIO 15
 * - Keypad Row 1: GPIO 13
 * - GPS: Pins 16 (RX2), 17 (TX2)
 * - RFID: SPI pins (SS=21, RST=22)
 * - Water Sensor: GPIO 34
 */

// -----------------------------------------------------------------
// ----------------- STEP 1: YOUR DETAILS (DONE!) ------------------
// -----------------------------------------------------------------

// --- Wi-Fi Details ---
const char* WIFI_SSID = "Neo Shren";
const char* WIFI_PASS = "BiNn0#08";

// --- Telegram Bot Details (UPDATED) ---
#define BOT_TOKEN "8353797139:AAEFcnT_SP4ibbOdCiqobRwGbFF-nnL_RwM" 
#define CHAT_ID "7418985949"

// --- Security Details ---
String authorizedUID = "A3 CF 17 F8";    // 🏷️ Your AUTHORIZED tag UID
String masterPassword = "1234";          // 🔑 Your keypad password


// --- Water Sensor Threshold ---
const int WATER_THRESHOLD = 2000; // 💧 Adjust this value based on your sensor's readings

// -----------------------------------------------------------------
// --------------- STEP 2: LIBRARIES & GLOBAL VARS -----------------
// -----------------------------------------------------------------

// --- Core Libraries ---
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>

// --- Component Libraries ---
#include <ESP32Servo.h>
#include <SPI.h>
#include <MFRC522.h>
#include <Keypad.h>
#include <TinyGPS++.h>       // For GPS
#include <HardwareSerial.h>  // For GPS

// --- Pins ---
#define WATER_SENSOR_PIN  34  // 💧 Water sensor
#define RFID_RST_PIN      22  // 🏷️ RFID Reset
#define RFID_SS_PIN       21  // 🏷️ RFID SS/SDA

// --- Keypad (Row 1 is on GPIO 13) ---
const byte ROWS = 4;
const byte COLS = 3;
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
byte rowPins[ROWS] = {13, 12, 14, 27}; 
byte colPins[COLS] = {26, 25, 33}; 

// --- Objects ---
WiFiClientSecure client;
UniversalTelegramBot bot(BOT_TOKEN, client);
Servo myServo;
MFRC522 mfrc522(RFID_SS_PIN, RFID_RST_PIN);
Keypad kpd = Keypad(makeKeymap(keys), rowPins, colPins, ROWS, COLS);
TinyGPSPlus gps;
HardwareSerial gpsSerial(2); // Use Serial Port 2 (Pins 16, 17)

// --- State Machine ---
enum State { LOCKED, KEYPAD_MODE, UNLOCKED };
State currentState = LOCKED;

// --- Global Variables & Timers ---
int rfidFailedAttempts = 0;
String keypadInput = "";
unsigned long unlockTime = 0;           // ⏳ Timer for auto-lock
const long unlockDuration = 15000;  // 15 seconds
unsigned long lastWaterCheck = 0;       // ⏳ Timer for water check
const long waterCheckInterval = 5000; // 5 seconds
bool waterAlertSent = false;

// --- Global GPS Variables ---
String lastKnownLat = "Waiting...";
String lastKnownLng = "Waiting...";

// -----------------------------------------------------------------
// ---------------- FUNCTION PROTOTYPES (THE FIX) ------------------
// -----------------------------------------------------------------
void checkGPS();
void checkWaterSensor();
void handleState_LOCKED(char key);
void handleState_KEYPAD(char key);
void handleState_UNLOCKED();
void sendTelegramAlert(String message, bool includeLocation = false);
String getScannedUID();
void unlockBag();
void lockBag();


// -----------------------------------------------------------------
// ----------------------- STEP 3: SETUP ---------------------------
// -----------------------------------------------------------------

void setup() {
  Serial.begin(115200);

  // --- Initialize Components ---
  myServo.attach(SERVO_PIN);
  myServo.write(SERVO_LOCK_POS); // Start 🔒 LOCKED

  SPI.begin();
  mfrc522.PCD_Init();

  // Start GPS Serial
  gpsSerial.begin(9600, SERIAL_8N1, 16, 17); 
  
  Serial.println("Smart Bag Initializing... 🎒");
  
  // --- Connect to Wi-Fi ---
  Serial.print("Connecting to Wi-Fi: ");
  Serial.print(WIFI_SSID);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  client.setCACert(TELEGRAM_CERTIFICATE_ROOT);
  
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(" 📶");
    delay(500);
  }
  Serial.println("\nWiFi Connected! ✅");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // --- No startup alert per your request ---
  // sendTelegramAlert("Smart Bag is online and LOCKED.", false);
  Serial.println("System Ready. Current State: 🔒 LOCKED");
}


// -----------------------------------------------------------------
// -------------------- STEP 4: MAIN LOOP --------------------------
// -----------------------------------------------------------------

void loop() {
  // These functions run all the time in the background
  checkGPS();           // 📍 Keep updating GPS
  checkWaterSensor();   // 💧 Keep checking for water
  char key = kpd.getKey(); // ⌨️ Always listen for a key

  // State Machine
  switch (currentState) {
    case LOCKED:
      handleState_LOCKED(key);
      break;
    case KEYPAD_MODE:
      handleState_KEYPAD(key);
      break;
    case UNLOCKED:
      handleState_UNLOCKED();
      break;
  }
}

// -----------------------------------------------------------------
// ----------------- STEP 5: HELPER FUNCTIONS ----------------------
// -----------------------------------------------------------------

/**
 * 📍 Continuously reads from the GPS module.
 */
void checkGPS() {
  while (gpsSerial.available() > 0) {
    if (gps.encode(gpsSerial.read())) {
      if (gps.location.isUpdated()) {
        lastKnownLat = String(gps.location.lat(), 6);
        lastKnownLng = String(gps.location.lng(), 6);
      }
    }
  }
}

/**
 * 🔒 Runs when the bag is locked.
 * Listens for RFID or '5' key.
 */
void handleState_LOCKED(char key) {
  // 1. Check for '5' key
  if (key == '5') {
    Serial.println("Key '5' pressed. Switching to Keypad Mode. ⌨️");
    currentState = KEYPAD_MODE;
    keypadInput = ""; // Clear input buffer
    Serial.println("Current State: ⌨️ KEYPAD_MODE");
    return; 
  }

  // 2. Check for an RFID tag
  if (!mfrc522.PICC_IsNewCardPresent() || !mfrc522.PICC_ReadCardSerial()) {
    return; // No tag
  }

  // A tag was scanned!
  String scannedUID = getScannedUID();
  Serial.print("Tag Scanned: 🏷️ ");
  Serial.println(scannedUID);

  // 3. Compare the scanned tag
  if (scannedUID == authorizedUID) {
    Serial.println("RFID Access GRANTED ✅");
    rfidFailedAttempts = 0;
    unlockBag(); 
    
  } else {
    // ACCESS DENIED
    Serial.println("RFID Access DENIED ❌");
    rfidFailedAttempts++;
    Serial.printf("Failed attempts: %d\n", rfidFailedAttempts);

    if (rfidFailedAttempts >= 3) {
      Serial.println("3 failed attempts! Switching to Keypad Mode. 🚨");
      // *** THIS IS ONE OF YOUR ALERTS ***
      sendTelegramAlert("ALERT: 3 failed RFID attempts on your bag! 🚨 Switching to keypad mode.", true);
      rfidFailedAttempts = 0;
      currentState = KEYPAD_MODE;
      Serial.println("Current State: ⌨️ KEYPAD_MODE");
    }
  }
  mfrc522.PICC_HaltA();
}

/**
 * ⌨️ Runs when in keypad mode.
 * Listens for the password (e.g., "1234#")
 */
void handleState_KEYPAD(char key) {
  if (key) { // A key was pressed
    if (key == '*') { // Clear
      keypadInput = "";
      Serial.println("Input cleared 🧹");
      
    } else if (key == '#') { // Enter
      Serial.print("Password entered: ");
      Serial.println(keypadInput); 
      
      if (keypadInput == masterPassword) {
        Serial.println("Keypad Access GRANTED ✅");
        unlockBag();
      } else {
        Serial.println("Keypad Access DENIED ❌");
        // *** THIS IS ONE OF YOUR ALERTS ***
        sendTelegramAlert("ALERT: Failed keypad password attempt! ❌", true);
        keypadInput = "";
        currentState = LOCKED; // Go back to default locked state
        Serial.println("Current State: 🔒 LOCKED");
      }
      
    } else { // A number was pressed
      keypadInput += key; // Add the digit
      Serial.print("Input: ");
      Serial.println(keypadInput); 
    }
  }
}

/**
 * 🔓 Runs when the bag is unlocked.
 * Checks the 15-second auto-lock timer.
 */
void handleState_UNLOCKED() {
  if (millis() - unlockTime > unlockDuration) {
    lockBag();
  }
}

/**
 * 💧 Checks the water sensor every 5 seconds.
 */
void checkWaterSensor() {
  if (millis() - lastWaterCheck > waterCheckInterval) {
    lastWaterCheck = millis(); // Reset timer
    
    int sensorValue = analogRead(WATER_SENSOR_PIN);
    // Serial.print("Water Sensor Value: "); // Uncomment for debugging
    // Serial.println(sensorValue);

    if (sensorValue > WATER_THRESHOLD && !waterAlertSent) {
      Serial.println("!!! WATER DETECTED !!! 💧");
      // *** THIS IS ONE OF YOUR ALERTS ***
      sendTelegramAlert("ALERT: Water has been detected inside your bag! 💧💧💧💧", false);
      waterAlertSent = true; 
      
    } else if (sensorValue <= WATER_THRESHOLD && waterAlertSent) {
      Serial.println("Water is no longer detected. 👍");
      // "Water clear" alert removed per your request
      sendTelegramAlert("Water threat is clear. 👍", false);
      // sendTelegramAlert("Water threat is clear. 👍",);
      waterAlertSent = false;
    }
  }
}

/**
 * 📬 Helper function to send a message to your Telegram.
 */
void sendTelegramAlert(String message, bool includeLocation) {
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("Wi-Fi disconnected. Cannot send Telegram message. 📵");
    return;
  }

  String fullMessage = message;

  if (includeLocation) {
    fullMessage += "\n\n📍 Last Known Location:\n";
    fullMessage += "Lat: " + lastKnownLat + "\n";
    fullMessage += "Lng: " + lastKnownLng + "\n";
    
    // Add a Google Maps link
    if (lastKnownLat != "Waiting...") {
      fullMessage += "https://www.google.com/maps?q=";
      fullMessage += lastKnownLat + "," + lastKnownLng;
    } else {
      fullMessage += "(GPS fix not yet available)";
    }
  }

  Serial.print("Sending Telegram message: 📬 ");
  Serial.println(message);
  bot.sendMessage(CHAT_ID, fullMessage, "");
}

/**
 * 🏷️ Helper function to read and format the RFID UID.
 */
String getScannedUID() {
  String scannedUID = "";
  for (byte i = 0; i < mfrc522.uid.size; i++) {
    scannedUID.concat(String(mfrc522.uid.uidByte[i], HEX));
    if (i < mfrc522.uid.size - 1) {
      scannedUID.concat(" ");
    }
  }
  scannedUID.toUpperCase();
  return scannedUID;
}

/**
M * 🔓 This function is called when access is granted.
 */
void unlockBag() {
  // "Unlocked" alert removed per your request
  // sendTelegramAlert("Bag has been UNLOCKED. 🔓", false);
  myServo.write(SERVO_UNLOCK_POS); // Rotate to "unlock"
  currentState = UNLOCKED;
  unlockTime = millis(); // Start auto-lock timer
  
  // Reset security
  keypadInput = "";
  rfidFailedAttempts = 0;
  
  Serial.println("Current State: 🔓 UNLOCKED. Auto-lock timer started. ⏳");
}

/**
 * 🔒 This function is called by the timer to lock the bag.
 */
void lockBag() {
  myServo.write(SERVO_LOCK_POS); // Rotate to "lock"
  currentState = LOCKED;
  Serial.println("15 seconds passed. Bag is now 🔒 LOCKED.");
  // "Auto-locked" alert removed per your request
  // sendTelegramAlert("Bag has been auto-LOCKED. 🔒", false);
}
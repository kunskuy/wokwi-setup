#include <WiFi.h>
#include <DHT.h>
#include <FirebaseESP32.h>

// WiFi
const char* ssid     = "Wokwi-GUEST";
const char* password = "";

// Firebase
#define FIREBASE_DATABASE_URL ""
#define FIREBASE_API_KEY      ""

// Sensor dan LED
#define DHTPIN    13
#define DHTTYPE   DHT22
#define LED_PIN   2

FirebaseData fbdo;
FirebaseConfig config;
FirebaseAuth auth;
DHT dht(DHTPIN, DHTTYPE);

void wifiConnection() {
  Serial.print("Connecting to WiFi");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(500);
  }
  Serial.println();
  Serial.println("WiFi Connected");
  Serial.println("IP: " + WiFi.localIP().toString());
}

void firebase() {
  Serial.println("Setting up Firebase connection...");
  config.database_url = FIREBASE_DATABASE_URL;
  config.api_key      = FIREBASE_API_KEY;
  auth.user.email    = "";
  auth.user.password = "";

  Serial.print("Signing up anonymously...");
  if (Firebase.signUp(&config, &auth, "", "")) {
    Serial.println(" OK");
  } else {
    Serial.println(" FAILED");
    Serial.print("  > reason: ");
    Serial.println(config.signer.signupError.message.c_str());
    return;
  }

  Firebase.begin(&config, &auth);
  Firebase.reconnectWiFi(true);
  Firebase.setReadTimeout(fbdo, 1000 * 60);
  Firebase.setwriteSizeLimit(fbdo, "tiny");

  Serial.println("Firebase connection setup completed");
}

void setup() {
  Serial.begin(115200);
  Serial.println("Starting setup...");
  wifiConnection();
  dht.begin();
  pinMode(LED_PIN, OUTPUT);
  firebase();
  Serial.println("Setup completed");
}

void loop() {
  float h = dht.readHumidity();
  float t = dht.readTemperature();

  Serial.printf("Temperature : %6.2f °C ", t);
  Serial.printf("Humidity    : %6.2f %%", h);

  if (Firebase.ready()) {
    // Kirim suhu langsung ke /temperature
    if (Firebase.setFloat(fbdo, "/temperature", t)) {
      Serial.println("→ Temp sent");
    } else {
      Serial.println("→ Temp send FAILED");
      Serial.println("  Reason: " + fbdo.errorReason());
    }

    // Kirim kelembaban langsung ke /humidity
    if (Firebase.setFloat(fbdo, "/humidity", h)) {
      Serial.println("→ Humidity sent");
    } else {
      Serial.println("→ Humidity send FAILED");
      Serial.println("  Reason: " + fbdo.errorReason());
    }
  } else {
    Serial.println("Firebase not ready");
  }

  Serial.println();
  delay(5000);
}

#define BLYNK_TEMPLATE_ID "TMPL6o0wJM7HG"
#define BLYNK_TEMPLATE_NAME "Hidroponikku"
#define BLYNK_AUTH_TOKEN "cKREZNX6qJveRiecbbMXKBudfZKcHS_P"

#include <Blynk.h>
#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>

#define BLYNK_PRINT Serial
char auth[] = "cKREZNX6qJveRiecbbMXKBudfZKcHS_P";  // Masukkan auth token Blynk Anda
// char ssid[] = "Safirudin 2";                             // Masukkan nama jaringan WiFi Anda
// char pass[] = "safirudin54321";                          // Masukkan kata sandi WiFi Anda
char ssid[] = "pkkmb";     // Masukkan nama jaringan WiFi Anda
char pass[] = "11111111";  // Masukkan kata sandi WiFi Anda

#define PIN_SENSOR_WATERFLOW 4    // Pin yang digunakan untuk sensor flow rate
#define FLOW_CALIBRATION_FACTOR 5  // Faktor kalibrasi untuk mengonversi pulsa menjadi liter
#define PIN_RELAY_A 17             // Pin yang mengontrol pompa nutrisi A
#define PIN_RELAY_B 13             // Pin yang mengontrol pompa nutrisi B
#define FLOW_PER_LITER 5           // Aliran sensor waterflow per liter air
#define led 2                      // LED untuk indikasi

int buttonState;

BlynkTimer timer;
volatile int pulseCount = 0;
// volatile unsigned long pulseCount = 0;  // Jumlah pulsa yang diterima dari sensor flow rate
float flowRate = 0.0;          // Flow rate dalam liter per menit
unsigned long lastMillis = 0;  // Waktu terakhir pulsa diterima
float totalLiters = 0.0;       // Total liter yang telah dilewati
// float totalLiters = 0;

// long currentMillis = 0;
// long previousMillis = 0;
// int interval = 1000;
// boolean ledState = LOW;
// float calibrationFactor = 5;
// volatile byte pulseCount;
// byte pulse1Sec = 0;
// float flowRate;
// unsigned int flowMilliLitres;
unsigned long totalMilliLitres;

int sliderValue = 0;       // Nilai dari slider V0
int valueA1 = 0;           // Nilai dari Virtual Pin A1
float targetFlowRate = 0;  // Hasil perhitungan dari nilai A1
bool pump1Active = false;
bool pump2Active = false;
bool nyalakan = false;

void IRAM_ATTR pulseCounter() {
  pulseCount++;
}

void setup() {
  Serial.begin(9600);
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, pass);
  Serial.print("Connecting");
  pinMode(led, OUTPUT);
  digitalWrite(led, LOW);
  int connecting_process_timed_out = 20;  //--> 20 = 20 seconds.
  connecting_process_timed_out = connecting_process_timed_out * 2;
  while (WiFi.status() != WL_CONNECTED) {  // wifi tidak sama dengan konek / jika wifi == wl connected
    Serial.print(".");
    digitalWrite(led, HIGH);
    delay(250);
    digitalWrite(led, LOW);
    delay(250);
    if (connecting_process_timed_out > 0) connecting_process_timed_out--;
    if (connecting_process_timed_out == 0) {
      delay(1000);
      ESP.restart();
    }
  }
  Serial.println("Berhasil terhubung ke internet");
  Serial.print("IP Address : ");
  Serial.println(WiFi.localIP());
  digitalWrite(led, LOW);
  Blynk.begin(auth, ssid, pass);
  pinMode(PIN_RELAY_A, OUTPUT);
  pinMode(PIN_RELAY_B, OUTPUT);
  digitalWrite(PIN_RELAY_A, HIGH);
  digitalWrite(PIN_RELAY_B, HIGH);
  pinMode(PIN_SENSOR_WATERFLOW, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(PIN_SENSOR_WATERFLOW), pulseCounter, FALLING);
  timer.setInterval(1000L, checkWaterFlow);
}

void loop() {
  Blynk.run();
  timer.run();
}

void checkWaterFlow() {
  static unsigned long lastMillis = 0;
  unsigned long currentMillis = millis();

  float flowRate = (pulseCount / FLOW_PER_LITER);  // Menghitung laju aliran dalam liter/detik
  pulseCount = 0;                                  // Reset hitungan pulsa

  totalLiters += flowRate;  // Mengakumulasi total aliran air dalam liter

  // Konversi flow rate ke mililiter
  // float flowRateMilliliters = flowRate * 1000;
  float flowRateMilliliters = flowRate;

  // unsigned long elapsedTime = millis() - lastMillis;                         // Waktu yang berlalu sejak update terakhir
  // flowRate = (10000.0 / elapsedTime) * pulseCount / FLOW_CALIBRATION_FACTOR;  // Hitung flow rate dalam liter per menit

  // // Reset jumlah pulsa dan waktu terakhir
  // pulseCount = 0;
  // lastMillis = millis();

  // // Menghitung total liter yang telah dilewati
  // totalLiters += flowRate / 60.0;
  Serial.print("Liter: ");
  Serial.print(totalLiters);
  Serial.print(" mL/s      ");
  Serial.print("Target flow nutrisi : ");
  Serial.println(targetFlowRate);


  if (nyalakan && totalLiters >= targetFlowRate) {
    pump1Active = true;              // Aktifkan pompa A
    digitalWrite(PIN_RELAY_A, LOW);  // Hidupkan pompa nutrisi A
    // Blynk.virtualWrite(V0, "Pompa A dihidupkan");
    Serial.println("Pompa A dihidupkan");
  } else if (pump1Active && totalLiters >= targetFlowRate) {
    nyalakan = false;
    pump1Active = false;
    pump2Active = true;
    digitalWrite(PIN_RELAY_A, HIGH);  // Matikan pompa nutrisi A
    digitalWrite(PIN_RELAY_B, LOW);   // Hidupkan pompa nutrisi B
    totalLiters = 0;                  // Reset total liters
    Blynk.virtualWrite(V0, "Pompa A dimatikan, Pompa B dihidupkan");
    Serial.println("Pompa A dimatikan, Pompa B dihidupkan");

  } else if (pump2Active && totalLiters >= targetFlowRate) {  // jika pompa 2 aktif & total liter >= target,
    pump2Active = false;
    digitalWrite(PIN_RELAY_B, HIGH);  // Matikan pompa nutrisi B
    totalLiters = 0;                  // Reset total liters
    // Blynk.virtualWrite(V0, "Pompa B dimatikan");
    Serial.println("Pompa B dimatikan");
  }

  lastMillis = currentMillis;
}

BLYNK_WRITE(V0) {
  sliderValue = param.asInt();  // Simpan nilai dari slider V0
  Serial.println("Nilai slider V0: " + String(sliderValue));
}

BLYNK_WRITE(V1) {
  int buttonState = param.asInt();  // Ambil nilai dari tombol V1
  if (buttonState == 1) {
    // Jika tombol V1 ditekan, kirim nilai slider V0
    Blynk.virtualWrite(V0, sliderValue);
    Serial.println("Nilai slider V0: " + String(sliderValue));  // Tampilkan nilai slider di Serial Monitor
    targetFlowRate = sliderValue * 5.0 / 1.0;                   // Lakukan perhitungan
    Serial.println("Hasil perhitungan: " + String(targetFlowRate));
    nyalakan = true;
    // pump1Active = true;              // Aktifkan pompa A
    // digitalWrite(PIN_RELAY_A, LOW);  // Hidupkan pompa nutrisi A
    // Blynk.virtualWrite(V0, "Pompa A dihidupkan");
    // Serial.println("Pompa A dihidupkan");
  }
}

// BLYNK_WRITE(V2) {
//   valueA1 = param.asInt();  // Ambil nilai dari Virtual Pin A1
//   targetFlowRate = valueA1 * 5.0 / 1.0;  // Lakukan perhitungan
//   Serial.println("Nilai A1: " + String(valueA1) + " Hasil perhitungan: " + String(targetFlowRate));
//   Blynk.virtualWrite(V3, targetFlowRate);  // Kirim hasil perhitungan ke Virtual Pin V3
// }

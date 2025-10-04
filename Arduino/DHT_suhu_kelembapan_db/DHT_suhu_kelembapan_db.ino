// --- Pustaka (Library) yang Digunakan ---
#include "DHT.h"
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>

// --- Konfigurasi Sensor DHT ---
#define DHTPIN D6     // Tentukan pin GPIO yang terhubung dengan sensor DHT (D6 pada NodeMCU).
#define DHTTYPE DHT22 // Tentukan tipe sensor DHT yang digunakan (DHT11, DHT21, atau DHT22).

DHT dht(DHTPIN, DHTTYPE); // Inisialisasi objek sensor DHT.

// --- Konfigurasi Jaringan WiFi ---
const char* ssid = "INCODE";     // Ganti dengan nama WiFi Anda.
const char* password = "12345678"; // Ganti dengan password WiFi Anda.

// --- Konfigurasi Alamat IP Statis ---
IPAddress local_IP(192, 168, 137, 202); // Atur alamat IP statis yang diinginkan untuk ESP8266.
IPAddress gateway(192, 168, 137, 1);    // Atur alamat gateway jaringan Anda (biasanya IP router).
IPAddress subnet(255, 255, 255, 0);   // Atur subnet mask jaringan Anda.
IPAddress primaryDNS(8, 8, 8, 8);     // Atur server DNS utama (contoh: Google DNS).
IPAddress secondaryDNS(8, 8, 4, 4);   // Atur server DNS sekunder.

// --- Konfigurasi Server ---
// Alamat URL skrip PHP untuk menerima data sensor.
// Pastikan ESP8266 dan server berada di jaringan yang sama atau dapat saling menjangkau.
const char* endpoint = "http://192.168.137.1/iot-room-monitoring/sensorData.php";

// --- Pengaturan Waktu Pengiriman Data ---
unsigned long lastSendTime = 0;
const long sendInterval = 5000; // Interval pengiriman data: 5000 milidetik = 5 detik.

// Fungsi untuk menghubungkan ESP8266 ke jaringan WiFi.
void connectWiFi() {
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);

  // Konfigurasi IP statis sebelum memulai koneksi WiFi.
  if (!WiFi.config(local_IP, gateway, subnet, primaryDNS, secondaryDNS)) {
    Serial.println("STA Failed to configure");
  }

  WiFi.begin(ssid, password); // Memulai koneksi ke WiFi.

  // Tunggu hingga status koneksi menjadi "WL_CONNECTED".
  while (WiFi.status() != WL_CONNECTED) {
    delay(500); // Beri jeda 0.5 detik sebelum mencoba lagi.
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP()); // Tampilkan alamat IP yang didapat.
}

// Fungsi untuk mengirim data sensor (suhu dan kelembapan) ke server.
void sendSensorData(float temperature, float humidity) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client; // Create a WiFiClient object
    HTTPClient http;

    // Use the updated begin method
    http.begin(client, endpoint);
    // Tentukan header HTTP, menandakan bahwa data yang dikirim berformat form.
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Buat data string untuk dikirim dalam format "key1=value1&key2=value2".
    String httpRequestData = "temp=" + String(temperature) + "&humidity=" + String(humidity);
    // Kirim request POST beserta datanya dan simpan kode respons dari server.
    int httpResponseCode = http.POST(httpRequestData);

    if (httpResponseCode > 0) {
      Serial.print("HTTP Response code: ");
      Serial.println(httpResponseCode);
    } else {
      Serial.print("Error on sending POST request: ");
      Serial.println(httpResponseCode); // Tampilkan kode error jika pengiriman gagal.
    }

    http.end();
  } else {
    Serial.println("WiFi not connected, cannot send data.");
  }
}

// Fungsi setup() dijalankan sekali saat ESP8266 pertama kali dinyalakan atau di-reset.
void setup() {
  Serial.begin(9600); // Inisialisasi komunikasi serial dengan baud rate 9600.
  Serial.println(F("DHT22 Monitoring Start!"));
  dht.begin();      // Inisialisasi sensor DHT.
  connectWiFi();    // Panggil fungsi untuk koneksi ke WiFi.
}

// Fungsi loop() dijalankan berulang-ulang setelah setup() selesai.
void loop() {
  // Cek koneksi WiFi. Jika terputus, coba sambungkan kembali.
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    connectWiFi();
  }

  // Gunakan millis() untuk eksekusi non-blocking, agar tidak menghentikan program.
  // Cek apakah sudah waktunya untuk membaca sensor dan mengirim data lagi.
  if (millis() - lastSendTime >= sendInterval) {
    lastSendTime = millis(); // Simpan waktu saat ini sebagai waktu terakhir pengiriman.

    // Baca kelembapan dari sensor.
    float h = dht.readHumidity();
    // Baca suhu dalam satuan Celsius.
    float t = dht.readTemperature();

    // Cek apakah pembacaan sensor gagal (hasilnya 'not a number').
    if (isnan(h) || isnan(t)) {
      Serial.println(F("Failed to read from DHT sensor!"));
      return; // Jika gagal, lewati sisa kode di loop() dan coba lagi nanti.
    }

    // Konversi suhu dari Celsius ke Fahrenheit. Ini lebih efisien daripada membaca sensor lagi.
    float f = (t * 1.8) + 32.0;

    // Hitung heat index (indeks panas) dalam Celsius dan Fahrenheit.
    float hic = dht.computeHeatIndex(t, h, false);
    float hif = dht.computeHeatIndex(f, h);

    // Tampilkan hasil pembacaan sensor ke Serial Monitor untuk debugging.
    Serial.print(F("Humidity: "));
    Serial.print(h);
    Serial.print(F("%  Temperature: "));
    Serial.print(t);
    Serial.print(F("°C / "));
    Serial.print(f);
    Serial.print(F("°F  Heat index: "));
    Serial.print(hic);
    Serial.print(F("°C / "));
    Serial.print(hif);
    Serial.println(F("°F"));

    // Panggil fungsi untuk mengirim data suhu (Celsius) dan kelembapan ke server.
    sendSensorData(t, h);
  }
}

// --- Pustaka (Library) yang Digunakan ---
#include <Adafruit_AHTX0.h>
#include <Adafruit_BMP280.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>

// --- Konfigurasi Sensor AHT20 & BMP280 ---
Adafruit_AHTX0 aht;
Adafruit_BMP280 bmp;

// --- Konfigurasi Sensor LDR ---
const int ldrPin = A0; // Pin analog yang terhubung dengan sensor LDR.

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

// --- Konfigurasi Web Server ---
ESP8266WebServer server(80); // Inisialisasi server web pada port 80.

// --- Variabel Global untuk Menyimpan Data Sensor Terakhir ---
float last_t = 0, last_h = 0, last_p = 0;
int last_lightLevel = 0;
String last_lightCategory = "N/A";
String last_read_status = "Belum ada data";

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

// Fungsi untuk mengirim data sensor (suhu, kelembapan, tekanan, cahaya, dan kategori cahaya) ke server.
void sendSensorData(float temperature, float humidity, float pressure, int lightLevel, String lightCategory) {
  if (WiFi.status() == WL_CONNECTED) {
    WiFiClient client; // Create a WiFiClient object
    HTTPClient http;

    // Use the updated begin method
    http.begin(client, endpoint);
    // Tentukan header HTTP, menandakan bahwa data yang dikirim berformat form.
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");

    // Buat data string untuk dikirim dalam format "key1=value1&key2=value2".
    String httpRequestData = "temp=" + String(temperature) + "&humidity=" + String(humidity) + "&pressure=" + String(pressure) + "&light=" + String(lightLevel) + "&light_category=" + lightCategory;
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

// Fungsi untuk mengkategorikan tingkat kecerahan berdasarkan nilai mentah LDR.
String getLightCategory(int ldrValue) {
  if (ldrValue > 1010) {
    return "Gelap";
  } else if (ldrValue > 780) {
    return "Redup";
  } else if (ldrValue > 400) {
    return "Terang";
  } else {
    return "Sangat Terang";
  }
}

// Fungsi yang akan dijalankan saat client mengakses alamat IP ESP8266 (root '/').
void handleRoot() {
  // Bangun halaman HTML sebagai String.
  String html = "<!DOCTYPE html><html><head><title>ESP8266 Monitoring</title>";
  // Tambahkan meta tag untuk refresh halaman setiap 10 detik.
  html += "<meta http-equiv='refresh' content='10'>";
  html += "<style>body { font-family: Arial, sans-serif; background-color: #f4f4f4; margin: 40px; }";
  html += ".container { max-width: 600px; margin: auto; background: #fff; padding: 20px; border-radius: 8px; box-shadow: 0 2px 4px rgba(0,0,0,0.1); }";
  html += "h1 { color: #333; } .label { font-weight: bold; } .value { color: #555; }</style>";
  html += "</head><body><div class='container'>";
  html += "<h1>Real-time Room Monitoring</h1>";
  html += "<p><span class='label'>Status:</span> <span class='value'>" + last_read_status + "</span></p>";
  html += "<p><span class='label'>Suhu:</span> <span class='value'>" + String(last_t) + " &deg;C</span></p>";
  html += "<p><span class='label'>Kelembapan:</span> <span class='value'>" + String(last_h) + " %</span></p>";
  html += "<p><span class='label'>Tekanan:</span> <span class='value'>" + String(last_p) + " hPa</span></p>";
  html += "<p><span class='label'>Tingkat Cahaya:</span> <span class='value'>" + String(last_lightLevel) + " (" + last_lightCategory + ")</span></p>";
  html += "</div></body></html>";

  server.send(200, "text/html", html); // Kirim halaman HTML ke client.
}

// Fungsi setup() dijalankan sekali saat ESP8266 pertama kali dinyalakan atau di-reset.
void setup() {
  Serial.begin(9600); // Inisialisasi komunikasi serial dengan baud rate 9600.
  Serial.println(F("Monitoring Start!"));

  if (!aht.begin()) {
    Serial.println("Could not find AHT20? Check wiring");
    while (1) delay(10);
  }
  Serial.println("AHT20 found");

  if (!bmp.begin()) {
    Serial.println("Could not find a valid BMP280 sensor, check wiring!");
    while (1);
  }
  Serial.println("BMP280 found");

  connectWiFi();    // Panggil fungsi untuk koneksi ke WiFi.  

  // Daftarkan fungsi 'handleRoot' untuk path "/"
  server.on("/", handleRoot);
  // Mulai server web
  server.begin();
  Serial.println("HTTP server started. Ready for client requests.");
}

// Fungsi loop() dijalankan berulang-ulang setelah setup() selesai.
void loop() {
  // Tangani permintaan client yang masuk ke web server.
  server.handleClient();

  // Cek koneksi WiFi. Jika terputus, coba sambungkan kembali.
  if (WiFi.status() != WL_CONNECTED) {
    Serial.println("WiFi disconnected. Reconnecting...");
    connectWiFi();
  }

  // Gunakan millis() untuk eksekusi non-blocking, agar tidak menghentikan program.
  // Cek apakah sudah waktunya untuk membaca sensor dan mengirim data lagi.
  if (millis() - lastSendTime >= sendInterval) {
    lastSendTime = millis(); // Simpan waktu saat ini sebagai waktu terakhir pengiriman.

    sensors_event_t humidity, temp_aht;
    aht.getEvent(&humidity, &temp_aht);

    float temp_bmp = bmp.readTemperature();
    float pressure = bmp.readPressure() / 100.0F; // Convert to hPa

    // Baca nilai kecerahan dari sensor LDR.
    int lightLevel = analogRead(ldrPin);
    
    // Dapatkan kategori kecerahan berdasarkan nilai LDR
    String lightCategory = getLightCategory(lightLevel);

    // Cek apakah pembacaan sensor gagal (hasilnya 'not a number').
    if (isnan(temp_aht.temperature) || isnan(temp_bmp)) {
      Serial.println(F("Failed to read from temperature sensors!"));
      last_read_status = "Gagal membaca sensor suhu!";
      return; // Jika gagal, lewati sisa kode di loop() dan coba lagi nanti.
    }

    float avg_temp = (temp_aht.temperature + temp_bmp) / 2.0;

    // Perbarui variabel global dengan nilai sensor terbaru
    last_t = avg_temp;
    last_h = humidity.relative_humidity;
    last_p = pressure;
    last_lightLevel = lightLevel;
    last_lightCategory = lightCategory;
    last_read_status = "OK";

    // Tampilkan hasil pembacaan sensor ke Serial Monitor untuk debugging.
    Serial.print(F("Humidity: "));
    Serial.print(last_h);
    Serial.print(F("%  Avg Temperature: "));
    Serial.print(last_t);
    Serial.print(F("°C  Pressure: "));
    Serial.print(last_p);
    Serial.print(F(" hPa  Light Raw: "));
    Serial.print(lightLevel);
    Serial.print(F(" ("));
    Serial.print(lightCategory);
    Serial.println(F(")"));

    // Panggil fungsi untuk mengirim data suhu (Celsius), kelembapan, nilai mentah kecerahan, dan kategori kecerahan ke server.
    sendSensorData(last_t, last_h, last_p, lightLevel, lightCategory);
  }
}

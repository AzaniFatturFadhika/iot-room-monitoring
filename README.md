# IoT Room Monitoring Dashboard

Dashboard real-time untuk monitoring kondisi ruangan menggunakan sensor IoT. Aplikasi ini menampilkan data suhu, kelembaban, tekanan udara, dan intensitas cahaya secara real-time dengan visualisasi grafik yang interaktif.

## 🚀 Fitur

- **Real-time Monitoring**: Menampilkan data sensor terbaru secara langsung
- **Visualisasi Data**: Grafik interaktif untuk riwayat data 1 jam terakhir
- **Responsive Design**: Interface yang optimal untuk desktop dan mobile
- **Multiple Sensor Support**: 
  - Suhu (°C)
  - Kelembaban (%)
  - Tekanan Udara (hPa)
  - Intensitas Cahaya dengan kategori
- **REST API**: Endpoint untuk pengiriman dan pengambilan data sensor

## 🛠️ Teknologi yang Digunakan

### Frontend
- **PHP** - Server-side scripting
- **HTML5 & CSS3** - Struktur dan styling
- **JavaScript** - Interaktivitas dan AJAX
- **TailwindCSS** - CSS framework untuk styling
- **Chart.js** - Library untuk visualisasi grafik

### Backend
- **PHP** - Backend utama dengan PDO untuk database
- **MySQL** - Database untuk menyimpan data sensor

### Build Tools
- **Node.js & npm** - Package manager untuk TailwindCSS

## 📋 Persyaratan Sistem

- **Web Server**: Apache/Nginx dengan PHP 7.4+
- **Database**: MySQL 8.0+
- **Node.js**: 16.0+ (untuk build TailwindCSS)
- **Browser**: Chrome, Firefox, Safari, Edge (versi terbaru)

## 🔧 Instalasi

### 1. Clone Repository
```bash
git clone <repository-url>
cd iot-room-monitoring
```

### 2. Setup Database
```bash
# Import database schema
mysql -u root -p < iot_room_monitoring.sql

# Atau melalui phpMyAdmin:
# - Buat database bernama 'iot_room_monitoring'
# - Import file iot_room_monitoring.sql
```

### 3. Konfigurasi Database
Edit file `conn.php` sesuai dengan konfigurasi database Anda:
```php
$host = 'localhost';
$db_name = 'iot_room_monitoring';
$username = 'root';
$password = ''; // Sesuaikan dengan password MySQL Anda
```

### 4. Install Dependencies
```bash
# Install TailwindCSS dan Chart.js
npm install
```

### 5. Build CSS (Opsional)
```bash
# Jika ingin mengcompile TailwindCSS custom
npx tailwindcss -i ./src/input.css -o ./assets/tailwind.css --watch
```

### 6. Jalankan Server
- **XAMPP/WAMP**: Letakkan folder project di `htdocs/`
- **Laragon**: Letakkan di `www/`
- **PHP Built-in Server**: 
  ```bash
  php -S localhost:8000
  ```

## 📁 Struktur Project

```
iot-room-monitoring/
├── assets/
│   └── tailwind.css          # TailwindCSS compiled
├── src/
│   └── input.css            # TailwindCSS source
├── node_modules/            # npm dependencies
├── conn.php                 # Database connection
├── getData.php              # API endpoint untuk data
├── sensorData.php           # API untuk menerima data sensor
├── index.php                # Dashboard utama
├── script.js                # JavaScript untuk frontend
├── iot_room_monitoring.sql  # Database schema
├── package.json             # npm dependencies
├── .gitignore               # Git ignore rules
└── README.md                # Documentation
```

## 🔌 API Endpoints

### GET `/getData.php`
Mengambil data sensor terbaru dan riwayat 1 jam terakhir.

**Response:**
```json
{
  "latest": {
    "temp": 25.6,
    "humidity": 65.2,
    "pressure": 1013.25,
    "light_level": 450,
    "light_category": "Terang",
    "created_at": "2024-01-01 12:00:00"
  },
  "history": [...]
}
```

### POST `/sensorData.php`
Menerima data dari sensor IoT.

**Request Body:**
```json
{
  "temp": 25.6,
  "humidity": 65.2,
  "pressure": 1013.25,
  "light_level": 450
}
```

## 📊 Format Data Sensor

| Field | Type | Unit | Description |
|-------|------|------|-------------|
| `temp` | float | °C | Suhu ruangan |
| `humidity` | float | % | Kelembaban relatif |
| `pressure` | double | hPa | Tekanan atmosfer |
| `light_level` | int | lux | Intensitas cahaya |
| `light_category` | string | - | Kategori cahaya (Gelap/Redup/Terang) |
| `created_at` | timestamp | - | Waktu pengukuran |

## 🎨 Kustomisasi

### Mengubah Interval Update
Edit file `script.js` pada bagian:
```javascript
// Update setiap 5 detik
setInterval(updateData, 5000);
```

### Mengubah Range Data Riwayat
Edit query di `getData.php`:
```sql
-- Ubah dari 1 HOUR ke interval lain
WHERE created_at >= NOW() - INTERVAL 2 HOUR
```

### Styling Custom
- Edit file `src/input.css` untuk custom TailwindCSS
- Jalankan build command: `npx tailwindcss -i ./src/input.css -o ./assets/tailwind.css`

## 🚨 Troubleshooting

### Database Connection Error
1. Pastikan MySQL service berjalan
2. Periksa konfigurasi di `conn.php`
3. Pastikan database `iot_room_monitoring` sudah dibuat

### Chart Tidak Muncul
1. Pastikan Chart.js sudah ter-load
2. Periksa console browser untuk error JavaScript
3. Pastikan data dari API valid

### TailwindCSS Tidak Ter-apply
1. Pastikan file `assets/tailwind.css` exists
2. Build ulang CSS: `npx tailwindcss -i ./src/input.css -o ./assets/tailwind.css`

## 🤝 Kontribusi

1. Fork repository ini
2. Buat branch fitur baru (`git checkout -b feature/AmazingFeature`)
3. Commit perubahan (`git commit -m 'Add some AmazingFeature'`)
4. Push ke branch (`git push origin feature/AmazingFeature`)
5. Buat Pull Request

## 📝 License

Project ini dilisensikan di bawah MIT License - lihat file [LICENSE](LICENSE) untuk detail.

## 📞 Kontak

Untuk pertanyaan atau saran, silakan buat issue di repository ini.

## 🙏 Acknowledgments

- [TailwindCSS](https://tailwindcss.com/) - CSS Framework
- [Chart.js](https://www.chartjs.org/) - Charting Library
- [PHP](https://php.net/) - Server-side Language
- [MySQL](https://mysql.com/) - Database System
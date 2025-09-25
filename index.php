<?php
$pageTitle = "IoT Room Monitoring Dashboard";
?>
<!DOCTYPE html>
<html lang="id">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title><?= htmlspecialchars($pageTitle) ?></title>
    <link rel="stylesheet" href="style.css">
    <!-- Memuat library Chart.js dari CDN -->
    <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/chartjs-adapter-date-fns/dist/chartjs-adapter-date-fns.bundle.min.js">
    </script>
</head>

<body>
    <header>
        <h1><?= htmlspecialchars($pageTitle) ?></h1>
    </header>

    <div class="container">
        <!-- Panel Kiri: Data Real-time -->
        <div class="panel left-panel">
            <h2>Pengukuran Saat Ini</h2>
            <div class="sensor-data-container">
                <div class="sensor-box temp-box">
                    <h3>Suhu</h3>
                    <p id="current-temp">-- °C</p>
                </div>
                <div class="sensor-box humidity-box">
                    <h3>Kelembaban</h3>
                    <p id="current-humidity">-- %</p>
                </div>
            </div>
            <div class="last-updated">
                <p>Terakhir diperbarui: <span id="last-updated-time">Tidak tersedia</span></p>
            </div>
        </div>

        <!-- Panel Kanan: Grafik Riwayat -->
        <div class="panel right-panel">
            <h2>Riwayat 1 Jam Terakhir</h2>
            <div class="chart-container">
                <canvas id="historyChart"></canvas>
            </div>
        </div>
    </div>

    <script src="script.js"></script>
</body>

</html>
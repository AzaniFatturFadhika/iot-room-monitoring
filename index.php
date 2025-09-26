<?php
$pageTitle = "IoT Room Monitoring Dashboard";
?>
<!DOCTYPE html>
<html lang="id">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <title><?= htmlspecialchars($pageTitle) ?></title>
    <!-- Memuat Tailwind CSS, Chart.js, dan adapter date-fns dari CDN -->
    <!-- <script src="https://cdn.tailwindcss.com"></script> -->
    <!-- <script src="https://cdn.jsdelivr.net/npm/chart.js"></script>
    <script src="https://cdn.jsdelivr.net/npm/chartjs-adapter-date-fns/dist/chartjs-adapter-date-fns.bundle.min.js"> </script> -->
    <!-- Memuat library dari file lokal -->
    <link href="./assets/tailwind.css" rel="stylesheet">
    <!-- <script src="assets/tailwind/tailwindcss.js"></script> -->
    <script src="assets/chartjs/chart.js"></script>
    <script src="assets/chartjs/chartjs-adapter-date-fns.bundle.min.js"></script>
</head>

<body class="bg-gray-100 font-sans text-gray-800 flex flex-col min-h-screen">
    <header class="bg-white shadow-md border-b border-gray-200">
        <div class="container mx-auto flex justify-between items-center p-4">
            <h1 class="text-2xl font-bold text-blue-700"><?= htmlspecialchars($pageTitle) ?></h1>
            <div id="current-date" class="text-lg text-gray-600 font-medium"></div>
        </div>
    </header>

    <main class="container mx-auto p-4 lg:p-8 flex-grow flex flex-col lg:flex-row gap-8">
        <!-- Panel Kiri: Data Real-time -->
        <div class="panel bg-white rounded-lg shadow-sm p-6 flex flex-col lg:w-[350px] lg:flex-shrink-0">
            <h2 class="text-xl font-semibold border-b-2 border-gray-100 pb-3 mb-6">Pengukuran Saat Ini</h2>
            <div class="flex flex-wrap gap-6 justify-center text-center">
                <div class="sensor-box text-white p-4 rounded-lg flex-1 basis-[calc(50%-1.5rem)] min-w-[120px] transition-all duration-200 hover:-translate-y-1 hover:shadow-lg bg-[linear-gradient(135deg,#ff8c00,#ffaf50)]">
                    <h3 class="text-base font-normal mb-2">Suhu</h3>
                    <p id="current-temp" class="text-4xl font-bold">-- °C</p>
                </div>
                <div class="sensor-box text-white p-4 rounded-lg flex-1 basis-[calc(50%-1.5rem)] min-w-[120px] transition-all duration-200 hover:-translate-y-1 hover:shadow-lg bg-[linear-gradient(135deg,#007bff,#56a7ff)]">
                    <h3 class="text-base font-normal mb-2">Kelembaban</h3>
                    <p id="current-humidity" class="text-4xl font-bold">-- %</p>
                </div>
                <div class="sensor-box text-white p-4 rounded-lg flex-1 basis-full min-w-[120px] transition-all duration-200 hover:-translate-y-1 hover:shadow-lg bg-[linear-gradient(135deg,#28a745,#5cb85c)]">
                    <h3 class="text-base font-normal mb-2">Tekanan</h3>
                    <p id="current-pressure" class="text-4xl font-bold">-- hPa</p>
                </div>
                <div class="sensor-box text-white p-4 rounded-lg flex-1 basis-full min-w-[120px] transition-all duration-200 hover:-translate-y-1 hover:shadow-lg bg-[linear-gradient(135deg,#ffc107,#ffd54f)]">
                    <h3 class="text-base font-normal mb-2">Intensitas Cahaya</h3>
                    <p id="current-light" class="text-4xl font-bold">--</p>
                </div>
                <div class="sensor-box text-white p-4 rounded-lg flex-1 basis-full min-w-[120px] transition-all duration-200 hover:-translate-y-1 hover:shadow-lg bg-[linear-gradient(135deg,#6c757d,#a1aab2)]">
                    <h3 class="text-base font-normal mb-2">Kategori Cahaya</h3>
                    <p id="current-light-category" class="text-4xl font-bold">--</p>
                </div>
            </div>
            <div class="mt-auto pt-4 text-center text-sm text-gray-500">
                <p>Terakhir diperbarui: <span id="last-updated-time">Tidak tersedia</span></p>
            </div>
        </div>

        <!-- Panel Kanan: Grafik Riwayat -->
        <div class="panel bg-white rounded-lg shadow-sm p-6 flex flex-col flex-grow min-w-0">
            <div class="flex justify-between items-center border-b-2 border-gray-100 pb-3 mb-6">
                <h2 class="text-xl font-semibold">Riwayat 1 Jam Terakhir</h2>
                <div class="chart-tabs" id="chart-tabs">
                    <button class="tab-button" data-chart="temp-humidity">Suhu & Kelembaban</button>
                    <button class="tab-button" data-chart="light-pressure">Cahaya & Tekanan</button>
                </div>
            </div>
            <div class="relative flex-grow h-full min-h-[300px]">
                <canvas id="historyChart"></canvas>
            </div>
        </div>
    </main>

    <script src="script.js"></script>
</body>

</html>
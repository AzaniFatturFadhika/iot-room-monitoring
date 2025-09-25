<?php

/**
 * File API untuk mengambil data sensor dari database.
 * Mengembalikan data dalam format JSON.
 */

// Set zona waktu default ke Waktu Indonesia Barat (WIB)
date_default_timezone_set('Asia/Jakarta');

// Set header untuk output JSON
header('Content-Type: application/json');

// Include file koneksi database
require_once 'conn.php';

$response = [
    'latest' => null,
    'history' => []
];

try {
    // 1. Ambil data terbaru untuk panel real-time
    $stmt_latest = $conn->query("SELECT temp, humidity, created_at FROM datasensor ORDER BY id DESC LIMIT 1");
    $latest_data = $stmt_latest->fetch(PDO::FETCH_ASSOC);
    if ($latest_data) {
        $response['latest'] = $latest_data;
    }

    // 2. Ambil data riwayat 1 jam terakhir untuk grafik
    // Menggunakan NOW() - INTERVAL 1 HOUR untuk mendapatkan data dari 1 jam yang lalu hingga sekarang.
    $stmt_history = $conn->query("
        SELECT temp, humidity, created_at 
        FROM datasensor 
        WHERE created_at >= NOW() - INTERVAL 1 HOUR 
        ORDER BY created_at ASC
    ");
    $history_data = $stmt_history->fetchAll(PDO::FETCH_ASSOC);
    if ($history_data) {
        $response['history'] = $history_data;
    }
} catch (PDOException $e) {
    // Jika terjadi error, kirim response error
    http_response_code(500); // Internal Server Error
    $response['error'] = "Database error: " . $e->getMessage();
}

// Tutup koneksi
$conn = null;

// Cetak response sebagai JSON
echo json_encode($response);

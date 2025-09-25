<?php

/**
 * File untuk menerima data sensor dari ESP32 dan menyimpannya ke database.
 */

// Include file koneksi database
require_once 'conn.php';

// Cek apakah request menggunakan metode POST
if ($_SERVER['REQUEST_METHOD'] === 'POST') {
    // Ambil data dari POST request
    $temp = isset($_POST['temp']) ? $_POST['temp'] : null;
    $humidity = isset($_POST['humidity']) ? $_POST['humidity'] : null;

    // Validasi data
    if ($temp !== null && $humidity !== null) {
        try {
            // Persiapkan query SQL untuk memasukkan data
            $stmt = $conn->prepare("INSERT INTO datasensor (temp, humidity) VALUES (:temp, :humidity)");

            // Bind parameter
            $stmt->bindParam(':temp', $temp);
            $stmt->bindParam(':humidity', $humidity);

            // Eksekusi query
            if ($stmt->execute()) {
                echo "Data sensor berhasil disimpan.";
            } else {
                echo "Gagal menyimpan data sensor.";
            }
        } catch (PDOException $e) {
            echo "Error: " . $e->getMessage();
        }
    } else {
        echo "Error: Data 'temp' atau 'humidity' tidak ditemukan dalam request POST.";
    }
} else {
    echo "Error: Hanya metode POST yang diizinkan.";
}

// Tutup koneksi (opsional, PHP akan menutupnya secara otomatis di akhir skrip)
$conn = null;
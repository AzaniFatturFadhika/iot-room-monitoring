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
    $light_level = isset($_POST['light']) ? $_POST['light'] : null;
    $light_category = isset($_POST['light_category']) ? $_POST['light_category'] : null;
    $pressure = isset($_POST['pressure']) ? $_POST['pressure'] : null;

    // Validasi data
    if ($temp !== null && $humidity !== null && $pressure !== null && $light_level !== null && $light_category !== null) {
        try {
            // Persiapkan query SQL untuk memasukkan data
            $stmt = $conn->prepare("INSERT INTO datasensor (temp, humidity, pressure, light_level, light_category) VALUES (:temp, :humidity, :pressure, :light_level, :light_category)");

            // Bind parameter
            $stmt->bindParam(':temp', $temp);
            $stmt->bindParam(':humidity', $humidity);
            $stmt->bindParam(':pressure', $pressure);
            $stmt->bindParam(':light_level', $light_level);
            $stmt->bindParam(':light_category', $light_category);

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
        echo "Error: Data tidak lengkap. Pastikan 'temp', 'humidity', 'light', dan 'light_category' dikirim.";
    }
} else {
    echo "Error: Hanya metode POST yang diizinkan.";
}

// Tutup koneksi (opsional, PHP akan menutupnya secara otomatis di akhir skrip)
$conn = null;

document.addEventListener("DOMContentLoaded", function () {
  // Elemen DOM
  const currentTempEl = document.getElementById("current-temp");
  const currentHumidityEl = document.getElementById("current-humidity");
  const lastUpdatedEl = document.getElementById("last-updated-time");
  const chartCanvas = document.getElementById("historyChart").getContext("2d");

  let historyChart;

  // Konfigurasi awal untuk Chart.js
  const chartConfig = {
    type: "line",
    data: {
      labels: [], // Sumbu X (waktu)
      datasets: [
        {
          label: "Suhu (°C)",
          data: [], // Sumbu Y (data suhu)
          borderColor: "rgba(255, 140, 0, 1)",
          backgroundColor: "rgba(255, 140, 0, 0.2)",
          yAxisID: "y-temp",
          tension: 0.1,
          fill: true,
          pointRadius: 0, // Menghilangkan titik pada data
          pointHoverRadius: 5, // Menampilkan titik saat kursor diarahkan
        },
        {
          label: "Kelembaban (%)",
          data: [], // Sumbu Y (data kelembaban)
          borderColor: "rgba(0, 123, 255, 1)",
          backgroundColor: "rgba(0, 123, 255, 0.2)",
          yAxisID: "y-humidity",
          tension: 0.1,
          fill: true,
          pointRadius: 0, // Menghilangkan titik pada data
          pointHoverRadius: 5, // Menampilkan titik saat kursor diarahkan
        },
      ],
    },
    options: {
      responsive: true,
      maintainAspectRatio: false,
      scales: {
        x: {
          type: "time",
          time: {
            unit: "minute",
            tooltipFormat: "HH:mm:ss",
            displayFormats: {
              minute: "HH:mm",
            },
          },
          title: {
            display: true,
            text: "Waktu",
          },
        },
        "y-temp": {
          type: "linear",
          position: "left",
          title: {
            display: true,
            text: "Suhu (°C)",
          },
          grid: {
            drawOnChartArea: false, // Hanya tampilkan grid utama
          },
        },
        "y-humidity": {
          type: "linear",
          position: "right",
          title: {
            display: true,
            text: "Kelembaban (%)",
          },
        },
      },
      plugins: {
        tooltip: {
          mode: "index",
          intersect: false,
        },
      },
    },
  };

  // Inisialisasi grafik
  historyChart = new Chart(chartCanvas, chartConfig);

  // Fungsi untuk mengambil data dari server
  async function fetchData() {
    try {
      const response = await fetch("getData.php");
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const data = await response.json();

      // Perbarui panel real-time
      updateRealtimePanel(data.latest);

      // Perbarui grafik
      updateChart(data.history);
    } catch (error) {
      console.error("Gagal mengambil data:", error);
      lastUpdatedEl.textContent = "Gagal memuat";
    }
  }

  // Fungsi untuk memperbarui panel real-time
  function updateRealtimePanel(latestData) {
    if (latestData) {
      currentTempEl.textContent = `${parseFloat(latestData.temp).toFixed(
        1
      )} °C`;
      currentHumidityEl.textContent = `${parseFloat(
        latestData.humidity
      ).toFixed(1)} %`;
      const updateTime = new Date(latestData.created_at.replace(" ", "T")); // Waktu sudah sesuai zona waktu server
      lastUpdatedEl.textContent = updateTime.toLocaleTimeString("id-ID");
    } else {
      currentTempEl.textContent = "-- °C";
      currentHumidityEl.textContent = "-- %";
      lastUpdatedEl.textContent = "Tidak ada data";
    }
  }

  // Fungsi untuk memperbarui data grafik
  function updateChart(historyData) {
    if (historyData && historyData.length > 0) {
      const labels = [];
      const tempData = [];
      const humidityData = [];

      historyData.forEach((record) => {
        labels.push(new Date(record.created_at.replace(" ", "T")));
        tempData.push(parseFloat(record.temp));
        humidityData.push(parseFloat(record.humidity));
      });

      historyChart.data.labels = labels;
      historyChart.data.datasets[0].data = tempData;
      historyChart.data.datasets[1].data = humidityData;
      historyChart.update();
    }
  }

  // Panggil fetchData() saat halaman dimuat, lalu setiap 5 detik
  fetchData();
  setInterval(fetchData, 5000); // Interval 5 detik
});

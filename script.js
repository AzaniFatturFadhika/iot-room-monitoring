document.addEventListener("DOMContentLoaded", function () {
  // Elemen DOM
  const currentTempEl = document.getElementById("current-temp");
  const currentHumidityEl = document.getElementById("current-humidity");
  const currentPressureEl = document.getElementById("current-pressure");
  const currentLightEl = document.getElementById("current-light");
  const currentLightCategoryEl = document.getElementById(
    "current-light-category"
  );
  const lastUpdatedEl = document.getElementById("last-updated-time");
  const chartCanvas = document.getElementById("historyChart").getContext("2d");
  const currentDateEl = document.getElementById("current-date");
  const chartTabs = document.getElementById("chart-tabs");

  let historyChart;
  let activeChart = "temp-humidity"; // 'temp-humidity' or 'light-pressure'

  // Konfigurasi untuk grafik Suhu & Kelembaban
  const tempHumidityConfig = {
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

  // Konfigurasi untuk grafik Cahaya
  const lightPressureConfig = {
    type: "line",
    data: {
      labels: [], // Sumbu X (waktu)
      datasets: [
        {
          label: "Intensitas Cahaya (lux)",
          data: [], // Sumbu Y (data cahaya)
          borderColor: "rgba(255, 193, 7, 1)",
          backgroundColor: "rgba(255, 193, 7, 0.2)",
          yAxisID: "y-light",
          tension: 0.1,
          fill: true,
          pointRadius: 0,
          pointHoverRadius: 5,
          stepped: true, // Membuat grafik terlihat seperti tangga, cocok untuk data kategori/level
        },
        {
          label: "Tekanan (hPa)",
          data: [], // Sumbu Y (data tekanan)
          borderColor: "rgba(40, 167, 69, 1)",
          backgroundColor: "rgba(40, 167, 69, 0.2)",
          yAxisID: "y-pressure",
          tension: 0.1,
          fill: true,
          pointRadius: 0,
          pointHoverRadius: 5,
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
        "y-light": {
          type: "linear",
          position: "left",
          title: {
            display: true,
            text: "Intensitas Cahaya (lux)",
          },
        },
        "y-pressure": {
          type: "linear",
          position: "right",
          title: {
            display: true,
            text: "Tekanan (hPa)",
          },
          grid: {
            drawOnChartArea: false,
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
  historyChart = new Chart(chartCanvas, tempHumidityConfig);

  // Fungsi untuk mengambil data dari server
  async function fetchData() {
    try {
      const response = await fetch("getData.php");
      if (!response.ok) {
        throw new Error(`HTTP error! status: ${response.status}`);
      }
      const data = await response.json();

      // --- DEBUGGING START ---
      console.log("Data mentah dari server:", data);
      console.log("Titik data terbaru:", data.latest);
      // --- DEBUGGING END ---

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
      currentPressureEl.textContent = `${parseFloat(
        latestData.pressure
      ).toFixed(2)} hPa`;

      // Perbarui nilai intensitas cahaya dan kategori di kartu terpisah
      currentLightEl.textContent = `${latestData.light_level}`;
      if (currentLightCategoryEl) {
        currentLightCategoryEl.textContent = latestData.light_category;
      }

      const updateTime = new Date(latestData.created_at.replace(" ", "T")); // Waktu sudah sesuai zona waktu server
      lastUpdatedEl.textContent = updateTime.toLocaleTimeString("id-ID", {
        hour: "2-digit",
        minute: "2-digit",
        second: "2-digit",
      });
    } else {
      currentTempEl.textContent = "-- °C";
      currentHumidityEl.textContent = "-- %";

      currentLightEl.textContent = "--";
      if (currentLightCategoryEl) {
        currentLightCategoryEl.textContent = "--";
      }
      lastUpdatedEl.textContent = "Tidak ada data";
    }
  }

  // Fungsi untuk memperbarui data grafik
  function updateChart(historyData) {
    if (historyData && historyData.length > 0) {
      const labels = [];
      const tempData = [];
      const humidityData = [];
      const lightData = [];
      const pressureData = [];

      historyData.forEach((record) => {
        labels.push(new Date(record.created_at.replace(" ", "T")));
        tempData.push(parseFloat(record.temp));
        humidityData.push(parseFloat(record.humidity));
        lightData.push(parseInt(record.light_level, 10));
        pressureData.push(parseFloat(record.pressure));
      });

      historyChart.data.labels = labels;

      if (activeChart === "temp-humidity") {
        historyChart.data.datasets[0].data = tempData;
        historyChart.data.datasets[1].data = humidityData;
      } else if (activeChart === "light-pressure") {
        historyChart.data.datasets[0].data = lightData;
        historyChart.data.datasets[1].data = pressureData;
      }

      historyChart.update();
    }
  }

  // Fungsi untuk menangani perpindahan tab grafik
  function handleTabClick(event) {
    const clickedButton = event.target.closest(".tab-button");
    if (!clickedButton) return;

    const chartType = clickedButton.dataset.chart;
    if (chartType === activeChart) return;

    // Update state
    activeChart = chartType;

    // Update tampilan tombol dengan kelas Tailwind
    updateTabStyles();

    // Hancurkan chart lama dan buat yang baru
    if (historyChart) {
      historyChart.destroy();
    }

    const newConfig =
      activeChart === "temp-humidity"
        ? tempHumidityConfig
        : lightPressureConfig;
    historyChart = new Chart(chartCanvas, newConfig);

    // Panggil fetchData lagi untuk langsung mengisi data ke chart yang baru
    fetchData();
  }

  // Fungsi untuk memperbarui style tab
  function updateTabStyles() {
    const buttons = document.querySelectorAll(".tab-button");
    buttons.forEach((btn) => {
      const chartType = btn.dataset.chart;
      // Hapus kelas aktif/non-aktif
      btn.classList.remove(
        "bg-blue-600",
        "text-white",
        "bg-gray-200",
        "text-gray-700",
        "hover:bg-gray-300"
      );

      // Tambahkan kelas berdasarkan apakah tab ini aktif atau tidak
      if (chartType === activeChart) {
        btn.classList.add("bg-blue-600", "text-white");
      } else {
        btn.classList.add("bg-gray-200", "text-gray-700", "hover:bg-gray-300");
      }
      // Tambahkan kelas dasar yang selalu ada
      btn.classList.add(
        "px-4",
        "py-2",
        "rounded-full",
        "text-sm",
        "cursor-pointer",
        "transition-colors",
        "duration-200"
      );
    });
  }

  // Fungsi untuk memperbarui tanggal di header
  function updateDate() {
    if (!currentDateEl) return;
    const now = new Date();
    const options = {
      weekday: "long",
      year: "numeric",
      month: "long",
      day: "numeric",
    };
    currentDateEl.textContent = now.toLocaleDateString("id-ID", options);
  }

  // Tambahkan event listener ke container tab
  chartTabs.addEventListener("click", handleTabClick);

  // Inisialisasi style tab saat pertama kali dimuat
  updateTabStyles();

  // Panggil fetchData() saat halaman dimuat, lalu setiap 5 detik
  fetchData();
  setInterval(fetchData, 5000); // Interval 5 detik

  // Perbarui tanggal saat halaman dimuat, lalu setiap menit
  updateDate();
  setInterval(updateDate, 60000); // Interval 1 menit
});

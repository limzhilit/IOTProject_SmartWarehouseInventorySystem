// ==========================
// FETCH TEMPERATURE & HUMIDITY
// ==========================

function fetchTempHum() {
  fetch("/temphum")
    .then(res => res.json())
    .then(data => {
      document.getElementById("temp").textContent = data.temperature;
      document.getElementById("hum").textContent = data.humidity;
    });
}

// Update every 2 seconds
setInterval(fetchTempHum, 2000);
fetchTempHum();

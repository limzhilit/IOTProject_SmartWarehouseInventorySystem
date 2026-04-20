// TEMPERATURE & HUMIDITY via WebSocket

// Listens for { type:"tempHum", temperature:..., humidity:... } messages
// from the shared WebSocket defined in AssignCard.js (window.ws).

// When the WebSocket receives a message from the server, run this function.

export function handleTempHum(msg) {
  // Convert to numbers if they are strings
  const temp = parseFloat(msg.temp);
  const hum = parseFloat(msg.hum);

  // Check if conversion worked
  if (isNaN(temp) || isNaN(hum)) {
    console.warn("Invalid tempHum payload:", msg);
    return;
  }

  // Update DOM
  document.getElementById("temp").textContent = temp;
  document.getElementById("hum").textContent = hum;
}

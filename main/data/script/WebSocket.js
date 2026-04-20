// ----------------- WebSocket connection -----------------
const protocol = location.protocol === "https:" ? "wss" : "ws";
export const ws = new WebSocket(`${protocol}://${location.host}/ws`);

export const heartbeat = { lastMessageTime: Date.now() };

ws.onopen = () => {
  console.log("WebSocket connected");
  updateStatus("Connected");
};

ws.onerror = (err) => {
  console.error("WebSocket error", err);
  updateStatus("Connection error");
};

ws.onclose = () => {
  console.warn("WebSocket closed. Reconnecting in 3s...");
  updateStatus("Disconnected. Reconnecting...");
  setTimeout(() => location.reload(), 3000);
};

/*
location.protocol → "https:" or "http:"
location.host     → "hostname:port"
final url         → ws://hostname:port/ws
*/

// ----------------- ESP32 alive check -----------------
setInterval(() => {
  if (Date.now() - heartbeat.lastMessageTime > 10000) {
    updateStatus("No data from device");
  }
}, 10000);

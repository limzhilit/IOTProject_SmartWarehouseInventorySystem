// Entry point — imports run all module side-effects in the correct order.
import { ws, heartbeat } from './WebSocket.js';
import { handleTempHum } from './TempHum.js';
import { handleAssignCard, assignSuccessful, cardTimeout } from './AssignCard.js';
import { handleAddItem } from './Table.js';
import { handleError } from './ErrorFetching.js';
import './ThemeToggle.js';

// ----------------- Central message router -----------------
// Lives here to avoid circular imports:
//   WebSocket.js exports ws  ← AssignCard.js & Table.js import ws
//   main.js imports from both sides safely
ws.addEventListener("message", event => {
  heartbeat.lastMessageTime = Date.now();
  let msg;
  try {
    msg = JSON.parse(event.data);
  } catch (e) {
    console.error("Invalid JSON:", event.data);
    return;
  }

  if (!msg.type) {
    console.error("No message type:", msg);
    return;
  }

  switch (msg.type) {
    case "temphum":
      console.log("Handle Temp");
      handleTempHum(msg);
      break;
    case "addItem":
      console.log("Calling Handle Add Item");
      handleAddItem(msg);
      break;
    case "assignCard":
      handleAssignCard(msg);
      break;
    case "assignSuccessful":
      console.log("Assign successful")
      assignSuccessful();
      break;
    case "cardTimeout":
      cardTimeout();
      break;
    case "error":
      handleError(msg);
      break;
    default:
      console.warn("Unknown message type:", msg.type);
  }
});



// if (msg.type === "addItemTimeout") {
//   console.log("Add item timed out");
//   resetUI();
// }

// if (msg.type === "itemList" && Array.isArray(msg.items)) {
//   items = msg.items;
//   renderTable();
// }
import { ws } from './WebSocket.js';

// ----------------- HELPERS -----------------
const $ = id => document.getElementById(id);
const show = (el, visible = true) => {
  const target = typeof el === "string" ? $(el) : el;
  if (target) target.style.display = visible ? "block" : "none";
};

const escapeHtml = s =>
  String(s).replace(/[&<>"']/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' }[c]));

// ----------------- DATA -----------------
let items = [
  { name: "Apple", count: 10 },
  { name: "Banana", count: 5 }
];

// ----------------- TABLE -----------------
function renderTable() {
  const tableBody = $("itemTable").querySelector("tbody");
  tableBody.innerHTML = "";

  items.forEach((item, index) => {
    const row = document.createElement("tr");
    row.innerHTML = `
      <td>${index + 1}</td>
      <td class="name">${escapeHtml(item.name)}</td>
      <td class="count">${item.count}</td>
      <td>
        <button onclick="editItem(${index})">Edit</button>
        <button onclick="deleteItem(${index})">Delete</button>
      </td>
    `;
    tableBody.appendChild(row);
  });
}

window.deleteItem = (index) => {
  if (confirm("Delete item?")) {
    const name = items[index].name;
    items.splice(index, 1);
    renderTable();
    ws.send(JSON.stringify({ type: "deleteItem", name }));
  }
};

// ----------------- ADD ITEM -----------------
let waitingForCard = false;

const resetUI = () => {
  show("addItem", true);
  show("cancelAddItem", false);
  show("waitingAddItem", false);
  waitingForCard = false;
};

$("addItem").onclick = () => {
  if (waitingForCard) return;
  waitingForCard = true;
  show("addItem", false);
  show("waitingAddItem", true);
  show("cancelAddItem", true);
  ws.send(JSON.stringify({ type: "addItem" }));
};

$("cancelAddItem").onclick = () => {
  if (!waitingForCard) return;
  ws.send(JSON.stringify({ type: "cancelAddItem" }));
  resetUI();
};

// ----------------- WebSocket: incoming item events -----------------
export function handleAddItem(msg) {
  console.log("Handle Add Item Started");

  const name = msg.item.trim();
  console.log("Card item: " + name);
  if (name && name !== "-1") {
    const existing = items.find(i => i.name.toLowerCase() === name.toLowerCase());
    if (existing) existing.count++;
    else items.push({ name, count: 1 });
    renderTable();
  }
  resetUI();

  console.log("Handle Add Item Finished");
}

// ----------------- EDIT ITEM -----------------
window.editItem = (index) => {
  renderTable();
  const row = $("itemTable").querySelector("tbody").children[index];
  const item = items[index];

  row.innerHTML = `
    <td>${index + 1}</td>
    <td><input id="edit-n" value="${escapeHtml(item.name)}"></td>
    <td><input id="edit-c" type="number" value="${item.count}"></td>
    <td>
      <button onclick="saveItem(${index})">Save</button>
      <button onclick="renderTable()">Cancel</button>
    </td>
  `;
};

window.saveItem = (index) => {
  const name = $("edit-n").value.trim();
  const count = parseInt($("edit-c").value) || 0;

  if (!name) return alert("Name is required");
  const oldName = items[index].name;
  items[index] = { name, count };
  renderTable();
  ws.send(JSON.stringify({ type: "editItem", oldName, name, count }));
};

// ----------------- Initial Request -----------------
ws.addEventListener("open", () => {
  ws.send(JSON.stringify({ type: "getItems" }));
});

renderTable();
resetUI();

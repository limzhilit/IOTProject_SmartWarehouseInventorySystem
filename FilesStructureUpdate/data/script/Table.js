// ==========================
// ITEM DATA STORAGE
// ==========================

// Array to store items (name + count)
let items = [
  { name: "Apple", count: 10 },
  { name: "Banana", count: 5 }
];

// ==========================
// RENDER TABLE FUNCTION
// ==========================

function renderTable() {
  // Select table body
  const tableBody = document.querySelector("#itemTable tbody");

  // Clear existing rows
  tableBody.innerHTML = "";

  // Loop through all items
  items.forEach((item, index) => {
    const row = document.createElement("tr");

    // Assign unique row ID
    row.id = `row-${index}`;

    // Create row HTML
    row.innerHTML = `
      <td>${index + 1}</td>
      <td class="name">${escapeHtml(item.name)}</td>
      <td class="count">${item.count}</td>
      <td class="actions">
        <button type="button" class="edit">Edit</button>
        <button type="button" class="delete">Delete</button>
      </td>
    `;

    // Add row to table
    tableBody.appendChild(row);

    // Delete button logic
    row.querySelector(".delete").addEventListener("click", () => {
      if (confirm("Delete item?")) {
        items.splice(index, 1); // Remove item
        renderTable();          // Refresh table
      }
    });

    // Edit button logic
    row.querySelector(".edit").addEventListener("click", () =>
      startEditRow(index)
    );
  });
}



// ==========================
// ADD ITEM BUTTON UI STATE
// ==========================

const addItemBtn = document.getElementById("addItemBtn");
const cancel2 = document.getElementById("cancel2");
const waiting2 = document.getElementById("waiting2");

// Initial UI state
addItemBtn.style.display = "block";
cancel2.style.display = "none";
waiting2.style.display = "none";

// ==========================
// ADD ITEM (ASYNC)
// ==========================

async function addItem() {
  // Update UI to waiting state
  waiting2.style.display = "block";
  cancel2.style.display = "block";
  addItemBtn.style.display = "none";

  let cancelled = false;

  // Cancel handler
  const onCancel = () => {
    cancelled = true;
  };
  cancel2.addEventListener("click", onCancel);

  try {
    // Ask server for item name
    const waitResponse = await fetch("/addItem");
    const waitText = (await waitResponse.text()).trim();

    // Stop if cancelled or invalid response
    if (cancelled || !waitText || waitText === "-1") return;

    // Check if item already exists
    const existing = items.find(
      item => item.name.toLowerCase() === waitText.toLowerCase()
    );

    if (existing) {
      existing.count += 1;
    } else {
      items.push({ name: waitText, count: 1 });
    }

    renderTable();
  } finally {
    // Reset UI
    addItemBtn.style.display = "block";
    cancel2.style.display = "none";
    waiting2.style.display = "none";

    // Remove temporary cancel listener
    cancel2.removeEventListener("click", onCancel);
  }
}

// Attach add item button
addItemBtn.addEventListener("click", addItem);

// ==========================
// SECURITY HELPERS (XSS SAFE)
// ==========================

function escapeHtml(s) {
  return (s || "").replace(/[&<>"']/g, c =>
  ({
    '&': '&amp;',
    '<': '&lt;',
    '>': '&gt;',
    '"': '&quot;',
    "'": '&#39;'
  }[c])
  );
}

function escapeAttr(s) {
  return escapeHtml(s).replace(/"/g, '&quot;');
}

// Initial table render
renderTable();

// ==========================
// CARD ASSIGNMENT UI
// ==========================

function resetAssignUI() {
  document.getElementById("assignCardBtn").style.display = "block";
  document.getElementById("waiting").style.display = "none";
  document.getElementById("getName").style.display = "none";
  document.getElementById("assign").style.display = "none";
  document.getElementById("cancel").style.display = "none";
}

resetAssignUI();

// ==========================
// ASSIGN ITEM TO CARD
// ==========================

async function assign() {
  const itemName = document.getElementById("assignInput").value;

  if (itemName && itemName.trim() !== "") {
    // Send item name to ESP32
    const res = await fetch(
      `/assign?item=${encodeURIComponent(itemName.substring(0, 16))}`
    );
    const msg = await res.text();
    alert(msg);
    resetAssignUI();
  } else {
    alert("Assignment cancelled (no name provided).");
    resetAssignUI();
  }
}

// ==========================
// CHARACTER COUNTER
// ==========================

const assignInput = document.getElementById("assignInput");
const charCount = document.getElementById("charCount");
const maxLen = assignInput.maxLength;

assignInput.addEventListener("input", () => {
  const length = assignInput.value.length;
  charCount.textContent = `${length} / ${maxLen}`;
  charCount.style.color = length === maxLen ? "red" : "gray";
});

// ==========================
// WAIT FOR CARD
// ==========================

async function assignCard() {
  let assignCancelled = false;

  // Update UI
  document.getElementById("assignCardBtn").style.display = "none";
  document.getElementById("waiting").style.display = "block";
  document.getElementById("cancel").style.display = "block";

  document.getElementById("cancel").addEventListener("click", () => {
    assignCancelled = true;
    resetAssignUI();
  });

  if (assignCancelled) return;

  const waitResponse = await fetch("/waitForCard");
  const waitText = await waitResponse.text();

  if (waitText.includes("detected")) {
    // Card detected → ask for name
    document.getElementById("assign").style.display = "block";
    document.getElementById("waiting").style.display = "none";
    document.getElementById("getName").style.display = "block";

    document.getElementById("assign").addEventListener("click", assign);
  } else {
    // No card detected
    document.getElementById("cancel").style.display = "none";
    document.getElementById("waiting").textContent = "No card detected.";

    setTimeout(() => {
      resetAssignUI();
      document.getElementById("waiting").textContent = "Waiting for card...";
    }, 3000);
  }
}

document.getElementById("assignCardBtn").addEventListener("click", assignCard);


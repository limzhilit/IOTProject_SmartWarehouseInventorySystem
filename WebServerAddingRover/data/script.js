let items = [{ name: "Apple", count: 10 }, { name: "Banana", count: 5 }];

function renderTable() {
  const tableBody = document.querySelector("#itemTable tbody");
  tableBody.innerHTML = "";
  items.forEach((item, index) => {
    const row = document.createElement("tr");
    row.id = `row-${index}`;
    row.innerHTML = `
      <td>${index + 1}</td>
      <td class="name">${escapeHtml(item.name)}</td>
      <td class="count">${item.count}</td>
      <td class="actions">
        <button type="button" class="edit">Edit</button>
        <button type="button" class="delete">Delete</button>
      </td>
    `;
    tableBody.appendChild(row);

    row.querySelector(".delete").addEventListener("click", () => {
      if (confirm("Delete item?")) {
        items.splice(index, 1);
        renderTable();
      }
    });

    row.querySelector(".edit").addEventListener("click", () => startEditRow(index));
  });
}

function startEditRow(index) {
  const row = document.getElementById(`row-${index}`);
  const nameCell = row.querySelector(".name");
  const countCell = row.querySelector(".count");
  nameCell.innerHTML = `<input class="edit-name" value="${escapeAttr(items[index].name)}">`;
  countCell.innerHTML = `<input class="edit-count" type="number" value="${items[index].count}">`;
  const actions = row.querySelector(".actions");
  actions.innerHTML = `<button type="button" class="save">Save</button><button type="button" class="cancel">Cancel</button>`;

  actions.querySelector(".save").addEventListener("click", () => {
    const newName = row.querySelector(".edit-name").value.trim();
    const newCount = parseInt(row.querySelector(".edit-count").value, 10) || 0;
    if (!newName) { alert("Name required"); return; }
    items[index] = { name: newName, count: newCount };
    renderTable();
  });

  actions.querySelector(".cancel").addEventListener("click", renderTable);
}

const addItemBtn = document.getElementById("addItemBtn");
const cancel2 = document.getElementById("cancel2");
const waiting2 = document.getElementById("waiting2");
addItemBtn.style.display = "block";
cancel2.style.display = "none";
waiting2.style.display = "none";
async function addItem() {
  waiting2.style.display = "block";
  cancel2.style.display = "block";
  addItemBtn.style.display = "none";

  let cancelled = false;

  // Temporary cancel handler
  const onCancel = () => {
    cancelled = true;
  };
  cancel2.addEventListener("click", onCancel);

  try {
    const waitResponse = await fetch("/addItem");
    const waitText = (await waitResponse.text()).trim();

    if (cancelled || !waitText || waitText === "-1") return;

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
    // ALWAYS reset UI here
    addItemBtn.style.display = "block";
    cancel2.style.display = "none";
    waiting2.style.display = "none";

    // Remove the temporary cancel listener
    cancel2.removeEventListener("click", onCancel);
  }
}
addItemBtn.addEventListener("click", addItem);


function escapeHtml(s) { return (s || "").replace(/[&<>"']/g, c => ({ '&': '&amp;', '<': '&lt;', '>': '&gt;', '"': '&quot;', "'": '&#39;' }[c])); }
function escapeAttr(s) { return escapeHtml(s).replace(/"/g, '&quot;'); }

renderTable();


// assign card to product name 
function resetAssignUI() {
  document.getElementById("assignCardBtn").style.display = "block";
  document.getElementById("waiting").style.display = "none";
  document.getElementById("getName").style.display = "none";
  document.getElementById("assign").style.display = "none";
  document.getElementById("cancel").style.display = "none";
}
resetAssignUI();
async function assign() {
  const itemName = document.getElementById("assignInput").value;
  if (itemName && itemName.trim() !== "") {
    // Step 3: Send item name to ESP32
    const res = await fetch(`/assign?item=${encodeURIComponent(itemName.substring(0, 16))}`);
    const msg = await res.text();
    alert(msg);
    resetAssignUI();
  } else {
    alert("Assignment cancelled (no name provided).");
    resetAssignUI();
  }
}
// input lenght for card assignment
const assignInput = document.getElementById("assignInput");
const charCount = document.getElementById("charCount");
const maxLen = assignInput.maxLength;
assignInput.addEventListener("input", () => {
  const length = assignInput.value.length;
  charCount.textContent = `${length} / ${maxLen}`;
  charCount.style.color = (length == maxLen) ? "red" : "gray";
});
async function assignCard() {
  // Step 1: Tell ESP32 to wait for a card
  let assignCancelled = false;
  document.getElementById("assignCardBtn").style.display = "none";
  document.getElementById("waiting").style.display = "block";
  document.getElementById("cancel").style.display = "block";
  document.getElementById("cancel").addEventListener("click", () => {
    resetAssignUI();
    assignCancelled = true;
  });
  if (assignCancelled) {
    return;
  }
  const waitResponse = await fetch("/waitForCard");
  const waitText = await waitResponse.text();
  if (waitText.includes("detected")) {
    // Step 2: Ask user for item name
    document.getElementById("assign").style.display = "block";
    document.getElementById("waiting").style.display = "none";
    document.getElementById("getName").style.display = "block";
    document.getElementById("cancel").addEventListener("click", () => {
      assignCancelled = true;
      resetAssignUI();
    });
    if (assignCancelled) {
      return;
    }
    document.getElementById("assign").addEventListener("click", () => {
      assign();
    });
  } else {
    document.getElementById("cancel").style.display = "none";
    document.getElementById("waiting").textContent = "No card detected. ";
    document.getElementById("waiting").style.display = "block";
    // wait 2 seconds then reset
    setTimeout(() => {
      document.getElementById("assignCardBtn").style.display = "block";
      document.getElementById("waiting").style.display = "none";
      document.getElementById("waiting").textContent = "Waiting for card... ";
      document.getElementById("cancel").style.display = "none";
    }, 3000);

  }
}

document.getElementById("assignCardBtn").addEventListener("click", assignCard);

// fetch temp and hum 
function fetchTempHum() {
  fetch("/temphum")
    .then(res => res.json())
    .then(data => {
      document.getElementById("temp").textContent = data.temperature;
      document.getElementById("hum").textContent = data.humidity;
    });
}

setInterval(fetchTempHum, 2000);
fetchTempHum();

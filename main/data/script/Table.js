// ITEM DATA STORAGE
// Array to store items (name + count)
let items = [
  { name: "Apple", count: 10 },
  { name: "Banana", count: 5 }
];

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
      editItem(index)
    );
  });
}

// Initial table render
renderTable();

// ADD ITEM BUTTON UI 
const addItemBtn = document.getElementById("addItemBtn");
const cancel2 = document.getElementById("cancel2");
const waiting2 = document.getElementById("waiting2");

// Initial UI state
addItemBtn.style.display = "block";
cancel2.style.display = "none";
waiting2.style.display = "none";

function addItem() {
  // Update UI to waiting state
  waiting2.style.display = "block";
  cancel2.style.display = "block";
  addItemBtn.style.display = "none";

  let cancelled = false;

  const onCancel = () => {
    cancelled = true;
  };
  cancel2.addEventListener("click", onCancel);

  fetch("/addItem")
    .then(waitResponse => {
      return waitResponse.text();
    })
    .then(waitText => {
      waitText = waitText.trim();

      // Stop if cancelled or invalid response
      if (cancelled || !waitText || waitText === "-1") {
        return;
      }

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
    })
    .catch(err => {
      console.error("Add item failed:", err);
    })
    .finally(() => {
      // Reset UI
      addItemBtn.style.display = "block";
      cancel2.style.display = "none";
      waiting2.style.display = "none";

      // Remove temporary cancel listener
      cancel2.removeEventListener("click", onCancel);
    });
}

// Attach add item button
addItemBtn.addEventListener("click", addItem);

function editItem(index) {
  const row = document.getElementById(`row-${index}`);

  // Get name and count cells
  const nameCell = row.querySelector(".name");
  const countCell = row.querySelector(".count");

  // Replace cells with input fields
  nameCell.innerHTML = `
    <input class="edit-name" value="${escapeAttr(items[index].name)}">
  `;
  countCell.innerHTML = `
    <input class="edit-count" type="number" value="${items[index].count}">
  `;

  // Replace action buttons
  const actions = row.querySelector(".actions");
  actions.innerHTML = `
    <button type="button" class="save">Save</button>
    <button type="button" class="cancel">Cancel</button>
  `;

  // Save changes
  actions.querySelector(".save").addEventListener("click", () => {
    const newName = row.querySelector(".edit-name").value.trim();
    const newCount = parseInt(
      row.querySelector(".edit-count").value,
      10
    ) || 0;

    if (!newName) {
      alert("Name required");
      return;
    }

    // Update item
    items[index] = { name: newName, count: newCount };
    renderTable();
  });

  // Cancel editing
  actions.querySelector(".cancel").addEventListener("click", renderTable);
}

// SECURITY HELPERS (XSS SAFE)
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
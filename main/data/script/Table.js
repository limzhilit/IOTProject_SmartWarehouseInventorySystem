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

// Initial table render
renderTable();



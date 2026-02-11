// ==========================
// EDIT ROW FUNCTION
// ==========================

function startEditRow(index) {
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
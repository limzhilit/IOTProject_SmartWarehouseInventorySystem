
let items = [{name: "Apple", count: 10} , {name: "Banana", count: 5}];

function renderTable() {
    const tableBody = document.querySelector("#itemTable tbody");
    tableBody.innerHTML = "";
    items.forEach((item, index) => {
        const row = document.createElement("tr");
        row.id = `row-${index + 1}`;

        const idCell = document.createElement("td");
        idCell.textContent = index + 1;
        row.appendChild(idCell);

        const itemCell = document.createElement("td");
        itemCell.textContent = item.name;
        row.appendChild(itemCell);

        const quantityCell = document.createElement("td");
        quantityCell.textContent = item.count;
        row.appendChild(quantityCell);

        const actionCell = document.createElement("td");
        
        const editBtn = document.createElement("button");
        editBtn.textContent = "Edit";
        editBtn.onclick = (event) => editCell(event);
        actionCell.appendChild(editBtn);

        const deleteBtn = document.createElement("button");
        deleteBtn.textContent = "Delete";
        deleteBtn.onclick = () => deleteItem(index);
        actionCell.appendChild(deleteBtn);

        row.appendChild(actionCell);

        tableBody.appendChild(row);
    });
}

function addItem() {
    const input = document.getElementById("itemInput");
    const value = input.value.trim();
    if (value) {
        items.push(value);
        input.value = "";
        renderTable();
    }
}

function editCell(event) {
    // Get the row
    const row = event.target.closest("tr");

    // --- ITEM CELL ---
    const itemCell = row.cells[1];
    const currentItemValue = itemCell.textContent;

    const itemInput = document.createElement("input");
    itemInput.type = "text";
    itemInput.value = currentItemValue;
    itemInput.style.width = "120px";
    itemInput.maxLength = 16; 
    itemCell.textContent = "";
    itemCell.appendChild(itemInput);
    itemInput.focus();

    // --- QUANTITY CELL ---
    const quantityCell = row.cells[2];
    const currentQuantityValue = quantityCell.textContent;

    const quantityInput = document.createElement("input");
    quantityInput.type = "number";
    quantityInput.value = currentQuantityValue;
    quantityInput.style.width = "30px";
    quantityCell.textContent = "";
    quantityCell.appendChild(quantityInput);

    // Change Edit button to Save
    const actionCell = row.cells[3];  
    const editButton = actionCell.querySelector("button");
    editButton.textContent = "Save";
    editButton.onclick = () => {
        saveEdits();
    };

    function saveEdits() {
        itemCell.textContent = itemInput.value;
        quantityCell.textContent = quantityInput.value;
        editButton.textContent = "Edit";
        editButton.onclick = (event) => editCell(event);
    }
}

function deleteItem(index) {
    if (confirm("Are you sure you want to delete this item?")) {
        items.splice(index, 1);
        renderTable();
    }
}

// Initial render
renderTable();

document.getElementById("waiting").style.display="none";
document.getElementById("getName").style.display="none";



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
    document.getElementById("waiting").style.display="block";

    const waitResponse = await fetch("/waitForCard");
    const waitText = await waitResponse.text();

    if (waitText.includes("Card detected")) {
        // Step 2: Ask user for item name
        document.getElementById("waiting").style.display="none";
        document.getElementById("getName").style.display="block";
        const itemName = document.getElementById("getName").textContent;
        if (itemName && itemName.trim() !== "") {
            // Step 3: Send item name to ESP32
            const res = await fetch(`/assign?item=${encodeURIComponent(itemName.substring(0, 16))}`);
            const msg = await res.text();
            alert(msg);
        } else {
            alert("Assignment cancelled (no name provided).");
        }
    } else {
        document.getElementById("waiting").textContent="No card detected (timeout). Try again.";
        document.getElementById("waiting").style.display="block";
    }
}

document.getElementById("assignBtn").addEventListener("click", assignCard);


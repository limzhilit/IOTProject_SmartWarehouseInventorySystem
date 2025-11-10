String homePage1 = F(R"=====(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <style>
    body {
        font-family: -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, sans-serif;
        padding: 5px 10px;
        background-color: #f9f9f9;
        color: #333;
        font-size: 11px;
    }

    h1 {
        text-align: center;
        margin-bottom: 2rem;
        font-size: medium;
        padding-top: 0px;
    }
    
    table {
        width: 100%;
        border-collapse: collapse;
        margin-bottom: 1rem;
    }

    th, td {
        border: 1px solid #ccc;
        padding: 0.5rem 1rem;
        text-align: left;
    }

    th {
        background-color: #eee;
    }

    input, button {
        padding: 0.4rem 0.8rem;
        margin-right: 0.5rem;
        margin-top: 0.5rem;
    }

    button {
        cursor: pointer;
    }

    #statusTable thead tr th, #statusTable tbody tr td {
        width: 33%;
        text-align: center;
    }

    #itemTable thead tr th, #itemTable tbody tr td {
        text-align: center;
    }
    
    </style>
    <title>Document</title>
</head>
<body>
    <h1>Smart Warehouse Inventory System </h1>
    <table id="statusTable">
        <thead>
            <tr>
                <th>Error</th>
                <th>Temp</th>
                <th>Humidity</th>
            </tr>
        </thead>
        <tbody>
            <tr>
                <td>x</td>
                <td>
)=====");


String homePage2 = F(R"=====(
</td>
                <td>
)=====");

String homePage3 = F(R"=====(
                 </td>
            </tr>
        </tbody>
    </table>

    <table id="itemTable">
        <thead>
            <tr>
                <th style="width: 6%;">#</th>
                <th>Item</th>
                <th style="width: 20%;">Quantity</th>
                <th style="width: 33%;">Actions</th>
            </tr>
        </thead>
        <tbody>
            <!-- Items will appear here -->
        </tbody>
    </table>
    <input type="text" id="assignInput" placeholder="Enter item name" maxlength="16">
    <small id="charCount" style="color:gray;">0 / 16</small>
    <button id="assignBtn">Assign</button>
    <button>Add</button>



    <script>
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

    document.getElementById("assignBtn").addEventListener("click", () => {
    const itemName = document.getElementById("assignInput").value.trim();
    if (!itemName) {
        alert("Please enter an item name first!");
        return;
    }

    // Send item name to ESP32
    fetch(`/assign?item=${encodeURIComponent(itemName)}`)
        .then(response => response.text())
        .then(data => {
            alert(data); // e.g. “Card assigned: Apple”
        })
        .catch(err => {
            console.error("Error:", err);
            alert("Failed to assign card.");
        });
    });

    const assignInput = document.getElementById("assignInput");
    const charCount = document.getElementById("charCount");
    const maxLen = assignInput.maxLength;

    assignInput.addEventListener("input", () => {
        const length = assignInput.value.length;
        charCount.textContent = `${length} / ${maxLen}`;
        charCount.style.color = (length == maxLen) ? "red" : "gray";
    });
        
</script>


</body>
</html>
)=====");





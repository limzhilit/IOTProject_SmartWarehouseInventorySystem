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

/*
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

*/
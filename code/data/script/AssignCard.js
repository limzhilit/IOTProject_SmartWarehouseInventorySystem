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

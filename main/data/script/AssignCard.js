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
function assign() {
  const itemName = document.getElementById("assignInput").value;

  if (itemName && itemName.trim() !== "") {
    fetch(`/assign?item=${encodeURIComponent(itemName.substring(0, 16))}`)
      .then(res => res.text())
      .then(msg => {
        alert(msg);
        resetAssignUI();
      })
      .catch(err => {
        console.error("Assign failed:", err);
        resetAssignUI();
      });
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

function assignCard() {
  let assignCancelled = false;

  // Update UI
  document.getElementById("assignCardBtn").style.display = "none";
  document.getElementById("waiting").style.display = "block";
  document.getElementById("cancel").style.display = "block";

  const cancelBtn = document.getElementById("cancel");

  const onCancel = () => {
    assignCancelled = true;
    resetAssignUI();
  };

  cancelBtn.addEventListener("click", onCancel);

  fetch("/waitForCard")
    .then(waitResponse => waitResponse.text())
    .then(waitText => {
      if (assignCancelled) return;

      if (waitText.includes("detected")) {
        // Card detected → ask for name
        document.getElementById("assign").style.display = "block";
        document.getElementById("waiting").style.display = "none";
        document.getElementById("getName").style.display = "block";

        document
          .getElementById("assign")
          .addEventListener("click", assign);
      } else {
        // No card detected
        document.getElementById("cancel").style.display = "none";
        document.getElementById("waiting").textContent = "No card detected.";

        setTimeout(() => {
          resetAssignUI();
          document.getElementById("waiting").textContent =
            "Waiting for card...";
        }, 3000);
      }
    })
    .catch(err => {
      console.error("Wait for card failed:", err);
      resetAssignUI();
    })
    .finally(() => {
      cancelBtn.removeEventListener("click", onCancel);
    });
}


document.getElementById("assignCardBtn").addEventListener("click", assignCard);

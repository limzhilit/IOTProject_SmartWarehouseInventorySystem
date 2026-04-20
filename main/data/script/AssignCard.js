import { ws } from './WebSocket.js';

const $ = id => document.getElementById(id);

// UI elements
const assignBtn = $("assignCardBtn");
const waiting = $("waiting");
const cancel = $("cancel");
const getName = $("getName");
const assign = $("assign");
const input = $("assignInput");
const charCount = $("charCount");
const timeout = $("timeout");
const assigning = $("assigning");

const maxLen = 15;

// ----------------- Helpers -----------------
const show = (el, visible = true) => el && (el.style.display = visible ? "block" : "none");

const reset = () => {
  show(assignBtn);
  show(waiting, false);
  show(cancel, false);
  show(getName, false);
  show(assign, false);
  show(timeout, false)
  show(assigning, false)
};

const updateCharCount = () => {
  if (!input || !charCount) return;
  const len = input.value.length;
  charCount.textContent = `${len} / ${maxLen}`;
  charCount.style.color = len === maxLen ? "red" : "gray";
};

const assignItem = () => {
  if (!input) return;
  const item = input.value.trim().substring(0, maxLen);
  if (!item) {
    console.log("No item name provided");
    return;
  }
  ws.send(JSON.stringify({ type: "assign", item }));
  show(getName, false);
  show(assign, false);
  show(cancel, false);
  show(assigning);

};

export const assignSuccessful = () => {

  reset();
};

const startWaiting = () => {
  show(assignBtn, false);
  show(waiting);
  show(cancel);
  ws.send(JSON.stringify({ type: "assignCard" }));
};

const cancelAssignCard = () => {
  ws.send(JSON.stringify({ type: "cancelAssignCard" }));
  reset();
};

export const cardTimeout = () => {
  show(timeout, true);
  show(waiting, false);
  setTimeout(() => {
    reset();
  }, 2000); // 2000 ms = 2 seconds
};

// ----------------- WebSocket Message Handler -----------------
export function handleAssignCard(msg) {
  show(getName);
  show(assign);
  show(waiting, false);
}

// ----------------- Event Listeners -----------------
// check if the element exists before adding event listener
assignBtn?.addEventListener("click", startWaiting);
cancel?.addEventListener("click", cancelAssignCard);
assign?.addEventListener("click", assignItem);
input?.addEventListener("input", updateCharCount);

// ----------------- Init -----------------
reset();
updateCharCount();

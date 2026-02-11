let lastSend = 0;
const SEND_INTERVAL = 120; // ms

const radial = document.getElementById("radial");
const knob = document.getElementById("knob");

let dragging = false;

const RADIUS = 150;      // half of radial size (200px)
const KNOB_RADIUS = 20;  // half of knob size (40px)
const MAX_DIST = RADIUS - KNOB_RADIUS;

// ------------------------
// Event Listeners
// ------------------------
knob.addEventListener("mousedown", startDrag);
knob.addEventListener("touchstart", startDrag);

document.addEventListener("mouseup", endDrag);
document.addEventListener("touchend", endDrag);

document.addEventListener("mousemove", handleDrag);
document.addEventListener("touchmove", handleDrag);

// ------------------------
// Functions
// ------------------------
function startDrag(e) {
  dragging = true;
  e.preventDefault(); // prevent text selection or scrolling
}

function endDrag() {
  dragging = false;
  resetKnob();

  // Reset displays
  document.getElementById("angleDisplay").textContent = "0°";
  document.getElementById("speedDisplay").textContent = "0";

  sendToRover(0, 0); // stop the rover
  setTimeout(() => sendToRover(0, 0), SEND_INTERVAL); // ensure stop signal
}

function resetKnob() {
  knob.style.left = "50%";
  knob.style.top = "50%";
}

function sendToRover(direction, speed) {
  const now = Date.now();
  if (now - lastSend < SEND_INTERVAL) return;

  fetch(`/handleRover?direction=${direction}&speed=${speed}`)
    .catch(err => console.error("ESP error:", err));

  lastSend = now;
}

function handleDrag(e) {
  if (!dragging) return;
  e.preventDefault();

  const rect = radial.getBoundingClientRect();
  const cx = rect.left + rect.width / 2;
  const cy = rect.top + rect.height / 2;

  const clientX = e.touches ? e.touches[0].clientX : e.clientX;
  const clientY = e.touches ? e.touches[0].clientY : e.clientY;

  let dx = clientX - cx;
  let dy = clientY - cy;

  let dist = Math.sqrt(dx * dx + dy * dy);
  const angle = Math.atan2(dy, dx);

  if (dist > MAX_DIST) dist = MAX_DIST;

  const x = dist * Math.cos(angle);
  const y = dist * Math.sin(angle);

  knob.style.left = `calc(50% + ${x}px)`;
  knob.style.top = `calc(50% + ${y}px)`;

  // Convert angle to degrees and normalize
  let degrees = angle * (180 / Math.PI) + 90;
  if (degrees < 0) degrees += 360;
  if (degrees >= 360) degrees -= 360;

  const distance = (dist / MAX_DIST) * 135 + 120;

  // Update displays
  document.getElementById("angleDisplay").textContent = `${Math.round(degrees)}°`;
  document.getElementById("speedDisplay").textContent = Math.round(distance / 255 * 100);

  console.log(`Degrees: ${degrees.toFixed(2)}, Distance: ${distance.toFixed(2)}`);
  sendToRover(degrees, distance);
}

// Optional: fetch the index page
function fetchIndex() {
  fetch("/").catch(err => console.error(err));
}


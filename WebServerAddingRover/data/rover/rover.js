const radial = document.getElementById("radial");
const knob = document.getElementById("knob");

let dragging = false;

const radius = 100;      // half of radial (200px)
const knobRadius = 20;   // half of knob (40px)
const maxDist = radius - knobRadius;

knob.addEventListener("mousedown", (e) => {
  dragging = true;
  e.preventDefault(); // important to avoid text selection
});

knob.addEventListener("touchstart", (e) => {
  dragging = true;
  e.preventDefault(); // important to avoid text selection
});

document.addEventListener("mouseup", () => {
  dragging = false;
  // Reset knob position
  knob.style.left = "50%";
  knob.style.top = "50%";
});

document.addEventListener("touchend", () => {
  dragging = false;
  // Reset knob position
  knob.style.left = "50%";
  knob.style.top = "50%";
});


let lastSend = 0;
const sendInterval = 30; // ms → about 33 times/second

function sendToRover(direction, speed) {
  const now = Date.now();
  if (now - lastSend < sendInterval) return;
  fetch("http://172.20.10.2/rover?direction=" + direction + "&speed=" + speed)
    .catch(err => console.log("ESP error:", err));
  lastSend = now;
}

function handleDrag(e) {
  if (!dragging) return;
  e.preventDefault();

  const rect = radial.getBoundingClientRect();
  const cx = rect.left + rect.width / 2;
  const cy = rect.top + rect.height / 2;

  // Get coordinates for mouse or touch
  const clientX = e.touches ? e.touches[0].clientX : e.clientX;
  const clientY = e.touches ? e.touches[0].clientY : e.clientY;

  const dx = clientX - cx;
  const dy = clientY - cy;

  let dist = Math.sqrt(dx * dx + dy * dy);
  const angle = Math.atan2(dy, dx);

  if (dist > maxDist) dist = maxDist;

  const x = dist * Math.cos(angle);
  const y = dist * Math.sin(angle);

  knob.style.left = `calc(50% + ${x}px)`;
  knob.style.top = `calc(50% + ${y}px)`;

  sendToRover(angle, dist / maxDist * 100);
}

document.addEventListener("mousemove", handleDrag);
document.addEventListener("touchmove", handleDrag);

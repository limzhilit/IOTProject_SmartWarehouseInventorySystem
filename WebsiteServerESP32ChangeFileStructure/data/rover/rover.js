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

document.addEventListener("mouseup", () => {
  dragging = false;
  // Reset knob position
  knob.style.left = "50%";
  knob.style.top = "50%";
});

document.addEventListener("mousemove", (e) => {
  if (!dragging) return;

  const rect = radial.getBoundingClientRect();
  const cx = rect.left + rect.width / 2;
  const cy = rect.top + rect.height / 2;

  const dx = e.clientX - cx;
  const dy = e.clientY - cy;

  let dist = Math.sqrt(dx * dx + dy * dy);
  const angle = Math.atan2(dy, dx);

  if (dist > maxDist) dist = maxDist;

  const x = dist * Math.cos(angle);
  const y = dist * Math.sin(angle);

  knob.style.left = `calc(50% + ${x}px)`;
  knob.style.top = `calc(50% + ${y}px)`;
});

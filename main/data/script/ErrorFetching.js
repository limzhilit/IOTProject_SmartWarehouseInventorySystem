// ----------------- Error Handling -----------------

export function handleError(msg) {
  console.error("Server error:", msg.message ?? msg);
}

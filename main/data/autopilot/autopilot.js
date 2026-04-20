

const video = document.getElementById('video');
navigator.mediaDevices.getUserMedia({ video: true }).then(stream => {
  video.srcObject = stream;
});

let src = new cv.Mat(video.height, video.width, cv.CV_8UC4);
let dst = new cv.Mat();
let cap = new cv.VideoCapture(video);

function processFrame() {
  cap.read(src);

  // Convert to grayscale (needed for ArUco detection)
  cv.cvtColor(src, dst, cv.COLOR_RGBA2GRAY);

  // ArUco detection
  let dictionary = new cv.aruco.getPredefinedDictionary(cv.aruco.DICT_6X6_250);
  let corners = new cv.MatVector();
  let ids = new cv.Mat();
  let rejected = new cv.MatVector();
  cv.aruco.detectMarkers(dst, dictionary, corners, ids, new cv.aruco.DetectorParameters(), rejected);

  if (!ids.empty()) {
    // For each detected marker, estimate pose
    let markerLength = 0.05; // meters
    let cameraMatrix = cv.matFromArray(3, 3, cv.CV_64F, [
      800, 0, 320,
      0, 800, 240,
      0, 0, 1
    ]);
    let distCoeffs = cv.Mat.zeros(5, 1, cv.CV_64F);
    let rvecs = new cv.Mat();
    let tvecs = new cv.Mat();

    cv.aruco.estimatePoseSingleMarkers(corners, markerLength, cameraMatrix, distCoeffs, rvecs, tvecs);

    // Draw markers and axes
    for (let i = 0; i < ids.rows; i++) {
      cv.aruco.drawDetectedMarkers(src, corners);
      let rvec = rvecs.row(i);
      let tvec = tvecs.row(i);
      cv.aruco.drawAxis(src, cameraMatrix, distCoeffs, rvec, tvec, 0.03);
      // Extract data to send back if needed:
      console.log(`ID:${ids.data32S[i]}, tvec:${tvec.data64F}`);
    }
  }

  cv.imshow('canvas', src);  // show result
  requestAnimationFrame(processFrame);
}

requestAnimationFrame(processFrame);

const socket = new WebSocket("ws://localhost:8080");

function sendMarkerData(id, tvec, rvec) {
  socket.send(JSON.stringify({ id, tvec: Array.from(tvec.data64F), rvec: Array.from(rvec.data64F) }));
}
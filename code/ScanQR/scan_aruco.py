import cv2
import numpy as np

# Load the dictionary
aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)
if hasattr(cv2.aruco, "DetectorParameters_create"):
    parameters = cv2.aruco.DetectorParameters_create()
else:
    parameters = cv2.aruco.DetectorParameters()
detector = cv2.aruco.ArucoDetector(aruco_dict, parameters) if hasattr(cv2.aruco, "ArucoDetector") else None

# Start camera
cap = cv2.VideoCapture(0)
print("Starting camera... Press Q to quit")

while True:
    ret, frame = cap.read()
    if not ret:
        break

    gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)

    # Detect markers
    if detector is not None:
        corners, ids, rejectedImgPoints = detector.detectMarkers(gray)
    else:
        corners, ids, rejectedImgPoints = cv2.aruco.detectMarkers(gray, aruco_dict, parameters=parameters)

    if ids is not None:
        # Draw detected markers
        cv2.aruco.drawDetectedMarkers(frame, corners, ids)

        for i, corner in enumerate(corners):
            c = corner[0]  # 4 corner points
            print(f"Marker ID: {ids[i][0]}")
            print(f"Corners: {c}")  # [top-left, top-right, bottom-right, bottom-left]

    cv2.imshow("ArUco Scanner", frame)

    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()



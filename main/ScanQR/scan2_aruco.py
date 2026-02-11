import cv2
import numpy as np

# --- Setup ---
# The Camera Matrix (K) defines focal length and optical center.
# It maps 3D world points to 2D image pixels.
camera_matrix = np.array([[800, 0, 320], [0, 800, 240], [0, 0, 1]], dtype=float)

# Distortion coefficients account for lens "curvature."
# Using zeros assumes a perfect "pinhole" lens (no distortion).
dist_coeffs = np.zeros((5, 1))

# Physical size of the marker in meters (e.g., 0.05 = 5cm).
# Critical for accurate distance (tvec) calculation.
marker_length = 0.05

# Select the ArUco dictionary (grid size 6x6, 250 unique IDs).
aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_250)
parameters = cv2.aruco.DetectorParameters()
detector = cv2.aruco.ArucoDetector(aruco_dict, parameters)

# Define the 3D coordinates of the four marker corners in its own local coordinate system.
# The Z-axis is 0 because the marker is a flat square.
obj_points = np.array([[-marker_length / 2, marker_length / 2, 0],
                       [marker_length / 2, marker_length / 2, 0],
                       [marker_length / 2, -marker_length / 2, 0],
                       [-marker_length / 2, -marker_length / 2, 0]], dtype=np.float32)

cap = cv2.VideoCapture(0)  # Open the default webcam

while True:
  ret, frame = cap.read()  # Read a single frame from the camera
  if not ret: break  # Exit if the camera fails

  # Detect 2D corners and IDs of any markers found in the frame
  corners, ids, _ = detector.detectMarkers(frame)

  if ids is not None:
    for i in range(len(ids)):
      # solvePnP calculates the pose (Rotation and Translation) by comparing
      # the known 3D obj_points to the detected 2D corners in the image.
      ret, rvec, tvec = cv2.solvePnP(obj_points, corners[i], camera_matrix, dist_coeffs)

      if ret:
        # Draws the Red(X), Green(Y), and Blue(Z) axes on the marker.
        cv2.drawFrameAxes(frame, camera_matrix, dist_coeffs, rvec, tvec, 0.03)
        cv2.aruco.drawDetectedMarkers(frame, corners)

        # Rodrigues converts the 3-element rotation vector into a 3x3 matrix.
        rmat, _ = cv2.Rodrigues(rvec)

        # Trigonometry to extract Euler angles (in degrees) from the rotation matrix.
        pitch = np.degrees(np.arctan2(-rmat[2, 0], np.sqrt(rmat[2, 1] ** 2 + rmat[2, 2] ** 2)))
        yaw = np.degrees(np.arctan2(rmat[1, 0], rmat[0, 0]))
        roll = np.degrees(np.arctan2(rmat[2, 1], rmat[2, 2]))

        # Get pixel coordinates for the top-left corner to place text labels.
        x, y = int(corners[i][0][0][0]), int(corners[i][0][0][1])

        # Create text strings for ID, Distance (tvec[2] is depth), and Angles.
        text = f"ID:{ids[i][0]} Dist:{tvec[2][0]:.2f}m"
        rot_text = f"P:{pitch:.1f} Y:{yaw:.1f} R:{roll:.1f}"

        # Overlay the text onto the video frame.
        cv2.putText(frame, text, (x, y - 40), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
        cv2.putText(frame, rot_text, (x, y - 15), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 0), 2)

  cv2.imshow('Live Tracking', frame)  # Show the window
  if cv2.waitKey(1) & 0xFF == ord('q'): break  # Exit on 'q' key

cap.release()
cv2.destroyAllWindows()



# Version 4
# import cv2
# import numpy as np
#
# # --- Setup ---
# camera_matrix = np.array([[800, 0, 320], [0, 800, 240], [0, 0, 1]], dtype=float)
# dist_coeffs = np.zeros((5, 1))
# marker_length = 0.05
#
# aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_250)
# parameters = cv2.aruco.DetectorParameters()
# detector = cv2.aruco.ArucoDetector(aruco_dict, parameters)
#
# # 3D points of marker
# obj_points = np.array([[-marker_length / 2, marker_length / 2, 0],
#                        [marker_length / 2, marker_length / 2, 0],
#                        [marker_length / 2, -marker_length / 2, 0],
#                        [-marker_length / 2, -marker_length / 2, 0]], dtype=np.float32)
#
# cap = cv2.VideoCapture(0)
#
# while True:
#   ret, frame = cap.read()
#   if not ret: break
#
#   corners, ids, _ = detector.detectMarkers(frame)
#
#   if ids is not None:
#     for i in range(len(ids)):
#       ret, rvec, tvec = cv2.solvePnP(obj_points, corners[i], camera_matrix, dist_coeffs)
#
#       if ret:
#         # 1. Draw 3D Axes and Square
#         cv2.drawFrameAxes(frame, camera_matrix, dist_coeffs, rvec, tvec, 0.03)
#         cv2.aruco.drawDetectedMarkers(frame, corners)
#
#         # 2. Convert rotation vector to degrees (Euler angles)
#         rmat, _ = cv2.Rodrigues(rvec)
#         # Extract angles from rotation matrix
#         pitch = np.degrees(np.arctan2(-rmat[2, 0], np.sqrt(rmat[2, 1] ** 2 + rmat[2, 2] ** 2)))
#         yaw = np.degrees(np.arctan2(rmat[1, 0], rmat[0, 0]))
#         roll = np.degrees(np.arctan2(rmat[2, 1], rmat[2, 2]))
#
#         # 3. Annotate Video Frame
#         x, y = int(corners[i][0][0][0]), int(corners[i][0][0][1])
#
#         # Format string for display
#         text = f"ID:{ids[i][0]} Dist:{tvec[2][0]:.2f}m"
#         rot_text = f"P:{pitch:.1f} Y:{yaw:.1f} R:{roll:.1f}"
#
#         cv2.putText(frame, text, (x, y - 40), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)
#         cv2.putText(frame, rot_text, (x, y - 15), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (255, 255, 0), 2)
#
#   cv2.imshow('Live Tracking', frame)
#   if cv2.waitKey(1) & 0xFF == ord('q'): break
#
# cap.release()
# cv2.destroyAllWindows()

######## Version 3
# import cv2
# import numpy as np
#
# # 1. Provide calibration data (Use your actual calibration values here)
# # If you don't have them, these are generic placeholders (less accurate)
# camera_matrix = np.array([[800, 0, 320], [0, 800, 240], [0, 0, 1]], dtype=float)
# dist_coeffs = np.zeros((5, 1))
#
# # 2. Define physical marker size (e.g., 0.05 meters = 5cm)
# marker_length = 0.05
#
# # Set up detector
# aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_250)
# parameters = cv2.aruco.DetectorParameters()
# detector = cv2.aruco.ArucoDetector(aruco_dict, parameters)
#
# # Define 3D points of a square marker in its own coordinate system
# obj_points = np.array([
#   [-marker_length / 2, marker_length / 2, 0],
#   [marker_length / 2, marker_length / 2, 0],
#   [marker_length / 2, -marker_length / 2, 0],
#   [-marker_length / 2, -marker_length / 2, 0]
# ], dtype=np.float32)
#
# cap = cv2.VideoCapture(0)
#
# while True:
#   ret, frame = cap.read()
#   if not ret: break
#
#   corners, ids, _ = detector.detectMarkers(frame)
#
#   if ids is not None:
#     for i in range(len(ids)):
#       # Solve for Pose (Rotation and Translation)
#       ret, rvec, tvec = cv2.solvePnP(obj_points, corners[i], camera_matrix, dist_coeffs)
#
#       if ret:
#         # Draw the marker border
#         cv2.aruco.drawDetectedMarkers(frame, corners)
#         # Draw the 3D axes (Red=X, Green=Y, Blue=Z)
#         cv2.drawFrameAxes(frame, camera_matrix, dist_coeffs, rvec, tvec, 0.03)
#
#         # rvec contains the rotation in Rodrigues format (radians)
#         # print(f"Rotation Vector for ID {ids[i]}: {rvec.flatten()}")
#
#   cv2.imshow('Pose Estimation', frame)
#   if cv2.waitKey(1) & 0xFF == ord('q'): break
#
# cap.release()
# cv2.destroyAllWindows()

######### Version 2
# import cv2
# import numpy as np
#
# # Initialize the camera (0 is usually the default webcam)
# cap = cv2.VideoCapture(0)
#
# # Set up the ArUco detector (matching your DICT_6X6_250 dictionary)
# aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_250)
# parameters = cv2.aruco.DetectorParameters()
# detector = cv2.aruco.ArucoDetector(aruco_dict, parameters)
#
# print("Press 'q' to quit.")
#
# while True:
#   # Capture frame-by-frame
#   ret, frame = cap.read()
#   if not ret:
#     break
#
#   # Convert to grayscale for detection
#   gray = cv2.cvtColor(frame, cv2.COLOR_BGR2GRAY)
#
#   # Detect markers
#   corners, ids, rejected = detector.detectMarkers(gray)
#
#   # If markers are detected, draw them on the frame
#   if ids is not None:
#     cv2.aruco.drawDetectedMarkers(frame, corners, ids)
#
#     # Optional: Log the IDs to the console
#     # print(f"Detected IDs: {ids.flatten()}")
#
#   # Display the resulting frame
#   cv2.imshow('ArUco Video Detection', frame)
#
#   # Exit if 'q' is pressed
#   if cv2.waitKey(1) & 0xFF == ord('q'):
#     break

# Clean up
cap.release()
cv2.destroyAllWindows()

######### Version 1
# import cv2
# import numpy as np
#
# # Load the image
# image = cv2.imread('/content/marker_42.png')
#
# # Convert the image to grayscale
# gray = cv2.cvtColor(image, cv2.COLOR_BGR2GRAY)
# aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_6X6_250)
# parameters = cv2.aruco.DetectorParameters()
#
# # Create the ArUco detector
# detector = cv2.aruco.ArucoDetector(aruco_dict, parameters)
# # Detect the markers
# corners, ids, rejected = detector.detectMarkers(gray)
# # Print the detected markers
# print("Detected markers:", ids)
# if ids is not None:
#     cv2.aruco.drawDetectedMarkers(image, corners, ids)
#     cv2.imshow('Detected Markers', image)
#     cv2.waitKey(0)
#     cv2.destroyAllWindows()
import cv2
import numpy as np

# Choose a dictionary of markers
aruco_dict = cv2.aruco.getPredefinedDictionary(cv2.aruco.DICT_4X4_50)  # 4x4 markers, 50 unique IDs

marker_id = 0  # ID of the marker to generate (0-49 in this dictionary)
marker_size = 200  # pixels

# Generate marker image
marker_img = np.zeros((marker_size, marker_size), dtype=np.uint8)
if hasattr(cv2.aruco, "generateImageMarker"):
    cv2.aruco.generateImageMarker(aruco_dict, marker_id, marker_size, marker_img, 1)
else:
    marker_img = cv2.aruco.drawMarker(aruco_dict, marker_id, marker_size, marker_img, 1)

# Save to file
cv2.imwrite(f"aruco_{marker_id}.png", marker_img)
print(f"Saved marker as aruco_{marker_id}.png")

# Optional: show marker
cv2.imshow("ArUco Marker", marker_img)
cv2.waitKey(0)
cv2.destroyAllWindows()

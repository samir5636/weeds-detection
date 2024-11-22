import cv2
from ultralytics import YOLO
import os
import sys

# Suppress model logs
class SuppressOutput:
    def __enter__(self):
        self._original_stdout = sys.stdout
        self._original_stderr = sys.stderr
        sys.stdout = open(os.devnull, 'w')
        sys.stderr = open(os.devnull, 'w')

    def __exit__(self, exc_type, exc_value, traceback):
        sys.stdout.close()
        sys.stderr.close()
        sys.stdout = self._original_stdout
        sys.stderr = self._original_stderr

# Load the trained YOLOv8 model
model = YOLO('/home/samir/weeds/weeds-detection/weeds-training-results/train6/weights/best.pt')  # Update with your model's path

# Replace the URL with the IP address and port of your IP Webcam server
url = 'http://100.86.77.151:8080/video'
cap = cv2.VideoCapture(url)

while True:
    ret, frame = cap.read()
    if not ret:
        print("Failed to capture frame. Exiting...")
        break
    
    frame = cv2.resize(frame, (950,950))

    # Convert the frame to RGB (YOLO expects RGB images)
    frame_rgb = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)

    # Suppress logs during prediction
    with SuppressOutput():
        results = model.predict(source=frame_rgb, imgsz=640, conf=0.5, device='cpu')  # Adjust conf and device as needed

    # Extract predictions
    for result in results:
        boxes = result.boxes.cpu().numpy()  # Bounding boxes
        for box in boxes:
            x1, y1, x2, y2 = map(int, box.xyxy[0])  # Coordinates
            conf = box.conf[0]  # Confidence
            cls = int(box.cls[0])  # Class ID
            label = f"{model.names[cls]} {conf:.2f}"

            # Draw bounding box and label
            cv2.rectangle(frame, (x1, y1), (x2, y2), (0, 255, 0), 2)
            cv2.putText(frame, label, (x1, y1 - 10), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (0, 255, 0), 2)

    # Display the frame with predictions
    cv2.imshow('Weed Detection', frame)

    # Press 'q' to exit
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

cap.release()
cv2.destroyAllWindows()

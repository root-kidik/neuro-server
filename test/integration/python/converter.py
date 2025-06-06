from ultralytics import YOLO

# Load a YOLOv8 model
model = YOLO("./yolo11l.pt")

# Export the model
model.export(format="onnx", opset=12, simplify=True, dynamic=False, imgsz=640)

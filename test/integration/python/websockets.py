import cv2
import asyncio
import test.integration.python.websockets as websockets
from concurrent.futures import ThreadPoolExecutor

async def video_stream(websocket, path):
    print("Client connected")
    
    cap = cv2.VideoCapture("/workspaces/neuro-server/test/integration/python/cat.mp4")
    fps = cap.get(cv2.CAP_PROP_FPS)
    frame_delay = 1 / fps if fps > 0 else 0.03
    
    try:
        while cap.isOpened():
            ret, frame = cap.read()
            if not ret:
                break
                
            # Кодируем кадр в JPEG
            _, jpeg = cv2.imencode(".jpg", frame, [int(cv2.IMWRITE_JPEG_QUALITY), 85])
            data = jpeg.tobytes()
            
            # Отправляем через WebSocket
            try:
                await websocket.send(data)
            except websockets.exceptions.ConnectionClosed:
                print("Client disconnected")
                break
                
            await asyncio.sleep(frame_delay)
    finally:
        cap.release()
        print("Stream stopped")

async def main():
    async with websockets.serve(video_stream, "0.0.0.0", 8765):
        print("WebSocket server started on ws://0.0.0.0:8765")
        await asyncio.Future()  # Бесконечное ожидание

if __name__ == "__main__":
    asyncio.run(main())

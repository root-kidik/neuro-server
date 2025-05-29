import cv2
import socket
import struct
import time

async def test_startup_shutdown(neuro_server):
    pass

async def test_video(neuro_server):
    CHUNK_SIZE = 1024

    cap = cv2.VideoCapture("/workspaces/neuro-server/test/integration/python/cat.mp4")

    sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

    server_addr = ("localhost", 8010)

    try:
        fps = cap.get(cv2.CAP_PROP_FPS)
        frame_delay = 1 / fps if fps > 0 else 0.03  # 30 FPS

        while True:
            ret, frame = cap.read()
            if not ret:
                break

            _, jpeg = cv2.imencode(".jpg", frame, [int(cv2.IMWRITE_JPEG_QUALITY), 85])
            data = jpeg.tobytes()

            total_bytes = len(data)

            sock.sendto(struct.pack("<I", total_bytes), server_addr)

            for offset in range(0, total_bytes, CHUNK_SIZE):
                chunk = data[offset:min(offset + CHUNK_SIZE, total_bytes)]
                sock.sendto(chunk, server_addr)
                time.sleep(frame_delay / (total_bytes / CHUNK_SIZE))

    finally:
        cap.release()
        sock.close()

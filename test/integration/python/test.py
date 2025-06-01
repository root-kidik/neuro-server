import socket
import cv2
import numpy as np

# Конфигурация UDP
UDP_IP = "192.168.0.12"
UDP_PORT = 8010
BUFFER_SIZE = 1500

# Словари для сборки кадров
current_frames = {}

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((UDP_IP, UDP_PORT))
print(f"UDP server started on {UDP_IP}:{UDP_PORT}")

while True:
    try:
        data, addr = sock.recvfrom(BUFFER_SIZE)
        if len(data) < 10:
            continue

        # Разбор заголовка
        frame_id = int.from_bytes(data[0:4], 'little')
        packet_idx = int.from_bytes(data[4:6], 'little')
        total_packets = int.from_bytes(data[6:8], 'little')
        data_size = int.from_bytes(data[8:10], 'little')
        payload = data[10:10+data_size]

        # Инициализация кадра
        if frame_id not in current_frames:
            current_frames[frame_id] = {
                'packets': [None] * total_packets,
                'received': 0
            }
        
        frame_data = current_frames[frame_id]
        
        # Сохраняем пакет
        if packet_idx < total_packets:
            if frame_data['packets'][packet_idx] is None:
                frame_data['received'] += 1
            frame_data['packets'][packet_idx] = payload

        # Проверяем готовность кадра
        if frame_data['received'] == total_packets:
            # Собираем полный кадр
            full_frame = b''.join(frame_data['packets'])
            
            # Декодируем изображение
            img = cv2.imdecode(np.frombuffer(full_frame, dtype=np.uint8), cv2.IMREAD_COLOR)
            if img is not None:
                cv2.imshow('UDP Stream', img)
            
            # Удаляем собранный кадр
            del current_frames[frame_id]
            
            # Очищаем старые кадры (> 5 секунд)
            for fid in list(current_frames.keys()):
                if abs(fid - frame_id) > 100:  # Примерно 3-5 секунд при 30 FPS
                    del current_frames[fid]
            
            if cv2.waitKey(1) == 27:
                break
                
    except Exception as e:
        print(f"Error: {e}")

sock.close()
cv2.destroyAllWindows()
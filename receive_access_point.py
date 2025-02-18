import socket
import csv

# Cấu hình địa chỉ IP và cổng để nhận dữ liệu từ ESP32
UDP_IP = "192.168.4.1"
HOST = '0.0.0.0'  # Lắng nghe trên tất cả các giao diện mạng
PORT = 8888       # Cổng UDP mà ESP32 sẽ gửi dữ liệu tới

# Tạo socket UDP để nhận dữ liệu
sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind((HOST, PORT))

sock.sendto(b"READY", (UDP_IP, PORT))

print(f"Listening for UDP packets at port {PORT}")

csv_file_name = 'imu_data_accessPoint.csv'

# Kiểm tra xem tệp đã tồn tại chưa, nếu chưa thì ghi tiêu đề
try:
    with open(csv_file_name, 'a', newline='') as file:
        writer = csv.writer(file)
        writer.writerow(["Time (s)", "Gyroscope X (deg/s)", "Gyroscope Y (deg/s)", "Gyroscope Z (deg/s)", "Accelerometer X (g)", "Accelerometer Y (g)", "Accelerometer Z (g)"])
except FileExistsError:
    pass

try:
    while True:
        # Chờ nhận gói tin từ ESP32
        message, addr = sock.recvfrom(1024)  # Buffer size is 1024 bytes

        # Giải mã thông điệp
        data = message.decode('utf-8').strip()
        print(f"Received data: {data}")  # Debug: print received data

        parts = data.split(';')
        esp_time = parts[0].split('=')[1].strip('s')
        accel_info = parts[1].split(': ')[1].strip(', ').split(',')
        gyro_info = parts[2].split(': ')[1].strip(', ').split(',')
        # mag_info = parts[3].split(': ')[1].strip(', ').split(',')

        with open(csv_file_name, 'a', newline='') as file:
            writer = csv.writer(file)
            writer.writerow([
                esp_time,
                *gyro_info, *accel_info
            ])  

except KeyboardInterrupt:
    print("Program stopped")
finally:
    sock.sendto(b"END", (UDP_IP, PORT))
    sock.close()

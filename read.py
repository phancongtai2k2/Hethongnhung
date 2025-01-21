import serial
import csv

# Cấu hình cổng COM và file CSV
PORT = 'COM3'  # Thay bằng cổng COM bạn đang sử dụng
BAUDRATE = 9600
CSV_FILE = 'D:\QTdesigner\data\outputtest.csv'

with open(CSV_FILE, mode='w') as f:
        f.write('Package received,time received,time send,package send,rssi,snr,freqError\n')

# Kết nối tới cổng serial
try:
    ser = serial.Serial(PORT, BAUDRATE, timeout=1)
    print(f"Kết nối thành công tới {PORT} với baudrate {BAUDRATE}")
    
    # Mở file CSV để ghi dữ liệu
    with open(CSV_FILE, mode='a') as f:        
        while True:
            try:
                # Đọc một dòng từ serial
                raw_data = ser.readline().decode('utf-8', errors='ignore').strip()
                arr = raw_data.split(',')
                print(f"Dữ liệu nhận được: {raw_data}")
                if raw_data and raw_data != 'E' and len(arr) == 7:  # Nếu dòng dữ liệu không rỗng
                    
                    # Ghi dữ liệu vào file CSV
                    f.write(raw_data + '\n')
            except KeyboardInterrupt:
                print("Đang dừng ghi dữ liệu...")
                break
            except Exception as e:
                print(f"Lỗi trong quá trình xử lý: {e}")
except serial.SerialException as e:
    print(f"Lỗi kết nối tới cổng COM: {e}")

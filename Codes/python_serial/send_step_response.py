import serial
import time
import csv
from datetime import datetime

PORT = 'COM3'
BAUD_RATE = 115200

try:
    ser = serial.Serial(PORT, BAUD_RATE, timeout=1)
    time.sleep(4)
    ser.reset_input_buffer()

    ser.write(b'40.0\n')
    print("Sent 40 RPM, stabilizing...")
    time.sleep(5)

    with open('step_response.csv', 'w', newline='') as f:
        writer = csv.writer(f)
        writer.writerow(['Timestamp', 'Setpoint', 'Measured_RPM', 'PWM'])

        ser.write(b'80.0\n')
        print("Sent 80 RPM, logging...")

        start = time.time()
        while time.time() - start < 12:
            line = ser.readline().decode().strip()
            try:
                s, m, p = map(float, line.split(','))
                writer.writerow([datetime.now(), s, m, p])
                print(s, m, p)
            except ValueError:
                pass

    print("Done. Data saved to step_response.csv")
    ser.close()

except serial.SerialException as e:
    print(f"Serial error: {e}")

# FP-IoT-A-2024

## Prerequisites

- ESP32 (duh)
- Python w/ YOLOv5
- Arduino IDE

## How to Use

If you have YOLOv5 installed, copy `conn.py` to the directory it's in (the directory where you cloned it). Use it like you would use the `detect.py` file:

```bash
python3 conn.py --weights fp_iot_v3.pt --source 0 --conf 0.4
```

Do not use the Serial Monitor in Arduino IDE, as it uses the active serial connection, like `dev/ttyUSB0` or `COMx`.

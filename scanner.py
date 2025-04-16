import serial
from pymongo import MongoClient
from datetime import datetime
import time

# Setup serial and MongoDB
ser = serial.Serial('/dev/tty.usbmodem1101', 9600, timeout=1)
client = MongoClient("mongodb+srv://misky0310:Miskat0310@cluster0.hupf5dv.mongodb.net/?retryWrites=true&w=majority&appName=Cluster0")
db = client["rfid_db"]
tags_collection = db["authorized_cards"]

print("📋 All tags in DB:")
for tag in tags_collection.find():
    print(f"- UID: '{tag['uid']}'")

print("Listening for RFID tags...")

buffer = []

while True:
    if ser.in_waiting > 0:
        line = ser.readline().decode("utf-8", errors="ignore").strip()
        if line:
            print(f"📥 Raw serial data: '{line}'")
            buffer.append(line)

            # If last line looks like a UID
            if len(line.split()) == 4 and all(len(x) == 2 for x in line.split()):
                uid = line.strip().upper()
                print(f"Scanned UID: {uid}")

                tag = tags_collection.find_one({"uid": uid})
                print(f"Tag found in database: {tag}")

                if tag:
                    print("✅ Access granted")
                    ser.write(b"UNLOCK\n")
                else:
                    print("❌ Access denied")
                    ser.write(b"DENIED\n")

                buffer.clear()
        time.sleep(0.1)

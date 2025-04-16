# 🔐 RFID Based Door Lock Control System with Data Logging

An experimental prototype that integrates an RFID scanner with an Arduino Uno to control a door locking mechanism. The system is powered by Flask (Python) for database handling and logging RFID access into MongoDB, with a simple web interface to manage RFID cards.

---



## 🛠️ Hardware Used

- Arduino Uno
- RC522 RFID Module
- Servo Motor
- 16x2 LCD Display (I2C)
- Buzzer + RGB LEDs
- Breadboard + Jumper Wires
- RFID Tags

---

## 🌐 Software Requirements

- Python 3.x
- Flask
- PyMongo
- Arduino IDE
- MongoDB (local or cloud)
- Git

---

## 🐍 Setting Up the Python Virtual Environment

```bash
# 1. Clone the repository
git clone https://github.com/yourusername/rfid-door-lock.git
cd rfid-door-lock

# 2. Create a virtual environment
# Linux/Mac:
python3 -m venv venv

# Windows:
python -m venv venv

# 3. Activate the virtual environment
# Linux/Mac:
source venv/bin/activate

# Windows:
venv\Scripts\activate

# 4. Install required dependencies
pip install -r requirements.txt

# 5. Run the .ino file in arduino ide by choosing usb modem port 1

# 6 . Run the app.py and the scanner.py in the virtual environment
venv/bin/python app.py
venv/bin/python scanner.py

#7. Scan the RFID tag , if not authorised add the tag from the port on which the app.py file is running



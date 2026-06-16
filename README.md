# Smart-helmet-SOS
A hardware-based impact detection and emergency alert system using MPU6050 and a buzzer for real-time rider safety.
### 🛠 Hardware Components
*   **Microcontroller:** ESP32 (Dual-core, built-in Wi-Fi/Bluetooth)
*   **Sensor:** MPU6050 (6-axis Accelerometer + Gyroscope)
*   **Alert:** High-Decibel Buzzer

*************************************************************************************************************************************

## 🔌 Hardware Architecture & Schematic

The complete circuit schematic for this project has been designed using KiCad and is available as a high-resolution vector PDF in the documentation folder.

🔗 **[Click here to view the Full Project Schematic (PDF)](docs/SMART_HELMET_SOS.pdf)**

### Pin Mapping Summary
For quick reference, the core system wiring is mapped as follows:
* **MPU6050 Accelerometer:** SDA $\rightarrow$ **GP21**, SCL $\rightarrow$ **GP22** (I2C Bus)
* **Crash Cancel Button:** Input $\rightarrow$ **GP13** (Configured with Internal Pull-Up)
* **SOS Alarm Buzzer:** Output $\rightarrow$ **GP27** (Drives the 2N2222 Transistor Base)

---

## 📱 How the System Works (The SOS Process)

The project creates a seamless bridge between local hardware sensors and mobile automation to send real-time coordinates during an emergency. The complete sequence flows horizontally as follows:

[ MPU6050 Acceleration Spike ] ──► [ ESP32-S3 Verification (10s Window) ] ──► [ Bluetooth Send: "ALARM_TRIGGER" ] ──► [ MacroDroid Automation Intercept ] ──► [ Mobile GPS Link Sent via SMS ]

### Detailed Step-by-Step Flow:
1. **Sensor Trigger:** The **MPU6050** continuous tracking algorithms detect a sudden deceleration or impact spike exceeding the programmed **22.0 m/s²** threshold.
2. **Local Pre-Alert & Grace Period:** The **ESP32-S3** sounds a local alert through the transistor-driven active buzzer. This starts a **10-second countdown**, allowing the rider to press the physical **Cancel Button (GP13)** if it is a false alarm.
3. **Bluetooth Transmission:** If the countdown expires without user cancellation, the ESP32-S3 establishes a connection to the paired smartphone via onboard Bluetooth Serial and transmits the string: `ALARM_TRIGGER`.
4. **MacroDroid Interception:** A background **MacroDroid automation macro** running on the Android device actively monitors the incoming Bluetooth Serial text log buffer.
5. **GPS & Notification Automation:** Upon identifying the `ALARM_TRIGGER` string, MacroDroid instantly awakens the phone's location services to pull live **GPS coordinates**, compiles an emergency message, and transmits an automated **SMS containing a Google Maps location link** to predetermined emergency contacts.
▼
[ SMS Sent: "ACCIDENT DETECTED + Google Maps Location Link" ]

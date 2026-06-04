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

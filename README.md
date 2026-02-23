# TRIOE_servo_control_web_DEMO


## What This Does

This project lets you control a servo motor from your phone or computer through a web browser. No router needed - the TRIOE board creates its own WiFi network that you connect to directly.

## What You Need

### Hardware
- TRIOE Board (ESP32-based)
- Servo motor (SG90 or similar)
- USB cable for TRIOE board
- Jumper wires (3 minimum)
- Computer for programming

### Software
- Arduino IDE (download from arduino.cc)
- ESP32 board support (instructions below)
- ESP32Servo library (instructions below)

## Step 1: Install Arduino IDE

1. Go to https://www.arduino.cc/en/software
2. Download Arduino IDE for your operating system
3. Install and open Arduino IDE

## Step 2: Add ESP32 Board Support

1. Open Arduino IDE
2. Go to **File** → **Preferences**
3. In "Additional Board Manager URLs" paste:
   ```
   https://dl.espressif.com/dl/package_esp32_index.json
   ```
4. Click **OK**
5. Go to **Tools** → **Board** → **Board Manager**
6. Search for "ESP32"
7. Install "ESP32 by Espressif Systems"
8. Wait for installation to complete
9. Close Board Manager

## Step 3: Install Required Library

1. Go to **Sketch** → **Include Library** → **Manage Libraries**
2. Search for "ESP32Servo"
3. Install "ESP32Servo by Kevin Harrington"
4. Close Library Manager

## Step 4: Wire the Servo

Connect the servo to your TRIOE board:

| Servo Wire Color | TRIOE Board Pin |
|-----------------|-----------------|
| Brown or Black  | GND             |
| Red             | 5V              |
| Orange or Yellow| GPIO 4          |

**Important:** Make sure TRIOE board is unplugged while wiring.

## Step 5: Upload the Code

1. Connect TRIOE board to computer with USB cable
2. Open the file: `TRIOE_servo_control_web_DEMO1.ino`
3. Select board: **Tools** → **Board** → **ESP32 Dev Module**
4. Select port: **Tools** → **Port** → (Choose your TRIOE board COM port)
5. Click the **Upload** button (right arrow icon)
6. Wait for "Done uploading" message

## Step 6: Open Serial Monitor

1. Click **Tools** → **Serial Monitor**
2. Set baud rate to **115200** (bottom right)
3. You should see startup messages
4. Look for these important lines:
   ```
   Network Name (SSID): TRIOE-Servo
   Password: 12345678
   AP IP Address: 192.168.4.1
   ```

## Step 7: Connect to TRIOE WiFi

On your phone or computer:

1. Open WiFi settings
2. Look for network: **TRIOE-Servo**
3. Connect to it
4. Password: **12345678**
5. Wait for connection

## Step 8: Open Web Interface

1. Open any web browser
2. Type in address bar: `http://192.168.4.1`
3. Press Enter
4. You should see the control interface

## Using the Web Interface

### Manual Control

**Slider**
- Drag left to right to move servo from 0 to 180 degrees
- Servo moves smoothly to the position you select

**Preset Buttons**
- Click any button (0, 45, 90, 135, 180) for quick positions
- Useful for common angles

### Automatic Modes

Click these buttons to activate automatic servo movements:

**Manual**
- Full control via slider and buttons
- Default mode

**Sweep**
- Servo sweeps back and forth smoothly
- Continuous motion from 0 to 180 degrees

**Step**
- Servo moves through preset positions
- Stops at: 0, 45, 90, 135, 180 degrees

**Jitter**
- Random movements
- Unpredictable positioning

**Fast Snap**
- Quick back-and-forth between 0 and 180
- Rapid movement

**Slow Creep**
- Very slow movement from 0 to 180
- Smooth and gradual

## Troubleshooting

### Cannot Upload Code

**Problem:** Upload fails or shows errors

**Solutions:**
- Check if correct board is selected (ESP32 Dev Module)
- Check if correct COM port is selected
- Try pressing and holding BOOT button on TRIOE board during upload
- Try a different USB cable
- Close Serial Monitor before uploading

### Cannot See TRIOE-Servo WiFi Network

**Problem:** WiFi network not appearing

**Solutions:**
- Check Serial Monitor - did it start successfully?
- Restart TRIOE board (press reset button)
- Make sure your device WiFi is turned on
- Wait 30 seconds after powering on TRIOE board

### Cannot Open Web Page

**Problem:** Browser cannot connect to 192.168.4.1

**Solutions:**
- Make sure you are connected to TRIOE-Servo WiFi
- Type exactly: `http://192.168.4.1` (include http://)
- Try different browser (Chrome, Firefox, Safari)
- Disconnect and reconnect to TRIOE-Servo WiFi
- Check Serial Monitor for error messages

### Servo Not Moving

**Problem:** Web interface loads but servo does not respond

**Solutions:**
- Check all wire connections
- Verify servo is connected to GPIO 4
- Ensure servo has power (5V and GND connected)
- Look at Serial Monitor when moving controls
- Try a different servo (might be broken)

### Servo Jittering or Weak

**Problem:** Servo shakes or does not have enough power

**Solutions:**
- Use external 5V power supply for servo
- Do not power servo from TRIOE board 5V if it is large
- Check battery/power supply voltage
- Add capacitor between servo power and ground

## Customization

### Change WiFi Network Name

Find this line in the code:
```cpp
const char* ap_ssid = "TRIOE-Servo";
```
Change "TRIOE-Servo" to your preferred name.

### Change WiFi Password

Find this line in the code:
```cpp
const char* ap_password = "12345678";
```
Change "12345678" to your password (minimum 8 characters).

### Change Servo Pin

Find this line in the code:
```cpp
#define SERVO_PIN 4
```
Change `4` to your desired GPIO pin number.

### Change Mode Duration

Find this line in the code:
```cpp
const unsigned long modeDuration = 10000;
```
Change `10000` (10 seconds) to your preferred time in milliseconds.

## Understanding the System

### How It Works

1. TRIOE board creates WiFi network (Access Point mode)
2. You connect your device to this network
3. TRIOE board runs a web server
4. Browser requests the control page
5. JavaScript sends commands to TRIOE board
6. TRIOE board moves servo based on commands
7. Position updates sent back to browser every second

### Network Details

- Network Name: TRIOE-Servo
- Password: 12345678
- IP Address: 192.168.4.1 (always the same)
- No internet connection (offline network)
- Multiple devices can connect simultaneously
- Range: approximately 10-30 meters

### Safety Notes

- Always disconnect power before wiring
- Double-check connections before powering on
- Do not reverse polarity on servo
- Do not connect large servos directly to TRIOE board
- Use external power for multiple or large servos
- Keep workspace clean and dry

## Technical Specifications

### Servo Control
- Angle Range: 0 to 180 degrees
- Update Rate: Real-time via web interface
- Position Accuracy: 1 degree steps
- Control Method: PWM signal on GPIO 4

### Web Server
- Protocol: HTTP
- Port: 80
- Update Interval: 1 second
- Multiple simultaneous connections supported

### WiFi Access Point
- Frequency: 2.4 GHz
- Encryption: WPA2
- Max Clients: Typically 4-8 devices
- Default IP: 192.168.4.1

## Need More Help?

### Resources

- Arduino Forum: https://forum.arduino.cc
- ESP32 Documentation: https://docs.espressif.com
- ESP32Servo Library: https://github.com/madhephaestus/ESP32Servo

### Common Questions

**Q: Can I control this over the internet?**
A: No, this is a local WiFi network only. You need to be within WiFi range.

**Q: Can multiple people control it at once?**
A: Yes, but the last command sent will take effect.

**Q: Does this work with Arduino Uno/Nano?**
A: No, this requires ESP32-based boards like TRIOE for WiFi capability.

**Q: Can I add more servos?**
A: Yes, you can modify the code to control multiple servos.

**Q: How do I change the servo pin?**
A: Change `SERVO_PIN` value in the code to your desired GPIO pin.

## Next Steps

Once you have this working, try:

1. Adding more servos
2. Creating custom movement patterns
3. Adding sensors to trigger movements
4. Designing a custom web interface
5. Creating a robot arm or pan-tilt camera mount

## Credits

This project uses:
- ESP32 Arduino Core by Espressif
- ESP32Servo Library by Kevin Harrington
- Inter Font by Google Fonts

---

**Happy Building!**

For questions or issues, check the troubleshooting section or consult TRIOE board documentation.

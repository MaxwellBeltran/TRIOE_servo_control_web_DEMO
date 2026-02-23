// TRIOE Board Servo Web Control - Access Point Mode (No Router Needed)
// Compatible with TRIOE ESP32-based boards
// Upload using: Tools > Board > ESP32 Dev Module (or TRIOE Board if available)

#include <ESP32Servo.h>
#include <WiFi.h>
#include <WebServer.h>

// Access Point Configuration - NO ROUTER NEEDED
const char* ap_ssid = "TRIOE-Servo";      // WiFi network name
const char* ap_password = "12345678";      // WiFi password (min 8 characters)

// Servo Setup - TRIOE Board Pin Configuration
#define SERVO_PIN 4  // Default TRIOE servo pin - verify with your TRIOE board specs
Servo myServo;

// Web Server
WebServer server(80);

// Current servo position
int currentPosition = 90;
int targetPosition = 90;

// Mode control
int mode = 0; // 0=Manual, 1=Sweep, 2=Step, 3=Jitter, 4=FastSnap, 5=SlowCreep
bool autoMode = false;
unsigned long lastModeSwitch = 0;
const unsigned long modeDuration = 10000;

void setup() {
    Serial.begin(115200);
    
    // Initialize Servo
    myServo.setPeriodHertz(50);
    myServo.attach(SERVO_PIN, 500, 2400);
    myServo.write(currentPosition);
    
    // Create WiFi Access Point (No Router Needed)
    Serial.println("\n--- TRIOE Board Servo Web Control Starting ---");
    Serial.println("Starting Access Point Mode...");
    Serial.print("Network Name (SSID): ");
    Serial.println(ap_ssid);
    Serial.print("Password: ");
    Serial.println(ap_password);
    
    WiFi.softAP(ap_ssid, ap_password);
    IPAddress IP = WiFi.softAPIP();
    
    Serial.println("\n==================================");
    Serial.println("Access Point Started Successfully!");
    Serial.println("==================================");
    Serial.print("AP IP Address: ");
    Serial.println(IP);
    Serial.println("\nInstructions:");
    Serial.println("1. Connect your phone/computer to WiFi:");
    Serial.print("   Network: ");
    Serial.println(ap_ssid);
    Serial.print("   Password: ");
    Serial.println(ap_password);
    Serial.println("2. Open browser and go to:");
    Serial.println("   http://192.168.4.1");
    Serial.println("==================================\n");
    
    // Setup web server routes
    server.on("/", handleRoot);
    server.on("/move", handleMove);
    server.on("/mode", handleMode);
    server.on("/status", handleStatus);
    
    server.begin();
    Serial.println("Web server started and ready!");
}

void loop() {
    server.handleClient();
    
    // Execute automatic modes
    if (autoMode) {
        if (millis() - lastModeSwitch > modeDuration) {
            mode = (mode % 5) + 1; // Cycle through modes 1-5
            lastModeSwitch = millis();
            Serial.print("Auto-switching to mode: ");
            Serial.println(mode);
        }
        
        switch (mode) {
            case 1: sweepMode(); break;
            case 2: stepMode(); break;
            case 3: jitterMode(); break;
            case 4: fastSnapMode(); break;
            case 5: slowCreepMode(); break;
        }
    } else {
        // Manual mode - smooth movement to target
        if (currentPosition != targetPosition) {
            if (currentPosition < targetPosition) {
                currentPosition++;
            } else {
                currentPosition--;
            }
            myServo.write(currentPosition);
            delay(15);
        }
    }
}

// Web page HTML
void handleRoot() {
    String html = R"rawliteral(
<!DOCTYPE html>
<html>
<head>
    <meta name="viewport" content="width=device-width, initial-scale=1">
    <title>TRIOE Servo Control</title>
    <style>
        @import url('https://fonts.googleapis.com/css2?family=Inter:wght@300;400;500;600;700&display=swap');
        
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
        }
        
        body {
            font-family: 'Inter', -apple-system, BlinkMacSystemFont, sans-serif;
            background: #0a0e27;
            min-height: 100vh;
            padding: 20px;
            display: flex;
            justify-content: center;
            align-items: center;
            position: relative;
            overflow: hidden;
        }
        
        body::before {
            content: '';
            position: absolute;
            width: 200%;
            height: 200%;
            background: radial-gradient(circle at 30% 50%, rgba(99, 102, 241, 0.15), transparent 50%),
                        radial-gradient(circle at 70% 50%, rgba(168, 85, 247, 0.15), transparent 50%);
            animation: gradient 15s ease infinite;
        }
        
        @keyframes gradient {
            0%, 100% { transform: rotate(0deg); }
            50% { transform: rotate(180deg); }
        }
        
        .container {
            background: rgba(255, 255, 255, 0.03);
            backdrop-filter: blur(20px);
            border: 1px solid rgba(255, 255, 255, 0.1);
            border-radius: 24px;
            padding: 40px;
            max-width: 480px;
            width: 100%;
            box-shadow: 0 8px 32px rgba(0, 0, 0, 0.3),
                        inset 0 1px 0 rgba(255, 255, 255, 0.1);
            position: relative;
            z-index: 1;
        }
        
        .header {
            text-align: center;
            margin-bottom: 40px;
        }
        
        h1 {
            color: #ffffff;
            font-size: 32px;
            font-weight: 700;
            letter-spacing: -0.5px;
            margin-bottom: 8px;
            background: linear-gradient(135deg, #ffffff 0%, #a8a8a8 100%);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            background-clip: text;
        }
        
        .subtitle {
            color: rgba(255, 255, 255, 0.5);
            font-size: 13px;
            font-weight: 400;
            letter-spacing: 0.5px;
            text-transform: uppercase;
        }
        
        .status-card {
            background: linear-gradient(135deg, rgba(99, 102, 241, 0.1) 0%, rgba(168, 85, 247, 0.1) 100%);
            border: 1px solid rgba(99, 102, 241, 0.2);
            border-radius: 16px;
            padding: 24px;
            margin-bottom: 32px;
            text-align: center;
            position: relative;
            overflow: hidden;
        }
        
        .status-card::before {
            content: '';
            position: absolute;
            top: 0;
            left: 0;
            right: 0;
            height: 1px;
            background: linear-gradient(90deg, transparent, rgba(255, 255, 255, 0.2), transparent);
        }
        
        .status-label {
            font-size: 11px;
            font-weight: 500;
            color: rgba(255, 255, 255, 0.4);
            text-transform: uppercase;
            letter-spacing: 1px;
            margin-bottom: 8px;
        }
        
        .position-display {
            font-size: 64px;
            font-weight: 700;
            background: linear-gradient(135deg, #6366f1 0%, #a855f7 100%);
            -webkit-background-clip: text;
            -webkit-text-fill-color: transparent;
            background-clip: text;
            line-height: 1;
            margin: 12px 0;
            letter-spacing: -2px;
        }
        
        .mode-badge {
            display: inline-block;
            padding: 6px 16px;
            background: rgba(99, 102, 241, 0.2);
            border: 1px solid rgba(99, 102, 241, 0.3);
            border-radius: 20px;
            font-size: 11px;
            font-weight: 500;
            color: #a5b4fc;
            text-transform: uppercase;
            letter-spacing: 0.5px;
        }
        
        .control-section {
            margin-bottom: 32px;
        }
        
        .section-label {
            font-size: 12px;
            font-weight: 600;
            color: rgba(255, 255, 255, 0.6);
            text-transform: uppercase;
            letter-spacing: 1px;
            margin-bottom: 16px;
        }
        
        .slider-container {
            padding: 24px;
            background: rgba(255, 255, 255, 0.02);
            border: 1px solid rgba(255, 255, 255, 0.05);
            border-radius: 16px;
            margin-bottom: 16px;
        }
        
        .slider {
            -webkit-appearance: none;
            width: 100%;
            height: 6px;
            border-radius: 3px;
            background: rgba(255, 255, 255, 0.1);
            outline: none;
            position: relative;
        }
        
        .slider::-webkit-slider-thumb {
            -webkit-appearance: none;
            width: 20px;
            height: 20px;
            border-radius: 50%;
            background: linear-gradient(135deg, #6366f1 0%, #a855f7 100%);
            cursor: pointer;
            box-shadow: 0 2px 8px rgba(99, 102, 241, 0.5),
                        0 0 0 4px rgba(99, 102, 241, 0.2);
            transition: all 0.2s ease;
        }
        
        .slider::-webkit-slider-thumb:hover {
            transform: scale(1.1);
            box-shadow: 0 4px 12px rgba(99, 102, 241, 0.6),
                        0 0 0 6px rgba(99, 102, 241, 0.25);
        }
        
        .slider::-moz-range-thumb {
            width: 20px;
            height: 20px;
            border: none;
            border-radius: 50%;
            background: linear-gradient(135deg, #6366f1 0%, #a855f7 100%);
            cursor: pointer;
            box-shadow: 0 2px 8px rgba(99, 102, 241, 0.5);
            transition: all 0.2s ease;
        }
        
        .angle-labels {
            display: flex;
            justify-content: space-between;
            margin-top: 12px;
            padding: 0 4px;
        }
        
        .angle-labels span {
            font-size: 11px;
            font-weight: 500;
            color: rgba(255, 255, 255, 0.3);
        }
        
        .preset-grid {
            display: grid;
            grid-template-columns: repeat(5, 1fr);
            gap: 8px;
            margin-top: 16px;
        }
        
        .btn {
            border: none;
            border-radius: 10px;
            font-family: 'Inter', sans-serif;
            font-weight: 500;
            cursor: pointer;
            transition: all 0.2s cubic-bezier(0.4, 0, 0.2, 1);
            outline: none;
        }
        
        .btn-preset {
            padding: 12px 8px;
            background: rgba(255, 255, 255, 0.05);
            border: 1px solid rgba(255, 255, 255, 0.1);
            color: rgba(255, 255, 255, 0.7);
            font-size: 13px;
        }
        
        .btn-preset:hover {
            background: linear-gradient(135deg, rgba(99, 102, 241, 0.3), rgba(168, 85, 247, 0.3));
            border-color: rgba(99, 102, 241, 0.5);
            color: #ffffff;
            transform: translateY(-2px);
            box-shadow: 0 4px 12px rgba(99, 102, 241, 0.3);
        }
        
        .btn-preset:active {
            transform: translateY(0);
        }
        
        .divider {
            height: 1px;
            background: linear-gradient(90deg, transparent, rgba(255, 255, 255, 0.1), transparent);
            margin: 32px 0;
        }
        
        .mode-grid {
            display: grid;
            grid-template-columns: repeat(2, 1fr);
            gap: 12px;
        }
        
        .btn-mode {
            padding: 16px;
            background: rgba(255, 255, 255, 0.05);
            border: 1px solid rgba(255, 255, 255, 0.1);
            color: rgba(255, 255, 255, 0.7);
            font-size: 13px;
            position: relative;
            overflow: hidden;
        }
        
        .btn-mode::before {
            content: '';
            position: absolute;
            top: 0;
            left: -100%;
            width: 100%;
            height: 100%;
            background: linear-gradient(90deg, transparent, rgba(255, 255, 255, 0.1), transparent);
            transition: left 0.5s;
        }
        
        .btn-mode:hover::before {
            left: 100%;
        }
        
        .btn-mode:hover {
            background: linear-gradient(135deg, rgba(99, 102, 241, 0.2), rgba(168, 85, 247, 0.2));
            border-color: rgba(99, 102, 241, 0.4);
            color: #ffffff;
            transform: translateY(-2px);
            box-shadow: 0 4px 16px rgba(99, 102, 241, 0.25);
        }
        
        .btn-mode.active {
            background: linear-gradient(135deg, #6366f1, #a855f7);
            border-color: transparent;
            color: #ffffff;
            box-shadow: 0 4px 16px rgba(99, 102, 241, 0.4),
                        inset 0 1px 0 rgba(255, 255, 255, 0.2);
        }
        
        .btn-mode.active::after {
            content: '●';
            position: absolute;
            top: 8px;
            right: 8px;
            font-size: 8px;
            color: rgba(255, 255, 255, 0.8);
        }
        
        .info-box {
            margin-top: 24px;
            padding: 16px;
            background: rgba(59, 130, 246, 0.1);
            border: 1px solid rgba(59, 130, 246, 0.2);
            border-radius: 12px;
        }
        
        .info-box p {
            font-size: 11px;
            color: rgba(147, 197, 253, 0.8);
            line-height: 1.6;
            margin: 0;
        }
        
        .info-box strong {
            color: #93c5fd;
            font-weight: 600;
        }
        
        @keyframes pulse {
            0%, 100% { opacity: 1; }
            50% { opacity: 0.5; }
        }
        
        .loading {
            animation: pulse 2s ease-in-out infinite;
        }
    </style>
</head>
<body>
    <div class="container">
        <div class="header">
            <h1>TRIOE CONTROL</h1>
            <div class="subtitle">Servo Control System</div>
        </div>
        
        <div class="status-card">
            <div class="status-label">Current Position</div>
            <div class="position-display" id="position">90&deg;</div>
            <div class="mode-badge">
                <span id="mode">Manual Mode</span>
            </div>
        </div>
        
        <div class="control-section">
            <div class="section-label">Manual Control</div>
            <div class="slider-container">
                <input type="range" min="0" max="180" value="90" class="slider" id="angleSlider">
                <div class="angle-labels">
                    <span>0&deg;</span>
                    <span>45&deg;</span>
                    <span>90&deg;</span>
                    <span>135&deg;</span>
                    <span>180&deg;</span>
                </div>
            </div>
            
            <div class="preset-grid">
                <button class="btn btn-preset" onclick="moveServo(0)">0&deg;</button>
                <button class="btn btn-preset" onclick="moveServo(45)">45&deg;</button>
                <button class="btn btn-preset" onclick="moveServo(90)">90&deg;</button>
                <button class="btn btn-preset" onclick="moveServo(135)">135&deg;</button>
                <button class="btn btn-preset" onclick="moveServo(180)">180&deg;</button>
            </div>
        </div>
        
        <div class="divider"></div>
        
        <div class="control-section">
            <div class="section-label">Automatic Modes</div>
            <div class="mode-grid">
                <button class="btn btn-mode" onclick="setMode(0)">Manual</button>
                <button class="btn btn-mode" onclick="setMode(1)">Sweep</button>
                <button class="btn btn-mode" onclick="setMode(2)">Step</button>
                <button class="btn btn-mode" onclick="setMode(3)">Jitter</button>
                <button class="btn btn-mode" onclick="setMode(4)">Fast Snap</button>
                <button class="btn btn-mode" onclick="setMode(5)">Slow Creep</button>
            </div>
        </div>
        
        <div class="info-box">
            <p>
                Network: <strong>TRIOE-Servo</strong><br>
                Address: <strong>http://192.168.4.1</strong>
            </p>
        </div>
    </div>
    
    <script>
        const slider = document.getElementById('angleSlider');
        const posDisplay = document.getElementById('position');
        const modeDisplay = document.getElementById('mode');
        
        slider.addEventListener('input', function() {
            posDisplay.innerHTML = this.value + '&deg;';
            moveServo(this.value);
        });
        
        function moveServo(angle) {
            fetch('/move?angle=' + angle)
                .then(response => response.text())
                .then(data => {
                    slider.value = angle;
                    posDisplay.innerHTML = angle + '&deg;';
                })
                .catch(error => console.error('Error:', error));
        }
        
        function setMode(m) {
            fetch('/mode?mode=' + m)
                .then(response => response.text())
                .then(data => {
                    updateModeDisplay(m);
                })
                .catch(error => console.error('Error:', error));
        }
        
        function updateModeDisplay(m) {
            const modes = ['Manual Mode', 'Sweep Mode', 'Step Mode', 'Jitter Mode', 'Fast Snap Mode', 'Slow Creep Mode'];
            modeDisplay.textContent = modes[m];
            
            document.querySelectorAll('.btn-mode').forEach((btn, index) => {
                btn.classList.toggle('active', index === m);
            });
        }
        
        setInterval(function() {
            fetch('/status')
                .then(response => response.json())
                .then(data => {
                    posDisplay.innerHTML = data.position + '&deg;';
                    slider.value = data.position;
                })
                .catch(error => console.error('Error:', error));
        }, 1000);
        
        updateModeDisplay(0);
    </script>
</body>
</html>
)rawliteral";
    
    server.send(200, "text/html", html);
}

// Handle servo movement
void handleMove() {
    if (server.hasArg("angle")) {
        int angle = server.arg("angle").toInt();
        angle = constrain(angle, 0, 180);
        targetPosition = angle;
        autoMode = false;
        mode = 0;
        
        Serial.print("Manual move to: ");
        Serial.println(angle);
        
        server.send(200, "text/plain", "OK");
    } else {
        server.send(400, "text/plain", "Missing angle parameter");
    }
}

// Handle mode change
void handleMode() {
    if (server.hasArg("mode")) {
        mode = server.arg("mode").toInt();
        mode = constrain(mode, 0, 5);
        
        if (mode == 0) {
            autoMode = false;
            Serial.println("Mode: Manual");
        } else {
            autoMode = true;
            lastModeSwitch = millis();
            Serial.print("Mode: Auto - ");
            Serial.println(mode);
        }
        
        server.send(200, "text/plain", "OK");
    } else {
        server.send(400, "text/plain", "Missing mode parameter");
    }
}

// Handle status request
void handleStatus() {
    String json = "{\"position\":" + String(currentPosition) + 
                  ",\"mode\":" + String(mode) + 
                  ",\"auto\":" + String(autoMode) + "}";
    server.send(200, "application/json", json);
}

// MODE 1: Smooth Sweep
void sweepMode() {
    for (int pos = 0; pos <= 180; pos += 2) {
        myServo.write(pos);
        currentPosition = pos;
        delay(15);
        server.handleClient();
    }
    for (int pos = 180; pos >= 0; pos -= 2) {
        myServo.write(pos);
        currentPosition = pos;
        delay(15);
        server.handleClient();
    }
}

// MODE 2: Precision Steps
void stepMode() {
    int positions[] = {0, 45, 90, 135, 180};
    for(int i = 0; i < 5; i++) {
        myServo.write(positions[i]);
        currentPosition = positions[i];
        delay(1000);
        server.handleClient();
    }
}

// MODE 3: Random Jitter
void jitterMode() {
    int randomPos = random(0, 181);
    myServo.write(randomPos);
    currentPosition = randomPos;
    delay(random(200, 600));
    server.handleClient();
}

// MODE 4: Fast Snap
void fastSnapMode() {
    myServo.write(0);
    currentPosition = 0;
    delay(300);
    server.handleClient();
    myServo.write(180);
    currentPosition = 180;
    delay(300);
    server.handleClient();
}

// MODE 5: Slow Creep
void slowCreepMode() {
    for (int pos = 0; pos <= 180; pos++) {
        myServo.write(pos);
        currentPosition = pos;
        delay(50);
        server.handleClient();
    }
}

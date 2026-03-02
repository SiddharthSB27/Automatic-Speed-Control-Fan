#include <DHT.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// Define pins and constants
#define DHT_PIN 2
#define DHT_TYPE DHT11
#define FAN_PIN 9
#define TEMP_THRESHOLD 20.0  // Fan turns on at 10°C
#define MAX_TEMP 29.0        // Temperature for maximum fan speed
#define MIN_FAN_PERCENT 30   // Minimum fan speed percentage when ON

// Initialize components
DHT dht(DHT_PIN, DHT_TYPE);
LiquidCrystal_I2C lcd(0x27, 16, 2);  // Use 0x3F if 0x27 doesn't work

// Variables
float temperature;
float humidity;
bool fanState = false;
int fanSpeed = 0;  // 0-255 PWM value
int fanPercent = 0; // 0-100% speed

// Store last valid readings
float lastValidTemperature = 0;
float lastValidHumidity = 0;

void setup() {
  Serial.begin(115200);
  
  // Initialize components
  dht.begin();
  lcd.init();
  lcd.backlight();
  lcd.clear();
  
  // Show welcome message
  lcd.setCursor(0, 0);
  lcd.print("  Auto Fan");
  lcd.setCursor(0, 1);
  lcd.print("  Controller");
  delay(2000);
  lcd.clear();
  
  // Set fan pin as output
  pinMode(FAN_PIN, OUTPUT);
  analogWrite(FAN_PIN, 0);  // Ensure fan is off at start
  
  Serial.println("Arduino Uno Smart Fan Started");
  Serial.println("Fan will start at 50% and increase with temperature");
}

void loop() {
  // Read sensor data
  temperature = dht.readTemperature();
  humidity = dht.readHumidity();
  
  // Check if sensor read failed
  if (isnan(temperature) || isnan(humidity)) {
    Serial.println("ERROR: Failed to read from DHT sensor! Using last valid values.");
    
    // Use last valid values and update display
    controlFanSpeedWithLastValues();
    updateDisplayWithLastValues();
    
    delay(2000);
    return;
  }
  
  // Store valid readings
  lastValidTemperature = temperature;
  lastValidHumidity = humidity;
  
  // Automatic temperature control with speed adjustment
  controlFanSpeed();
  
  // Update LCD display
  updateDisplay();
  
  // Print to Serial Monitor
  Serial.print("Temperature: ");
  Serial.print(temperature);
  Serial.print("°C | Humidity: ");
  Serial.print(humidity);
  Serial.print("% | Fan Speed: ");
  Serial.print(fanSpeed);
  Serial.print("/255 (");
  Serial.print(fanPercent);
  Serial.println("%)");
  
  delay(2000);
}

void controlFanSpeed() {
  if (temperature <= TEMP_THRESHOLD) {
    // Temperature below threshold - turn fan off
    fanSpeed = 0;
    fanPercent = 0;
    fanState = false;
  } else {
    // Temperature above threshold - fan ON with speed based on temperature
    fanState = true;
    
    if (temperature >= MAX_TEMP) {
      // Temperature at or above maximum - full speed
      fanPercent = 100;
    } else {
      // Temperature between threshold and max - linear speed control from 50% to 100%
      fanPercent = map(temperature * 10, TEMP_THRESHOLD * 10, MAX_TEMP * 10, MIN_FAN_PERCENT, 100);
      fanPercent = constrain(fanPercent, MIN_FAN_PERCENT, 100);
    }
    
    // Convert percentage to PWM value
    fanSpeed = map(fanPercent, 0, 100, 0, 255);
  }
  
  // Apply fan speed using PWM
  analogWrite(FAN_PIN, fanSpeed);
}

void controlFanSpeedWithLastValues() {
  // Use last valid temperature for fan control
  if (lastValidTemperature <= TEMP_THRESHOLD) {
    // Temperature below threshold - turn fan off
    fanSpeed = 0;
    fanPercent = 0;
    fanState = false;
  } else {
    // Temperature above threshold - fan ON with speed based on temperature
    fanState = true;
    
    if (lastValidTemperature >= MAX_TEMP) {
      // Temperature at or above maximum - full speed
      fanPercent = 100;
    } else {
      // Temperature between threshold and max - linear speed control from 50% to 100%
      fanPercent = map(lastValidTemperature * 10, TEMP_THRESHOLD * 10, MAX_TEMP * 10, MIN_FAN_PERCENT, 100);
      fanPercent = constrain(fanPercent, MIN_FAN_PERCENT, 100);
    }
    
    // Convert percentage to PWM value
    fanSpeed = map(fanPercent, 0, 100, 0, 255);
  }
  
  // Apply fan speed using PWM
  analogWrite(FAN_PIN, fanSpeed);
}

void updateDisplay() {
  // Clear display and show temperature, humidity, fan status and speed
  lcd.clear();
  
  // Line 1: Temperature
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(temperature, 1);
  lcd.write(223);  // Degree symbol
  lcd.print("C");
  
  // Line 1: Fan Status
  lcd.setCursor(9, 0);
  lcd.print("F:");
  if (fanState) {
    lcd.print("ON ");
  } else {
    lcd.print("OFF");
  }
  
  // Line 2: Humidity and Fan Speed
  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(humidity, 0);
  lcd.print("%");
  
  lcd.setCursor(8, 1);
  lcd.print("S:");
  if (fanState) {
    lcd.print(fanPercent);
    lcd.print("%");
  } else {
    lcd.print("OFF");
  }
}

void updateDisplayWithLastValues() {
  // Display last valid temperature, humidity, and fan speed when sensor has error
  lcd.clear();
  
  // Line 1: Temperature (show last valid value)
  lcd.setCursor(0, 0);
  lcd.print("T:");
  lcd.print(lastValidTemperature, 1);
  lcd.write(223);  // Degree symbol
  lcd.print("C");
  
  // Line 1: Fan Status
  lcd.setCursor(9, 0);
  lcd.print("F:");
  if (fanState) {
    lcd.print("ON ");
  } else {
    lcd.print("OFF");
  }
  
  // Line 2: Humidity (show last valid value)
  lcd.setCursor(0, 1);
  lcd.print("H:");
  lcd.print(lastValidHumidity, 0);
  lcd.print("%");
  
  // Line 2: Fan Speed
  lcd.setCursor(8, 1);
  lcd.print("S:");
  if (fanState) {
    lcd.print(fanPercent);
    lcd.print("%");
  } else {
    lcd.print("OFF");
  }
} 
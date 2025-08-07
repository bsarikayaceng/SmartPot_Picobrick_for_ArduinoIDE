#include <Wire.h>
#include "picobricks.h"

// === Object Declaration ===
SSD1306 oled(0x3C, 128, 64);
SHTC3 shtc3;
IRPico ir(17);
#define SOIL_PIN 39  // Soil sensor analog pin
// === Pins ===
#define LDR_PIN 35
#define BUZZER_PIN 16
#define LIGHT_THRESHOLD 1000

// === Values ===
volatile bool irReceived = false;
int irCode = 0;
bool humiditySettingMode = false;
int humidityThreshold = -1;
int targetSoilPercent = -1; // Kullanıcının belirlediği eşik * 10

motorDriver motor;

// === IR Interrupt Handler ===
void irInterruptHandler() {
  if (ir.decode()) {
    irCode = ir.getCode();
    irReceived = true;
  }
}

void setup() {
  Wire.begin();
  Serial.begin(115200);
  oled.init();
  oled.clear();
  shtc3.begin();
  delay(500);

  pinMode(BUZZER_PIN, OUTPUT);

  // Start interrupt for IR
  attachInterrupt(digitalPinToInterrupt(17), irInterruptHandler, FALLING);
}

void loop() {
  // Read soil moisture
  int soilValue = analogRead(SOIL_PIN);  // 0–4095 
  float soilPercent = map(soilValue, 3200, 2700, 0, 100); // You should adjust these values according to your sensor by observing analogRead() values via Serial Monitor
  
  soilPercent = constrain(soilPercent, 0, 100); // Ensure the soil percentage stays between 0% and 100%.

  // === If IR Signal is received ===
  if (irReceived) {
    irReceived = false;

    //Manuel Watering
    if(irCode==button_ok && !humiditySettingMode){
      motor.dc(1,100,0);
      oled.clear();
      oled.setCursor(0, 20);
      oled.print("Manuel Watering...");
      oled.show();
      delay(2000);
      motor.dc(1, 0, 0);
    }

    if (irCode == button_star) {
      humiditySettingMode = true;
      oled.clear();
      oled.setCursor(0, 20);
      oled.print("Humidity Setting");
      oled.setCursor(0, 30);
      oled.print("Select 0-9 keys ");
      
      oled.setCursor(5, 40);
      oled.print("Press'#' for");
      oled.setCursor(5, 50);
      oled.print("Main Menu");
      oled.show();
      return;
    }

    else if (humiditySettingMode) {
        switch (irCode) {
          case number_0: humidityThreshold = 0; break;
          case number_1: humidityThreshold = 1; break;
          case number_2: humidityThreshold = 2; break;
          case number_3: humidityThreshold = 3; break;
          case number_4: humidityThreshold = 4; break;
          case number_5: humidityThreshold = 5; break;
          case number_6: humidityThreshold = 6; break;
          case number_7: humidityThreshold = 7; break;
          case number_8: humidityThreshold = 8; break;
          case number_9: humidityThreshold = 9; break;
          case button_sharp: humiditySettingMode = false; break;
          default: break;
        }

        if (humidityThreshold>=0 && humidityThreshold<=9) {
          targetSoilPercent = humidityThreshold * 10;

          oled.clear();
          oled.setCursor(0, 25);
          oled.print("Humidity Set = ");
          oled.print(String(humidityThreshold).c_str());
          oled.show();

          Serial.print("Selected Threshold: ");
          Serial.println(humidityThreshold);

          delay(2000);
          humiditySettingMode = false;

      }
    }

    irCode = 0; // reset code
  }


  // === Watering Control ===
  if (targetSoilPercent != -1) {
    if (soilPercent < targetSoilPercent) {
      motor.dc(1, 100, 0);
      digitalWrite(BUZZER_PIN, HIGH);
      Serial.println("Watering... DC Motor ON");
    } else {
      motor.dc(1, 0, 0);
      digitalWrite(BUZZER_PIN, LOW);
      Serial.println("Soil is moist. DC Motor OFF");
    }
  }

  // === Home Screen Data ===
  float temperature = shtc3.readTemperature();
  float humidity = shtc3.readHumidity();
  int lightValue = analogRead(LDR_PIN);
  if (!humiditySettingMode||irCode==button_sharp){
    oled.clear();
    oled.setCursor(0, 0);
    if (lightValue < LIGHT_THRESHOLD) {
      oled.print("Good Evening");
    } else {
      oled.print("Have a Nice Day");
    }
    oled.setCursor(0, 10);
    oled.print("Temp: ");
    oled.print(String(temperature, 1).c_str());
    oled.print("C");

    oled.setCursor(0, 20);
    oled.print("Soil: ");
    oled.print(String(soilPercent, 0).c_str());
    oled.print("%");

    oled.setCursor(0, 30);
    oled.print("Humidity Level:");
    if (humidityThreshold != -1) {
      oled.print(String(humidityThreshold).c_str());
    } else {
      oled.print("None");
    }

    oled.setCursor(0, 40);
    oled.print("'OK' Manuel Watering");

    oled.setCursor(0, 50);
    oled.print("Setting with '*'");

    oled.show();
  }
  delay(300);
}

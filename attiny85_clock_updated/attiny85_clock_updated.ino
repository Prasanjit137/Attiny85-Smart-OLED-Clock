#include <TinyWireM.h> // To use Adafruit's TinyWireM library:
#include <Tiny4kOLED.h> // Tiny4kOLED library to draw on the display
#include <TinyRTClib.h> // For the RTC module

#define button1Pin 4
#define button2Pin 3
#define triggerPin 1

unsigned long debounceDelay = 50; // Debounce delay time
unsigned long lastDebounceTime1 = 0;
unsigned long lastDebounceTime2 = 0;
int lastButtonState1;
int lastButtonState2;
int buttonState1;
int buttonState2;

#include "FONTTAMAMINI02.h"
#include "FONT16X32DIGITS.h"



RTC_DS1307 rtc;

char t[32];
byte ss = 0, mi = 0, hh = 0, wd = 6, dd = 1, mo = 1, yy = 0, disp = 0;
byte editField = 0;  // 0: day, 1: month, 2: year, 3: hour, 4: minute, 5: second

unsigned long currentMillis;
unsigned long startMillis = 0;
unsigned long period = 5000;
const unsigned long eventInterval = 500;
unsigned long previousTime = 0;

void setup() {
  pinMode(triggerPin, OUTPUT);
  digitalWrite(triggerPin, HIGH);

  pinMode(button1Pin, INPUT);
  pinMode(button2Pin, INPUT);

  TinyWireM.begin();
  oled.begin(132, 64, sizeof(tiny4koled_init_128x64br), tiny4koled_init_128x64br);
  oled.enableChargePump(); 
  oled.setRotation(1); 
  oled.clear(); 
  oled.on(); 

  rtc.begin();
}

void loop() {
  unsigned long currentMillis = millis();
  
  // Debounce logic for button 1
  int reading1 = digitalRead(button1Pin);
  if (reading1 != lastButtonState1) {
    lastDebounceTime1 = currentMillis;
  }
  if ((currentMillis - lastDebounceTime1) > debounceDelay) {
    if (reading1 != buttonState1) {
      buttonState1 = reading1;
      if (buttonState1 == HIGH) {
        click1();
      }
    }
  }
  lastButtonState1 = reading1;

  // Debounce logic for button 2
  int reading2 = digitalRead(button2Pin);
  if (reading2 != lastButtonState2) {
    lastDebounceTime2 = currentMillis;
  }
  if ((currentMillis - lastDebounceTime2) > debounceDelay) {
    if (reading2 != buttonState2) {
      buttonState2 = reading2;
      if (buttonState2 == HIGH) {
        click2();
      }
    }
  }
  lastButtonState2 = reading2;

  // Display time or edit mode every event interval
  if (currentMillis - previousTime >= eventInterval) {
    if (disp == 0) {
      
      boolean gotTheTime = grabTime();
      if (gotTheTime) {
        printTime(); 
      }

        // Reset the trigger pin after a period of inactivity
      if (currentMillis - startMillis >= period) {
          digitalWrite(triggerPin, LOW);
      }
  
    } else if (disp == 1) {
      displayEditMode(); // Show editable fields
    }
    previousTime = currentMillis;
  }


}

// Fetches time from RTC module
boolean grabTime() {
  TinyWireM.beginTransmission(0x68); 
  TinyWireM.write((byte)0); 
  TinyWireM.endTransmission();
  TinyWireM.requestFrom(0x68, 7);
  
  if (TinyWireM.available() >= 7) {
    ss = bcd2bin(TinyWireM.read());
    mi = bcd2bin(TinyWireM.read());
    hh = bcd2bin(TinyWireM.read());
    wd = bcd2bin(TinyWireM.read());
    dd = bcd2bin(TinyWireM.read());
    mo = bcd2bin(TinyWireM.read());
    yy = bcd2bin(TinyWireM.read());
    return true;
  }
  return false;
}

byte bcd2bin(byte x) {
  return ((((x >> 4) & 0xF) * 10) + (x & 0xF));
}

// Display the time on OLED
void printTime() {
  oled.setFont(FONT8X16P); 
  oled.setCursor(32, 6);
  if (dd < 10) oled.print("0"); oled.print(dd, DEC); oled.print("-");
  if (mo < 10) oled.print("0"); oled.print(mo, DEC); oled.print("-");
  oled.print("20"); 
  if (yy < 10) oled.print("0"); oled.print(yy, DEC);

  oled.setFont(FONT16X32DIGITS); 
  oled.setCursor(4, 1);
  if (hh < 10) oled.print("0"); oled.print(hh, DEC); oled.print(":");
  if (mi < 10) oled.print("0"); oled.print(mi, DEC); oled.print(":");
  if (ss < 10) oled.print("0"); oled.print(ss, DEC);

  getVcc();
}

// Display editable fields for date/time adjustment
void displayEditMode() {
  oled.setFont(FONT8X16P); 
  oled.setCursor(30, 0);
  oled.print("Edit Mode");

  oled.setCursor(30, 3);
  switch (editField) {
    case 0: oled.print("Day: "); oled.print(dd); break;
    case 1: oled.print("Month: "); oled.print(mo); break;
    case 2: oled.print("Year: "); oled.print(yy + 2000); break;
    case 3: oled.print("Hour: "); oled.print(hh); break;
    case 4: oled.print("Minute: "); oled.print(mi); break;
    case 5: oled.print("Second: "); oled.print(ss); break;
  }
}

// Function called when Button 1 is pressed (increment value)
void click1() {
  oled.clear();
  if (disp == 1) { // If in edit mode
    switch (editField) {
      case 0: dd = (dd == 31) ? 1 : dd + 1; break;
      case 1: mo = (mo == 12) ? 1 : mo + 1; break;
      case 2: yy = (yy == 99) ? 0 : yy + 1; break;
      case 3: hh = (hh == 23) ? 0 : hh + 1; break;
      case 4: mi = (mi == 59) ? 0 : mi + 1; break;
      case 5: ss = (ss == 59) ? 0 : ss + 1; break;
    }
  } else {
    disp = 1; // Enter edit mode
  }
}

// Function called when Button 2 is pressed (decrement value or navigate fields)
void click2() {
  oled.clear();
  if (disp == 1) { // If in edit mode
    if (editField == 5) { 
      // If we are at the second field, save the changes
      rtc.adjust(DateTime(yy + 2000, mo, dd, hh, mi, ss));
      disp = 0; // Exit edit mode after setting the time
      editField = 0;
    } else {
      editField++; // Move to the next field
    }
  }
}

// Convert voltage measurement
float getVcc() {
  ADMUX = _BV(MUX3) | _BV(MUX2); // Select internal 1.1V reference for measurement.
  ADCSRA |= _BV(ADSC);           // Start measuring.
  while (ADCSRA & _BV(ADSC));    // Wait for measurement to complete.
  uint16_t adcReading = ADC;
  float vcc = 1.1 * 1024.0 / adcReading; // Calibration might be needed
  oled.setFont(FONTTAMAMINI02); 
  oled.setCursor(100, 0);
  //oled.print(vcc);
  //oled.print("V");
    // Map the voltage to a percentage (3.1V = 0%, 4.5V = 100%)
  float percentage = ((vcc - 3.1) / (4.1 - 3.1)) * 100;

  // Ensure percentage stays within bounds
  if (percentage < 0) percentage = 0;
  if (percentage > 100) percentage = 100;

  // Display the percentage on OLED
  //oled.setFont(FONT8X16P); 
  oled.print(percentage, 1); // Print percentage with 1 decimal place
  oled.print("%");

}

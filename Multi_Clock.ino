/* 
 *  Multi_Clock.ino
 *  
 *  Nhi Dinh
 *  EEL3003 Spring 2020
 *  April 22, 2020
 *  
 *  DS3231 library by Henning Karlsen 
 *  Downloaded from website: http://www.rinkydinkelectronics.com/
 *  
 *  REFERENCES
 * - Arduino Starter Kit Example Builds 3,11
 *     created 13 Sep 2012 by Scott Fitzgerald
 *     http://www.arduino.cc/starterKit
 * - EEL3003 Modified Arduino Example Build 15
 *     created 18 Sep 2012 by Scott Fitzgerald
 *     Modified 1 Nov 2019 by David Cheney
 * - Tones for piezo https://www.arduino.cc/en/Tutorial/toneMelody
 * - RTC with LCD: https://howtomechatronics.com/tutorials/arduino/arduino-ds3231-real-time-clock-tutorial/
 * - Instantaneous State Change Detection: https://www.arduino.cc/en/Tutorial/StateChangeDetection
 * - Control LED brightness https://www.engineersgarage.com/arduino/fading-led-with-potentiometer-using-arduino/
*/
#include <Wire.h>
#include <LiquidCrystal.h>
#include <DS3231.h>
#include "Multi_Clock.h"

//initialize objects
LiquidCrystal lcd(12, 11, 5, 4, 3, 2);
DS3231 rtc(SDA, SCL);
Multi_Clock * mc = new Multi_Clock(A0,A1,10,9,8,7,6);
int clkCnt = 0;

void displayTimeAndTemp() {
  //time
  lcd.setCursor(0, 0);
  lcd.print(rtc.getTimeStr());
  lcd.print("    ");
  
  //temperature
  clkCnt++;
  if(clkCnt > 100) {
    int sensorVal = analogRead(mc->getTemperaturePin()); 
    mc->temperature->setTemperature(sensorVal);
    clkCnt = 0;
  }
  lcd.print((int)mc->temperature->temperature);
  lcd.print((char)223);
  lcd.print("C");  

  //set alarm time
  lcd.setCursor(0, 1);
  lcd.print(mc->timeStr(mc->alarm->alarmHour, mc->alarm->alarmMin));
  lcd.print("  ");

  //show if alarm is on or off
  if (mc->alarm->alarmOn) lcd.print(" ALARM ON");
  else lcd.print("ALARM OFF");
  
  delay(50);
}

void setup() {
  Serial.begin(9600);
  lcd.begin(16,2);
  rtc.begin();
  //set time in hh:mm:ss form by uncommenting below, then upload, then comment it out again
  //rtc.setTime(0,0,0); 
  mc->begin();
}

void loop() {
  mc->ledNetwork();
  displayTimeAndTemp();
  mc->checkAlarmOn(); 
  mc->incrementHour(); 
  mc->incrementMinute();
  mc->playAlarm(rtc.getTimeStr());
}

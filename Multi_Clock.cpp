/* 
 *  Multi_Clock.cpp
 *  
 *  Nhi Dinh
 *  EEL3003 Spring 2020
 *  April 22, 2020
 *  
 *  DS3231 library by Henning Karlsen 
 *  Downloaded from website: http://www.rinkydinkelectronics.com/
 *  
 *  REFERENCES
 * - Arduino Starter Kit Example Builds 3 and 11
 *     created 13 Sep 2012 by Scott Fitzgerald
 * - EEL3003 Modified Arduino Example Build 15
 *     created 18 Sep 2012 by Scott Fitzgerald, Modified 1 Nov 2019 by David Cheney
 *     modified by Nhi Dinh 22 Apr 2020 for lines 69-73 to add potentiometer
 * - Tones for piezo https://www.arduino.cc/en/Tutorial/toneMelody
 *     modified by Nhi Dinh 22 Apr 2020 for lines 88-102 with a nested for-loop playing an array of melodies
 * - RTC with LCD: https://howtomechatronics.com/tutorials/arduino/arduino-ds3231-real-time-clock-tutorial/
 * - Instantaneous State Change Detection: https://www.arduino.cc/en/Tutorial/StateChangeDetection
 *     created  27 Sep 2005, modified 30 Aug 2011 by Tom Igoe
 *     modified by Nhi Dinh 22 Apr 2020 for lines 79-87 and 111-133 to suit desired functions
 * - Control LED brightness https://www.engineersgarage.com/arduino/fading-led-with-potentiometer-using-arduino/
 *     modified by Nhi Dinh 22 Apr 2020 for lines 69-73 to add optocoupler
*/
#include "Multi_Clock.h"

Multi_Clock::Multi_Clock(int _temp, int _led, int _opto, int _alarm, int _min, int _hr, int _piezo) {
  //set pins
  this->temperaturePin = _temp;
  this->ledCtrlPin = _led;
  this->optoPin = _opto;
  this->alarmOnPin = _alarm;
  this->incMinutePin = _min;
  this->incHourPin = _hr;
  this->piezoPin = _piezo;
  
  this->alarm = new Alarm();
  this->temperature = new Temperature();
}

void Multi_Clock::begin() {
  //set initial states
  this->incHourState = 0;
  this->prevIncHourState = 0;
  this->incMinState = 0;
  this->prevIncMinState = 0;
  this->alarmState = 0;
  this->prevAlarmState = 0;

  //get initial temperature
  int initTemp = analogRead(getTemperaturePin());
  temperature->setTemperature(initTemp); 

  //set pin modes
  pinMode(getAlarmOnPin(),INPUT);
  pinMode(getIncHourPin(),INPUT);
  pinMode(getIncMinutePin(),INPUT);
  pinMode(getOptoPin(),OUTPUT);
}

String Multi_Clock::timeStr(int _hour, int _min) {
  //for converting int values of hour and minutes into a single string
  String hrStr, minStr;
  if(_hour < 10) hrStr = "0" + String(_hour);
  else hrStr = String(_hour); 
  if(_min < 10) minStr = "0" + String(_min);
  else minStr = String(_min); 
  return hrStr + ":" + minStr;
}

void Multi_Clock::ledNetwork() {
  //control LED brightness
  int senseLight = analogRead(getLedCtrlPin());
  int brightness = senseLight / 4;
  analogWrite(getOptoPin(), brightness);
}

void Multi_Clock::checkAlarmOn() {
  this->alarmState = digitalRead(getAlarmOnPin());
  if (this->alarmState != this->prevAlarmState) {
    if (this->alarmState == LOW)
      alarm->alarmOn = !alarm->alarmOn;
    delay(50);
  }
  this->prevAlarmState = this->alarmState;
}

void Multi_Clock::piezoSound() {
    //just a simple do-re-mi-fa-sol sequence
  int alarmMelody[] = {262, 294, 330, 349, 392, 349, 330, 294};
  int noteDur = 1000 / 4; //fourth note
  for (int i = 0; i < 5; i++) {
    for (int j = 0; j < 8; j++) {
      tone(getPiezoPin(), alarmMelody[j], noteDur);
      delay(noteDur * 1.3);
      noTone(getPiezoPin());
    }
  }
  tone(getPiezoPin(), 262, noteDur); //play final C note
  delay(50);
  noTone(getPiezoPin());
}

void Multi_Clock::playAlarm(String _time) {
  String setAlarm = timeStr(alarm->alarmHour,alarm->alarmMin) + ":00";
  String currTime = _time;
  if((setAlarm==currTime) && alarm->alarmOn) {
    piezoSound();
  }
}

void Multi_Clock::incrementHour() {
  this->incHourState = digitalRead(getIncHourPin());
  if(this->incHourState != this->prevIncHourState) {
    if(this->incHourState == LOW) {
      alarm->alarmHour++;
      if(alarm->alarmHour > 23) alarm->alarmHour = 0;
    }
    delay(50);
  }
  this->prevIncHourState = this->incHourState;
}

void Multi_Clock::incrementMinute() {
  this->incMinState = digitalRead(getIncMinutePin());
  if(this->incMinState != this->prevIncMinState) {
    if(this->incMinState == LOW) {
      alarm->alarmMin++;
      if(alarm->alarmMin > 59) alarm->alarmMin = 0;
    }
    delay(50);
  }
  this->prevIncMinState = this->incMinState;
}

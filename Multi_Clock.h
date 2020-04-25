/* 
 *  Multi_Clock.h
 *  
 *  Nhi Dinh
 *  EEL3003 Spring 2020
 *  April 22, 2020
 *    
*/
#include "Arduino.h"

struct Alarm {
  bool alarmOn;
  int alarmHour, alarmMin;
  Alarm() { //initial constructor    
    this->alarmOn = false; 
    this->alarmHour = 0;
    this->alarmMin = 0;
  }
};
struct Temperature {
  float temperature;
  void setTemperature(int sensorVal){
     this->temperature = (((sensorVal / 1024.0) * 5.0) - .5) * 100;
  }
};

class Multi_Clock {
  private:
    //private variables, pins and states
    int temperaturePin, ledCtrlPin, optoPin, alarmOnPin, incMinutePin, incHourPin, piezoPin;
    int alarmState, prevAlarmState;
    int alarmHour, incHourState, prevIncHourState;
    int alarmMin, incMinState, prevIncMinState;

	public: 
    //make pointers for Alarm and Temperature objects   
    Alarm * alarm;
    Temperature * temperature;
    
    //constructor
		Multi_Clock(int _temp, int _led, int _opto, int _alarm, int _min, int _hr, int _piezo);
   
		//getters for pins
		const int getTemperaturePin(){ return this->temperaturePin; }
		const int getLedCtrlPin() { return this->ledCtrlPin; }
		const int getOptoPin() { return this->optoPin; }
		const int getAlarmOnPin() { return this->alarmOnPin; }
		const int getIncMinutePin() { return this->incMinutePin; }
		const int getIncHourPin() { return this->incHourPin; }
		const int getPiezoPin() { return this->piezoPin; }
    
    //methods
    void begin();
    String timeStr(int _hour, int _min);
		void ledNetwork();
		void incrementHour();
		void incrementMinute();
		void checkAlarmOn();
		void playAlarm(String _time);
    void piezoSound();
};

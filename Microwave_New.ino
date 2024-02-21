#define COOKER_OFF  0
#define LAMP_ON     HIGH
#define LAMP_OFF    LOW
#define READ        HIGH
#define START_TIMER HIGH
#define RESET_TIMER HIGH

//------ Prototype functions---------
bool doorSwOpen(bool state);
bool millisTimer(bool startTimer, bool reset,unsigned long countTime);
void timerCountdown(bool startCountdown);
  
  //getter functions
int   getState();
int   getCookTime();
bool  getMillisTimer();

  //setter functions
void  setReadKeys(bool _state);
int   setCookTime(int _setTime);
void  setMillisTimer(bool _startTimer, bool _resetTimer, int _timePeriod);
int   setState(int _state);
void  setTypedNum(int _KeypadTypedNum);

//------ Prototype functions---------
//#include "StateMachine.h"
#include "IOSetup.h"
#include "MCP.h"
#include "PowerPot.h"
#include "Keypad.h"
#include "Display.h"
#include "Lamp.h"
#include "Cooker.h"
#include <TimerOne.h>

//global variables
enum microwaveStates {STAND_BY, COOKING, COOKING_FINISHED, PAUSE, DOOR_OPEN};
volatile int currentState = microwaveStates:: STAND_BY;  

  //INTERNAL VALUES
static int secondsTimer;
static int typedNum;
int powerSetting;

    //millis timer
bool _startMillisTimer;
bool _resetMillisTimer;
int  _millisCountPeriod;
bool oneSecPassed;

 
void setup() {
  // put your setup code here, to run once:
  
  pinMode(BTN_START, INPUT);
  pinMode(BTN_STOP,  INPUT);
  pinMode(DOOR_SW,   INPUT);
  pinMode(POT_PIN,   INPUT);
  pinMode(LAMP, OUTPUT);

  digitalWrite(LAMP,LOW);

  setPinModes();
  MCPsetup(); 
  Serial.begin(9600);
  Timer1.initialize(1000000);               // interrupt set to one second 
  Timer1.attachInterrupt(timerCountdown);   // set clock to tick every second
}

//Interrupt button logic
bool doorSwOpen(bool state){
  static bool doorOpen = !state; //doorSwOpen is TRUE when state is doorState is 0(zero)
  return doorOpen;
}

int getState(){
  return currentState;
}
int setState(int _state){
  currentState = _state;
  return currentState;
}
int getCookTime(){
  return secondsTimer;
}

int setCookTime(int _setTime){
  secondsTimer = _setTime;
  return secondsTimer;
}

bool getMillisTimer(){
  return oneSecPassed;
}

void setReadKeys(bool _state){
  readKeys(_state);
}
void setMillisTimer(bool _startTimer, bool _resetTimer, int _timePeriod){
  
  _startMillisTimer  = _startTimer;
  _resetMillisTimer  = _resetTimer;
  _millisCountPeriod = _timePeriod;
}

void setTypedNum(int _KeypadTypedNum){
  typedNum = _KeypadTypedNum;
}

void loop() {
  // put your main code here, to run repeatedly:

  //check DOOR and break from any currentState if its OPEN
  //if(doorSwOpen){
    //currentState = microwaveStates::DOOR_OPEN;  
  //}

  displayTime(secondsTimer);
  
  //State Machine
  switch (currentState){

    case STAND_BY:
      powerSetting = readPowerPot();
      displayPowerSetting(powerSetting);
      secondsTimer = readKeys(READ);
      millisTimer(!START_TIMER,
                  RESET_TIMER ,
                  1000        );//stop timer and reset
      turnLamp(LAMP_OFF);
      cook(COOKER_OFF,secondsTimer);
      break;

    case COOKING:
      
      if(secondsTimer > 0){
        //stop timer and reset
        millisTimer(!START_TIMER,
                    RESET_TIMER,
                    1000);
        //reset keypad
        readKeys(!READ);
        cook(powerSetting,secondsTimer);
      }
      
      else if (secondsTimer == 0){
        currentState = microwaveStates::COOKING_FINISHED;
      }
      break;

    case COOKING_FINISHED:
      cook(COOKER_OFF,secondsTimer);
      break;

    case DOOR_OPEN:
      cook(COOKER_OFF,secondsTimer);
      turnLamp(LAMP_ON);
      break;

    case PAUSE:
      oneSecPassed = millisTimer(_startMillisTimer,
                                 _resetMillisTimer,
                                 _millisCountPeriod);
      cook(COOKER_OFF,secondsTimer);
      turnLamp(LAMP_OFF);
      break;
  }

}

//-------------TIMER MILLIS variable-------------------
bool millisTimer(bool startTimer, bool reset, unsigned long countTime){
  // outputs
  static bool doneCount;
  // internal values
  unsigned long currentTimestamp;
  static unsigned long callTimestamp;
  unsigned long difference;

  enum timerStates {TIME_STAMP_COLLECT,COUNT, COUNT_DONE};
  static int currentState= timerStates::TIME_STAMP_COLLECT;

  switch(currentState){
    
    case TIME_STAMP_COLLECT:
        if(startTimer){
          callTimestamp = millis();
          currentState = timerStates::COUNT;
        }
        doneCount = LOW;     
        return doneCount;
      break;
         
    case COUNT:
        currentTimestamp = millis();
        difference = currentTimestamp-callTimestamp;
        if(difference >= countTime){
          currentState = timerStates::COUNT_DONE;    
        }
        doneCount = LOW;
        return doneCount;
      break;
         
    case COUNT_DONE:
        if(reset){
          currentState = timerStates::TIME_STAMP_COLLECT;
        }
        doneCount = HIGH;
        return doneCount;
      break;    
  }
}

  
// -------------- TIMER INTERRUPT 1s ------------------------
void timerCountdown(){
  static int tickCount =0; 
   //------------ Clock logic ------------
   if (currentState == microwaveStates::COOKING && secondsTimer > 0){         
     secondsTimer --;
     tickCount=0;
   }         
   else if (secondsTimer <= 0) {                   
      secondsTimer = 0;        
   }
 
   //------ Buzzer logic -----
   if(currentState == microwaveStates::COOKING_FINISHED){
      tickCount++;
      if(tickCount <= 5) // buzz 5 times
      {
        tone(BUZZER,NOTE_GS6,500);
      }
   }
}

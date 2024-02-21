 #define BUZZER 3            // define Buzzer pin
 #define LATCH  4            // define Latch led pin
 #define LAMP   5            // define Lamp led pin
 #define CLK    7            // define Clk led pin
 #define DATA   8            // define Data led pin

 #define POT_PIN   A0            // define Potentiometer pin
 #define DOOR_SW   A1            // define Door button pin
 #define BTN_STOP  A3            // define Start button pin
 #define BTN_START A2            // define Stop button pin

 #define NOTE_GS6 1661 //for number keys


//prototype functions

void pciSetup(byte pin); // setup what will be an interrupt
ISR(PCINT1_vect);        //Interupt Service Rutine is called when interrupt value changes

void setPinModes() 
{
  pinMode(DATA,  OUTPUT);        // set DATA  lamp to output
  pinMode(CLK,   OUTPUT);        // set CLK   lamp to output
  pinMode(LATCH, OUTPUT);        // set LATCH lamp to output
  pinMode(LAMP,  OUTPUT);        // set LAMP  lamp to output
  pinMode(BUZZER,  OUTPUT);      // set BUZZER     to output
  pinMode(BTN_START, INPUT);
  pinMode(BTN_STOP,  INPUT);
  pinMode(DOOR_SW,   INPUT);
  pinMode(POT_PIN,   INPUT);


  pciSetup(BTN_START);  //set START button as an interrupt
  pciSetup(BTN_STOP);   //set STOP  button as an interrupt
  pciSetup(DOOR_SW);    //set STOP  button as an interrupt

  digitalWrite(BUZZER, LOW);

}

ISR(PCINT1_vect)
{
  
    enum microwaveStates {STAND_BY, COOKING, COOKING_FINISHED, PAUSE, DOOR_OPEN};
    int currentMicrowaveState = microwaveStates::STAND_BY;
    int newMicrowaveState;
    static int currentSecondsTime;
    static int newSecondsTime;
    
//---------------------START BUTTON LOGIC---------------------    
    bool startState = digitalRead(BTN_START);       //read Start button
    static bool lastStartState;
    
    if (!startState && lastStartState){
        currentMicrowaveState = getState();
        currentSecondsTime = getCookTime();
        
      tone(BUZZER,NOTE_GS6,100); //buzz
      
      switch(currentMicrowaveState){
          case microwaveStates::STAND_BY:
            
            if (currentSecondsTime > 0){
                newMicrowaveState = microwaveStates::COOKING;
                setState(newMicrowaveState); 
            }
          break;
          case PAUSE:
             if (currentSecondsTime > 0){
                newMicrowaveState = microwaveStates::COOKING;
                setState(newMicrowaveState);   
             }
        } 
    }
    lastStartState = startState;
    
//---------------------STOP BUTTON PRESSED LOGIC--------------------- 
    bool stopState = digitalRead(BTN_STOP);
    static bool lastStopState; 
    
    if (!stopState && lastStopState){
        tone(BUZZER,NOTE_GS6,100);
        currentMicrowaveState = getState();
        currentSecondsTime = getCookTime();
 
        switch(currentMicrowaveState){
          case STAND_BY:
            newMicrowaveState = microwaveStates::STAND_BY;
            setState(newMicrowaveState); 
            newSecondsTime = 0;
            setCookTime(newSecondsTime);
            setReadKeys(LOW); //reset keypad and set seconds to 0           
            break;
            
          case COOKING:
            setMillisTimer(true,false,1000);
            newMicrowaveState = microwaveStates::PAUSE;
            setState(newMicrowaveState); 
            break;
          case COOKING_FINISHED:
            newMicrowaveState = microwaveStates::STAND_BY;
            setState(newMicrowaveState);
            break;
          case PAUSE:
            bool oneSecPassed = getMillisTimer();
            if(oneSecPassed){
              newSecondsTime = 0;
              setCookTime(newSecondsTime);
              setReadKeys(LOW); //reset keypad and set timer to 0
              
              newMicrowaveState = microwaveStates::STAND_BY; 
              setState(newMicrowaveState); 
            }
            break;

        }
    }
    lastStopState = stopState;


//---------------------DOOR SWITCH LOGIC---------------------   
    bool doorState = digitalRead(DOOR_SW);
    static bool lastDoorState;
    
    //Door switch change state
    if(doorState != lastDoorState){
        currentMicrowaveState = getState();
        doorSwOpen(doorState);// return to main loop 
        
      //Door open
      if(!doorState && lastDoorState){
          newMicrowaveState = microwaveStates::DOOR_OPEN;
          setState(newMicrowaveState);  
           
      }
      
      //Door closed
      else if(doorState && !lastDoorState){
          currentSecondsTime = getCookTime();
          if (currentSecondsTime == 0){
            newMicrowaveState = microwaveStates::STAND_BY;
            setState(newMicrowaveState);
          }
          else if (currentSecondsTime > 0){
            newMicrowaveState = microwaveStates::PAUSE;
            setState(newMicrowaveState);
        }
      }
      
      lastDoorState = doorState; 
    }


}




void pciSetup(byte pin)
{
  *digitalPinToPCMSK(pin) |= 1 << digitalPinToPCMSKbit(pin);

  PCIFR |= 1 << digitalPinToPCICRbit(pin);
 
  PCICR |= 1 << digitalPinToPCICRbit(pin);
}

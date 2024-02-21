//Prototype of functions
void runStateMachine();

//declara variables
enum microwaveStates {STAND_BY, COOKING, COOKING_FINISHED, PAUSE, DOOR_OPEN};



//initial write of parameters
//static int currentState= STAND_BY;
static int timer = 0;

//State currentState = microwaveStates:: STAND_BY;
volatile int currentState = microwaveStates:: DOOR_OPEN;



void runStateMachine(){

  int doorOpen = digitalRead(DOOR_SW);

  //run functions based on the state
  switch (currentState){

    case STAND_BY:
    Serial.print("STAND_BY");
    Serial.println(doorOpen);

    //State Functions -> what does the microwave do in this state
      //time input
      //power input
      //cooker off

    //Break State Conditions -> under what conditions the state switches
      //door opens
      if (doorOpen){
        currentState = microwaveStates::DOOR_OPEN;
      }

      //Start button pressed
      if (BTN_START){
        currentState = microwaveStates::COOKING;
      }
      break;


    case COOKING:
    //Serial.println("COOKING");

    //State Functions
      //cooker on
      //timer countdown
      //power no input

    //Break State Conditions
      if (timer == 0) {
        currentState = microwaveStates::COOKING_FINISHED;
      }
      if (BTN_STOP) {
        currentState = microwaveStates::PAUSE;
      }
      break;


    case COOKING_FINISHED:
    //Serial.println("COOKING_FINISHED");

    //State Functions
      //time=0
      //cooker off

    //Break State Conditions
      if (DOOR_SW){
        currentState = microwaveStates::DOOR_OPEN;
      }
      break;

  
    case PAUSE:
    //State Functions
    //Serial.println("PAUSE");

    //Break State Conditions
      if (BTN_STOP){
        currentState = microwaveStates::STAND_BY;
      }
      if (DOOR_SW){
        currentState = microwaveStates::DOOR_OPEN;
      }
      break;

  
    case DOOR_OPEN:
    //State Functions
    Serial.print("DOOR_OPEN");
    Serial.println(doorOpen);

    //Break State Conditions
      if (!doorOpen && timer == 0){
        currentState = microwaveStates::STAND_BY;
      }
  
      else if (!doorOpen && timer > 0){
        currentState = microwaveStates::PAUSE;
      }
      break;
  }
}

void displayTime(int t);
void displayInt(int num);
void displayChar(char powerSetting);
void displayDigit(int place, int number);



boolean potDisp = false;

const int letterP       = 13;
const int degree        = 12;
const int colon         = 11;
const int decimalPoint  = 10;

const byte digit[]={0X01,  //0b00000001 1st digit
                    0X02,  //0b00000010 2nd digit
                    0X04,  //0b00000100 3rd digit
                    0X08,  //0b00001000 4th digit
                    0X10}; //0b00010000 decimal point place


const byte displaySegments []= {0b00111111, //0
                                0b00000110, //1
                                0b01011011, //2
                                0b01001111, //3
                                0b01100110, //4
                                0b01101101, //5
                                0b01111101, //6
                                0b00000111, //7
                                0b01111111, //8
                                0b01101111, //9
                                0b10000000, //DP
                                0b00000100, //colon
                                0b00001000, //degree
                                0b01110011};//P

void displayInt(int num)
{
  
  int dig;
  if(!potDisp)
  {
    for (int i = 0; i<4; i++)
    {
      dig = num % 10;
      displayDigit(i, dig);
      num = num / 10; 
    }
  }
}
void displayTime(int t)
{
  int seconds;
  int minutes;
  if(!potDisp){
    minutes = t / 60;
    seconds = t % 60;

    displayDigit(0,minutes/10);
    displayDigit(1,minutes%10); 
    displayDigit(2,seconds/10);
    displayDigit(3,seconds%10);
    displayDigit(4,colon);
  }
}

void displayPowerSetting(int powerSetting)
{
  static int prevPowerSetting;
  static int displayTime = 0;
  static bool oneSecPassed;
  const bool runTimer = HIGH;
  const bool resetTimer = HIGH;
  
  if (powerSetting != prevPowerSetting)
  {
    potDisp = true;
    oneSecPassed = millisTimer(runTimer, !resetTimer, 1000);
   
    if(!oneSecPassed)
    {
      displayDigit(3, powerSetting);
      displayDigit(2, letterP);
    }
    else if(oneSecPassed)
    {
       prevPowerSetting = powerSetting;
       millisTimer(!runTimer,resetTimer,1000);
       potDisp = false;
    }    
  } 
}

void displayDigit(int place, int number)
{
  digitalWrite(LATCH,HIGH);
  shiftOut(DATA,CLK,MSBFIRST,displaySegments[number]);
  shiftOut(DATA,CLK,MSBFIRST,digit[place]);
  digitalWrite(LATCH,LOW);
    delay(1);      
}

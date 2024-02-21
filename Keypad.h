#define numOfKeys 12

#define NOTE_A4  440
#define NOTE_DS7 2489 //done program
#define NOTE_B7  3951
#define NOTE_GS6 1661 //for number keys
#define NOTE_F3  175

int callbackKeypad(int key ,int prevTypedNum);
int numToSec(int typedNum);

// --------- declare variables ----------
byte mcpRegA;
byte mcpRegB;
static byte prevMcpRegA;
static byte prevMcpRegB;



// --------- declare functions ----------
int readKeys();
//int typedValue(int typedDig);

// --------- define structure ----------
struct keypad
{ 
  byte regVal;
  int  numVal;
};

struct keypad button[numOfKeys] = {{0b00000001, 1   },  //1
                                   {0b00000010, 2   },  //2
                                   {0b00000100, 3   },  //3
                                   {0b00001000, 4   },  //4
                                   {0b00010000, 5   },  //5
                                   {0b00100000, 6   },  //6
                                   {0b00010000, 7   },  //7
                                   {0b00001000, 8   },  //8
                                   {0b00000100, 9   },  //9
                                   {0b10000000, 0xA },  //A
                                   {0b01000000, 0   },  //0
                                   {0b00100000, 0xB}};  //B


int readKeys(bool Read)
{
    static int typedNum;
    static int typedSeconds;
    
    if (!Read){
      typedNum = 0;
      typedSeconds = 0; 
      
      return typedSeconds;
      
    }
    else if(Read){
      //-------- READ MCP -------- 
      mcpRegA = mcpRead(GPIOA);
      delay(1);                 //delay gives time for Arduino to read MCP reg A before it starts reading reg B
      mcpRegB = mcpRead(GPIOB);
    
      // ----------- read from Reg B   -------------
        if(prevMcpRegB !=  mcpRegB)
        { 
          for(int key = 0; key < 6; key++)
          {
            if(mcpRegB & button[key].regVal)
            {
              typedNum = callbackKeypad(button[key].numVal, typedNum);
              typedSeconds = numToSec(typedNum);
            }  
          }
        } 
        // ----------- read from Reg B    -------------
  
        // ----------- read from Reg A -------------
        if(prevMcpRegA != mcpRegA)
        {
          for(int key = 6; key< 12; key++)
          {
            if(mcpRegA & button[key].regVal)
            {
              typedNum = callbackKeypad(button[key].numVal,typedNum);
              typedSeconds = numToSec(typedNum);
            }  
          }
        }
      // ----------- read from Reg A -------------
  
      // store previous MCP chip readings
      prevMcpRegA = mcpRegA;
      prevMcpRegB = mcpRegB;

      setTypedNum(typedNum);
      return typedSeconds;
    }   
}

int callbackKeypad(int key ,int prevTypedNum){
  int typedDig;
  int typedNum;
  
    if ((key >=0)&&(key<=9)){
       typedDig = key;
       typedNum = ((prevTypedNum%1000)*10)+ typedDig;
       tone(BUZZER,NOTE_GS6,100);   
    } 
   
    else if(key == 0xA){
      typedNum = prevTypedNum + 30;
      tone(BUZZER,NOTE_GS6,100);  
    }

    else if(key == 0xB){
      typedNum= prevTypedNum +60;
      tone(BUZZER,NOTE_GS6,100);  
    }
    return typedNum;
}

int numToSec(int typedNum){
  int seconds = (typedNum%100);
  int minutes = (typedNum/100)*60;
  int typedSeconds = minutes + seconds;
  return typedSeconds;
}  

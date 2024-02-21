void cook(int powerLevel, int secondsTimer)
{
  static int prevSecondsTimer = 0;
  static int counter = 0;
  int maxPowerLevel = 5;
  
  if(powerLevel == 0){
    digitalWrite(LAMP,LOW);
    counter=0;
  }
  else{
    if(secondsTimer != prevSecondsTimer){
      counter ++;
    }

    if(counter / powerLevel == 0){
      analogWrite(LAMP,128); //50% of max power (255)
    }
    else if(counter / powerLevel <= maxPowerLevel){
      analogWrite(LAMP,51); //20% of max power (255)
    }
    
    if(counter == maxPowerLevel){
      counter = 0;
    }
      prevSecondsTimer = secondsTimer;
  }
  
}

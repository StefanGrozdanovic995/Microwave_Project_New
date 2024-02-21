
int readPowerPot()
{
  int potVal = analogRead(POT_PIN);
  
  //analog read to integer range: 1 -> 5;
  float powerReading = ((5.0-1.0)/1023.0)*potVal + 1.0;
  int powerLevel = round(powerReading);
  return powerLevel;
}

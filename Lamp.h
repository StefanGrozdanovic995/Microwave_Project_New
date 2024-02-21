void turnLamp(bool lampOn)
{
  if (lampOn)
  {
    analogWrite(LAMP,255);
  }
  else if (!lampOn)
  {
    digitalWrite(LAMP,LOW); //20% of 255 
  }
}

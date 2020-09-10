void inputsRead()
{

  fret1New = map(analogRead(string1Pin), minAnalogReadString, maxAnalogReadString, 0, 22);    //String 1 analog input mapping
  fret1New = constrain(fret1New, 0, 22);
  fret2New = map(analogRead(string2Pin), minAnalogReadString, maxAnalogReadString, 0, 22);    //String 2 analog input mapping
  fret2New = constrain(fret2New, 0, 22);
  fret3New = map(analogRead(string3Pin), minAnalogReadString, maxAnalogReadString, 0, 22);    //String 3 analog input mapping
  fret3New = constrain(fret3New, 0, 22);
  
  jsxlMap = map(analogRead(jsxPin), 480, 0, 0, 127);    //Joystick horizontal axis left side
  jsxlMap = constrain(jsxlMap, 0, 127);
  jsxrMap = map(analogRead(jsxPin), 580, 1023, 0, 127);    //Joystick horizontal axis right side
  jsxrMap = constrain(jsxrMap, 0, 127);
  jsxMapNew = jsxlMap + jsxrMap;    //Joystick horizontal axis total
  jsyuMap = map(analogRead(jsyPin), 480, 0, 0, 127);    //Joystick vetical axis upper side
  jsyuMap = constrain(jsyuMap, 0, 127);
  jsydMap = map(analogRead(jsyPin), 580, 1023, 0, 127);    //Joystick vertical axis down side
  jsydMap = constrain(jsydMap, 0, 127);
  jsyMapNew = jsyuMap + jsydMap;    //Joystick vertical axis total


  sw1New = digitalRead(sw1Pin);    //Switch pins new state

  if(sw1Old != sw1New)   //For creating a note only using the analog strings and the joystick button
  {
    if(sw1New == LOW)
    {
      multiString = true;
    }
    if(sw1New == HIGH)
    {
      multiString = false;
      shutDownAll();
    }
  }
  sw1Old = sw1New;

}

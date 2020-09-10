/*
 * This is the program developed by the Visual Music Z team for making an electronic guitar using an Arduino UNO, to replicate the
 * basic functions of an electric guitar. Our website: http://visualmusicz.com/
 * 
 * It uses a MIDI X3M chip as the sound module, which recieves the MIDI commands to create notes,
 * audio modulation and sound effects, and sends them directly to an AUX output.
 * See MIDI Messages List: https://www.midi.org/specifications-old/item/table-2-expanded-messages-list-status-bytes
 * See MIDI Control Change Messages List: https://www.midi.org/specifications-old/item/table-3-control-change-messages-data-bytes-2
 * 
 * It also uses a Bluetooth communication module (HC-05) to interact with our mobile app to switch between instrument sounds,
 * effects and more options to increase the electronic guitar experience.
 * See BT communication and AT commands explanation video: https://youtu.be/5SmKOUHhmWk?t=240
 * 
 * List of materials used in this project:
 * x1 Generic Arduino UNO Rev3 board
 * x3 Spectra Symbol Softpot Ribbon Sensors (https://www.mouser.com/ProductDetail/Spectra-Symbol/SP-L-0500-203-3-ST?qs=rVuZtzXoae5%2FClXxQbhQtw%3D%3D)
 * x1 Nextion NX4827T043 HMI TFT LED Touch Display 4.3" (https://nextion.tech/datasheets/nx4827t043/)
 * x1 Generic Joystick Module (https://components101.com/modules/joystick-module)
 * x1 Serdaco X3M MIDI Sound Module (https://www.excelvalley.com/product/midi-usb-sound-module/)
 * x1 MIDI DIN Female Connector (https://www.sparkfun.com/products/9536)
 * x1 Generic HM-10 or HC-05 Bluetooth Module (http://www.electronica60norte.com/mwfls/pdf/newBluetooth.pdf)
 * x2 220ohms resistors
 * x3 10Kohms resistors
 * x3 Red laser diodes
 * x1 Powerbank 
 * Jumper Wires Male to Female
 * 
 * Created by: Alejandro Mendoza Rojas
 * Last edited: September-10-2020
 */

//--Libraries--//
#include <SoftwareSerial.h>     //Used for communicating the Arduino with the MIDI and Bluetooth modules, and the TFT LED Display 

//--Objects--//
SoftwareSerial mididin(2, 3);     //RX and TX on the Arduino, Pin 3 is the MIDI OUT
SoftwareSerial btmodule(4, 5);    //RX and TX on the Arduino, BT TX to AR RX, BT RX to AR TX
SoftwareSerial tftlcd(6, 7);      //RX and TX on the Arduino

//--Pins--//
#define jsxPin A0       //Joystick analog input pins
#define jsyPin A1
#define sw1Pin 8        //Joystick push button digital input pin
#define string1Pin A2   //Guitar strings analog pins
#define string2Pin A3
#define string3Pin A4

//--Notes--//
byte DO[11] =     {0, 0, 24, 36, 48, 60, 72, 84, 96,108,120};  //C
byte DO_S[11] =   {0, 0, 25, 37, 49, 61, 73, 85, 97,109,121};  //C#
byte RE[11] =     {0, 0, 26, 38, 50, 62, 74, 86, 98,110,122};  //D
byte RE_S[11] =   {0, 0, 27, 39, 51, 63, 75, 87, 99,111,123};  //D#
byte MI[11] =     {0, 0, 28, 40, 52, 64, 76, 88,100,112,124};  //E
byte FA[11] =     {0, 0, 29, 41, 53, 65, 77, 89,101,113,125};  //F
byte FA_S[11] =   {0, 0, 30, 42, 54, 66, 78, 90,102,114,126};  //F#
byte SOL[11] =    {0, 0, 31, 43, 55, 67, 79, 91,103,115,127};  //G
byte SOL_S[11] =  {0, 0, 32, 44, 56, 68, 80, 92,104,116,  0};  //G#
byte LA[11] =     {0, 0, 33, 45, 57, 69, 81, 93,105,117,  0};  //A
byte LA_S[11] =   {0, 0, 34, 46, 58, 70, 82, 94,106,118,  0};  //A#
byte SI[11] =     {0, 0, 35, 47, 59, 71, 83, 95,107,119,  0};  //B

//--Guitar strings arrays--//
//Arrays containing the MIDI note numbers of each string
int C1[23] = {DO_S[3], SI[4], LA_S[4], LA[4], SOL_S[4], SOL[4], FA_S[4], FA[4], MI[4],
RE_S[4], RE[4], DO_S[4], DO[4], SI[3], LA_S[3], LA[3], SOL_S[3], SOL[3], FA_S[3], FA[3], MI[3], RE_S[3], RE[3]};
int C2[23] = {SOL_S[3], FA_S[5], FA[5], MI[5], RE_S[5], RE[5], DO_S[5], DO[5], SI[4], LA_S[4], LA[4], SOL_S[4],
SOL[4], FA_S[4], FA[4], MI[4], RE_S[4], RE[4], DO_S[4], DO[4], SI[3], LA_S[3], LA[3]};
int C3[23] = {RE_S[4], DO_S[6], DO[6], SI[5], LA_S[5], LA[5], SOL_S[5], SOL[5], FA_S[5], FA[5], MI[5], RE_S[5],
RE[5], DO_S[5], DO[5], SI[4], LA_S[4], LA[4], SOL_S[4], SOL[4], FA_S[4], FA[4], MI[4]};

//--Sequences--//
//MIDI note numbers for the sequences activated by using the app
byte littlestar[] = {57, 57, 64, 64,  66, 66, 64, 62, 62, 61, 61, 59, 59, 57, 64, 64, 62, 62, 61, 61, 59, 64,
64, 62, 62, 61, 61, 59, 57, 57, 64, 64,  66, 66, 64, 62, 62, 61, 61, 59, 59, 57 };
byte littlestarh[] = {57, 57, 64, 64,  66, 66, 64, 62, 62, 61, 61, 59, 59, 57, 64, 64, 62, 62, 61, 61, 59,
64, 64, 62, 62, 61, 61, 59, 57, 57, 64, 64,  66, 66, 64, 62, 62, 61, 61, 59, 59, 57 };
byte minuet[] = {62, 55, 57, 59, 60, 62, 55, 55, 64, 60, 62, 64, 66, 67, 55, 55, 60, 62, 60, 59, 57, 59, 60, 59, 57,
55, 54, 55, 57, 59, 55, 59, 57};
byte minueth[] = {62, 55, 57, 59, 60, 62, 55, 55, 64, 60, 62, 64, 66, 67, 55, 55, 60, 62, 60, 59, 57, 59, 60, 59, 57,
55, 54, 55, 57, 59, 55, 59, 57, 62, 55, 57, 59, 60, 62, 55, 55, 64, 60, 62, 64, 66, 67, 55, 55, 60, 62, 60, 59, 57,
59, 60, 59, 57, 55, 57, 59, 57, 55, 54, 55};
byte oda[] = {66, 66, 67, 69, 69, 67, 66, 64, 62, 62, 64, 66, 66, 64, 64, 66, 66, 67, 69, 69, 67, 66, 64, 62, 62, 64,
66, 64, 62, 62, 64, 64, 66, 62, 64, 66, 67, 66, 62, 64, 66, 67, 66, 64, 62, 64, 57, 66, 66, 67, 69, 69, 67, 66, 64,
62, 62, 64, 66, 64, 62, 62};
byte odah[] = {66, 66, 67, 69, 69, 67, 66, 64, 62, 62, 64, 66, 66, 64, 64, 66, 66, 67, 69, 69, 67, 66, 64, 62, 62,
64, 66, 64, 62, 62, 64, 64, 66, 62, 64, 66, 67, 66, 62, 64, 66, 67, 66, 64, 62, 64, 57, 66, 66, 67, 69, 69, 67, 66,
64, 62, 62, 64, 66, 64, 62, 62};

//--Variables--//
int jsxlMap = 0;    //Joystick variables
int jsxrMap = 0;
int jsyuMap = 0;
int jsydMap = 0;
int jsxMapOld = 0;
int jsxMapNew = 0;
int jsyMapOld = 0;
int jsyMapNew = 0;
int fret1Old = 0;   //Fret variables for the guitar strings
int fret1New = 0;
int fret2Old = 0;
int fret2New = 0;
int fret3Old = 0;
int fret3New = 0;
byte newSlider = 0;   //Guitar digital strings (TFT slider) states
byte oldSlider = 0;
bool sw1Old = LOW;    //Joystick push button states
bool sw1New = LOW;
bool multiString = false;   //For creating a note only using the analog strings and the joystick button
byte analogParameter1 = 176;    //MIDI analog function to send
byte analogParameter2 = 176;
byte analogParameter3 = 224;

//--Others--//
byte instrument = 30;   //The default instrument is an acoustic nylon guitar
int minAnalogReadString = 40;   //Softpot limits
int maxAnalogReadString = 1023; //Softpot limits
char btdata = " ";    //Character variable we will receive from the Bluetooth module
char tftchar = " ";
String tftdata = " "; //String variable we will receive from the TFT LCD
bool possibleSlide1 = false;  //For creating slides
bool possibleSlide2 = false;
bool possibleSlide3 = false;

/*
 * Used MIDI commands:
 * 144, x, 127: Note on, "x" is the note in MIDI number, 127 means full intensity
 * 128, x, 00: Note off, "x" is the note in MIDI number, third number doesn't matter
 * 176, 01, x: Control change, modulation wheel or lever, "x" is a value from 0 to 127
 * 176, 64, x: Control change, damper pedal on/off (sustain), "x" <= 63 means off, "x" >= 64 means on
 * 176, 91, x: Control change, depth reverb, "x" is a value from 0 to 127
 * 176, 126, 00: Control change, mono mode on + poly mode off
 * 192, 00, x: Program/instrument change, "x" is the instrument number
 * 224, 01, x: Pitch bend change, "x" is a value from 0 to 127
 */

/*
 * Used instruments (*):
 * 24 - Acoustic guitar (nylon) *
 * 25 - Acoustic guitar (steel)
 * 26 - Electric guitar (jazz)
 * 27 - Electric guitar (clean)
 * 28 - Electric guitar (muted)
 * 29 - Overdriven guitar       *
 * 30 - Distortion guitar
 * 31 - Guitar harmonics
 * 32 - Acoustic bass           *
 * 33 - Electric bass (finger)  *
 * 40 - Violin                  *
 * 56 - Trumpet                 *
 * 73 - Flute                   *
 * 79 - Ocarina                 *
 * 
 * See Full MIDI Instruments list: http://fmslogo.sourceforge.net/manual/midi-instrument.html
 */

  //By last, down in the code we will refer to the strings shown on the TFT Display as "Digital strings",
  //and the physical strings or softpot ribbon sensors as "Analog strings", all to avoid creating confusion
  //between these two and the String type variables.

void setup()    //Part of the code that runs first and only once
{
  Serial.begin(9600);   //Initializing Serial communication with devices
  mididin.begin(31250);
  //btmodule.begin(38400);
  tftlcd.begin(9600);
  Serial.println("Serial on");
  noteSendAll(192, 00, instrument);    //Assign the default instrument for channels 1-3
  delay(50);
  noteSendAll(176, 91, 127);   //Reverb enabled for channels 1-3
  delay(50);
  noteSendAll(176, 124, 00);
  delay(50);
  noteSendAll(176, 126, 00);   //Mono Mode On for channels 1-3
  pinMode(sw1Pin, INPUT_PULLUP);    //Joystick push button as input pullup
}

void loop()   //Part of the code that runs over and over
{
  //Void for reading the inputs (sensors, Bluetooth and TFT data)
  inputsRead();

  //Instrument change function (with Bluetooth)
  if(btmodule.available())
    {
   btdata = btmodule.read();
   Serial.println(btdata);
    
      if(btdata == '3')
      {
        instrument = 24;    //New instrument: Acoustic guitar (nylon)
        shutDownAll();
        noteSendAll(192,00,instrument);
      }
      else if(btdata == '4')
      {
        instrument = 29;    //New instrument: Electric guitar (clean)
        shutDownAll();
        noteSendAll(192,00,instrument);
      }
      else if(btdata == '1')
      {
        instrument = 32;    //New instrument: Acoustic bass
        shutDownAll();
        noteSendAll(192,00,instrument);
      }
      else if(btdata == '2')
      {
        instrument = 33;    //New instrument: Electric bass (finger)
        shutDownAll();
        noteSendAll(192,00,instrument);
      }
      else if(btdata == '7')
      {
        instrument = 40;    //New instrument: Violin
        shutDownAll();
        noteSendAll(192,00,instrument);
      }
      else if(btdata == '8')
      {
        instrument = 56;    //New instrument: Trumpet
        shutDownAll();
        noteSendAll(192,00,instrument);
      }
      else if(btdata == '5')
      {
        instrument = 73;    //New instrument: Flute
        shutDownAll();
        noteSendAll(192,00,instrument);
      }
      else if(btdata == '6')
      {
        instrument = 79;    //New instrument: Ocarina
        shutDownAll();
        noteSendAll(192,00,instrument);
      }
      else if(btdata == 's')
      {
        shutDownAll();
        noteSendAll(176, 64, 00);     //Turn off sustain
      }
      else if(btdata == 't')
      {
        shutDownAll();
        noteSendAll(176, 64, 127);     //Turn on sustain
      }
      else if(btdata == 'u')
      {
        shutDownAll();
        noteSendAll(176, 91, 00);     //Turn off reverb
      }
      else if(btdata == 'v')
      {
        shutDownAll();
        noteSendAll(176, 91, 127);    //Enable reverb to the maximum
      }
      else if(btdata = 'w')
      {
        shutDownAll();
        noteSendAll(176, 65, 00);    //Portamento off for channels 1-3
      }
      else if(btdata = 'x')
      {
        shutDownAll();
        noteSendAll(176, 65, 127);    //Portamento on for channels 1-3
      }
      /*
      else if(btdata == 'y')
      {
        shutDownAll();
        btmodule.end();
        tftlcd.begin(9600);
      }
      */
      else if(btdata == 'z')    //Activate TFT communication and end Bluetooth communication
      {
        shutDownAll();
        btmodule.end();
        tftlcd.begin(9600);
      }
      if(Serial.read()=='9')  //Little star sequence
      {
        Serial.println("Comenzando secuencia");
        for(int i=0; i<42;i)
        {
          inputsRead();
          if(C1[fret1New] == littlestar[i] || C2[fret2New] == littlestar[i] || C3[fret3New] == littlestar[i]) //If the actual note is correct
          {
            Serial.println("a");
            btmodule.write('a');
            i=i+1;
          }
        }
        Serial.println("Secuencia terminada");
      }
      else
      btdata = " ";   //Clean the data variable
  }

  if(tftlcd.available())    //If data is comming from the tft display
  {
    char tftchar = " ";   //Clean variables
    String tftdata = " ";
    delay(10);
    while(tftlcd.available())    //While data is comming from the tft display  
    {
      tftchar = char(tftlcd.read());
      tftdata += tftchar;   //Construct and save the whole message
    }
  if(tftdata.toInt() >= 0 && tftdata.toInt() <= 16)       //Digital string 1
  newSlider = 1;
  if(tftdata.toInt() >= 17 && tftdata.toInt() <= 33)      //Digital string 2
  newSlider = 2;
  if(tftdata.toInt() >= 34 && tftdata.toInt() <= 50)      //Digital string 3
  newSlider = 3;
  if(tftdata.toInt() >= 51 && tftdata.toInt() <= 5051)    //Release TFT slider
  newSlider = 0;
  if(tftdata.toInt() >= 5052)   
  {
    shutDownAll();    //Activate Bluetooth communication and end TFT communication
    tftlcd.end();
    btmodule.begin(38400);    
  }
  }
  //--Note On/Off command factory--//
  if(newSlider != oldSlider)    //If the TFT slider changes state
  {
    if(newSlider == 0)    //If it's released
    {
      Serial.println(newSlider);
      shutDownAll();
      possibleSlide1 = false;
      possibleSlide2 = false;
      possibleSlide3 = false;
    }
    if(newSlider == 1)    //String 1
    {
      Serial.println(newSlider);
      noteSend(144, C1[fret1New], 127);   //Send a note on message for channel 1. The fret is the actual value on the first softpot
      possibleSlide1 = true;
    }
    if(newSlider == 2)    //String 2
    {
      Serial.println(newSlider);
      noteSend(145, C2[fret2New], 127);   //Send a note on message for channel 2. The fret is the actual value on the second softpot
      possibleSlide2 = true;
    }
    if(newSlider == 3)    //String 3
    {
      Serial.println(newSlider);
      noteSend(146, C3[fret3New], 127);   //Send a note on message for channel 3. The fret is the actual value on the third softpot
      possibleSlide3 = true;
    }
  }
  oldSlider = newSlider;
  if(((multiString == true && fret1New != 0) || possibleSlide1 == true) && fret1New != fret1Old)    //Slider effect enabled for string 1
  {
    Serial.println(C1[fret1New]);
    noteSend(128, C1[fret1Old], 127);
    noteSend(144, C1[fret1New], 127);
  }
  fret1Old = fret1New;
  if(((multiString == true && fret2New != 0) || possibleSlide2 == true) && fret2New != fret2Old)    //Slider effect enabled for string 2
  {
    Serial.println(C2[fret2New]);
    noteSend(129, C2[fret2Old], 127);
    noteSend(145, C2[fret2New], 127);
  }
  fret2Old = fret2New;
  if(((multiString == true && fret3New != 0) || possibleSlide3 == true) && fret3New != fret3Old)    //Slider effect enabled for string 3
  {
    Serial.println(C3[fret3New]);
    noteSend(130, C3[fret3Old], 127);
    noteSend(146, C3[fret3New], 127);
  }
  fret3Old = fret3New;
  if(multiString == true && fret1New == 0 && fret2New == 0 && fret3New == 0)
  {
    shutDownAll();
  }
  

  //--Joystick pitch bending--//

  if(jsxMapNew - jsxMapOld >= 100 || jsxMapOld - jsxMapNew >= 100)    //If joystick value changes by 100 (X axis)
  {
    jsxMapOld = jsxMapNew;
    noteSendAll(analogParameter1, 01, jsxMapNew);
    Serial.print("Js on X: "); Serial.println(jsxMapNew);
  }

  if(jsyMapNew - jsyMapOld >= 100 || jsyMapOld - jsyMapNew >= 100)    //If joystick value changes by 100 (Y axis)
  {
    jsyMapOld = jsyMapNew;
    noteSendAll(analogParameter3, 00, jsyMapNew);
    Serial.print("Js on Y: "); Serial.println(jsyMapNew);
  }
}

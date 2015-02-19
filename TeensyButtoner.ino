#define REPEATRATE 100 // milliseconds

const int pinBtnUp = 0;
const int pinBtnRight = 1;
const int pinBtnDown = 2;
const int pinBtnLeft = 3;
const int pinBtnStart = 4;
const int pinBtnB = 5;
const int pinBtnA = 6;
const int pinBtnTrigLeft = 21;
const int pinBtnTrigRight = 20;
const int pinLEDOutput = 11;

const int pinBattLEDOutput1 = 18;
const int pinBattLEDOutput2 = 19;

const int pinBattProbe = 17;

bool low_batt = false;

byte buttons[] = { 
  pinBtnUp,
  pinBtnRight,
  pinBtnDown,
  pinBtnLeft,
  pinBtnStart,
  pinBtnB,
  pinBtnA,
  pinBtnTrigLeft,
  pinBtnTrigRight
}; 

/* A true in this lookup table means that the input is HIGH - ON and LOW - OFF,
as opposed to INPUT_PULLUP on, HIGH - OFF and LOW - ON. */
bool is_inverted[] = {
  false, /* Up */
  false, /* Right */
  false, /* Down */
  false, /* Left */
  false, /* Start */
  false, /* B */
  false, /* A */
  true, /* LeftTrig */
  true  /* RightTrig */
};

short keys[] = {
  KEY_UP, /* Up */
  KEY_RIGHT, /* Right */
  KEY_DOWN, /* Down */
  KEY_LEFT, /* Left */
  KEY_ENTER, /* Start */
  KEY_B,  /* B */
  KEY_A,  /* A */
  KEY_P, /* LeftTrig */
  KEY_Q  /* RightTrig */
};

#define NUMBUTTONS sizeof(buttons)
typedef void KeyFunction_t(uint8_t c);
KeyFunction_t* buttonActive[NUMBUTTONS];
KeyFunction_t* keyList[] = {myset_key6, myset_key5, myset_key4, myset_key3, myset_key2, myset_key1};
int keySlot = sizeof(keyList) / sizeof(KeyFunction_t*);

void setup()
{
  //Setup the pin modes.
  pinMode(pinLEDOutput, OUTPUT);
  pinMode(pinBattLEDOutput1, OUTPUT);
  pinMode(pinBattLEDOutput2, OUTPUT);
  for (byte i=0; i< NUMBUTTONS; i++) {
    /* We don't want INPUT_PULLUPs on a HIGH - ON LOW - OFF button. */
    if (!is_inverted[i]){
      pinMode(buttons[i], INPUT_PULLUP);
    } else {
      pinMode(buttons[i], INPUT);
    }
  }

  for (int i=0; i < NUMBUTTONS; i++) {
    buttonActive[i] = 0;
  }
}

void loop()
{
  /* Uncomment this so the onboard LED is has direct feedback with the start button for an easy indication that everything
  is working ok.*/
  //digitalWrite ( pinLEDOutput, digitalRead(pinBtnStart));
  fcnProcessButtons();
  fcnCheckBatteryLevel();
}

void fcnCheckBatteryLevel()
{
  int currentBatteryLevel = analogRead(pinBattProbe);
  float batteryVoltage = currentBatteryLevel * (5.0 / 1023.0);
  if (batteryVoltage < 3.3){
     /* Battery floor cutoff is coming very soon... */  
     batteryIndicator(true);
  } else {
      /* This could alternatively be done by reading the state of one of the LED output pins to determine
      what the LED is currently doing, however the boolean is less cryptic. */
      if (low_batt == true){
          batteryIndicator(false);
      }
  }
}

void batteryIndicator(bool low_battery)
{
    if (low_battery){
      digitalWrite(pinBattLEDOutput1, LOW);
      digitalWrite(pinBattLEDOutput2, HIGH);
      low_batt = true;
    } else {
      digitalWrite(pinBattLEDOutput1, HIGH);
      digitalWrite(pinBattLEDOutput2, LOW);
      low_batt = false;
    }
}

//Function to process the buttons from the SNES controller
void fcnProcessButtons()
{
  bool keysPressed = false;
  bool keysReleased = false;
  // run through all the buttons
  for (byte i = 0; i < NUMBUTTONS; i++) {
    // are any of them pressed?
    if (buttonState(i)){ //this button is pressed
      keysPressed = true;
      if (!buttonActive[i]){ //was it pressed before?
        activateButton(i); //no - activate the keypress
      }
    } else { //this button is not pressed
      if (buttonActive[i]) { //was it pressed before?
        releaseButton(i); //yes - release the keypress
        keysReleased = true;
      }
      
    }
  }
  
  if (keysPressed || keysReleased){
    Keyboard.send_now(); //update all the keypresses
  }
}

/* ButtonState - This will do a digital read with the proper logic for the input being polled.
i is the index of the button being polled. */
bool buttonState(int i)
{
  if (!is_inverted[i]){
    /* Code to handle non-inverting (with respect to the INPUT PULLUP) buttons */
    if (! digitalRead(buttons[i])){
        return true;
    } else {
        return false;
    }
  } else {
    /* Code to handle inverting buttons */
    if (digitalRead(buttons[i])){
      return true;
    } else {
      return false;
    }  
  }
}
  
void activateButton(byte index)
{
  if (keySlot) //any key slots left?
  {
    keySlot--; //Push the keySlot stack
    buttonActive[index] = keyList[keySlot]; //Associate the keySlot function pointer with the button
    (*keyList[keySlot])(keys[index]); //Call the key slot function to set the key value
  }
}

void releaseButton(byte index)
{
  keyList[keySlot] = buttonActive[index]; //retrieve the keySlot function pointer
  buttonActive[index] = 0; //mark the button as no longer pressed
  (*keyList[keySlot])(0); //release the key slot
  keySlot++; //pop the keySlot stack
}

void myset_key1(uint8_t c)
{
  Keyboard.set_key1(c);
}
void myset_key2(uint8_t c)
{
  Keyboard.set_key2(c);
}
void myset_key3(uint8_t c)
{
  Keyboard.set_key3(c);
}
void myset_key4(uint8_t c)
{
  Keyboard.set_key4(c);
}
void myset_key5(uint8_t c)
{
  Keyboard.set_key5(c);
}
void myset_key6(uint8_t c)
{
  Keyboard.set_key6(c);
}


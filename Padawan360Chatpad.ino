// =======================================================================================
// /////////////////////////Padawan360 Body Code - Mega I2C v2.0 /////////////////////////
// =======================================================================================
/*
//                     Base Chatpad with 2 MAESTROS code for R2D2
//                                    Mod by Lanthi
//
*/

// ************************** Options, Configurations, and Settings ***********************


// SPEED AND TURN SPEEDS
//set these 3 to whatever speeds work for you. 0-stop, 127-full speed.
const byte DRIVESPEED1 = 50;  //speed of
// Recommend beginner: 50 to 75, experienced: 100 to 127, I like 100.
// These may vary based on your drive system and power system
const byte DRIVESPEED2 = 100;
//Set to 0 if you only want 2 speeds.
const byte DRIVESPEED3 = 127;

// Default drive speed at startup
byte drivespeed = DRIVESPEED1;

// the higher this number the faster the droid will spin in place, lower - easier to control.
// Recommend beginner: 40 to 50, experienced: 50 $ up, I like 70
// This may vary based on your drive system and power system
const byte TURNSPEED = 70;

// Set isLeftStickDrive to true for driving  with the left stick
// Set isLeftStickDrive to false for driving with the right stick (legacy and original configuration)
boolean isLeftStickDrive = true;

// If using a speed controller for the dome, sets the top speed. You'll want to vary it potenitally
// depending on your motor. My Pittman is really fast so I dial this down a ways from top speed.
// Use a number up to 127 for serial
const byte DOMESPEED = 127;

// Ramping- the lower this number the longer R2 will take to speedup or slow down,
// change this by incriments of 1
const byte RAMPING = 3;

// Compensation is for deadband/deadzone checking. There's a little play in the neutral zone
// which gets a reading of a value of something other than 0 when you're not moving the stick.
// It may vary a bit across controllers and how broken in they are, sometimex 360 controllers
// develop a little bit of play in the stick at the center position. You can do this with the
// direct method calls against the Syren/Sabertooth library itself but it's not supported in all
// serial modes so just manage and check it in software here
// use the lowest number with no drift
// DOMEDEADZONERANGE for the left stick, DRIVEDEADZONERANGE for the right stick
const byte DOMEDEADZONERANGE = 20;
const byte DRIVEDEADZONERANGE = 20;

// Set the baud rate for the Sabertooth motor controller (feet)
// 9600 is the default baud rate for Sabertooth packet serial.
// for packetized options are: 2400, 9600, 19200 and 38400. I think you need to pick one that works
// and I think it varies across different firmware versions.
const int SABERTOOTHBAUDRATE = 9600;

// Set the baud rate for the Syren motor controller (dome)
// for packetized options are: 2400, 9600, 19200 and 38400. I think you need to pick one that works
// and I think it varies across different firmware versions.
const int DOMEBAUDRATE = 9600;

// Set the baud rate for AstroPixels Plus
const int MARCDUINOBAUDRATE = 2400;

// Default sound volume at startup
// 0 = full volume, 255 off
byte vol = 50;

// Automation Delays
// set automateDelay to min and max seconds between sounds
byte automateDelay = random(5, 15);
//How much the dome may turn during automation.
int turnDirection = 55;

#include <Sabertooth.h>
#include <MP3Trigger.h>
#include <Wire.h>
#include <XBOXRECV.h>
#include <Chatpad.h>
/////////////////////////////////////////////////////////////////
Sabertooth Sabertooth2x(128, Serial1);
Sabertooth Syren10(128, Serial2);

// Satisfy IDE, which only needs to see the include statment in the ino.
#ifdef dobogusinclude
#include <spi4teensy3.h>
#endif

// Set some defaults for start up
// false = drive motors off ( right stick disabled ) at start
boolean isDriveEnabled = false;

// Automated functionality
// Used as a boolean to turn on/off automated functions like periodic random sounds and periodic dome turns
boolean isInAutomationMode = false;
unsigned long automateMillis = 0;
// Action number used to randomly choose a sound effect or a dome turn
byte automateAction = 0;

int driveThrottle = 0;
int throttleStickValue = 0;
int domeThrottle = 0;
int turnThrottle = 0;

boolean firstLoadOnConnect = false;

AnalogHatEnum throttleAxis;
AnalogHatEnum turnAxis;
AnalogHatEnum domeAxis;
ButtonEnum speedSelectButton;
ButtonEnum hpLightToggleButton;

boolean isHPOn = false;

MP3Trigger mp3Trigger;
USB Usb;
XBOXRECV Xbox(&Usb);

#include <PololuMaestro.h>

/* On boards with a hardware serial port available for use, use
that port to communicate with the Maestro. For other boards,
create a SoftwareSerial object using pin 10 to receive (RX) and
pin 11 to transmit (TX) */


#include <SoftwareSerial.h>
SoftwareSerial maestroSerial(10, 11);
SoftwareSerial maestroSerial2(12, 13);




/* Next, create a Maestro object using the serial port.

Uncomment one of MicroMaestro or MiniMaestro below depending
on which one you have. */
//MicroMaestro maestro(maestroSerial);
MiniMaestro maestrobody(maestroSerial);
MiniMaestro maestrodome(maestroSerial2);


/************* Xbox360 Chatpad Configuration  ******************/
#define CHATPAD  //Uncomment if using a chatpad
bool Mod_Shift = false;
bool Mod_Green = false;
bool Mod_Messenger = false;
bool Mod_Orange = false;

/************** End Configuration *******************************/

void setup() {
  Serial1.begin(SABERTOOTHBAUDRATE);
  Serial2.begin(DOMEBAUDRATE);
  Serial3.begin(MARCDUINOBAUDRATE);
  maestroSerial.begin(9600);
  maestroSerial2.begin(9600);

#if defined(SYRENSIMPLE)
  Syren10.motor(0);
#else
  Syren10.autobaud();
#endif

  // Send the autobaud command to the Sabertooth controller(s).

  Sabertooth2x.autobaud();
  // The Sabertooth won't act on mixed mode packet serial commands until
  // it has received power levels for BOTH throttle and turning, since it
  // mixes the two together to get diff-drive power levels for both motors.
  Sabertooth2x.drive(0);
  Sabertooth2x.turn(0);


  Sabertooth2x.setTimeout(950);
  Syren10.setTimeout(950);

  mp3Trigger.setup();
  mp3Trigger.setVolume(vol);

  if (isLeftStickDrive) {
    throttleAxis = LeftHatY;
    turnAxis = LeftHatX;
    domeAxis = RightHatX;
    speedSelectButton = L3;
    hpLightToggleButton = R3;

  } else {
    throttleAxis = RightHatY;
    turnAxis = RightHatX;
    domeAxis = LeftHatX;
    speedSelectButton = R3;
    hpLightToggleButton = L3;
  }


  // Start I2C Bus. The body is the master.
  Wire.begin();

  Serial.begin(115200);
  // Wait for serial port to connect - used on Leonardo, Teensy and other boards with built-in USB CDC serial connection
  while (!Serial)
    ;
  if (Usb.Init() == -1) {
    Serial.print(F("\r\nOSC did not start"));
    while (1)
      ;  //halt
  }
  Serial.println(F("\r\nXbox Wireless Receiver Library Started"));
}

void loop() {
  Usb.Task();
  // if we're not connected, return so we don't bother doing anything else.
  // set all movement to 0 so if we lose connection we don't have a runaway droid!
  // a restraining bolt and jawa droid caller won't save us here!
  if (!Xbox.XboxReceiverConnected || !Xbox.Xbox360Connected[0]) {
    Sabertooth2x.drive(0);
    Sabertooth2x.turn(0);
    Syren10.motor(1, 0);
    firstLoadOnConnect = false;
    return;
  }

  // After the controller connects, Blink all the LEDs so we know drives are disengaged at start
  if (!firstLoadOnConnect) {
    firstLoadOnConnect = true;
    mp3Trigger.play(1);
    Xbox.setLedMode(ROTATING, 0);
  }

  if (Xbox.getButtonClick(XBOX, 0)) {
    Serial.println(F("Button XBOX"));
    if (Xbox.getButtonPress(L1, 0) && Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button L1 + R1"));
      Xbox.disconnect(0);
    }
  }

  // enable / disable right stick (droid movement) & play a sound to signal motor state
  if (Xbox.getButtonClick(START, 0)) {
    Serial.println(F("Button START"));
    if (isDriveEnabled) {
      isDriveEnabled = false;
      Xbox.setLedMode(ROTATING, 0);
      mp3Trigger.play(23);
    } else {
      isDriveEnabled = true;
      mp3Trigger.play(17);
      // //When the drive is enabled, set our LED accordingly to indicate speed
      if (drivespeed == DRIVESPEED1) {
        Xbox.setLedOn(LED1, 0);
      } else if (drivespeed == DRIVESPEED2 && (DRIVESPEED3 != 0)) {
        Xbox.setLedOn(LED2, 0);
      } else {
        Xbox.setLedOn(LED3, 0);
      }
    }
  }

  //Toggle automation mode with the BACK button
  if (Xbox.getButtonClick(BACK, 0)) {
    Serial.println(F("Button BACK"));
    if (isInAutomationMode) {
      isInAutomationMode = false;
      automateAction = 0;
      mp3Trigger.play(30);
    } else {
      isInAutomationMode = true;
      mp3Trigger.play(52);
    }
  }

  // Plays random sounds or dome movements for automations when in automation mode
  if (isInAutomationMode) {
    unsigned long currentMillis = millis();

    if (currentMillis - automateMillis > (automateDelay * 1000)) {
      automateMillis = millis();
      automateAction = random(1, 5);

      if (automateAction > 1) {
        mp3Trigger.play(random(6, 23));
      }
      if (automateAction < 4) {
#if defined(SYRENSIMPLE)
        Syren10.motor(turnDirection);
#else
        Syren10.motor(1, turnDirection);
#endif

        delay(350);

#if defined(SYRENSIMPLE)
        Syren10.motor(0);
#else
        Syren10.motor(1, 0);
#endif

        if (turnDirection > 0) {
          turnDirection = -55;
        } else {
          turnDirection = 55;
        }
      }

      // sets the mix, max seconds between automation actions - sounds and dome movement
      automateDelay = random(3, 10);
    }
  }

  // Volume Control of MP3 Trigger
  // Hold R1 and Press Up/down on D-pad to increase/decrease volume
  if (Xbox.getButtonClick(UP, 0)) {
    // volume up
    if (Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button R1 + UP"));
      if (vol > 0) {
        vol--;
        mp3Trigger.setVolume(vol);
        Serial.print(F("Volume: "));
        Serial.println(vol);
      }
    }
  }
  if (Xbox.getButtonClick(DOWN, 0)) {
    //volume down
    if (Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button R1 + DOWN"));
      if (vol < 255) {
        vol++;
        mp3Trigger.setVolume(vol);
        Serial.print(F("Volume: "));
        Serial.println(vol);
      }
    }
  }

  // Logic display brightness.
  // Hold L1 and press up/down on dpad to increase/decrease brightness
  if (Xbox.getButtonClick(UP, 0)) {
    if (Xbox.getButtonPress(L1, 0)) {
      Serial.println(F("Button L1 + UP"));
      triggerI2C(10, 24);
    }
  }
  if (Xbox.getButtonClick(DOWN, 0)) {
    if (Xbox.getButtonPress(L1, 0)) {
      Serial.println(F("Button L1 + DOWN"));
      triggerI2C(10, 25);
    }
  }

#if defined(CHATPAD)
  Check_Chatpad();
#endif

  // GENERAL SOUND PLAYBACK AND DISPLAY CHANGING
  // Y Button and Y combo buttons
  if (Xbox.getButtonClick(Y, 0)) {
    if (Xbox.getButtonPress(L1, 0)) {
      Serial.println(F("Button L1 + Y"));
      mp3Trigger.play(8);
      //logic lights, random
      //triggerI2C(10, 0);
    } else if (Xbox.getButtonPress(L2, 0)) {
      Serial.println(F("Button L2 + Y"));
      mp3Trigger.play(8);
      //logic lights, random
      //triggerI2C(10, 0);
    } else if (Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button R1 + Y"));
      mp3Trigger.play(9);
      //logic lights, random
      //triggerI2C(10, 0);
    } else {
      mp3Trigger.play(random(13, 17));
      Serial.println(F("Button Y"));
      //logic lights, random
      //triggerI2C(10, 0);
    }
  }

  // A Button and A combo Buttons
  if (Xbox.getButtonClick(A, 0)) {
    if (Xbox.getButtonPress(L1, 0)) {
      Serial.println(F("Button L1 + A"));
      mp3Trigger.play(6);
      //logic lights
      //triggerI2C(10, 6);
      // HPEvent 11 - SystemFailure - I2C
      //triggerI2C(25, 11);
      //triggerI2C(26, 11);
      //triggerI2C(27, 11);
    } else if (Xbox.getButtonPress(L2, 0)) {
      Serial.println(F("Button L2 + A"));
      mp3Trigger.play(8);
      //logic lights, alarm
      //triggerI2C(10, 1);
      //  HPEvent 3 - alarm - I2C
      //triggerI2C(25, 3);
      //triggerI2C(26, 3);
      //triggerI2C(27, 3);
    } else if (Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button R1 + A"));
      mp3Trigger.play(11);
      //logic lights, alarm2Display
      //triggerI2C(10, 11);
    } else {
      mp3Trigger.play(random(17, 25));
      Serial.println(F("Button A"));
      //logic lights, random
      //triggerI2C(10, 0);
    }
  }

  // B Button and B combo Buttons
  if (Xbox.getButtonClick(B, 0)) {
    if (Xbox.getButtonPress(L1, 0)) {
      Serial.println(F("Button L1 + B"));
      mp3Trigger.play(7);
      //logic lights, random
      //triggerI2C(10, 0);
    } else if (Xbox.getButtonPress(L2, 0)) {
      Serial.println(F("Button L2 + B"));
      mp3Trigger.play(32);
      //logic lights, random
      //triggerI2C(10, 0);
    } else if (Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button R1 + B"));
      mp3Trigger.play(10);
      //logic lights bargrap
      //triggerI2C(10, 10);
      // HPEvent 1 - Disco - I2C
      //triggerI2C(25, 10);
      //triggerI2C(26, 10);
      //triggerI2C(27, 10);
    } else {
      mp3Trigger.play(random(3, 9));
      Serial.println(F("Button B"));
      //logic lights, random
      //triggerI2C(10, 0);
    }
  }

  // X Button and X combo Buttons
  if (Xbox.getButtonClick(X, 0)) {
    // leia message L1+X
    if (Xbox.getButtonPress(L1, 0)) {
      Serial.println(F("Button L1 + X"));
      mp3Trigger.play(5);
      //logic lights, leia message
      //triggerI2C(10, 5);
      // Front HPEvent 1 - HoloMessage - I2C -leia message
      //triggerI2C(25, 9);

    } else if (Xbox.getButtonPress(L2, 0)) {
      Serial.println(F("Button L2 + X"));
      mp3Trigger.play(4);
      //logic lights
      //triggerI2C(10, 4);
    } else if (Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button R1 + X"));
      mp3Trigger.play(12);
      //logic lights, random
      //triggerI2C(10, 0);
    } else {
      mp3Trigger.play(random(9, 22));
      Serial.println(F("Button X"));
      //logic lights, random
      //triggerI2C(10, 0);
    }
  }

  // turn hp light on & off with Right Analog Stick Press (R3) for left stick drive mode
  // turn hp light on & off with Left Analog Stick Press (L3) for right stick drive mode
  if (Xbox.getButtonClick(hpLightToggleButton, 0)) {
    Serial.println(F("Button R3"));
    // if hp light is on, turn it off
    if (isHPOn) {
      isHPOn = false;
      // turn hp light off
      // Front HPEvent 2 - ledOFF - I2C
      triggerI2C(25, 2);
    } else {
      isHPOn = true;
      // turn hp light on
      // Front HPEvent 4 - whiteOn - I2C
      triggerI2C(25, 1);
    }
  }


  // Change drivespeed if drive is enabled
  // Press Left Analog Stick (L3) for left stick drive mode
  // Press Right Analog Stick (R3) for right stick drive mode
  // Set LEDs for speed - 1 LED, Low. 2 LED - Med. 3 LED High
  if (Xbox.getButtonClick(speedSelectButton, 0) && isDriveEnabled) {
    //if in lowest speed
    if (drivespeed == DRIVESPEED1) {
      //change to medium speed and play sound 3-tone
      drivespeed = DRIVESPEED2;
      Xbox.setLedOn(LED2, 0);
      mp3Trigger.play(53);
      //triggerI2C(10, 22);
    } else if (drivespeed == DRIVESPEED2 && (DRIVESPEED3 != 0)) {
      //change to high speed and play sound scream
      drivespeed = DRIVESPEED3;
      Xbox.setLedOn(LED3, 0);
      mp3Trigger.play(1);
      //triggerI2C(10, 23);
    } else {
      //we must be in high speed
      //change to low speed and play sound 2-tone
      drivespeed = DRIVESPEED1;
      Xbox.setLedOn(LED1, 0);
      mp3Trigger.play(52);
      //triggerI2C(10, 21);
    }
  }



  // FOOT DRIVES
  // Xbox 360 analog stick values are signed 16 bit integer value
  // Sabertooth runs at 8 bit signed. -127 to 127 for speed (full speed reverse and  full speed forward)
  // Map the 360 stick values to our min/max current drive speed
  throttleStickValue = (map(Xbox.getAnalogHat(throttleAxis, 0), -32768, 32767, -drivespeed, drivespeed));
  if (throttleStickValue > -DRIVEDEADZONERANGE && throttleStickValue < DRIVEDEADZONERANGE) {
    // stick is in dead zone - don't drive
    driveThrottle = 0;
  } else {
    if (driveThrottle < throttleStickValue) {
      if (throttleStickValue - driveThrottle < (RAMPING + 1)) {
        driveThrottle += RAMPING;
      } else {
        driveThrottle = throttleStickValue;
      }
    } else if (driveThrottle > throttleStickValue) {
      if (driveThrottle - throttleStickValue < (RAMPING + 1)) {
        driveThrottle -= RAMPING;
      } else {
        driveThrottle = throttleStickValue;
      }
    }
  }

  turnThrottle = map(Xbox.getAnalogHat(turnAxis, 0), 32768, -32767, -TURNSPEED, TURNSPEED);

  // DRIVE!
  // right stick (drive)
  if (isDriveEnabled) {
    // Only do deadzone check for turning here. Our Drive throttle speed has some math applied
    // for RAMPING and stuff, so just keep it separate here
    if (turnThrottle > -DRIVEDEADZONERANGE && turnThrottle < DRIVEDEADZONERANGE) {
      // stick is in dead zone - don't turn
      turnThrottle = 0;
    }
    Sabertooth2x.turn(-turnThrottle);
    Sabertooth2x.drive(driveThrottle);
  }

  // DOME DRIVE!
  domeThrottle = (map(Xbox.getAnalogHat(domeAxis, 0), -32768, 32767, DOMESPEED, -DOMESPEED));
  if (domeThrottle > -DOMEDEADZONERANGE && domeThrottle < DOMEDEADZONERANGE) {
    //stick in dead zone - don't spin dome
    domeThrottle = 0;
  }

  Syren10.motor(1, domeThrottle);
}  // END loop()

void triggerI2C(byte deviceID, byte eventID) {
  Wire.beginTransmission(deviceID);
  Wire.write(eventID);
  Wire.endTransmission();
}

void Check_Chatpad() {
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D1, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + 1"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + 1"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + 1"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + 1"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad 1"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D2, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + 2"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + 2"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + 2"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + 2"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad 2"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D3, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + 3"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + 3"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + 3"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + 3"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad 3"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D4, 0)) {
   if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + 4"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + 4"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + 4"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + 4"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad 4"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D5, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + 5"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + 5"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + 5"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + 5"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad 5"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D6, 0)) {
   if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + 6"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + 6"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + 6"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + 6"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad 6"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D7, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + 7"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + 7"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + 7"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + 7"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad 7"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D8, 0)) {
   if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + 8"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + 8"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + 8"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + 8"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad 8"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D9, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + 9"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + 9"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + 9"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + 9"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad 9"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D0, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + 0"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + 0"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + 0"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + 0"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad 0"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_A, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = A"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = ~"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + A"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = á"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad a"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_B, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = B"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = |"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + B"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = +"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad b"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_C, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = C"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = »"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + C"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = Ç"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad c"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_D, 0)) {
     if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = D"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = {"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + D"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = Ç"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad d"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_E, 0)) {
 if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = E"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = €"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + E"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = é"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad e"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_F, 0)) {
     if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = F"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = }"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + F"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = £"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad f"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_G, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = G"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = ¨"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + G"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = ¥"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad g"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_H, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = H"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = /"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + H"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println("Chatpad Modifier Orange + H ");
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad h"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_I, 0)) {
     if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = I"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = *"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + I"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = í"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad i"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_J, 0)) {
     if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = J"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = '"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + J"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = ''"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad j"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_K, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = K"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = ["));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + K"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + K"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad k"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_L, 0)) {
   if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = L"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = ]"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + L"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = Ø"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad l"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_M, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = M"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = >"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + M"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = µ"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad m"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_N, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = N"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = <"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + N"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = Ñ"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad n"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_O, 0)) {
   if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = O"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = ("));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + O"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = ó"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad o"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_P, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = P"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = )"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + P"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = ="));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad p"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_Q, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = Q"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = !"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + Q"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = ¡"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad q"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_R, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = R"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = #"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + R"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = $"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad r"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_S, 0)) {
   if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = S"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + S"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + S"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = ß"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad s"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_T, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = T"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = %"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + T"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = þ"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad t"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_U, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = U"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = &"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + U"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = ú"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad u"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_V, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = V"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = -"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + V"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = _"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad v"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_W, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = W"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = @"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + w"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = å"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad w"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_X, 0)) {
   if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = X"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = «"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + X"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = æ"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad x"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_Y, 0)) {
   if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = Y"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = ^"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + Y"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = ý"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad y"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_Z, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift = Z"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green = `"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + Z"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange = æ"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad z"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_SPACE, 0)) {
   if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + Espace"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + Espace"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + Espace"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + Espace"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad Espace"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_RIGHT, 0)) {
   if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + Button Right"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + Button Right"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + Button Right"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + Button Right"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad Button Right"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_LEFT, 0)) {
   if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + Button Left"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + Button Left"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + Button Left"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + Button Left"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad Button Left"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_PERIOD, 0)) {
   if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + ."));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + ."));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + ."));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + ."));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad ."));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_COMMA, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + ,"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + ,"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + ,"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + ,"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad ,"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_ENTER, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + Enter"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + Enter"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + Enter"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + Enter"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad Enter"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_BACK, 0)) {
    if (Mod_Shift == true) {
      Serial.println(F("Chatpad Modifier Shift + Button Back"));
      Mod_Shift = false;
      //Add your line of code for this function here
    } else if (Mod_Green == true) {
      Serial.println(F("Chatpad Modifier Green + Button Back"));
      Mod_Green = false;
      //Add your line of code for this function here
      } else if (Mod_Messenger == true) {
      Serial.println(F("Chatpad Modifier Messenger + Button Back"));
      Mod_Messenger = false;
      //Add your line of code for this function here
      } else if (Mod_Orange == true) {
      Serial.println(F("Chatpad Modifier Orange + Button Back"));
      Mod_Orange = false;
      //Add your line of code for this function here
    } else {
      Serial.println(F("Chatpad Button Back"));
      //Add your line of code for this function here
    }
  }
  if (Xbox.getChatpadModifierState(MODIFIER_GREENBUTTON, 0)) {
    if (Mod_Green == false) {
      Serial.println(F("Chatpad Button Green Press, Press a button on the chatpad to continue"));
      Mod_Green = true;
      //Press the Green button and any button on the chatpad, you get double the functions
    }
  }
  if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)) {
    if (Mod_Shift == false) {
      Serial.println(F("Chatpad Button Shift, Press a button on the chatpad to continue"));
      Mod_Shift = true;
      //Press the Shift button and any button on the chatpad, you get double the functions
    }
  }
  if (Xbox.getChatpadModifierState(MODIFIER_ORANGEBUTTON, 0)) {
    if (Mod_Orange == false) {
      Serial.println(F("Chatpad Button Orange, Press a button on the chatpad to continue"));
      Mod_Orange = true;
      //Press the Orange button and any button on the chatpad, you get double the functions
    }
  }
  if (Xbox.getChatpadModifierState(MODIFIER_MESSENGER, 0)) {
    if (Mod_Messenger == false) {
      Serial.println(F("Chatpad Button MESSENGER, Press a button on the chatpad to continue"));
      Mod_Messenger = true;
      //Press the Messenger button and any button on the chatpad, you get double the functions
    }
  }
}

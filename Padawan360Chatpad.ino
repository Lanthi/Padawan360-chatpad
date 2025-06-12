// =======================================================================================
// /////////////////////////Padawan360 Body Code - Mega I2C v2.0 /////////////////////////
// =======================================================================================
/*
//                     Base Chatpad with 2 MAESTROS code for R2D2
//                            Coding done By JUSTIN HALL
//                                   Mod by Lanthi

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
  Serial.print(F("\r\nXbox Wireless Receiver Library Started"));
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
      Serial.println(F("Button L1"));
      Serial.println(F("Button R1"));
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
    Serial.println(F("Button UP"));
    // volume up
    if (Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button R1"));
      if (vol > 0) {
        vol--;
        mp3Trigger.setVolume(vol);
      }
    }
  }
  if (Xbox.getButtonClick(DOWN, 0)) {
    Serial.println(F("Button DOWN"));
    //volume down
    if (Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button R1"));
      if (vol < 255) {
        vol++;
        mp3Trigger.setVolume(vol);
      }
    }
  }

  // Logic display brightness.
  // Hold L1 and press up/down on dpad to increase/decrease brightness
  if (Xbox.getButtonClick(UP, 0)) {
    Serial.println(F("Button UP"));
    if (Xbox.getButtonPress(L1, 0)) {
      Serial.println(F("Button L1"));
      triggerI2C(10, 24);
    }
  }
  if (Xbox.getButtonClick(DOWN, 0)) {
    Serial.println(F("Button DOWN"));
    if (Xbox.getButtonPress(L1, 0)) {
      Serial.println(F("Button L1"));
      triggerI2C(10, 25);
    }
  }

#if defined(CHATPAD)
  Check_Chatpad();
#endif

  // GENERAL SOUND PLAYBACK AND DISPLAY CHANGING
  // Y Button and Y combo buttons
  if (Xbox.getButtonClick(Y, 0)) {
    Serial.println(F("Button Y"));
    if (Xbox.getButtonPress(L1, 0)) {
      Serial.println(F("Button L1"));
      mp3Trigger.play(8);
      //logic lights, random
      //triggerI2C(10, 0);
    } else if (Xbox.getButtonPress(L2, 0)) {
      Serial.println(F("Button L2"));
      mp3Trigger.play(8);
      //logic lights, random
      //triggerI2C(10, 0);
    } else if (Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button R1"));
      mp3Trigger.play(9);
      //logic lights, random
      //triggerI2C(10, 0);
    } else {
      mp3Trigger.play(random(13, 17));
      //logic lights, random
      //triggerI2C(10, 0);
    }
  }

  // A Button and A combo Buttons
  if (Xbox.getButtonClick(A, 0)) {
    Serial.println(F("Button A"));
    if (Xbox.getButtonPress(L1, 0)) {
      Serial.println(F("Button L1"));
      mp3Trigger.play(6);
      //logic lights
      //triggerI2C(10, 6);
      // HPEvent 11 - SystemFailure - I2C
      //triggerI2C(25, 11);
      //triggerI2C(26, 11);
      //triggerI2C(27, 11);
    } else if (Xbox.getButtonPress(L2, 0)) {
      Serial.println(F("Button L2"));
      mp3Trigger.play(8);
      //logic lights, alarm
      //triggerI2C(10, 1);
      //  HPEvent 3 - alarm - I2C
      //triggerI2C(25, 3);
      //triggerI2C(26, 3);
      //triggerI2C(27, 3);
    } else if (Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button R1"));
      mp3Trigger.play(11);
      //logic lights, alarm2Display
      //triggerI2C(10, 11);
    } else {
      mp3Trigger.play(random(17, 25));
      //logic lights, random
      //triggerI2C(10, 0);
    }
  }

  // B Button and B combo Buttons
  if (Xbox.getButtonClick(B, 0)) {
    Serial.println(F("Button B"));
    if (Xbox.getButtonPress(L1, 0)) {
      Serial.println(F("Button L1"));
      mp3Trigger.play(7);
      //logic lights, random
      //triggerI2C(10, 0);
    } else if (Xbox.getButtonPress(L2, 0)) {
      Serial.println(F("Button L2"));
      mp3Trigger.play(32);
      //logic lights, random
      //triggerI2C(10, 0);
    } else if (Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button R1"));
      mp3Trigger.play(10);
      //logic lights bargrap
      //triggerI2C(10, 10);
      // HPEvent 1 - Disco - I2C
      //triggerI2C(25, 10);
      //triggerI2C(26, 10);
      //triggerI2C(27, 10);
    } else {
      mp3Trigger.play(random(3, 9));
      //logic lights, random
      //triggerI2C(10, 0);
    }
  }

  // X Button and X combo Buttons
  if (Xbox.getButtonClick(X, 0)) {
    Serial.println(F("Button X"));
    // leia message L1+X
    if (Xbox.getButtonPress(L1, 0)) {
      Serial.println(F("Button L1"));
      mp3Trigger.play(5);
      //logic lights, leia message
      //triggerI2C(10, 5);
      // Front HPEvent 1 - HoloMessage - I2C -leia message
      //triggerI2C(25, 9);

    } else if (Xbox.getButtonPress(L2, 0)) {
      Serial.println(F("Button L2"));
      mp3Trigger.play(4);
      //logic lights
      //triggerI2C(10, 4);
    } else if (Xbox.getButtonPress(R1, 0)) {
      Serial.println(F("Button R1"));
      mp3Trigger.play(12);
      //logic lights, random
      //triggerI2C(10, 0);
    } else {
      mp3Trigger.play(random(9, 22));
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
    Serial.println(F("1"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_D2, 0)) {
    Serial.println(F("2"));
  }


  if (Xbox.getChatpadClick(XBOX_CHATPAD_D3, 0)) {
    Serial.println(F("3"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_D4, 0)) {
    Serial.println(F("4"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_D5, 0)) {
    Serial.println(F("5"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_D6, 0)) {
    Serial.println(F("6"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_D7, 0)) {
    Serial.println(F("7"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_D8, 0)) {
    Serial.println(F("8"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_D9, 0)) {
    Serial.println(F("9"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_D0, 0)) {
    Serial.println(F("0"));
  }


  if (Xbox.getChatpadClick(XBOX_CHATPAD_A, 0)) {
    Serial.println(F("A"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_B, 0)) {
    Serial.println(F("B"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_C, 0)) {
    Serial.println(F("C"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_D, 0)) {
    Serial.println(F("D"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_E, 0)) {
    Serial.println(F("E"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_F, 0)) {
    Serial.println(F("F"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_G, 0)) {
    Serial.println(F("G"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_H, 0)) {
    Serial.println(F("H"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_I, 0)) {
    Serial.println(F("I"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_J, 0)) {
    Serial.println(F("J"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_K, 0)) {
    Serial.println(F("K"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_L, 0)) {
    Serial.println(F("L"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_M, 0)) {
    Serial.println(F("M"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_N, 0)) {
    Serial.println(F("N"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_O, 0)) {
    Serial.println(F("O"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_P, 0)) {
    Serial.println(F("P"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_Q, 0)) {
    Serial.println(F("Q"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_R, 0)) {
    Serial.println(F("R"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_S, 0)) {
    Serial.println(F("S"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_T, 0)) {
    Serial.println(F("T"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_U, 0)) {
    Serial.println(F("U"));
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_V, 0)) {
    Serial.println(F("V"));
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_W, 0)) {
    Serial.println(F("W"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_X, 0)) {
    Serial.println(F("X"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_Y, 0)) {
    Serial.println(F("Y"));
  }

  if (Xbox.getChatpadClick(XBOX_CHATPAD_Z, 0)) {
    Serial.println(F("Z"));
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_SPACE, 0)) {
    Serial.println(F("Espace"));
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_RIGHT, 0)) {
    Serial.println(F("Button Right"));
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_LEFT, 0)) {
    Serial.println(F("Button Left"));
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_PERIOD, 0)) {
    Serial.println(F("."));
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_COMMA, 0)) {
    Serial.println(F(","));
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_ENTER, 0)) {
    Serial.println(F("Enter"));
  }
  if (Xbox.getChatpadClick(XBOX_CHATPAD_BACK, 0)) {
    Serial.println(F("Back"));
  }
  if (Xbox.getChatpadModifierState(MODIFIER_GREENBUTTON, 0)) {
    Serial.println(F("Button Green Press key"));
  }
  if (Xbox.getChatpadModifierState(MODIFIER_SHIFT, 0)) {
    Serial.println(F("Button Shift Press key"));
  }
  if (Xbox.getChatpadModifierState(MODIFIER_ORANGEBUTTON, 0)) {
    Serial.println(F("Button Orange Press key"));
  }
  if (Xbox.getChatpadModifierState(MODIFIER_MESSENGER, 0)) {
    Serial.println(F("Button MESSENGER Press key"));
  }
}

Guys, I present my Padawan360 mod with ChatPad. With all the functional keys and their modifiers.

The keyboard has 43 keys and 4 modifiers, giving you 215 completely independent functions. You can program them to your liking, with commands for servos, lights, motors, etc.

Each key has 5 functions: the normal one with direct pressing, plus 4 modifiers: "Shift, Green Button, Orange Button, and Messager." Press the modifier and then press the key. You don't need to press both at the same time. Press the modifier and then any of the 43 keys on the keyboard.

The "USB Host Shield 2.0" library has been modified to add the chatpad. (There are other libraries on GitHub, but they don't currently work.)

You can use this code for your Arduino project; to do so, add the following lines to your code:

Main code:
```
/************* Xbox360 Chatpad Configuration ******************/
#include <Chatpad.h>
#define CHATPAD //Uncomment if using a chatpad
bool Mod_Shift = false;
bool Mod_Green = false;
bool Mod_Messenger = false;
bool Mod_Orange = false;
/************** End Configuration *******************************/
```
In void loop:
```
#if defined(CHATPAD) 
Check_Chatpad();
#endif
```
And at the end of your code add "void Check_Chatpad()"
```
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
```

Don't forget to add all the libraries to the "Library" folder. Especially "Chatpad-master.zip", "SeqTimer-master.zip", and "USB_Host_Shield_2.0-xbox-chatpad.zip". If you already have them and it asks you to overwrite them, do so. They're modified to avoid errors.
If you have the "USB_Host_Shield_20" library, you should delete it to avoid conflicts.

I hope it's useful to you, and I hope you see your R2D2 working, with thousands of cool movements.
Thanks

  

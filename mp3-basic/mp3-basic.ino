// Arduino code for Kids MP3 player.


#include <SPI.h>
#include <FreeStack.h>

//Add the SdFat Libraries
#include <SdFat.h>

//and the MP3 Shield Library
#include <vs1053_SdFat.h>

// Below is not needed if interrupt driven. Safe to remove if not using.
#if defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_Timer1
  #include <TimerOne.h>
#elif defined(USE_MP3_REFILL_MEANS) && USE_MP3_REFILL_MEANS == USE_MP3_SimpleTimer
  #include <SimpleTimer.h>
#endif

//-----------------------------------------------
// prepare for keypad
#include <Keypad.h>

const byte ROWS = 4; //four rows
const byte COLS = 3; //three columns
char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};
/*
Keypad Pin 1 - col 1 - 10
Keypad Pin 2 - row 0 - 5
Keypad Pin 3 - col 0 - 18
Keypad Pin 4 - row 3 - 17
Keypad Pin 5 - col 2 - 16
Keypad Pin 6 - row 2 - 15
Keypad Pin 7 - row 1 - 14
*/
byte rowPins[ROWS] = {5, 14, 15, 17}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {18, 10, 16}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );
//-------------------------------------------------
  

/**
 * \brief Object instancing the SdFat library.
 *
 * principal object for handling all SdCard functions.
 */
SdFat sd;

/**
 * \brief Object instancing the vs1053 library.
 *
 * principal object for handling all the attributes, members and functions for the library.
 */
vs1053 MP3player;


#define fileNameSize 50
char fileName[fileNameSize];
int dirN = -1;
int fileN = 1;
/**
 * Restart play list
 */
void rewind(){
  fileN = 1;
}

/**
 * Initialize current playing:
 * 1. create file name
 * 2. check file exists
 * 3. if not try the other extension
 * 4. if already tried the other extension restart play list
 */
void get_path(){
  MP3player.stopTrack();
  String fnS = String(dirN);
  fnS += "/";
  fnS += String(fileN);
  fnS += ".mp3";
  fnS.toCharArray(fileName, fileNameSize);
  
  if(!sd.exists(fileName)){//file does not exists
    Serial.println("end of list");
    rewind(); //all extensions checked: restart play list
    
    }
}

//------------------------------------------------------------------------------
/**
 * \brief Setup the Arduino Chip's feature for our use.
 *
 * After Arduino's kernel has booted initialize basic features for this
 * application, such as Serial port and MP3player objects with .begin.
 * Along with displaying the Help Menu.
 *
 * \note returned Error codes are typically passed up from MP3player.
 * Whicn in turns creates and initializes the SdCard objects.
 *
 * \see
 * \ref Error_Codes
 */
void setup() {

  uint8_t result; //result code from some function as to be tested at later time.

  Serial.begin(115200);

  
  //Initialize the SdCard.
  if(!sd.begin(SD_SEL, SPI_FULL_SPEED)) sd.initErrorHalt();
  // depending upon your SdCard environment, SPI_HAVE_SPEED may work better.
  if(!sd.chdir("/")) sd.errorHalt("sd.chdir");

  //Initialize the MP3 Player Shield
  result = MP3player.begin();
  //check result, see readme for error codes.
  if(result != 0) {
    Serial.print(F("Error code: "));
    Serial.print(result);
    Serial.println(F(" when trying to start MP3 player"));
  }
  // enable mono mode
  MP3player.setMonoMode(1);
  Serial.println(F("Mono Enabled."));
    

}

//------------------------------------------------------------------------------
/**
 * \brief Main Loop the Arduino Chip
 *
 * This is called at the end of Arduino kernel's main loop before recycling.
 * And is where the user's serial input of bytes are read and analyzed by
 * parsed_menu.
 *
 * Additionally, if the means of refilling is not interrupt based then the
 * MP3player object is serviced with the availaible function.
 *
 * \note Actual examples of the libraries public functions are implemented in
 * the parse_menu() function.
 */
void loop() {
  char key = keypad.getKey();
  
  if (key){
    Serial.println(key);
    parse_menu(key); // get command from serial input
  }

  
//  delay(10);
}

uint32_t  millis_prv;

//------------------------------------------------------------------------------
/**
 * \brief Decode the Menu.
 *
 * Parses through the characters of the users input, executing corresponding
 * MP3player library functions and features then displaying a brief menu and
 * prompting for next input command.
 */
void parse_menu(byte key_command) {

  uint8_t result; // result code from some function as to be tested at later time.

  Serial.print(F("Received command: "));
  Serial.write(key_command);
  Serial.println(F(" "));

  //if *, stop the current track
  if(key_command == '*') {
    Serial.println(F("Stopping"));
    if (MP3player.isPlaying()) {
        MP3player.stopTrack();
        fileN -= 1;
    }
    else {
      fileN = 0; // there is no file 0, but this will put #1 in the next time the current folder is pressed.
    }

  //if 1-9, play corresponding track
  } else if(key_command >= '0' && key_command <= '9') {
    //convert ascii numbers to real numbers
    key_command = key_command - 48;
    if (MP3player.isPlaying())
        MP3player.stopTrack();
    if (dirN == key_command) {
      fileN++;
    } else {
      dirN = key_command;
      rewind();
    }
  delay(100);
  String fnS = String(dirN);
  fnS += "/";
  fnS += fileN;
  fnS += ".mp3";
  fnS.toCharArray(fileName, fileNameSize);
  delay(10);
  Serial.println(fileName);
  if(!sd.exists(fileName)){//file does not exists
    Serial.println(F("end of list"));
    rewind(); //all extensions checked: restart play list
  } else {
    //tell the MP3 Shield to play a file
    result = MP3player.playMP3(fileName);
    //check result, see readme for error codes.
    if(result != 0) {
      Serial.print(F("Error code: "));
      Serial.print(result);
      Serial.println(F(" when trying to play track"));
    } else {
      Serial.println(F("Playing"));
    }
  }


  } else if(key_command == '#') {
    if( MP3player.getState() == playback) {
      MP3player.pauseMusic();
      Serial.println(F("Pausing"));
    } else if( MP3player.getState() == paused_playback) {
      MP3player.resumeMusic();
      Serial.println(F("Resuming"));
    } else {
      Serial.println(F("Not Playing!"));
    }




  } else if(key_command == 'O') {
    MP3player.end();
    Serial.println(F("VS10xx placed into low power reset mode."));

  } else if(key_command == 'o') {
    MP3player.begin();
    Serial.println(F("VS10xx restored from low power reset mode."));


  // print prompt after key stroke has been processed.
  Serial.print(F("Time since last command: "));  
  Serial.println((float) (millis() -  millis_prv)/1000, 2);  
  millis_prv = millis();
  Serial.print(F("Enter s,1-9,+,-,>,<,f,F,d,i,p,t,S,b"));
#if !defined(__AVR_ATmega32U4__)
  Serial.print(F(",m,e,r,R,g,k,O,o,D,V,B,C,T,E,M:"));
#endif
  Serial.println(F(",h :"));
}
}




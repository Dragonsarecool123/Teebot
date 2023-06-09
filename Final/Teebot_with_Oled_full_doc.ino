

/**************************************************************************
 
This is the code with full documentation for those who would like to modify and reuse it. 
This version will not work with a SSD1306 Oled because of its size limitations. 

see teebot.ino for the Oled compatible version

Written by Divija Durga, with help from the contributions of the open source community. 


 **************************************************************************/
// importing libraries
#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Keypad.h>
#include <Servo.h>

#define SCREEN_WIDTH 128  // OLED display width, in pixels
#define SCREEN_HEIGHT 64  // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);


//initialise servo objects


int pos[4] = { 0, 0, 0, 0 };
Servo servo[4];
const byte servoPins[] = { 9, 10, 11, 12 };
int j = 0;
int timedelay = 1000;      // variable to store delay between servos
int skip = 3;    

char keys[4][4] = {
  { '1', '2', '3', 'A' },
  { '4', '5', '6', 'B' },
  { '7', '8', '9', 'C' },
  { '*', '0', '#', 'D' }
};

byte pin_rows[4] = { 9, 8, 7, 6 };    //connect to the row pinouts of the keypad
byte pin_column[4] = { 5, 4, 3, 2 };  //connect to the column pinouts of the keypad

Keypad keypad = Keypad(makeKeymap(keys), pin_rows, pin_column, 4, 4);

String inputString;
long inputInt;


void setup() {
  Serial.begin(9600);
  inputString.reserve(3);  // maximum number of digit for a number is 3
  //loop to initialise servos
  for (int n = 0; n < 4; n++) { 
    servo[n].attach(servoPins[n]);
  }
  for (int m = 0; m < 4; m++) {
    servo[m].write(0);
  }


  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {

    for (;;)
      ;  // Don't proceed, loop forever
  }
  Serial.println("Success");
  delay(1000);
  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(30, 10);
  // Display static text
  display.println("Teebot\n at your\n Service!");
  display.display();
  delay(3000);
}

void loop() {

  display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 10);
  // Display static text
  display.println("Enter no. of Tshirts\n\n# to enter\n* to void");
  display.display();

  //code for getting input from keypad
  char key = keypad.getKey();

  if (key) {

    if (key >= '0' && key <= '9') {  // only act on numeric keys
      inputString += key;

      display.setCursor(0, 50);
      // Display static text
      display.println(inputString);
      display.display();
      delay(1000);


    } else if (key == '#') {
      if (inputString.length() > 0) {
        inputInt = inputString.toInt();  // typecast to int

        // Display static text
        display.println(inputInt);
        display.display();
        inputString = "";
        delay(1000);  // clear input
      }
    } else if (key == '*') {
      inputString = "";  // clear input
    }
  }

  for (int servocount = 0; servocount < inputInt; servocount += 1) {
    Serial.println(servocount);
    display.clearDisplay();
    display.setCursor(0, 30);
    display.println("Lay the Tshirt");
    display.display();

    delay(5000);
    display.clearDisplay();
    display.println("Folding...");
    display.display();
    for (int j = 0; j < 4; j++) {
      for (pos[j]; pos[j] <= 150; pos[j]++) {  // goes from 0 degrees to 150 degrees
        // in steps of 1 degree
        servo[j].write(pos[j]);
        delay(skip);
      }
      for (pos[j] = 150; pos[j] >= 0; pos[j] -= 1) {  // goes from 150 degrees to 0 degrees
        servo[j].write(pos[j]);
        delay(skip);
      }
      delay(timedelay);
      Serial.println(j);
    }
  }
  display.clearDisplay();
  key = 0;
  inputInt = 0;
}

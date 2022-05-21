//-------------------TEAM ID = T47----------------//
//----------------TEAM NAME = Operam-------------------//


const int buzzer =8; //initializing buzzer

#include <Servo.h>
Servo myservo;    // Include Servo Motor library for I2C
#include <Wire.h>   // Include Arduino Wire library for I2C
#include <LiquidCrystal.h>    // Include LCD display library for I2C
#include <Keypad.h>       // Include Keypad library


#define LED 13    //IMAGENCY LED pin connection
#define LED1 12   //INVALID ID LED pin connection
#define Password_Length 8   // Length of password + 1 for * character
#define ID_Length 6     // Length of ID + 1 for * character


char Data[Password_Length];   // Character to hold id input
char Datav[ID_Length];      //Character to holdpassword input

// ID and Password
char Master1[Password_Length] = "145278*"; // 1st ID
char Master2[Password_Length] = "354691*"; // 2nd ID
char Master3[Password_Length] = "789541*"; // 3rd ID

char Master_r1[ID_Length] = "#001*";  // 1st password
char Master_r2[ID_Length] = "#002*";  // 2nd password
char Master_r3[ID_Length] = "#003*";  // 3rd password

int lockOutput = 2;   // Pin connected to lock relay input
int pos=0;    // position of servo motor
int Button =10;   //kill button pin

// Counter for character entries
byte data_count = 0;
byte data_countv = 0;

char customID;    // Character to hold ID input
char customKey;   // Character to hold Password input

const byte ROWS = 4;    // Constants for row sizes
const byte COLS = 3;    // Constants for column sizes

char hexaKeys[ROWS][COLS] = {   // Array to represent keys on keypad
   {'1','2','3'},
   {'4','5','6'},
   {'7','8','9'},
   {'*','0','#'}
};

byte rowPins[ROWS] = {0,1,2,3};   //rowPins Connections to Arduino
byte colPins[COLS] = {4,5,6};     //colPins Connections to Arduino

Keypad customKeypad = Keypad(makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS); // Create keypad object
LiquidCrystal lcd(A4, A5, A3, A2, A1, A0);    // Create LCD object

int c1=0; //counter for ID
int c2=0; //Counter for password
int counter=0; //Counter for Device lock


void setup() {
  pinMode(LED,OUTPUT);    //Door open LED
  pinMode(LED1,OUTPUT);   //Door open LED
  pinMode(lockOutput, OUTPUT);    // Set lockOutput as an OUTPUT pin
  myservo.attach(9);    //Servo motor connection
  pinMode(Button, INPUT);   //kill button
  displayscreen();      // Initialize LCD and print
  pinMode(buzzer,OUTPUT); //set buzzer
}

    void loop() {
           
           
           
            customID= customKeypad.getKey();    // Look for keypress to ID
      if (customID) {     // Enter keypress into array and increment counter
        Datav[data_countv] = customID;
        lcd.setCursor(data_countv+3,0);   //ID display position
        lcd.print(Datav[data_countv]);
        data_countv++;
       }
                              if (data_countv == ID_Length-1) {// See if we have reached the ID length
                                customKey= customKeypad.getKey();
                                lcd.clear();
                                if (!strcmp(Datav, Master_r1)) // ID is correct 
                                { 
                                  customKey= customKeypad.getKey();
                                  lcd.clear();
                                  lcd.setCursor(0,0);
                                  lcd.print("Enter passwrd:");
                                  delay(1000);
                                  c1=0;
                                  customKey = customKeypad.getKey();   // Look for keypress
                                  if (customKey)    // Enter keypress into array and increment counter
                                  {                  
                                     Data[data_count] = customKey;   //PASSWORD display position
                                     lcd.setCursor(data_count+3,0); //Display Pasword in * Characters
                                     lcd.print("*");
                                     data_count++;
                                  }
                                  if (data_count == Password_Length-1) {
                                  lcd.clear();
                                      if (!strcmp(Data,Master1))      // Password is correct
                                      {
                                      lcd.print("UNLOCKED");
                                      c2=0;             //remove door freezing
                                      unlockdoor();      //Unlocked Door for 5 seconds
                                      delay(5000);
                                      }
                                      else {     // Password is incorrect
                                        lcd.setCursor(0,0);
                                          lcd.print("Incorrect Password");
                                          lcd.setCursor(0,1);
                                          lcd.print("Enter password again");
                                          delay(1000);
                                          c2=c2+1;
                                  }}}
                                else {      // ID is incorrect
                                  lcd.print("The wrong ID!!!");
                                  lcd.setCursor(0,1);
                                  lcd.print("Enter ID again");
                                  delay(1000);
                                  c1=c1+1; //Checking for freeced
                                }
                             // Clear data and LCD display
                                lcd.clear();
                                clearData();
                                clearDatav();
                               }
       while (digitalRead(Button)== HIGH)     //Emagency button pressed
       {
        emaunlockdoor();      //Emagency Locking
       }
    
        if(c1>=3||c2>=3){     //if Id entered 3 times or PW entered 3 times
        
        while(counter<=30){        //For 30 minutes
        lcd.setCursor(0,0);
        lcd.print("Door Locked:");    
        lcd.setCursor(14, 1);
        digitalWrite(LED1,HIGH);      //Turn on the LED
        lcd.print(counter);     //Printing counter
       
        delay(1000);      //Indicste 1 second
        counter++;
        lcd.clear();
        }
         digitalWrite(LED1,LOW);      //Turn off the LED
       // goto theBigin;
        }}



//--------------------------Clear Data-----------------------//
void clearDatav() {
  // Go through array and clear data
  while (data_countv != 0) {
    Datav[data_countv--] = 0;
  }
  return;
}
void clearData() {
  // Go through array and clear data
  while (data_count != 0) {
    Data[data_count--] = 0;
  }
  return;
}

//----------------------DISPLAY SCREEN-----------------------
void displayscreen()
{
lcd.clear();
delay(1000);
lcd.setCursor(2,0);
lcd.println("!!!WOLCOME!!!");
delay(2000);
lcd.clear();
lcd.clear();
lcd.setCursor(0,0);
lcd.println("ENTER THE ID and");
delay(900);
lcd.setCursor(0,2);
lcd.println("Press * to cnf.");
delay(2000);
lcd.clear();

}
//------------------------UNLOCKED DOOR-----------------
void unlockdoor()
{
delay(900);
 
lcd.setCursor(0,0);
lcd.println(" ");
lcd.setCursor(1,0);
lcd.print("EMAGENCY!");
lcd.setCursor(4,1);
lcd.println("WELCOME!!");
lcd.setCursor(15,1);
lcd.println(" ");
lcd.setCursor(16,1);
lcd.println(" ");
lcd.setCursor(14,1);
lcd.println(" ");
lcd.setCursor(13,1);
lcd.println(" ");
digitalWrite(LED,HIGH);      //Turn ON the Door on LED
for(pos = 180; pos>=0; pos-=5) // open the door
{
myservo.write(pos); 
delay(5); 
}
delay(2000);
for(pos = 0; pos <= 180; pos +=5) // close the door
{ // in steps of 1 degree
myservo.write(pos); 
delay(15);
delay(2000);
digitalWrite(LED,LOW);      //Turn off the Door on LED
lcd.clear();
}
digitalWrite(LED,LOW);
}

//------------------------Emagency UNLOCKED DOOR-----------------
void emaunlockdoor()
{
delay(900);
lcd.setCursor(0,0);
lcd.print("EMAGENCY!");
digitalWrite(LED,HIGH);             //Turn ON the Emagency LED
tone(buzzer, 1000); //Send 1KHz sound signal
for(pos = 180; pos>=0; pos-=5) // open the door
{
myservo.write(pos); 
delay(5); 
}
delay(2000);
for(pos = 0; pos <= 180; pos +=5) // close the door
{ 
myservo.write(pos); 
delay(15);
delay(2000);
digitalWrite(LED,LOW);
noTone(buzzer);
lcd.clear();
}
digitalWrite(LED,LOW);
lcd.clear();
}

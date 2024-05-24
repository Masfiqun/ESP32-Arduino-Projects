

// #include <LiquidCrystal.h>  //Header file for LCD from https://www.arduino.cc/en/Reference/LiquidCrystal
#include  <Keypad.h> //Header file for Keypad from https://github.com/Chris--A/Keypad

const  byte ROWS = 4; // Four rows
const byte COLS = 4; // Three columns

// Define  the Keymap
char keys[ROWS][COLS] = {

  {'1','2','3','A'},

  {'4','5','6','B'},

  {'7','8','9','C'},

  {'*','0','#','D'}

};


const int segmentPin[2][7] = {
  {7, 8, 9, 10, 11, 12, 13},
  {23, 22, 2, 3, 4, 5, 6,}
  
};
const int number[11][7] = {
  {1, 0, 0, 0, 0, 0, 0}, // 0
  {1, 1, 1, 0, 0, 1, 1}, // 1
  {0, 1, 0, 0, 1, 0, 0}, // 2
  {0, 1, 0, 0, 0, 0, 1}, // 3
  {0, 0, 1, 0, 0, 1, 1}, // 4
  {0, 0, 0, 1, 0, 0, 1}, // 5
  {0, 0, 0, 1, 0, 0, 0}, // 6
  {1, 1, 0, 0, 0, 1, 1}, // 7
  {0, 0, 0, 0, 0, 0, 0}, // 8
  {0, 0, 0, 0, 0, 0, 1},  // 9
  {0, 1, 1, 1, 1, 1, 1}  // -
};

int num1;
int num2;

byte rowPins[ROWS]  = { 24, 26, 28, 30 };// Connect keypad ROW0, ROW1, ROW2 and ROW3 to these Arduino pins.
byte  colPins[COLS] = { 32, 34, 36, 38 }; // Connect keypad COL0, COL1 and COL2 to these Arduino  pins.

Keypad kpd = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS  ); //  Create the Keypad

const int rs = 8, en = 9, d4 = 10, d5 = 11, d6 =  12, d7 = 13; //Pins to which LCD is connected
// LiquidCrystal lcd(rs, en, d4, d5,  d6, d7);

 long Num1,Num2,Number;
 char key,action;
 boolean result  = false;
 
void setup() {

  Serial.begin(9600);
  
  // Set segment pins as output
  for (int i = 0; i < 2; i++) {
    for (int j = 0; j < 7; j++) {
      pinMode(segmentPin[i][j], OUTPUT);
    }
  
}
}

void loop() {
  
key = kpd.getKey();  //storing pressed key value in a char

if (key!=NO_KEY)
DetectButtons();

if  (result==true)
CalculateResult();

// DisplayResult(); 

if(Number < 100){
    num1= Number / 10;
    num2= Number % 10;
    DisplayResult(num1,0);
    DisplayResult(num2,1);
}else{
  DisplayResult(10,0);
  DisplayResult(10,1);
}
    Serial.print("Number: ");
    Serial.print(Number);
    Serial.print("  ---> ");
    Serial.print("Num 1 d: ");
    Serial.print(num1);
    Serial.print(" ");
    Serial.print("Num 2 m: ");
    Serial.println(num2);

}

void  DetectButtons()
{ 
    //  lcd.clear(); //Then clean it
    if (key=='*')  //If cancel Button is pressed
    {Serial.println ("Button Cancel"); Number=Num1=Num2=0;  result=false;}
    
     if (key == '1') //If Button 1 is pressed
    {Serial.println  ("Button 1"); 
    if (Number==0)
    Number=1;
    else
    Number  = (Number*10) + 1; //Pressed twice
    }
    
     if (key == '4') //If  Button 4 is pressed
    {Serial.println ("Button 4"); 
    if (Number==0)
    Number=4;
    else
    Number = (Number*10) + 4; //Pressed twice
    }
    
     if (key == '7') //If Button 7 is pressed
    {Serial.println ("Button  7");
    if (Number==0)
    Number=7;
    else
    Number = (Number*10)  + 7; //Pressed twice
    } 
  

    if (key == '0')
    {Serial.println  ("Button 0"); //Button 0 is Pressed
    if (Number==0)
    Number=0;
    else
    Number = (Number*10) + 0; //Pressed twice
    }
    
     if  (key == '2') //Button 2 is Pressed
    {Serial.println ("Button 2"); 
     if  (Number==0)
    Number=2;
    else
    Number = (Number*10) + 2; //Pressed  twice
    }
    
     if (key == '5')
    {Serial.println ("Button  5"); 
     if (Number==0)
    Number=5;
    else
    Number = (Number*10)  + 5; //Pressed twice
    }
    
     if (key == '8')
    {Serial.println  ("Button 8"); 
     if (Number==0)
    Number=8;
    else
    Number  = (Number*10) + 8; //Pressed twice
    }   
  

    if (key == '#')
    {Serial.println ("Button Equal"); 
    Num2=Number;
    result = true;
    }
    
     if (key == '3')
    {Serial.println ("Button 3"); 
     if (Number==0)
    Number=3;
    else
    Number = (Number*10) + 3;  //Pressed twice
    }
    
     if (key == '6')
    {Serial.println  ("Button 6"); 
    if (Number==0)
    Number=6;
    else
    Number  = (Number*10) + 6; //Pressed twice
    }
    
     if (key == '9')
    {Serial.println ("Button 9");
    if (Number==0)
    Number=9;
    else
    Number = (Number*10) + 9; //Pressed twice
    }  

      if (key ==  'A' || key == 'B' || key == 'C' || key == 'D') //Detecting Buttons on Column 4
  {
    Num1 = Number;    
    Number =0;
    if (key == 'A')
    {Serial.println  ("Addition"); action = '+';}
     if (key == 'B')
    {Serial.println ("Subtraction");  action = '-'; }
     if (key == 'C')
    {Serial.println ("Multiplication");  action = '*';}
     if (key == 'D')
    {Serial.println ("Devesion"); action  = '/';}  

    delay(100);
  }
  
}

void CalculateResult()
{
  if (action=='+')
    Number = Num1+Num2;

  if (action=='-')
    Number  = Num1-Num2;

  if (action=='*')
    Number = Num1*Num2;

  if (action=='/')
    Number = Num1/Num2; 
}

void DisplayResult(int num, int displayNum)
{
  // // Serial.setCursor(0,  0);   // set the cursor to column 0, line 1
  // Serial.print(Num1); Serial.print(action);  Serial.print(Num2); 
  
  // if (result==true)
  // {Serial.print(" ="); Serial.println(Number);}  //Display the result

  for (int i = 0; i < 7; i++) {
    digitalWrite(segmentPin[displayNum][i], number[num][i]);
    // Serial.print(number[num][i]);
  }
  Serial.println();
  
  
}

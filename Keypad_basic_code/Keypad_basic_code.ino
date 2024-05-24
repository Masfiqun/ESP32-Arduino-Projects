//YouTube | Tech at Home

#include <Keypad.h>

const char number_of_rows = 4;
const char number_of_columns = 4;

char row_pins[number_of_rows] = {24, 26, 28, 30};
char column_pins[number_of_columns] = {32, 34, 36, 38};

char key_array[number_of_rows][number_of_columns] = {  
  {'1', 2, 3, 'A'},
  {4, 5, 6, 0},
  {7, 8, 9, 0},
  {0, 0, 0, 0}
  };

Keypad k = Keypad(makeKeymap(key_array),row_pins , column_pins, number_of_rows, number_of_columns);

void setup()
{
  Serial.begin(9600);
}

void loop()
{
  char key_pressed = k.getKey();
  if(key_pressed)
  {
    // (int)key_pressed;
    Serial.println((int)key_pressed-48);
    // Serial.println(typeid(key_pressed).name());
  }
}
  

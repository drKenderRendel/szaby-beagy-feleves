#include <Adafruit_LiquidCrystal.h>
#include <Keypad.h>

// --- LCD SETUP ---
Adafruit_LiquidCrystal lcd_1(0);

// --- KEYPAD SETUP ---
const byte ROWS = 4; // Four rows
const byte COLS = 4; // Four columns

// Define the symbols on the buttons of the keypads
char hexaKeys[ROWS][COLS] = {
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'*','0','#','D'}
};

// Connect to the row pinouts of the keypad
byte rowPins[ROWS] = {9, 8, 7, 6}; 
// Connect to the column pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; 

// Initialize an instance of class NewKeypad
Keypad customKeypad = Keypad( makeKeymap(hexaKeys), rowPins, colPins, ROWS, COLS);
int pos = 0;
char keys[4] = {'0','0','0','0'};
char code[4] = {'F','F','F','F'};

bool setCode() {
  lcd_1.setCursor(0, 0);
  lcd_1.print("Current code:");
  
    while (pos < 4) {
        lcd_1.setCursor(pos, 1);

        // Read the key
        char customKey = customKeypad.getKey();

        if (customKey && customKey != '#' && customKey != '*'){
        lcd_1.setCursor(pos, 1); // Move to second line
        lcd_1.print(customKey); // Print the character pressed
        code[pos] = customKey;
        pos = pos + 1;
        }
        else if (customKey == '*') {
        if (pos != 0) {
            pos = pos - 1;
        }
        lcd_1.setCursor(pos, 1);
        lcd_1.print(' ');
        }
    }

    if (checkCode()) {
        lcd_1.clear();
        pos = 0;
        lcd_1.setCursor(0, 0);
        lcd_1.print("Enter new code:");

        while (pos < 4) {
            lcd_1.setCursor(pos, 1);

            // Read the key
            char customKey = customKeypad.getKey();

            if (customKey && customKey != '#' && customKey != '*') {
                lcd_1.setCursor(pos, 1); // Move to second line
                lcd_1.print(customKey); // Print the character pressed
                code[pos] = customKey;
                pos = pos + 1;
            }
            else if (customKey == '*') {
                if (pos != 0) {
                    pos = pos - 1;
                }
                lcd_1.setCursor(pos, 1);
                lcd_1.print(' ');
            }
        }

        for (int i = 0; i < 4; i++) {
            keys[i] = code[i];
        }

        return true;
    }
    
    return false;
}
  
void setup()
{
  Serial.begin(9600);
  lcd_1.begin(16, 2);
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(A0, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);

  lcd_1.print("Initialized");
  lcd_1.setCursor(0, 1);
  lcd_1.print("successfully!");
  delay(1000); // Wait for 1000 millisecond(s)
  lcd_1.clear();
}

bool checkCode()
{
  int idx = 0;
  while (idx < 4 && code[idx] == keys[idx]) {
    idx++;
  }
  return idx == 4;
}

void loop()
{
  lcd_1.setCursor(0, 0);
  lcd_1.print("Code:");
  
  // Read the key
  char customKey = customKeypad.getKey();
  
  // If a key was pressed (customKey is not NO_KEY)
  if (customKey && customKey == '#' && pos == 0) {
    if (setCode()) {
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Key changed");
        lcd_1.setCursor(0, 1);
        lcd_1.print("successfully!");

        digitalWrite(A0, HIGH);
    }
    else {
        lcd_1.clear();
        lcd_1.setCursor(0, 0);
        lcd_1.print("Incorrect code!");
    }
    delay(2500);
    digitalWrite(A0, LOW);
    pos = 0;
    lcd_1.clear();
  }
  else if (customKey && customKey != '#' && customKey != '*' && pos < 4){
    lcd_1.setCursor(pos, 1); // Move to second line
    lcd_1.print(customKey); // Print the character pressed
    code[pos] = customKey;
    pos = pos + 1;
  }
  else if (customKey == '*') {
    pos = pos - 1;
    lcd_1.setCursor(pos, 1);
    lcd_1.print(' ');
  }
  else if (pos == 4) {
    lcd_1.clear();
    
    if (checkCode()) {
      lcd_1.print("Correct!");
      digitalWrite(A0, HIGH);
    }
    else {
      lcd_1.print("Incorrect!");
    }
    
    delay(2000);
    digitalWrite(A0, LOW);
    for (int i = 0; i < 4; i++) {
    	code[i] = 'F';
  	}
    pos = 0;
    
	lcd_1.clear();
  }
}
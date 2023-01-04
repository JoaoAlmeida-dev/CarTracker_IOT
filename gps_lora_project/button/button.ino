// constants won't change. They're used here to set pin numbers:
const byte buttonPin = 2; // the number of the pushbutton pin
const byte buzzpin = 13;  // the number of the LED pin

// variables will change:
byte newButton = 0; // variable for reading the pushbutton status
byte oldButton = 0;
byte ledState = 0;

void setup()
{
    // initialize the LED pin as an output:
    // pinMode(buzzpin, OUTPUT);
    // initialize the pushbutton pin as an input:
    pinMode(buttonPin, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("setup done");
}

void loop()
{
    // read the state of the pushbutton value:
    newButton = digitalRead(buttonPin);

    Serial.print("newButton: ");
    Serial.print(newButton);
    Serial.print(" ;");
   
    Serial.print("oldButton: ");
    Serial.print(oldButton);
    Serial.print(" ;");

    Serial.print("ledState: ");
    Serial.println(ledState);

    // check if the pushbutton is pressed. If it is, the buttonState is HIGH:
    if (oldButton == 0 && newButton == 1)
    {
        if (ledState == 0)
        {
            // ON
            //tone(buzzpin, 2750);
            Serial.println("turned on");
            digitalWrite(LED_BUILTIN, HIGH);
            ledState = 1;
        }
        else
        {
            // OFF
            Serial.println("turned off");
            digitalWrite(LED_BUILTIN, LOW);
            ledState = 0;
        }
    }
    oldButton = newButton;
   

    delay(200);
}
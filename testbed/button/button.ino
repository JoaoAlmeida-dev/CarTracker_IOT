// constants won't change. They're used here to set pin numbers:
constexpr uint8_t BUTTON_PIN = 2; // the number of the pushbutton pin
constexpr uint8_t BUZZ_PIN = 13;  // the number of the buzzer pin

// variables will change:
uint8_t newButton = 0; // variable for reading the pushbutton status
uint8_t oldButton = 0;
uint8_t ledState = 0;

void setup()
{
    // initialize the LED pin as an output:
    // pinMode(buzzpin, OUTPUT);
    // initialize the pushbutton pin as an input:
    pinMode(BUTTON_PIN, INPUT_PULLUP);
    pinMode(LED_BUILTIN, OUTPUT);
    digitalWrite(LED_BUILTIN, LOW);
    Serial.print("Button setup done");
}

void loop()
{
    // read the state of the pushbutton value:
    newButton = digitalRead(BUTTON_PIN);

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
            Serial.println("BUTTON turned on");
            digitalWrite(LED_BUILTIN, HIGH);
            ledState = 1;
        }
        else
        {
            // OFF
            Serial.println("BUTTON turned off");
            digitalWrite(LED_BUILTIN, LOW);
            ledState = 0;
        }
    }
    oldButton = newButton;
   

    delay(200);
}
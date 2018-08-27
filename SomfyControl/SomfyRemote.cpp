#include "Arduino.h"
#include "SomfyRemote.h"

#define LED pins[0]
#define BUTTON_1 pins[1]
#define BUTTON_2 pins[2]
#define BUTTON_3 pins[3]
#define BUTTON_4 pins[4]

SomfyRemote::SomfyRemote(int button_1, int button_2, int button_3, int button_4, int led_pin)
{
    int pins_array[5] = {led_pin, button_1, button_2, button_3, button_4};
    for (int i = 0; i < 5; i++)
    {
        pins[i] = pins_array[i];
    }

    // init pins
    pinMode(led_pin, INPUT);
    for (int i = 1; i < 5; i++)
    {
        pinMode(pins[i], OUTPUT);
        if (i == 2)
            digitalWrite(pins[2], HIGH);
    }
}

// use default pins
SomfyRemote::SomfyRemote()
{
    // init pins
    pinMode(pins[0], INPUT);
    for (int i = 1; i < 5; i++)
    {
        pinMode(pins[i], OUTPUT);
        if (i == 2)
            digitalWrite(pins[2], HIGH);
    }
}

bool SomfyRemote::clickButton(int button_index)
{
    if (button_index >= 1 && button_index <= 4)
    {
        int button = pins[button_index];
        if (button == BUTTON_2)
        {
            digitalWrite(button, LOW);
            delay(500);
            digitalWrite(button, HIGH);
        }
        else
        {
            digitalWrite(button, HIGH);
            delay(500);
            digitalWrite(button, LOW);
        }

        return true;
    }

    return false;
}

// check if the led on the remote is blinking
bool SomfyRemote::ledIsBlinking()
{
    int start = millis();

    while ((millis() - start) <= 100)
    {
        if (analogRead(LED) > 50)
            return true;
    }
    
    return false;
}
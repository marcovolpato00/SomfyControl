#ifndef somfyremote_h
#define somfyremote_h

class SomfyRemote
{
    // default pins
    int pins[5] = {A0, 5, 4, 12, 14};

    public:
        SomfyRemote(int button_1, int button_2, int button_3, int button_4, int led_pin);
        SomfyRemote();
        bool clickButton(int button_index);
        bool ledIsBlinking();
};

#endif
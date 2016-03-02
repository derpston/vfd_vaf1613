#include "vfd_vaf1613.h"

VFD_VAF1613::VFD_VAF1613(uint8_t _clock_pin, uint8_t _data_pin, uint8_t _reset_pin)
{
    clock_pin = _clock_pin;
    data_pin  = _data_pin;
    reset_pin = _reset_pin;

    init();
}

void VFD_VAF1613::init()
{
    pinMode(clock_pin, OUTPUT);
    pinMode(data_pin, OUTPUT);
    pinMode(reset_pin, OUTPUT);

    power_off();
}

void VFD_VAF1613::power_off()
{
    // Holding reset low keeps the module in low power mode. (~19-22mA)
    digitalWrite(reset_pin, LOW);
}

void VFD_VAF1613::power_on()
{
    digitalWrite(reset_pin, HIGH);
    delay(10);
    brightness(BRIGHTNESS_MAX);
    position(POSITION_MIN);
}


void VFD_VAF1613::reset(uint8_t us)
{
    digitalWrite(reset_pin, LOW);
    digitalWrite(data_pin, LOW);
    delayMicroseconds(50);
    digitalWrite(reset_pin, HIGH);
    delayMicroseconds(50);
    digitalWrite(data_pin, HIGH);
}

void VFD_VAF1613::short_reset()
{
    reset(50);
}

void VFD_VAF1613::long_reset()
{
    reset(2000);
}

void VFD_VAF1613::write_byte(uint8_t byte)
{
    for(int8_t index = 7; index >= 0; index--)
    {
        digitalWrite(clock_pin, HIGH);
        digitalWrite(data_pin, (byte >> index) & 1);
        delayMicroseconds(2);
        digitalWrite(clock_pin, LOW); // Bit is read on falling edge.
        delayMicroseconds(2);
    }
    delayMicroseconds(50);
}

void VFD_VAF1613::brightness(uint8_t brightness)
{
    if(brightness > BRIGHTNESS_MAX)
        brightness = BRIGHTNESS_MAX;

    write_byte(VFD_DUTY_CYCLE + brightness);
}

void VFD_VAF1613::position(uint8_t position)
{
    if(position > POSITION_MAX)
        position = POSITION_MAX;

    /*
    * Deal with an unusual off by one configuration that the display
    * addressing seems to use, where 0xaf is the leftmost character,
    * 0xa0 is the second from left, 0xa1 the third from left, etc.
    *
    * For the full 16 position display, the positions and indices are:
    * [F0123456789ABCDE]
    *
    * I have no idea why, but this is how this particular display seems
    * to work.
    */
    position = (position + 15) % 16;

    write_byte(VFD_POSITION + position);
}

void VFD_VAF1613::write(char *message)
{
    for(uint8_t i = 0; i < strlen(message); i++)
        write_byte(lookup(message[i]));
}

uint8_t VFD_VAF1613::lookup(uint8_t chr)
{
    // Get a pointer to where chr appears in LUT.
    uint8_t *chrptr = (uint8_t*) strchr((const char*) LUT, chr);

    if(chrptr == NULL)
        // Not found, return the value for a question mark.
        return VFD_QUESTION_MARK;
    else
        // Use pointer math to get the index of chr in LUT.
        return chrptr - LUT;
}

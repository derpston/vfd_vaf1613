#ifndef VFD_H
#define VFD_H

#include <Arduino.h>

#define POSITION_MIN 0
#define POSITION_MAX 15

#define BRIGHTNESS_MIN 0
#define BRIGHTNESS_MAX 31

#define VFD_DUTY_CYCLE 0xe0
#define VFD_POSITION 0xa0
#define VFD_QUESTION_MARK 0x3f

class VFD_VAF1613
{
    /* Lookup table from vaf1613-kv12cs datasheet */
    const uint8_t *LUT = (const uint8_t*) "@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_ !\"#$%&'<>*+,-./0123456789:;{=}?";

    uint8_t clock_pin;
    uint8_t data_pin;
    uint8_t reset_pin;

    private:
        uint8_t lookup(uint8_t);
        void init();
        void reset(uint8_t);
    public:
        VFD_VAF1613(uint8_t, uint8_t, uint8_t);
        void write(char *);
        void write_byte(uint8_t);
        void power_off();
        void power_on();
        void long_reset();
        void short_reset();
        void brightness(uint8_t);
        void position(uint8_t);
};

#endif

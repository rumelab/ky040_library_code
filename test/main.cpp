#include <Arduino.h>
#include <ky040.hpp>

constexpr const PinName clk_pin = PinName::p13;
constexpr const PinName dt_pin = PinName::p12;
constexpr const PinName sw_pin = PinName::p11;
KY040<clk_pin, dt_pin, sw_pin> input_device;

void setup()
{
    delay(2000);
    Serial.begin(9600);
    input_device.begin();
    while (!Serial)
    {
    }
}

void loop()
{
    input_device.process();
    const int16_t rotation = input_device.get_rotation();
    const bool pressed = input_device.is_pressed();

    if (input_device.is_just_pressed())
    {
        Serial.print("Just pressed!");
        return;
    }

    if (input_device.is_just_released())
    {
        Serial.print("Just released!");
        return;
    }

    Serial.print("Rotation: ");
    Serial.print(rotation);
    Serial.print("; Pressed: ");
    Serial.print(pressed ? "true" : "false");
    Serial.print("\n");
}
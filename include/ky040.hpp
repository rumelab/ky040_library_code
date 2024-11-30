#ifndef KY040_HPP
#define KY040_HPP

#include <Arduino.h>

namespace RumeLab::Detail
{

    template <PinName PCLKPin, PinName PDTPin, PinName PSWPin>
    class KY040
    {
    public:
        static constexpr const PinName CLK_PIN = PCLKPin;
        static constexpr const PinName DT_PIN = PDTPin;
        static constexpr const PinName SW_PIN = PSWPin;

    private:
        PinStatus previous_clk_pin_state;
        int16_t rotation;
        int8_t last_step;
        bool pressed;
        bool last_pressed;

        auto process_rotation() -> void
        {
            const PinStatus current_clk_pin_state = digitalRead(CLK_PIN);
            const bool is_rotated = current_clk_pin_state != previous_clk_pin_state;
            previous_clk_pin_state = current_clk_pin_state;

            if (!is_rotated)
            {
                last_step = 0;
                return;
            }

            const bool is_clockwise_rotation = digitalRead(DT_PIN) != current_clk_pin_state;
            if (is_clockwise_rotation)
            {
                ++rotation;
                last_step = 1;
                return;
            }

            --rotation;
            last_step = -1;
        }

        auto process_pressed() -> void
        {
            last_pressed = pressed;
            pressed = digitalRead(SW_PIN) == LOW;
        }

    public:
        KY040()
            : previous_clk_pin_state(PinStatus::LOW) {}

        auto begin() -> void
        {
            pinMode(CLK_PIN, INPUT);
            pinMode(DT_PIN, INPUT);
            pinMode(SW_PIN, INPUT_PULLUP);

            reset_rotation();
        }

        auto process() -> void
        {
            process_rotation();
            process_pressed();
        }

        auto reset_rotation() -> void
        {
            rotation = 0;
        }

        auto get_rotation() const -> int16_t
        {
            return rotation;
        }

        auto get_last_step() const -> int8_t
        {
            return last_step;
        }

        auto is_pressed() const -> bool
        {
            return pressed;
        }

        auto is_just_pressed() const -> bool
        {
            return pressed && !last_pressed;
        }

        auto is_just_released() const -> bool
        {
            return !pressed && last_pressed;
        }
    };

} // namespace RumeLab::Detail

template <PinName PCLKPin, PinName PDTPin, PinName PSWPin>
using KY040 = RumeLab::Detail::KY040<PCLKPin, PDTPin, PSWPin>;

#endif // KY040_HPP
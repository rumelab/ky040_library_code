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

    public:
        KY040()
            : previous_clk_pin_state(PinStatus::LOW) {}

        auto begin() -> void
        {
            pinMode(CLK_PIN, INPUT);
            pinMode(DT_PIN, INPUT);
            pinMode(SW_PIN, INPUT_PULLUP);
        }

        auto process() -> int8_t
        {
            const PinStatus current_clk_pin_state = digitalRead(CLK_PIN);
            const bool is_rotated = current_clk_pin_state != previous_clk_pin_state;
            previous_clk_pin_state = current_clk_pin_state;

            if (!is_rotated)
                return 0;

            const bool is_clockwise_rotation = digitalRead(DT_PIN) != current_clk_pin_state;
            if (is_clockwise_rotation)
            {
                ++rotation;
                return 1;
            }

            --rotation;
            return -1;
        }

        auto get_rotation() const -> int16_t
        {
            return rotation;
        }

        auto is_pressed() const -> bool
        {
            return digitalRead(SW_PIN) == LOW;
        }
    };

} // namespace RumeLab::Detail

template <PinName PCLKPin, PinName PDTPin, PinName PSWPin>
using KY040 = RumeLab::Detail::KY040<PCLKPin, PDTPin, PSWPin>;

#endif // KY040_HPP
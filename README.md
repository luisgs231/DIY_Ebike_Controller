Idea:
Convert a normal bike into e-bike using a hub motor, existing ESC and a battery.
Added components: brake and throttle.

Goal of this project is to generate the control signal for the ESC according to the inputs.

Functional features:
- read the throttle analog signal
- read the brake digital signal
- generate control pulses for the ESC

Secondary features, (not implemented, yet?):
- calibrate throttle range
- acceleration curve
- cruise speed with extra input

The custom e-bike controller uses a PIC12F683 running at 4MHz internal clock, yielding 1 mips.
Due tho the 4 MHz clock, the minimum PWM frequency that can be generated in hardware is around 250 Hz, this seems to work fine with most ESCs that expect 50 Hz.

On power up, the controller waits for the brake signal, after a brake signal is received the pulses are generated and the motor can be controlled with the throttle.

When braking an interrupt on change is generated, the interrupt routine checks if the input came from the brake and if so triggers the brake function. Using directly the external interrupt pin was not possible because this micro controller generates the hardware PWM on the same pin assigned to the external interrupt.

The braking sequence does not (and should never) just turn off the pulse generation. Instead it generates the minimal pulse with a duty cycle of 1500 μS, this turns off the motor. Stopping the pulses is in general a bad idea because some ESC keep the last input for several seconds to compensate for short signal interruptions, rendering the brake useless in this situation.
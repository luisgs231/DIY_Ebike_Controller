#include <12F675.h>
#device ADC=10
#fuses NOWDT, NOPROTECT, NOMCLR, INTRC, INTRC_IO
#use delay(internal=4000000)
#use standard_io(a)

#define throttlePin          sAN3      // Pin 3
#define brakePin             PIN_A2    // Pin 5
#define signalPin            PIN_A0    // Pin 7

void brake();
//output_toggle(signalPin);

#INT_EXT
void EXT_isr(void)
{
   output_toggle(signalPin);
   brake();
}

void setup()
{
   delay_ms(10);
   EXT_INT_EDGE(0, H_TO_L);
   enable_interrupts(GLOBAL);
   PORT_A_PULLUPS(0b000100);
   setup_adc(ADC_CLOCK_DIV_32);
   setup_adc_ports(throttlePin, VSS_VDD);
   set_adc_channel(3);
}

void calibrate()
{
   while(input(brakePin));    // wait for the brake signal
   delay_ms(50);              // debounce delay
   while(!input(brakePin));   // wait for the brake release
   while(input(brakePin))     // wait for the brake signal
   {
      // read min & max throttle values   // TODO ADC
   }
   // store values   // TODO EEPROM
}

void brake()
{
   // pulse neutral    // TODO PWM
   delay_ms(100);
   while(!input(brakePin));   // wait for the brake release
   clear_interrupt(INT_EXT);
   // re-sample before setting pwm!
}

void run()
{
   clear_interrupt(INT_EXT);
   enable_interrupts(INT_EXT);
   // pulse neutral    // TODO PWM
   while(true)
   {
      // poll throttle  // TODO ADC
      // map values     // TODO map()
      // set PWM        // TODO PWM
   }
}

void main()
{
   setup();
   calibrate();
   run();
}

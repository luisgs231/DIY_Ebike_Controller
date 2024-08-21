#include <12F683.h>
#device ADC=10
#fuses NOWDT, NOPROTECT, NOMCLR, INTRC, INTRC_IO
#use delay(internal=4000000)
#use standard_io(a)

#define throttlePin          sAN3      // Pin 3
#define brakePin             PIN_A0    // Pin 5
#define signalPin            PIN_A2    // Pin 7

#use pwm(CCP1,TIMER=2,OUTPUT=signalPin,FREQUENCY=250)

unsigned int16 duty = 373;
void brake();

#INT_RA
void RA_isr(void)
{
   if (!input(brakePin))
   {
      brake();
   }
   clear_interrupt(INT_RA);
}

void setup()
{
   pwm_off();
   //pwm_set_frequency(pulseFrequency);
   enable_interrupts(GLOBAL);
   PORT_A_PULLUPS(0b000001);
   setup_adc(ADC_CLOCK_DIV_32);
   setup_adc_ports(throttlePin, VSS_VDD);
   set_adc_channel(3);
}

void calibrate()
{
   while(input(brakePin));    // wait for the brake signal
   delay_ms(100);              // debounce delay
   while(!input(brakePin));   // wait for the brake release
   while(input(brakePin))     // wait for the brake signal
   {
      // read min & max throttle values   // TODO ADC
   }
   // store values   // TODO EEPROM
}

void brake()
{
   pwm_set_duty(++duty);
   delay_ms(100);
   while(!input(brakePin));   // wait for the brake release
   clear_interrupt(INT_RA);
   // re-sample before setting pwm!
}

void run()
{
   clear_interrupt(INT_RA);
   enable_interrupts(INT_RA);
   // pulse neutral    // TODO PWM
   pwm_set_duty(duty);
   pwm_on();
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

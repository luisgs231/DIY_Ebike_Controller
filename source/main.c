#include <12F683.h>
#device ADC=8
#fuses NOWDT, NOPROTECT, NOMCLR, INTRC, INTRC_IO
#use delay(internal=4000000)
#use standard_io(a)

#define throttlePin          sAN3      // Pin 3
#define brakePin             PIN_A0    // Pin 5
#define signalPin            PIN_A2    // Pin 7

#use pwm(CCP1,TIMER=2,OUTPUT=signalPin,FREQUENCY=250)
//#use rs232 (baud=9600, parity=N, xmit=PIN_A5, bits=8, stream=serial)

const unsigned int16 minDuty = 373;
const unsigned int16 maxDuty = 500;
unsigned int16 duty = minDuty;
unsigned int16 minAdc = 42;
unsigned int16 maxAdc = 215;
unsigned int8 adc = 0;

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

unsigned int16 map_range(unsigned int8 input)
{
    unsigned int16 temp;

    if (input < minAdc)
    {
        input = minAdc;                   // Clamping input to avoid underflow
    }
    else if (input > maxAdc)
    {
        input = maxAdc;                   // Clamping input to avoid overflow
    }

    temp = (unsigned int16)(input - minAdc) * (maxDuty - minDuty);
    temp = temp / (maxAdc - minAdc);
    return (unsigned int16)(temp + minDuty);
}

void setup()
{
   pwm_off();
   enable_interrupts(GLOBAL);
   PORT_A_PULLUPS(0b000001);
   setup_comparator(NC_NC_NC_NC);
   setup_adc(ADC_CLOCK_DIV_64);
   setup_adc_ports(throttlePin | VSS_VDD);
   set_adc_channel(3);
   delay_ms(50);
}

void calibrate()
{
   if(input(brakePin))  return;
   delay_ms(100);
   while(input(brakePin));                // wait for the brake signal
   delay_ms(120);                         // debounce delay
   while(!input(brakePin));               // wait for the brake release
   delay_ms(120);                         // debounce delay
   while(input(brakePin))                 // wait for the brake signal
   {
      // read min & max throttle values   // TODO
   }
   delay_ms(120);                         // debounce delay
   // store values                        // TODO EEPROM
}

void brake()
{
   pwm_set_duty(minDuty);
   duty = minDuty;
   delay_ms(100);
   while(!input(brakePin));               // wait for the brake release
   clear_interrupt(INT_RA);
}

void run()
{
   clear_interrupt(INT_RA);
   enable_interrupts(INT_RA);
   pwm_set_duty(duty);                    // pulse neutral
   pwm_on();
   while(true)
   {
      adc = read_adc();                   // poll throttle
      pwm_set_duty(map_range(adc));       // set duty
   }
}

void main()
{
   setup();
   calibrate();
   run();
}

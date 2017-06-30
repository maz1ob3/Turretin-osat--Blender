#include <TimerOne.h>                                 // Header file for TimerOne library

#define trig1 8
#define trig2 9                                          // Pin 12 trigger output
#define echo1 2
#define echo2 3                                    // Interrupt id for echo pulse

#define TIMER_US 50                                   // 50 uS timer duration 
#define TICK_COUNTS 4000                              // 200 mS worth of timer ticks

volatile long start1 = 0;                         // Records start of echo pulse 
volatile long end1 = 0;                           // Records end of echo pulse
volatile long dur1 = 0;                      // Duration - difference between end and start
volatile long start2 = 0;                         // Records start of echo pulse 
volatile long end2 = 0;                           // Records end of echo pulse
volatile long dur2 = 0;        
volatile int trigger_time_count = 0;                  // Count down counter to trigger pulse time
volatile long range_flasher_counter = 0;              // Count down counter for flashing distance LED


void setup() 
{
  pinMode(trig1, OUTPUT);                           // Trigger pin set to output
  pinMode(echo1, INPUT);                            // Echo pin set to input
  pinMode(trig2, OUTPUT);                           // Trigger pin set to output
  pinMode(echo2, INPUT);
  
  Timer1.initialize(TIMER_US);                        // Initialise timer 1
  Timer1.attachInterrupt( timerIsr );                 // Attach interrupt to the timer service routine 
  attachInterrupt(echo1, echo_interrupt, CHANGE);
  attachInterrupt(echo2, echo_interrupt2,CHANGE);  // Attach interrupt to the sensor echo input
  Serial.begin (9600);                                // Initialise the serial monitor output
}

void loop()
{
 
    Serial.print("dis1: ");
    Serial.println(dur1 / 58.2);  
    Serial.print("dis2: ");
    Serial.println(dur2 / 58.2);     // Print the distance in centimeters              // Print the distance in centimeters
    delay(100);                                       // every 100 mS
}

void timerIsr()
{
    trigger_pulse();                                 // Schedule the trigger pulses
    //distance_flasher();                              // Flash the onboard LED distance indicator
}

void trigger_pulse()
{
      static volatile int state = 0;                 // State machine variable

      if (!(--trigger_time_count))                   // Count to 200mS
      {                                              // Time out - Initiate trigger pulse
         trigger_time_count = TICK_COUNTS;           // Reload
         state = 1;                                  // Changing to state 1 initiates a pulse
      }
    
      switch(state)                                  // State machine handles delivery of trigger pulse
      {
        case 0:                                      // Normal state does nothing
            break;
        
        case 1:                                      // Initiate pulse
           digitalWrite(trig1 && trig2, HIGH);              // Set the trigger output high
           state = 2;                                // and set state to 2
           break;
        
        case 2:                                      // Complete the pulse
        default:      
           digitalWrite(trig1 && trig2, LOW);               // Set the trigger output low
           state = 0;                                // and return state to normal 0
           break;
     }
}

void echo_interrupt()
{
  switch (digitalRead(echo1))                     // Test to see if the signal is high or low
  {
    case HIGH:                                      // High so must be the start of the echo pulse
      end1 = 0;                                 // Clear the end time
      start1 = micros();                        // Save the start time
      break;
      
    case LOW:                                       // Low so must be the end of hte echo pulse
      end1 = micros();                          // Save the end time
      dur1 = end1 - start1;        // Calculate the pulse duration
      break;
  }
}

void echo_interrupt2()
{
  switch (digitalRead(echo2))                     // Test to see if the signal is high or low
  {
    case HIGH:                                      // High so must be the start of the echo pulse
      end2 = 0;                                 // Clear the end time
      start2 = micros();                        // Save the start time
      break;
      
    case LOW:                                       // Low so must be the end of hte echo pulse
      end1 = micros();                          // Save the end time
      dur2 = end2 - start2;        // Calculate the pulse duration
      break;
  }
}
/*
void distance_flasher()
{
      if (--range_flasher_counter <= 0)                // Decrement and test the flash timer
      {                                                // Flash timer time out
         if (echo_duration < 25000)                    // If the echo duration is within limits
         {
           range_flasher_counter = echo_duration * 2;  // Reload the timer with the current echo duration
         }
         else
         {
           range_flasher_counter = 25000;              // If out of range use a default
         }
      }
}*/

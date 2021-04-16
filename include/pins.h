/**********************************************************************************
 * 
 * This program allows operation of the garage door, garage door bell and outside/inside temperatue
 *     
Label  Pin Function
1  GPIO16  
2  GPIO5 1Wire
3  GPIO4 
4  GPIO0 Door Bell
5  GPIO2 Blue Led
6  3.3V  
7  GND 
8  GPIO14  Garage Door Opener
9  GPIO12  
10 GPIO13  
11 GPIO15  
12 GPIO3/RX  
13 GPIO1/TX  
14 GND 
15 NC  
 * 
 * ********************************************************************************
 */
#ifndef _PINS_H
#define _PINS_H

// hardware pin definitions
#define pgm_pin 0
#define blueLED 2 // blue LED light on ESP12e
#define SCL_pin 1
#define SDA_pin 3
#define ONE_WIRE_BUS 5
#define GARAGE_DOOR 14
#define pgm_pin 0

#endif
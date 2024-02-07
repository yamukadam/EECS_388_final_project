#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "eecs388_lib.h"
void steering(int gpio, int pos)
{
int pos_range = 2400 -544;
int convertion = pos * pos_range / 180;
int pulse = 544 + convertion;
    // Task-3: 
    // Your code goes here (Use Lab 05 for reference)
    // Check the project document to understand the task
    gpio_write(gpio, ON);
    delay_usec(pulse);
    gpio_write(gpio, OFF);
    delay_usec(20000-pulse);
    //delay_usec(SERVO_PERIOD - pulse_width);
}
int read_from_pi(int devid)
{
    // Task-2: 
    //after performing Task-2 at dnn.py code, modify this part to read angle values
    //from Raspberry Pi
    // You code goes here (Use Lab 09 for reference)
    int angle;
    if (ser_isready(devid)){
        char data[16];
        ser_readline(1,sizeof(data),data);
        sscanf(data, "%d", &angle);
    }
    printf("angle: %d", &angle);
    return angle;
}
void auto_brake(int devid)
{
    int dist = 0;
    // Task-1: 
    // Your code here (Use Lab 02 - Lab 04 for reference)
    // You must use the directions given in the project document to recieve full 
credit
// while(1){
    if ('Y' == ser_read(devid) && 'Y' == ser_read(devid)){
        dist = ser_read(devid) | ser_read(devid) << 8;
    
        if (dist > 200){
        gpio_write(GREEN_LED, ON);
        gpio_write(RED_LED,OFF);
        }
        else if (dist < 200 && dist >100){
        gpio_write(RED_LED, ON);
        gpio_write(GREEN_LED,ON);
        }
        else if (dist<100 && dist>60){
            gpio_write(RED_LED, ON);
            gpio_write(GREEN_LED,OFF);
        }
        else if(dist <60){
            gpio_write(RED_LED, ON);
            gpio_write(GREEN_LED, OFF);
            delay(100);
            gpio_write(RED_LED, OFF);
            delay(100);
     }
    // }
}
}
int main()
{
    // initialize UART channels
    ser_setup(0); // uart0
    ser_setup(1); // uart1
    int pi_to_hifive = 1; //The connection with Pi uses uart 1
    int lidar_to_hifive = 0; //the lidar uses uart 0
    
    printf("\nUsing UART %d for Pi -> HiFive", pi_to_hifive);
    printf("\nUsing UART %d for Lidar -> HiFive", lidar_to_hifive);
    
    //Initializing PINs
    gpio_mode(PIN_19, OUTPUT);
    gpio_mode(RED_LED, OUTPUT);
    gpio_mode(BLUE_LED, OUTPUT);
    gpio_mode(GREEN_LED, OUTPUT);
    printf("Setup completed.\n");
    printf("Begin the main loop.\n");
    while (1) {
        auto_brake(lidar_to_hifive); // measuring distance using lidar and braking
        int angle = read_from_pi(pi_to_hifive); //getting turn direction from pi
        printf("\nangle=%d", angle) 
        int gpio = PIN_19; 
        //for (int i = 0; i < 10; i++){
            // Here, we set the angle to 180 if the prediction from the DNN is a 
positive angle
            // and 0 if the prediction is a negative angle.
            // This is so that it is easier to see the movement of the servo.
            // You are welcome to pass the angle values directly to the steering 
function.
            // If the servo function is written correctly, it should still work,
            // only the movements of the servo will be more subtle
            if(angle>0){
                steering(gpio, 180);
            }
            else {
                steering(gpio,0);
            }
        //}
    }
    return 0;
}
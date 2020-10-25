#include "mbed.h"

DigitalOut led_orange(p6);
DigitalOut led_blu(p7);
PwmOut led_gren(p8);
InterruptIn Button(p5);

char trigger;
int tBt = 0;//time Button pressed

void int_button1() {trigger |= 0x01;}
void int_button2() {trigger = 0;}

// estado 0 = up
// estado 1 = down
// estado 2 = fall
// estado 3 = min
// estado 4 = rise
// estado 5 = max

//trigger & 0x01

int main() {
    
    //estado inicial
    int state = 0;
    led_blu = !led_blu;
    led_gren = 1.0f;
    int blinkR = 0;
    int blinkB = 0;
    //estado inicial
    
    Button.rise(&int_button1);
    Button.fall(&int_button2);
    
    while (1) {
        
        printf("gren led I: %f tBt: %d\n", led_gren.read(), tBt);
        
        switch(state){
            
            case 0:
            
            if(tBt < 1000 && tBt != 0 && trigger == 0){
                state = 1;
                led_blu = !led_blu;
                led_orange = !led_orange;
                tBt = 0;
            }else if(tBt >= 1000 && led_gren < 1.0f){
                state = 4;
                blinkB = 1;
            }else if(tBt >= 1000 && led_gren == 1.0f){
                tBt = 0;
            }
            
            
            break;
            
            case 1:
            
            if(tBt < 1000 && tBt != 0 && trigger == 0){
                state = 0;
                led_orange = !led_orange;
                led_blu = !led_blu;
                tBt = 0;
            }else if(tBt >= 1000 && led_gren > 0.0f){
                state = 2;
                blinkR = 1;
                
            }else if(tBt >= 1000 && led_gren == 0.0f){
                tBt = 0;
            }
            
            break;
            
            case 2:
            
            if( trigger == 0 && led_gren > 0.0f){
                state = 1;
                blinkR = 0;
                if(led_orange == 0){
                    led_orange = !led_orange;
                }
                tBt = 0;
            }else if(trigger & 0x01 && led_gren > 0.0f){
                led_gren = led_gren - 0.01f;
            }else if(trigger & 0x01 && led_gren == 0.0f){
                state = 3;
            }
            
            break;
            
            case 3:
            
            if(trigger == 0){
                state = 0;
                blinkR = 0;
                if(led_orange == 1){
                    led_orange = !led_orange;
                }
            }
            
            break;
            
            case 4:
            
            if( trigger == 0 && led_gren < 1.0f){
                state = 0;
                blinkB = 0;
                if(led_blu == 0){
                    led_blu = !led_blu;
                }
                tBt = 0;
            }else if(trigger & 0x01 && led_gren < 1.0f){
                led_gren = led_gren + 0.01f;
            }else if(trigger & 0x01 && led_gren == 1.0f){
                state = 5;
            }
            
            break;
            
            case 5:
            if(trigger == 0){
                state = 1;
                blinkB = 0;
                if(led_blu == 1){
                    led_blu = !led_blu;
                }
            }
            break;
        }
        
        if(blinkR == 1){
            led_orange = !led_orange;
        }
        
        if(blinkB == 1){
            led_blu = !led_blu;
        }
        
        wait_ms(200);
        
        if (trigger & 0x01){
            tBt = tBt+200;
        }
        
    }
    
}
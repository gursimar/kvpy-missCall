void servo_centre()
{
           PWMDTY2 = 0x05;                // Pulse Width 1.5ms: Center Position
           PWMDTY3 = 0xDC;                //1.5ms       
}

void servo_init()
{
        PWMCTL_CON23=1;
        
        PWME_PWME2=1;                //PWM channel 2 Enable
        PWME_PWME3=1;                //PWM channel 3 Enable
        
        PWMPOL_PPOL2=1;              //PWM pulse High at begining of Period
        PWMPOL_PPOL3=1;              //PWM pulse High at begining of Period
        
        PWMCLK_PCLK2=1;              // clock SA as clock source for PWM
        PWMCLK_PCLK3=1;              // clock SB as clock source for PWM
        
            
        PWMPRCLK =0x00;              //clock A = 2MHz  clockB = 2MHz
        
        // Time of this clock pulse is 1 us
        PWMSCLA =1;                //clock  SA = clock A / (2 * 1) = 1MHz
        PWMSCLB =1;                //clock  SB = clock B / (2 * 1) = 1MHz
        
        // Value of the register as PWMPER2-PWMPER3 is 20,000.
        // So the value of period of the flipping is 20ms
        PWMPER2 = 0x4E;               // PWM Period 20ms           50Hz
        PWMPER3 = 0x20;               // PWM Period 20ms           50Hz
		  
     servo_centre();
}

void servo_left()
{
          //	Values of register is 1100
          PWMDTY2 = 0x04;                //Pulse Width: Left Position
          PWMDTY3 = 0x6A;                //1.1ms
}


void servo_right()
{
          //	Values of register is 1700
          PWMDTY2 = 0x06;                //Pulse Width 1.7 ms: Right Position
          PWMDTY3 = 0xA4;      
}

void servo_set(unsigned int b)
{
          //	a should range somewhere in 1100 to 1700
          PWMDTY2 = (b>>8);
          b=b<<8;  
          b=b>>8;                
          PWMDTY3 = b;      	
}




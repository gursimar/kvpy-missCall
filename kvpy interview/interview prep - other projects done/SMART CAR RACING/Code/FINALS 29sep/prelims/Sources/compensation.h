/*Compensation ratio = 256.0 / (Black value – White value)*/


/*Beware of overflows. Two figure resolution is quite a safe limit ..
With 3.00 as compensation ratio we have max white val as 44.. which is quite large .. so quite safe ..*/

unsigned int com_ratio[7]= 
{
177,
209,
175,
216,
186,
232,
211
};

// MAX VALUES FOR w_val. SEPARATE w_val COMPUTATION ALGORITHM
unsigned char w_val[7]=
{
11,
10,
10,
10,
10,
10,
10
};

// MIN VALUES FOR b_val. SEPARATE b_val COMPUTATION ALGORITHM
unsigned char b_val[7] =
{
156,
132,
156,
128,
147,
120,
131
};

void compensate_value(unsigned char* val,unsigned char* com_val)
{         
          int i;
          for(i=0;i<=6;i++)
          {
                     if(*(val+i)<w_val[i])        //lower bound
                     {
                              *(com_val+i)=0;
                     }
                     
                     else if (*(val+i)> (b_val[i]))    //higher bound
                     {
                              *(com_val+i)=255;
                     }
                     else
                     {
                              // This will round off the value properly ....
                              *(com_val+i) = (unsigned char)(((*(val+i) - w_val[i]) * com_ratio[i])/DIV100);
                     }
     
          }
}

 
/**********************************************
Function - sort_array
Input - One compensated array
Output - Null
***********************************************/


void sort_array(unsigned char* a)
{
          register int i;
          //index=0;
          for(i=0;i<7;i++)
          {
                    if(*(a+index) < *(a+i))
                    {
                              index=i;
                    }      
          }
}


void ATD_init()
{
	// New code to implement interrupts
	
	//ATD0CTL2 registers
	ATD0CTL2_ADPU=1;   //ATD Power Up
	
	ATD0CTL2_AFFC=1;   //The flag CCF clears automatically if we read the appropriate result register
	
	//No need to use it. It will decrease the response of the system
	//ATD0CTL2_AFFC=0;   //The flag clears when we read the ATDSTAT1
	
	ATD0CTL2_ASCIE=1;  //The interrupt will be called when ASCIF is 1. Clearing this flag is necessary.
	
	//ATD0CTL3
	//ATD0CTL3_S8C=1;           // 8 adc channels sequence

	ATD0CTL3_S8C=0;           // 7 ADC channels sequence
	ATD0CTL3_S4C=1;           // from 0-7 
	ATD0CTL3_S2C=1;           // 
	ATD0CTL3_S1C=1;           // 
	
                    //ATD0CTL4
                    ATD0CTL4_SRES8=1;  //8 bit resoluation
                    
                     //Fastest possible settings already
                    ATD0CTL4_SMP=0;     //2 clock cycles.
                    ATD0CTL4_PRS=0;    //divinde BUS clock by 2. However the MAX BUS clock is 4mhz and min is 2mhx
                    
                    //ATD0CTL5
                    ATD0CTL5_DJM=0;     //Left Justified
                    ATD0CTL5_MULT=1;
                    //ATD0CTL5_SCAN=0;    //single conversion mode

}
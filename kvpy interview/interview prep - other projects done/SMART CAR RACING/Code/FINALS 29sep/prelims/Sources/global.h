//Global definations
#define ON 1
#define OFF 0
#define LED_ON 0
#define LED_OFF 1

#define LED1 PTT_PTT4
#define LED2 PTT_PTT5
#define LED3 PTT_PTT6
#define LED4 PTT_PTT7

#define ADT_LED  LED1
#define BUTTON_LED  LED2

#define SW2  PTP_PTP5
#define SW3  PTP_PTP7

// EEPROMs VETOR ADDRESS
#define EEPROM_KP ((IEE1_TAddress)0x13F000)       //one byte
#define EEPROM_KI ((IEE1_TAddress)0x13F008)      
#define EEPROM_KD ((IEE1_TAddress)0x13F010)      
#define EEPROM_DELAYDC ((IEE1_TAddress)0x13F018)        //Need 1 byte address..
#define EEPROM_SETPOINT ((IEE1_TAddress)0x13F020)        //Need 2 byte address..

#define EEPROM_B_VAL0 ((IEE1_TAddress)0x13F030)        //Need 1 byte address..
#define EEPROM_B_VAL1 ((IEE1_TAddress)0x13F038)        //Need 1 byte address..
#define EEPROM_B_VAL2 ((IEE1_TAddress)0x13F040)        //Need 1 byte address..
#define EEPROM_B_VAL3 ((IEE1_TAddress)0x13F048)        //Need 1 byte address..
#define EEPROM_B_VAL4 ((IEE1_TAddress)0x13F050)        //Need 1 byte address..
#define EEPROM_B_VAL5 ((IEE1_TAddress)0x13F058)        //Need 1 byte address..
#define EEPROM_B_VAL6 ((IEE1_TAddress)0x13F060)        //Need 1 byte address..

#define EEPROM_W_VAL0 ((IEE1_TAddress)0x13F068)        //Need 1 byte address..
#define EEPROM_W_VAL1 ((IEE1_TAddress)0x13F070)        //Need 1 byte address..
#define EEPROM_W_VAL2 ((IEE1_TAddress)0x13F078)        //Need 1 byte address..
#define EEPROM_W_VAL3 ((IEE1_TAddress)0x13F080)        //Need 1 byte address..
#define EEPROM_W_VAL4 ((IEE1_TAddress)0x13F088)        //Need 1 byte address..
#define EEPROM_W_VAL5 ((IEE1_TAddress)0x13F090)        //Need 1 byte address..
#define EEPROM_W_VAL6 ((IEE1_TAddress)0x13F098)        //Need 1 byte address..

#define EEPROM_COM_RATIO0 ((IEE1_TAddress)0x13F0A0)        //Need 2 byte address..
#define EEPROM_COM_RATIO1 ((IEE1_TAddress)0x13F0B0)        //Need 2 byte address..
#define EEPROM_COM_RATIO2 ((IEE1_TAddress)0x13F0C0)        //Need 2 byte address..
#define EEPROM_COM_RATIO3 ((IEE1_TAddress)0x13F0D0)        //Need 2 byte address..
#define EEPROM_COM_RATIO4 ((IEE1_TAddress)0x13F0E0)        //Need 2 byte address..
#define EEPROM_COM_RATIO5 ((IEE1_TAddress)0x13F0F0)        //Need 2 byte address..
#define EEPROM_COM_RATIO6 ((IEE1_TAddress)0x13F100)        //Need 2 byte address..

#define EEPROM_KP_DEFAULT                       9
#define EEPROM_KI_DEFAULT                       2           //20
#define EEPROM_KD_DEFAULT                      0
#define EEPROM_DELAYDC_DEFAULT           3
#define EEPROM_SETPOINT_DEFAULT         90

// FLOAT POINT - EMULATION
#define DIV10000 10000
#define DIV1000 1000
#define DIV100 100
#define DIV10 10

//Global Variables
unsigned char sen_data[7]=
{
0,
0,
0,
0,
0,
0,
0
};
// SWITCH interrupt CONTROL EEPROM VARIABLES. ...
//DONT CHANGE THE VARIABLE types LINKED TO EEPROM ... !!!!!!!!!!!!!!!!!!!


unsigned char kp=0,ki=0,kd=0;
// kp(80,240)/1000       ki(20,100)/10000     kd(20,100)/10000
// kp(8,24)/100       ki(20,100)/10000     kd(2,10)/10000
unsigned char delay_dc=3;     //variable related to START_CROSS detection

unsigned int setpoint =1;    //set point is +ve and generally small values..   VAR RELATED TO PULSE counter
unsigned int setpointa=4;

unsigned char dc_newval_flag=0;
unsigned int pulse_count=0;   //16 bit regsiter for accumulator

unsigned char index=3;          // changed to remember the value of index..

unsigned char START = 1;

unsigned int upper=90;



void Leds_and_Switches_Init()
{
          DDRT=0xF0;                // Setting Port T pin 4,5,6,7 as output
          
          DDRP_DDRP5=0;                 //Port P Pin 5 & 7 set to input 
          DDRP_DDRP7=0;        
          PERP_PERP5=1;                 //Port P Pin 5 & 7 Pullup Enable
          PERP_PERP7=1;

          LED1=LED_OFF;
          LED2=LED_OFF;
          LED3=LED_OFF;
          LED4=LED_OFF;

}


void Delay(unsigned char a) 
{
          unsigned int i,j;

          for(j=1;j<=a;j++)            //Delay
          for(i=0;i<=60000;i++);  
}
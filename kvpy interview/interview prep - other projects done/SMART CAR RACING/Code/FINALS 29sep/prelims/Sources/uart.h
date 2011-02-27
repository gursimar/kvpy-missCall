void uart0_9600_init(void);
unsigned char uart0_rx(void);
void uart0_tx(unsigned char a);
void uart0_tx_string(char *s) ;
void uart0_tx_number(unsigned char a);
void uart0_tx_newline(void);
void uart0_tx_int(unsigned int a);


//UART functions..... 
void uart0_9600_init(void) {
SCI0BDL=13;
SCI0CR1=0;
SCI0CR2=12; 
}


void uart0_tx_int(unsigned int a) {
unsigned int temp;

temp=a/10000;
uart0_tx(temp + 0x30);
a=a-temp*10000;

temp=a/1000;
uart0_tx(temp + 0x30);
a=a-temp*1000;

temp=a/100;
uart0_tx(temp + 0x30);
a=a-temp*100;

temp=a/10;
uart0_tx(temp + 0x30);
a=a-temp*10;

uart0_tx(a + 0x30);
}


void uart0_tx_number(unsigned char a) {
unsigned char temp;
temp=a/100;
uart0_tx(temp + 0x30);
a=a-temp*100;
temp=a/10;
uart0_tx(temp + 0x30);
a=a-temp*10;
uart0_tx(a + 0x30);
}

void uart0_tx_string(char *s) {

	while (*s) 
	{
		uart0_tx(*s);
		s++;
	}
}

void uart0_tx_newline(void) { 
uart0_tx(0x0A);
uart0_tx(0x0D);
}

unsigned char uart0_rx(void) {
  while(SCI0SR1_RDRF==0);
  return SCI0DRL;
}


void uart0_tx(unsigned char a) {
  SCI0DRL=a;
  while(SCI0SR1_TC==0);
}
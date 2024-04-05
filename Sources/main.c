#include "MK60D10.h"

/* Macros for bit-level registers manipulation */
#define GPIO_PIN_MASK	0x1Fu
#define GPIO_PIN(x)		(((1)<<(x & GPIO_PIN_MASK)))
/* Buttons masks */
#define BUTTON_UP_MASK 0x4000000
#define BUTTON_LEFT_MASK 0x8000000
#define BUTTON_DOWN_MASK 0x1000
#define BUTTON_RIGHT_MASK 0x400


/* Constants specifying delay loop duration */
#define	tdelay1			10000
#define tdelay2 		200

int x[] = {8, 7, 6, 5};
int y[] = {4, 4, 4, 4};
int sDir[] = {3, 3, 3, 3};


/* Configuration of the necessary MCU peripherals */
void SystemConfig() {
	/* Turn on all port clocks */
	SIM->SCGC5 = SIM_SCGC5_PORTA_MASK | SIM_SCGC5_PORTE_MASK;


	/* Set corresponding PTA pins (column activators of 74HC154) for GPIO functionality */
	PORTA->PCR[8] = ( 0|PORT_PCR_MUX(0x01) );  // A0
	PORTA->PCR[10] = ( 0|PORT_PCR_MUX(0x01) ); // A1
	PORTA->PCR[6] = ( 0|PORT_PCR_MUX(0x01) );  // A2
	PORTA->PCR[11] = ( 0|PORT_PCR_MUX(0x01) ); // A3

	/* Set corresponding PTA pins (rows selectors of 74HC154) for GPIO functionality */
	PORTA->PCR[26] = ( 0|PORT_PCR_MUX(0x01) );  // R0
	PORTA->PCR[24] = ( 0|PORT_PCR_MUX(0x01) );  // R1
	PORTA->PCR[9] = ( 0|PORT_PCR_MUX(0x01) );   // R2
	PORTA->PCR[25] = ( 0|PORT_PCR_MUX(0x01) );  // R3
	PORTA->PCR[28] = ( 0|PORT_PCR_MUX(0x01) );  // R4
	PORTA->PCR[7] = ( 0|PORT_PCR_MUX(0x01) );   // R5
	PORTA->PCR[27] = ( 0|PORT_PCR_MUX(0x01) );  // R6
	PORTA->PCR[29] = ( 0|PORT_PCR_MUX(0x01) );  // R7

	/* Set corresponding PTE pins (output enable of 74HC154) for GPIO functionality */
	PORTE->PCR[28] = ( 0|PORT_PCR_MUX(0x01) ); // #EN


	/* Change corresponding PTA port pins as outputs */
	PTA->PDDR = GPIO_PDDR_PDD(0x3F000FC0);

	/* Change corresponding PTE port pins as outputs */
	PTE->PDDR = GPIO_PDDR_PDD( GPIO_PIN(28) );
}


/* Variable delay loop */
void delay(int t1, int t2)
{
	int i, j;

	for(i=0; i<t1; i++) {
		for(j=0; j<t2; j++);
	}
}


/* Conversion of requested column number into the 4-to-16 decoder control.  */
void column_select(unsigned int col_num)
{
	unsigned i, result, col_sel[4];

	for (i =0; i<4; i++) {
		result = col_num / 2;	  // Whole-number division of the input number
		col_sel[i] = col_num % 2;
		col_num = result;

		switch(i) {

			// Selection signal A0
		    case 0:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(8))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(8)));
				break;

			// Selection signal A1
			case 1:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(10))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(10)));
				break;

			// Selection signal A2
			case 2:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(6))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(6)));
				break;

			// Selection signal A3
			case 3:
				((col_sel[i]) == 0) ? (PTA->PDOR &= ~GPIO_PDOR_PDO( GPIO_PIN(11))) : (PTA->PDOR |= GPIO_PDOR_PDO( GPIO_PIN(11)));
				break;

			// Otherwise nothing to do...
			default:
				break;
		}
	}
}

void row_select(unsigned int row_num) {
	switch(row_num){
		// Selection signal R0
		case 0:
			PTA->PDOR |= GPIO_PDOR_PDO(GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(29));
			break;
		// Selection signal R1
		case 1:
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(26));
			PTA->PDOR |= GPIO_PDOR_PDO(GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(29));
			break;
		// Selection signal R2
		case 2:
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(24));
			PTA->PDOR |= GPIO_PDOR_PDO(GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(29));
			break;
		// Selection signal R3
		case 3:
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(9));
			PTA->PDOR |= GPIO_PDOR_PDO(GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(29));
			break;
		// Selection signal R4
		case 4:
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(25));
			PTA->PDOR |= GPIO_PDOR_PDO(GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(29));
			break;
		// Selection signal R5
		case 5:
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(28));
			PTA->PDOR |= GPIO_PDOR_PDO(GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(29));
			break;
		// Selection signal R6
		case 6:
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(7));
			PTA->PDOR |= GPIO_PDOR_PDO(GPIO_PIN(27));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(29));
			break;
		// Selection signal R7
		case 7:
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(26));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(24));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(9));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(25));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(28));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(7));
			PTA->PDOR &= ~GPIO_PDOR_PDO(GPIO_PIN(27));
			PTA->PDOR |= GPIO_PDOR_PDO(GPIO_PIN(29));
			break;
	}

}
/* Lightning pixels of the snake */
void snakeOutput() {
	for(int i = 0; i < 4; i++) {
		column_select(x[i]);
		row_select(y[i]);
		delay(400, 50);
	}
}
/* Changing direction of the first pixel of the snake */
void turnSnake(int dir){
	sDir[0] = dir;
}
/* Changing direction of second third and forth pixels of the snake */
void changeTailSnake() {
	sDir[3] = sDir[2];
	sDir[2] = sDir[1];
	sDir[1] = sDir[0];
}
/* Changing coordinates of the snake */
void moveSnake(){
	for(int i = 0; i < 4; i++) {
		int res = 0;
		switch(sDir[i]) {
			case 0:
				x[i] -= 1;
				break;
			case 1:
				res = (y[i] + 1) % 8;
				y[i] = res;
				break;
			case 2:
				res = (y[i] - 1) % 8;
				y[i] = (res < 0) ? res + 8 : res;
				break;
			case 3:
				x[i] += 1;
				break;
			default:
				break;
		}
	}
}
/* Handling interrupts */
void PORTE_IRQHandler(void) {

    if(PORTE_ISFR & BUTTON_UP_MASK) {
    	if(!(GPIOE_PDDR & BUTTON_UP_MASK))
    		PTA->PDOR = 0x00;
    		turnSnake(0);
    }
    else if(PORTE_ISFR & BUTTON_DOWN_MASK){
    	if(!(GPIOE_PDDR & BUTTON_DOWN_MASK))
    		PTA->PDOR = 0x00;
       		turnSnake(3);
    }

    else if(PORTE_ISFR & BUTTON_LEFT_MASK){
        if(!(GPIOE_PDDR & BUTTON_LEFT_MASK))
        	PTA->PDOR = 0x00;
        	turnSnake(1);
    }
    else if(PORTE_ISFR & BUTTON_RIGHT_MASK){
        if(!(GPIOE_PDDR & BUTTON_RIGHT_MASK))
        	PTA->PDOR = 0x00;
        	turnSnake(2);
    }

    PORTE_ISFR = PORT_ISFR_ISF_MASK;

}

void init_hardware(void) {
	uint8_t i[] = {10, 12, 27, 26};

	MCG->C4 |= (MCG_C4_DMX32_MASK | MCG_C4_DRST_DRS(0x01));
	SIM->CLKDIV1 |= SIM_CLKDIV1_OUTDIV1(0x00);

	SIM->SCGC5 |= SIM_SCGC5_PORTE_MASK;

	for(int j=0; j<=4; j++) {
		PORTE->PCR[i[j]] = ( PORT_PCR_ISF(0x01) /* Nuluj ISF (Interrupt Status Flag) */
				| PORT_PCR_IRQC(0x0A) /* Interrupt enable on failing edge */
				| PORT_PCR_MUX(0x01) /* Pin Mux Control to GPIO */
				| PORT_PCR_PE(0x01) /* Pull resistor enable... */
				| PORT_PCR_PS(0x01)); /* ...select Pull-Up */

	}

//	PORTE->PCR[9] |= PORT_PCR_MUX(0x01);  /* Pin Mux Control - GPIO (RED LED) */
//	PORTE->PCR[13] |= PORT_PCR_MUX(0x01); /* Pin Mux Control - GPIO (BZZZZZZ) */

	NVIC_ClearPendingIRQ(PORTE_IRQn); /* Nuluj priznak preruseni od portu B */
	NVIC_EnableIRQ(PORTE_IRQn);       /* Povol preruseni od portu B */
}

int main(void)
{
	init_hardware();
	SystemConfig();
	PTA->PDOR = 0x00;

	for(;;) {
        snakeOutput();
		moveSnake();
		changeTailSnake();
		delay(2000, 100);
	}

    /* Never leave main */
    return 0;
}

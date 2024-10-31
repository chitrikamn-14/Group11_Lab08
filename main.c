#include <stdint.h>
#include "tm4c123gh6pm.h"

void UART0_Init(void);
char UART0_Received(void);
void UART0_Send(char data);
void LED_Init(void);
void LED_ON_OFF(char choice);

int main(void) {
    LED_Init();          // Initialize LEDs
    UART0_Init();       // Initialize UART

    char receivedChar;
    while (1) {
        receivedChar = UART0_Received();
        UART0_Send(receivedChar);
        LED_ON_OFF(receivedChar);
    }
}

void LED_Init(void) {
    SYSCTL_RCGCGPIO_R |= 0x20;          // Activate clock for Port F
    GPIO_PORTF_DIR_R |= 0x0E;           // Set PF1, PF2, PF3 as output (LEDs)
    GPIO_PORTF_DEN_R |= 0x0E;           // Enable digital I/O for PF1, PF2, PF3
}

void UART0_Init(void) {
    SYSCTL_RCGCUART_R |= 0x01;          // Enable UART0
    SYSCTL_RCGCGPIO_R |= 0x01;          // Enable PORTA

    UART0_CTL_R &= ~0x01;                // Disable UART0
    UART0_IBRD_R = 104;                  // Set integer part for baud rate (assuming 16 MHz and 9600 baud)
    UART0_FBRD_R = 11;                   // Set fractional part for baud rate
    UART0_LCRH_R = 0x70;                 // 8-bit, no parity, 1-stop bit, FIFOs
    UART0_CTL_R = 0x301;                 // Enable UART0, TX, and RX

    GPIO_PORTA_AFSEL_R |= 0x03;          // Enable PA0 and PA1 as alternate function
    GPIO_PORTA_PCTL_R = (GPIO_PORTA_PCTL_R & 0xFFFFFF00) | 0x11; // Set PA0 and PA1 as UART
    GPIO_PORTA_DEN_R |= 0x03;            // Enable digital function for PA0 and PA1
}

char UART0_Received(void) {
    while ((UART0_FR_R & 0x10) != 0);    // Wait until RXFE is 0 (FIFO not empty)
    return UART0_DR_R & 0xFF;            // Read and return the received character
}

// Send data to UART0
void UART0_Send(char data) {
    while ((UART0_FR_R & 0x20) != 0);    // Wait until the transmitter is not full
    UART0_DR_R = data;                   // Send the character
}

void LED_ON_OFF(char choice) {
    GPIO_PORTF_DATA_R &= ~0x0E;          // Clear previous LED states
    if(choice == 'R') {
        GPIO_PORTF_DATA_R |= 0x02;       // Turn on Red LED
    }
    else if(choice == 'B') {
        GPIO_PORTF_DATA_R |= 0x04;       // Turn on Blue LED
    }
    else if(choice == 'G') {
        GPIO_PORTF_DATA_R |= 0x08;       // Turn on Green LED
    }
    // No need for else condition since LEDs are already cleared
}

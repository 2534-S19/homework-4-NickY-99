#include <ti/devices/msp432p4xx/driverlib/driverlib.h>
#include "homework4.h"

int main(void)
{
    char rChar;
    char *response = "\n\n\r2534 is the best course in the curriculum!\r\n\n";

    // TODO: Declare the variables that main uses to interact with your state machine.


    // Stops the Watchdog timer.
    //WDT_A_holdTimer();
    initBoard();
    // TODO: Declare a UART config struct as defined in uart.h.
    //       To begin, configure the UART for 9600 baud, 8-bit payload (LSB first), no parity, 1 stop bit.
    eUSCI_UART_ConfigV1 uartConfig = {
            EUSCI_A_UART_CLOCKSOURCE_SMCLK,               // SMCLK Clock Source = 3MHz
            19,                                           // UCBR   = 19
            8,                                            // UCBRF  = 8
            0x55,                                         // UCBRS  = 0x55
            EUSCI_A_UART_NO_PARITY,                      // No Parity
            EUSCI_A_UART_LSB_FIRST,                       // LSB First
            EUSCI_A_UART_ONE_STOP_BIT,                   // One stop bit
            EUSCI_A_UART_MODE,                            // UART mode
            EUSCI_A_UART_OVERSAMPLING_BAUDRATE_GENERATION, // Oversampling
            EUSCI_A_UART_8_BIT_LEN                       //8 bit payload
    };

    // TODO: Make sure Tx AND Rx pins of EUSCI_A0 work for UART and not as regular GPIO pins.
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P1,
        GPIO_PIN2 , GPIO_PRIMARY_MODULE_FUNCTION);

    GPIO_setAsPeripheralModuleFunctionOutputPin(
            GPIO_PORT_P1, GPIO_PIN3,
            GPIO_PRIMARY_MODULE_FUNCTION);
    // TODO: Initialize EUSCI_A0
    UART_initModule(EUSCI_A0_BASE, &uartConfig);


    // TODO: Enable EUSCI_A0
    UART_enableModule(EUSCI_A0_BASE);

    while(1)
    {
        // TODO: Check the receive interrupt flag to see if a received character is available.
        //       Return 0xFF if no character is available.
        if (UART_getInterruptStatus(
                EUSCI_A0_BASE,
                EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)

            rChar = UART_receiveData(EUSCI_A0_BASE);

        else
            rChar = 0xFF;

        // TODO: If an actual character was received, echo the character to the terminal AND use it to update the FSM.
        //       Check the transmit interrupt flag prior to transmitting the character.
        if(rChar != 0xFF)
        {
            if(charFSM(rChar))
            UARTPutString(EUSCI_A0_BASE,&rChar);
        }

        // TODO: If the FSM indicates a successful string entry, transmit the response string.
        //       Check the transmit interrupt flag prior to transmitting each character and moving on to the next one.
        //       Make sure to reset the success variable after transmission.
        //if(UARTCanSend(EUSCI_A0_BASE))


    }
}

void initBoard()
{
    WDT_A_hold(WDT_A_BASE);
}

// TODO: FSM for detecting character sequence.
bool charFSM(char rChar)
{
    typedef enum {SA, S2, S25, S253, S2534} passcode_state_t;
    static passcode_state_t currentState = SA;
    bool finished = false;
            switch (currentState)
            {
            case SA:

                if (rChar == 2)
                    currentState = S2;
                break;

            case S2:
                if (rChar == 5)
                    currentState = S25;
                else
                    currentState = SA;
                break;

            case S25:
                if (rChar == 3)
                    currentState = S253;
                else
                    currentState = SA;
                break;

            case S253:
                if (rChar == 4)
                {
                    currentState = S2534;
                    finished = true;
                }
                else
                    currentState = SA;
                break;

            case S2534:
                currentState = SA;
                break;
            }
            if (finished&&UARTHasChar(EUSCI_A0_BASE)&&UARTCanSend(EUSCI_A0_BASE))
                UARTPutString(EUSCI_A0_BASE, &rChar);

             /*  else
               finished=UARTHasChar(EUSCI_A0_BASE);*/

    return finished;
}

bool UARTHasChar(uint32_t moduleInstance){
    bool flag = false;
    if (UART_getInterruptStatus(
                    EUSCI_A0_BASE,
                    EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG) == EUSCI_A_UART_RECEIVE_INTERRUPT_FLAG)
    {
        //Data is available
        flag = true;
    }

    return flag;
}
void UARTPutChar(uint32_t moduleInstance, uint8_t transmittedChar){
// This time you must specify the character you want to send. But there is no need for a returned value this time.
    if(UARTCanSend( EUSCI_A0_BASE))
        UART_transmitData(EUSCI_A0_BASE, transmittedChar);
}
void UARTPutString(uint32_t moduleInstance, char *str){
    char s1[100], i;
    for (i = 0; s1[i] != '\0'; ++i)
    {
        &s1[i]=*str;
        UARTPutChar(EUSCI_A0_BASE, s1[i]);
    }
}
bool UARTCanSend(uint32_t moduleInstance){
    bool send=false;
    if (UART_getInterruptStatus(
                       EUSCI_A0_BASE,
                       EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG ) == EUSCI_A_UART_TRANSMIT_INTERRUPT_FLAG )
        send = true;

    return send;
}

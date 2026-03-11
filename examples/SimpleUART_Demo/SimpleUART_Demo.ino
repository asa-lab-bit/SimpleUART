#include <SimpleUART.h>
#include <string.h>

// Initialize with 16 bytes buffer
SimpleUART<16> myUART;

// Interrupt handler
ISR(USART0_RXC_vect) {
    myUART.handle_interrupt();
}

void setup() {
    PORTA.DIRSET = PIN1_bm; // PA1 as Output (LED)
    PORTA.OUTCLR = PIN1_bm; 

    myUART.init(115200);
    myUART.send_string("System Online. Send 'ON' or 'OFF'\r\n");
}

void loop() {
    if (myUART.is_line_ready()) {
        const char* cmd = myUART.get_line();

        if (strcmp(cmd, "ON") == 0) {
            PORTA.OUTSET = PIN1_bm;
            myUART.send_string("-> LED is now ON\r\n");
        } 
        else if (strcmp(cmd, "OFF") == 0) {
            PORTA.OUTCLR = PIN1_bm;
            myUART.send_string("-> LED is now OFF\r\n");
        }

        myUART.clear_line();
    }

}

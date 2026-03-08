#ifndef SIMPLE_UART_H
#define SIMPLE_UART_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>

/**
 * @brief SimpleUART Library for ATtiny 0-series
 * @tparam S Buffer size (e.g. 16, 32, 64)
 */
template <uint8_t S>
class SimpleUART {
private:
    volatile char rx_buffer[S];
    volatile uint8_t rx_index = 0;
    volatile bool line_ready = false;

public:
    // Initialize UART (Default 20MHz clock)
    void init(uint32_t baud, uint32_t f_cpu = 20000000) {
        USART0.BAUD = (uint16_t)((64ULL * f_cpu) / (16ULL * baud));
        PORTA.DIRSET = PIN6_bm; 
        USART0.CTRLA = USART_RXCIE_bm;
        USART0.CTRLB = USART_TXEN_bm | USART_RXEN_bm;
        sei();
    }

    void send_char(char c) {
        while (!(USART0.STATUS & USART_DREIF_bm));
        USART0.TXDATAL = c;
    }

    void send_string(const char* str) {
        while (*str) send_char(*str++);
    }

    void handle_interrupt() {
        char c = USART0.RXDATAL;
        send_char(c); // Echo back
        if (!line_ready) {
            if (c == '\r' || c == '\n') {
                if (rx_index > 0) {
                    rx_buffer[rx_index] = '\0';
                    line_ready = true;
                }
            } else if (rx_index < S - 1) {
                rx_buffer[rx_index++] = c;
            }
        }
    }

    bool is_line_ready() { return line_ready; }
    const char* get_line() { return (const char*)rx_buffer; }
    void clear_line() { rx_index = 0; line_ready = false; }
};

#endif
#ifndef SIMPLE_UART_H
#define SIMPLE_UART_H

#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdbool.h>
#include <stdlib.h> // itoaを使用するために追加

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

    // --- 基本の1文字送信 ---
    void send_char(char c) {
        while (!(USART0.STATUS & USART_DREIF_bm));
        USART0.TXDATAL = c;
    }

    // --- 既存のGitHub互換用関数 ---
    void send_string(const char* str) {
        this->print(str); // printを呼び出すようにして共通化
    }

    // --- 新機能：標準ライブラリ風 print 関数 ---
    void print(char c) {
        this->send_char(c);
    }

    void print(const char* s) {
        while (*s) {
            this->send_char(*s++);
        }
    }

    void print(int n) {
        char buf[7]; // -32768〜32767
        itoa(n, buf, 10);
        this->print(buf);
    }

    // --- 新機能：標準ライブラリ風 println 関数 ---
    void println(const char* s) {
        this->print(s);
        this->print("\r\n");
    }

    void println(int n) {
        this->print(n);
        this->print("\r\n");
    }

    void println() {
        this->print("\r\n");
    }

    // --- 受信・割り込み関連（既存通り） ---
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

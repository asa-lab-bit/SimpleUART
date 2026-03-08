# SimpleUART for ATtiny 0-series (202/402)

[English]
A lightweight, header-only UART library designed for ATtiny 0-series microcontrollers. 
It uses C++ templates to allow flexible buffer sizes while minimizing RAM footprint.

[日本語]
ATtiny 0シリーズ向けに設計された、超軽量・ヘッダーオンリーのUARTライブラリです。
C++テンプレートを活用し、メモリ使用量を抑えつつバッファサイズを自由に変更可能です。

## Features / 特徴
- **Extremely Lightweight**: No unnecessary overhead, perfect for 2KB Flash / 128B RAM devices.
- **Template-based**: Define buffer size in your sketch like `SimpleUART<16>`.
- **Interrupt Driven**: Efficient RX processing using `USART0_RXC` interrupt.
- **Easy to Use**: Single header file. No `.cpp` file needed.

## Installation / インストール
Just download `SimpleUART.h` and place it in your sketch folder.
`SimpleUART.h` をスケッチフォルダに配置するだけで使用できます。

## Quick Start / 使い方
```cpp
#include "SimpleUART.h"

SimpleUART<16> myUART;

ISR(USART0_RXC_vect) {
    myUART.handle_interrupt();
}

void setup() {
    myUART.init(115200);
}

void loop() {
    if (myUART.is_line_ready()) {
        myUART.send_string("Received: ");
        myUART.send_string(myUART.get_line());
        myUART.clear_line();
    }
}
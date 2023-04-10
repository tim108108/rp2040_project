# rp2040_project
[MarkDown語法大全](https://hackmd.io/@mrcoding/ryZE7k8cN)  
[FreeRTOS - 成大資工](https://wiki.csie.ncku.edu.tw/embedded/freertos)  
[Pico C SDK](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html)  
Use the development board as [RP2040-Zero](https://www.waveshare.net/wiki/RP2040-Zero).  
- [ ] freertos 
- [ ] multithread
- [ ] arm assembly
- [ ] dual core
## Let's start !!  
如果pico-sdk沒有submodule update，某些模組(tinyusb)沒安裝，會導致PC偵測不到rp2040  
```bash
git clone https://github.com/tim108108/rp2040_project.git
git submodule update --init 
cd pico-sdk
git submodule update --init
cd ..
```
建立toolchain，pico-sdk與FRTOS-Kernel位置固定在目錄最上層  
```bash
mkdir build
cd build
cmake ..
```
當要編譯時在build路徑下執行
```bash
rp2040_project/build/make -j
```
接著把build目錄下的`rp2040_project.uf2`燒錄到rp2040
## GPIO examlpe
```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

int main() {
    stdio_init_all();
    printf("gpio test");
    int led=29;        //設定led為GPIO29
    int bot=28;        //設定bot為GPIO28
    gpio_init(bot);    //初始化bot
    gpio_init(led);    //初始化led
    gpio_set_dir(led,GPIO_OUT);   //設定led為輸出
    gpio_set_dir(bot,GPIO_IN);    //設定bot為輸入
    gpio_pull_up(bot);            //pull_up，預設high準位
    while(1){
        if(gpio_get(bot)==0){
            gpio_put(led,1);
            printf("gpio on\n");
            sleep_ms(500);
            gpio_put(led,false);
            printf("gpio off\n");
            sleep_ms(500);
        }
    }
}
```


# Raspberry Pi Pico SDK
The Raspberry Pi Pico SDK (henceforth the SDK) provides the headers, libraries and build system
necessary to write programs for the RP2040-based devices such as the Raspberry Pi Pico
in C, C++ or assembly language.

The SDK is designed to provide an API and programming environment that is familiar both to non-embedded C developers and embedded C developers alike.
A single program runs on the device at a time and starts with a conventional `main()` method. Standard C/C++ libraries are supported along with
C level libraries/APIs for accessing all of the RP2040's hardware include PIO (Programmable IO).

Additionally the SDK provides higher level libraries for dealing with timers, synchronization, USB (TinyUSB) and multi-core programming
along with various utilities.

The SDK can be used to build anything from simple applications, to fully fledged runtime environments such as MicroPython, to low level software
such as RP2040's on-chip bootrom itself.

Additional libraries/APIs that are not yet ready for inclusion in the SDK can be found in [pico-extras](https://github.com/raspberrypi/pico-extras).


# Documentation

See [Getting Started with the Raspberry Pi Pico](https://rptl.io/pico-get-started) for information on how to setup your
hardware, IDE/environment and for how to build and debug software for the Raspberry Pi Pico
and other RP2040-based devices.

See [Raspberry Pi Pico C/C++ SDK](https://rptl.io/pico-c-sdk) to learn more about programming using the
SDK, to explore more advanced features, and for complete PDF-based API documentation.

See [Online Raspberry Pi Pico SDK API docs](https://rptl.io/pico-doxygen) for HTML-based API documentation.


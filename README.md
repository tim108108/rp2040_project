# rp2040_project
[MarkDown語法大全](https://hackmd.io/@mrcoding/ryZE7k8cN)  
[FreeRTOS - 成大資工](https://wiki.csie.ncku.edu.tw/embedded/freertos)  
[Pico C/C++ SDK](https://www.waveshare.net/w/upload/5/5f/Pico_c_sdk.pdf)  
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
## GPIO example
```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"

#define led=29;        //設定led為GPIO29
#define bot=28;        //設定bot為GPIO28

int main() {
    stdio_init_all();
    printf("gpio test");
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
## ADC example
```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"

#define ADC3 29 //set ADC in GPIO29 

int main() {
    stdio_init_all();
    printf("ADC Example, measuring GPIO29\n");

    adc_init();
    adc_gpio_init(ADC3);    //初始化GPIO29為ADC
    adc_select_input(3);    //選擇ADC3(GPIO29)

    while (1) {
        // 12-bit conversion, assume max value == ADC_VREF == 3.3 V
        const float conversion_factor = 3.3f / (1 << 12);   //計算12bit解析度，ADC_VERF=3.3V，3.3/(2^12)
        uint16_t result = adc_read();   
        printf("Raw value: 0x%03x, voltage: %f V\n", result, result * conversion_factor);
        sleep_ms(500);
    }
}
```
## i2c example 待驗正
```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"

#define I2C0_PORT i2c0
#define I2C0_SDA_PIN 4
#define I2C0_SCL_PIN 5

#define I2C1_PORT i2c1
#define I2C1_SDA_PIN 2
#define I2C1_SCL_PIN 3

#define DEVICE_ADDRESS 0x42

int main() {
    // 初始化I2C0
    i2c_init(I2C0_PORT, 100 * 1000);
    gpio_set_function(I2C0_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C0_SDA_PIN);
    gpio_pull_up(I2C0_SCL_PIN);

    // 初始化I2C1
    i2c_init(I2C1_PORT, 100 * 1000);
    i2c_set_slave_mode(I2C1_PORT, true, DEVICE_ADDRESS);
    gpio_set_function(I2C1_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C1_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C1_SDA_PIN);
    gpio_pull_up(I2C1_SCL_PIN);
    while(1){
    uint8_t tx_data[] = "Hello, I2C!";  // 要傳送的字串
    uint8_t rx_data[sizeof(tx_data)];  // 要接收的字串
    i2c_write_blocking(I2C0_PORT, DEVICE_ADDRESS, tx_data, sizeof(tx_data), false);  // 在I2C0上傳送字串
    i2c_read_blocking(I2C1_PORT, DEVICE_ADDRESS, rx_data, sizeof(rx_data), false);  // 在I2C1上接收字串

    printf("Received: %s\n", rx_data);  // 輸出接收到的字串
    }
    return 0;
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


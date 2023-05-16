# rp2040_project
[MarkDown語法大全](https://hackmd.io/@mrcoding/ryZE7k8cN)，[FreeRTOS - 成大資工](https://wiki.csie.ncku.edu.tw/embedded/freertos)  
[Pico C/C++ SDK](https://www.waveshare.net/w/upload/5/5f/Pico_c_sdk.pdf)  
[Raspberry Pi Pico SDK Examples](https://github.com/raspberrypi/pico-examples)  
[SDK](https://datasheets.raspberrypi.com/pico/raspberry-pi-pico-c-sdk.pdf)  
[Hardware APIs](https://www.raspberrypi.com/documentation/pico-sdk/hardware.html)  
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
## i2c example 待修正
```c
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"

#define I2C0_PORT i2c0
#define I2C0_SDA_PIN 4
#define I2C0_SCL_PIN 5

#define I2C1_PORT i2c1
#define I2C1_SDA_PIN 2
#define I2C1_SCL_PIN 3

#define DEVICE_ADDRESS 0x42
bool reserved_addr(uint8_t addr) {
    return (addr & 0x78) == 0 || (addr & 0x78) == 0x78;
}
int main() {
    stdio_init_all();
    
    // 初始化I2C0
    i2c_init(I2C0_PORT, 100 * 1000);
    gpio_set_function(I2C0_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C0_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C0_SDA_PIN);
    gpio_pull_up(I2C0_SCL_PIN);

    // 初始化I2C1
    i2c_init(I2C1_PORT, 100 * 1000);
    gpio_set_function(I2C1_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(I2C1_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(I2C1_SDA_PIN);
    gpio_pull_up(I2C1_SCL_PIN);
    i2c_set_slave_mode(I2C1_PORT, true, DEVICE_ADDRESS);
    while(1){
        printf("i2c example\n");
        printf("Scanning I2C bus...\n");
        printf("\nI2C Bus Scan\n");
        printf("   0  1  2  3  4  5  6  7  8  9  A  B  C  D  E  F\n");
        for (int addr = 0; addr < (1 << 7); ++addr) {
            if (addr % 16 == 0) {
                printf("%02x ", addr);
            }
            int ret;
            uint8_t rxdata;
            if (reserved_addr(addr))
                ret = PICO_ERROR_GENERIC;
            else
                ret = i2c_read_blocking(I2C0_PORT, addr, &rxdata, 1, false);

            printf(ret < 0 ? "." : "@");
            printf(addr % 16 == 15 ? "\n" : "  ");
        }
        // uint8_t tx_data[] = "0xF0F0";  // 要傳送的字串
        // uint8_t rx_data[sizeof(tx_data)];  // 要接收的字串
        // i2c_write_blocking(I2C0_PORT, DEVICE_ADDRESS, tx_data, sizeof(tx_data), false);  // 在I2C0上傳送字串
        // i2c_read_blocking(I2C1_PORT, DEVICE_ADDRESS, rx_data, sizeof(rx_data), false);  // 在I2C1上接收字串
        // printf("Received: %x\n", rx_data);  // 輸出接收到的字串
        sleep_ms(100);
    }
}
```
## hardware_flash 待修正
```C
#include <stdio.h>
#include <stdlib.h>

#include "pico/stdlib.h"
#include "hardware/flash.h"
#include "hardware/sync.h"

// We're going to erase and reprogram a region 256k from the start of flash.
// Once done, we can access this at XIP_BASE + 256k.
#define FLASH_TARGET_OFFSET (256*1024)

const uint8_t *flash_target_contents = (const uint8_t *) (XIP_BASE + FLASH_TARGET_OFFSET);

void print_buf(const uint8_t *buf, size_t len) {
    for (size_t i = 0; i < len; ++i) {
        printf("%02x", buf[i]);
        if (i % 16 == 15)
            printf("\n");
        else
            printf(" ");
    }
}

int main() {
    stdio_init_all();
    while(1){
        sleep_ms(500);
        printf("FLASH_PAGE_SIZE:%d\n",FLASH_PAGE_SIZE);
        printf("FLASH_TARGET_OFFSET:%d\n",FLASH_TARGET_OFFSET);
        printf("FLASH_SECTOR_SIZE:%d\n",FLASH_SECTOR_SIZE);
        uint8_t random_data[FLASH_PAGE_SIZE];
        for (int i = 0; i < FLASH_PAGE_SIZE; ++i)
            random_data[i] = rand() >> 16;

        printf("Generated random data:\n");
        print_buf(random_data, FLASH_PAGE_SIZE);
        
        sleep_ms(500);
        uint32_t saved_interrupts = save_and_disable_interrupts();

        // Note that a whole number of sectors must be erased at a time.
        printf("\nErasing target region...\n");
        flash_range_erase(FLASH_TARGET_OFFSET, FLASH_PAGE_SIZE);
        
        printf("Done. Read back target region:\n");
        print_buf(flash_target_contents, FLASH_PAGE_SIZE);

        printf("\nProgramming target region...\n");
        flash_range_program(FLASH_TARGET_OFFSET, random_data, FLASH_PAGE_SIZE);
        printf("Done. Read back target region:\n");
        print_buf(flash_target_contents, FLASH_PAGE_SIZE);
        printf("here\n");

        bool mismatch = false;
        for (int i = 0; i < FLASH_PAGE_SIZE; ++i) {
            if (random_data[i] != flash_target_contents[i])
                mismatch = true;
        }
        if (mismatch)
            printf("Programming failed!\n");
        else
            printf("Programming successful!\n");
        restore_interrupts(saved_interrupts);
    }
}
```
## hardware_timer

```c
#include <stdio.h>
#include "pico/stdlib.h"

volatile bool timer_fired = false;

int64_t alarm_callback(alarm_id_t id, void *user_data) {
    printf("Timer %d fired!\n", (int) id);
    timer_fired = true;
    // 可以在這裡返回一個未來觸發的時間（以微秒為單位）
    return 0;
}

bool repeating_timer_callback(struct repeating_timer *t) {
    printf("Repeat at %lld\n", time_us_64());
    return true;
}

int main() {
    stdio_init_all();
    printf("Hello Timer!\n");

    // 在5秒後調用alarm_callback函數
    add_alarm_in_ms(5000, alarm_callback, NULL, false);

    // 等待alarm回調設置timer_fired標誌
    while (!timer_fired) {
        tight_loop_contents();
    }

    // 創建一個重複定時器，調用repeating_timer_callback函數
    // 如果延遲時間 > 0，則是前一個回調結束和下一個回調開始之間的延遲時間
    // 如果延遲時間為負（請參見下面的說明），則下一次回調將在上一次回調開始後的500毫秒之後
    struct repeating_timer timer;
    add_repeating_timer_ms(500, repeating_timer_callback, NULL, &timer);
    sleep_ms(3000);
    bool cancelled = cancel_repeating_timer(&timer);
    printf("cancelled... %d\n", cancelled);
    sleep_ms(2000);

    // 延遲時間為負，這意味著我們將調用repeating_timer_callback函數，並在之後的500毫秒內再次調用它，
    // 無論上一次回調執行花費了多長時間
    add_repeating_timer_ms(-500, repeating_timer_callback, NULL, &timer);
    sleep_ms(3000);
    cancelled = cancel_repeating_timer(&timer);
    printf("cancelled... %d\n", cancelled);
    sleep_ms(2000);
    printf("Done\n");
    return 0;
}
```
## hardware_irq
## hardware_watchdog

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


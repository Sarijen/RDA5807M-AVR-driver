# RDA5807M

> [!IMPORTANT]  
> This document is still work-in-progress!

### Wiring cheatsheet
| RDA5807M | Other components   |
| :------: | :----------------: |
| GND      | GND                |
| VDD      | 2.7 - 3.3V Supply  |
| SCLK     | MCU SCL            |
| SDIO     | MCU SDA            |
| RCLK     | 32.768 kHz Crystal |
| LOUT     | Left Audio Output  |
| ROUT     | Right Audio Output |
| FMIN     | Antenna circuitry  |

# Building (CMake)
- This library uses [AVR-LibC](https://github.com/avrdudes/avr-libc), make sure you have it and its dependencies installed and set correctly.


1. `git clone` this repo inside your project
2. modify your CMakeLists.txt 

    add the library directory using
    ``` 
    add_subdirectory(RDA5807M-AVR-driver)
    ```
    add `RDA5807M` keyword to target_link_libraries  



# Usage
1. include the header `#include "RDA5807M.h"`
2. Create an instance of `RDA5807M`, no constructor
3. Call `init()`

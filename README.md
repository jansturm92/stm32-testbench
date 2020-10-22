# STM32 Testbench
 Template to quickly start **benchmarking** and **unit testing** code on supported development boards. The testbench is based on the [PlatformIO](https://platformio.org) framework, using [libopencm3](https://github.com/libopencm3/libopencm3) as firmware library.
 
 ## Features
 - no configuration of boards neccessary
 - accurately measure the CPU cycle count of functions
 - easily unit test functions directly on the board with PlatformIO 
 - TRNG support where available
 
 ## Supported boards
 - [STM32F4DISCOVERY](https://docs.platformio.org/en/latest/boards/ststm32/disco_f407vg.html)
 - [ST B-L475E-IOT01A Discovery kit](https://docs.platformio.org/en/latest/boards/ststm32/disco_l475vg_iot01a.html)
 - [STM32F0DISCOVERY](https://docs.platformio.org/en/latest/boards/ststm32/disco_f051r8.html)
 
## Usage
Make sure PlatformIO is installed (see also the [PlatformIO documentation](https://docs.platformio.org/en/latest/core/installation.html)):
```bash
$ pip install -U platformio
```

#### Serial output:
TODO

#### Add custom functions:
- add function declarations and definitions in `include/myfunc.h` and `src/myfunc.c` or `src/myfunc_asm.S` or add your own files.
- add a `MEASURE()` entry to `src/benchmark.c` for benchmarking
- add a `RUN_TEST()` entry to `test/unit.c` for unit testing

Connect a development board and choose the respective \<environment\> from the following list:
- disco_f407vg
- disco_l475vg_iot01a
- disco_f051r8

Supported environments can also be listed with: `pio project config`

#### Benchmarking:
```bash
$ pio run -e <environment> -t upload -t monitor
```
#### Unit testing:
```bash
$ pio test -e <environment>
```

## License
<p xmlns:dct="http://purl.org/dc/terms/" xmlns:vcard="http://www.w3.org/2001/vcard-rdf/3.0#">
  <a rel="license"
     href="http://creativecommons.org/publicdomain/zero/1.0/">
    <img src="https://licensebuttons.net/p/zero/1.0/88x31.png" style="border-style: none;" alt="CC0" />
  </a>
  <br />
  To the extent possible under law,
  <span resource="[_:publisher]" rel="dct:publisher">
    <span property="dct:title">Jan Sturm</span></span>
  has waived all copyright and related or neighboring rights to
  <span property="dct:title">STM32 Testbench</span>.
This work is published from:
<span property="vcard:Country" datatype="dct:ISO3166"
      content="DE" about="[_:publisher]">
  Germany</span>.
</p>
# Y LPC loader

### What is it ###

Yet another lpc3250 loader

### Features ###

* Easy configurable with config file
* Can be configured in comand line
* Load sequence of executables

### Dependencies ###

    >cmake-2.6
    libyaml

### Compiling ###

    mkdir build
    cd build
    cmake ..
    make

### Installation ###

    make install

### Config file ###

Default config file `~/.lpc3250.yaml`

Example can be found in source dir named '.y-lpc-loader.yaml'

    Port: /dev/ttyS0
    PrintAllChar: 1
    Exec:
    - PrimaryFileName: "erase_nand.bin"
    - PrimaryFileName: "burner_kickstart.bin"
      SecondaryFileName: "kickstart.bin"
    - PrimaryFileName: "burner_s1app.bin"
      IRAMaddress: 0x0000
      SecondaryFileName: "s1l_from_kick.bin"
      SDRAMaddress: 0x80000004

* Port - COM port
* Exec - Sequence of executables
* PrimaryFileName - Primary Boot
* IRAMaddress - IRAM address
* SecondaryFileName - Secondary executable
* SDRAMaddress - SDRAM address
* PrintAllChar - Output all incoming char

Load `erase_nand.bin` on default IRAM address and executet. Load `burner_kickstart.bin` on default IRAM address, then load `kickstart.bin` on default SDRAM address. Load `burner_s1app.bin` on 0x0000 then load `s1l_from_kick.bin` on 0x80000004

Config can have difference exeks count.

### Command line ###

    -h, --help               help
    -c, --config             file use alternate config file
    -p, --port               port
    -f, --first              primary execurable file
    -s, --second             secondary execurable file
    -i, --primary-address    primary file load address
    -d, --secondary-address  secondary file load address
    -a, --print-all-char     Output all char
    -w, --wait               Wait, dont exit after last exec

Comand line options have greater priority vs config file options. If specified `primary execurable file` sequence of execs in config would be ignored.

### Using ###

Start y-lpc-loader, reset board. Wait for execs or burning. For start next exec reset board twice

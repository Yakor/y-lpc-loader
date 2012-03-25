# Y LPC loader

### What is it ###

Yet another lpc3250 loader

### Features ###

* Easy configurable with config file
* Can be configured in comand line
* Load sequence of executables

### Compiling ###

    mkdir build
    cd build
    cmake ..
    make

### Installation ###

    make install

### Config file ###

Default config file `~/.lpc3250.yaml`

Example

    Port: /dev/ttyS0
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

1. load `erase_nand.bin` on default IRAM address and executet
2. load `burner_kickstart.bin` on default IRAM address, then load `kickstart.bin` on default SDRAM address
3. load `burner_s1app.bin` on 0x0000 then load `s1l_from_kick.bin` on 0x80000004

Config can have difference exeks count.

### Command line ###

    -h, --help               help
    -c, --config             file use alternate config file
    -p, --port               port
    -f, --first              first execurable file
    -s, --second             secondary execurable file
    -i, --primary-address    primary file load address
    -d, --secondary-address  secondary file load address

### Using ###

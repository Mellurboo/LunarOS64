# Lunar OS for 64bit and UEFI
Lunar OS is a hobby OS I'm writing but I wanted it to work on all modern hardware if possible

Lunar written with the Limine bootloader by [@mintsuki](https://github.com/mintsuki) https://github.com/limine-bootloader/

Lunar OS is still growing and I aim to have a working and fleshed out terminal soon.

## Build Sequence

`make all` compiles the operating system and installs all dependencies
`make run` does all compilation and runs the operating system in qemu
`make debug` does all compilation and run the operating system with the qemu monitor attached to the terminal. this will soon include qemu logging

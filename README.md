# loofOS
A novice OS. I'm still learning OS theory and computer architecture. A complete OS for one architecture will probably take me 1-2 years.

**NOTE:** Currently I have paused work on the ARM arch kernel, as qemu for some reason decided to leave out emulation for the timer. Without the timer, I will not be able to test out context switching, multitasking, nor will I be able to write the scheduler. So until I find a solution that doesn't involve overwriting the kernel of my only ARM system that I need for development, I will be dropping all work on that end.

### Building
Just `make` at the root path of the repo to see what architectures are currently available. Then, do `make <arch>`. Also, the Makefile is incredibly scuffed, so basically you will need to compile on an `ARM` system in order to compile for `ARM` and you will need the LLVM and cross compile toolchain in order to compile for `x86`.

### Running
Currently, the only supported architecture is 32bit `ARMv7l` and `x86`. Also, the drivers for `ARM` will only work on a raspberry pi board (rpi 2/3). I will try to add a device tree in the future and more drivers for different devices. It is recommended that you use the qemu emulator to test/run.
#### For ARM
```sh
# Run with qemu emulator
qemu-system-aarch64 -M raspi2 -kernel PATH/TO/KERNEL
```
#### For x86
```sh
# Run with qemu emulator also
qemu-system-x86_64 -drive file=build/x86_64/loof.img,index=0,media=disk,format=raw
```

### Checklist
(Will expand in the future)
- Stupid Bootloader (DONE ARM, DONE x86)
- Processes (DONE x86)
- Scheduler (DONE x86)
- Paging (In Progress x86)
- Drivers
	- Serial Output (DONE ARM, DONE x86)
	- Keyboard (DONE x86)
	- Hard-disk
	- Terminal
- Filesystem
- Shell
- libc

### Contributing
#### For ARM
For the peripherals and programming the drivers, refer to the documentation for bcm2837 or the header files under `include/peripherals`. Provide verbose comments.
#### For x86
For the peripherals and programming the drivers, uh just look at some driver specifications online and the intel manuals. Again, you can look at the current implementation for reference or for help.

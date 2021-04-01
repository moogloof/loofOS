# loofOS
A novice OS. I'm still learning OS theory and computer architecture. A complete OS for one architecture will probably take me 1-2 years.

### Building
Just `make arm` at the root path of the repo.

### Running
Currently, the only supported architecture is 32bit `ARMv7l`. Also, the drivers will probably only work on a raspberry pi board (rpi 2/3). I will try to add a device tree in the future. It is recommended that you use the qemu emulator to test/run.
```sh
# Run with qemu emulator
qemu-system-aarch64 -M raspi2 -kernel PATH/TO/KERNEL
```

### Checklist
(Will expand in the future)
- Stupid Bootloader (DONE)
- Processes
- Drivers
	- Serial Output (DONE)
	- Keyboard
	- Hard-disk
	- Terminal
- Filesystem
- Shell

### Contributing
For the peripherals and programming the drivers, refer to the documentation for bcm2837 or the header files under `include/peripherals`. Provide verbose comments.

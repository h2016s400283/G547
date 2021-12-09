# Linux Device Driver for I2C to UART Converter
Linux Device Driver to interface Arduino Uno as a slave device to Raspberry Pi 4 over I2C. 
	Arduino UNO acts as I2C to serial converter using UNO. 
	Data is sent to UNO from Raspberry Pi over I2C, and UNO in turn sends it over through UART.
	The control of the driver lies with user space.

Explaining the implementation:
	Initially after connectiong the UNO with the Pi, we run the command i2cdetect -y 1 to see the address at which the UNO is getting detected. 
	In our case its 0x25. Using I2C_BOARD_INFO macro, the device address is added to the list of i2c devices with a specific name (ArduinoCOM) in the information table.
	We also need to declare the i2c bus number being used using i2c_add_adapter()
	Next work is to instantiate ArduinoCOM using i2c_new_client_device, with the bus no being used (1 in our case).
	Now, to use i2c_add_driver, we must instantiate struct i2c_driver(ArduinoCOM_i2c_driver) with all the functionalities we want it to handle.
	Along with additional functions, the actual functionality of transferring bytes has been handled using the probe function, where i2c_master_send has been used.
	To transfer the data from user to kernel space, the ioctl function call has been made to call the probe function internally.
	Based on the argument passed to ioctl function, the corresponding ASCII value is passed to probe function.
	
Execution of the Code:
	The entire process goes like this:
	We enter the message byte from the user space, by executing the userapp.
	Based on the input recieved, the corresponding byte is getting displaying in the serial port of our computer.
		1) Run make.sh
		2) You are prompted to give input parameter(from 0 to 9)
		3) On receiving it, the same is reflected in the com port

Extra:  
	We have written an Arduino sketch that gets the byte data over I2C and transmits it over UART.
	To ease the compilation process, we are making a shell script file (make.sh) that
		1)compiles both user and kernel space files.
		2)inserting the driver into the kernel
		3)executes userapp
		4)removes the driver from the Kernel
		5)prints the tail of kernel log output

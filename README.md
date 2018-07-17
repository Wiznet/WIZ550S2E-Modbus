# WIZ550S2E-Modbus
Serial to Ethernet Module based on W5500 &amp; Cortex-M0

## Revision History

<p align="left">
  <img width="100%" src="https://wizwiki.net/wiki/lib/exe/fetch.php?media=products:wiz550s2e:new_wireframe_2.png" />
</p>

### v1.4.0

- Added features
	- Modbus
		- v1.20 Configuration Tool is required.

### v1.1.5

- Added features
	- AT+MPASS : (AT command) Change the module's name(automatically saved)
	- AT+MNAME : (AT command) Change the module's password(automatically saved)
- Changed
	- AT+NMODE -> AT+MMODE

### v1.1.4

- Fixed Problems receiving data in AT Mode

### v1.1.3

- Fixed UDP Send Fail in AT Mode

### v1.1.2

- Fixed recevied data loss(During trans to serial) problem in AT Mode
	- Limit max size(1000byte) of receive data from ethernet.

### v1.1.1

- Added inactivity time function in Server/Mixed Mode

### v1.1.0

- ioLibrary Update
- Dependency Removal (remove "board.h" in "w5500.h")
- Remove garbage file

### v1.0.4

- Fixed bug
	- Time packing problem in UDP mode.
		- Before : Not work normally after the first time.
		- After : Work normally
	- Upper and lower case problems in config Data.
		- Before : Only uppercase letters work normally.
		- After : Works in both uppercase and lowercase.

### v1.0.3

- Fixed bug
	- AT Command Trigger
		- Before : AT command is actived regardless of AT command enable.
		- After : AT command is actived regard of AT command enable.
- Added features
	- AT+NMODE : (AT command) Change the settings that operates in data mode(automatically saved)

### v1.0.2

- Fixed bug 
	- In mixed mode
		- Before : when connecting to the server, the '+' had to be include in the serial data.
		- After : when serial data is generated directly connecting.
	- In AT command
		- Before : at = a t. in other words, ignore space.
		- After : at != a t. in other words, admitte space.
- Added features
	- Check unlink in mixed mode
	- AT+FDNS = domain name

### v1.0.1

- Fixed Project set

### v1.0.0

- First release : Apr. 2015

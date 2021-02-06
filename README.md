# Arduino-capacitive-touch-RGB

## About
Library Arduino for capacitive touch and RGB led
- Detect touch and hold
- Control RGB
	- Color
	- Brightness
	- etc
	
## Demo
> video will be uploaded soon...

## Instalation guide
Just download and upload it to arduino board using arduino IDE
> NOTE:  
> - Sensitivity may vary depending on touch area and pressure.
> - Capacitive pin may be sensitive to current, use a good power supply or battery.
> - In some cases, requires touching body / ground to detect capacitive touch.
> - If you found a better library for capacitive please let me know... ^___^ thx b4
## Components
- Arduino board
- RGB Led strip / RGB led (common anode / cathode)
- N channel FET (optional)
- Power supply

## Wiring
- RGB common anode
![Wiring1](wiring/wiring_mini_anode.png)
- RGB common cathode
![Wiring2](wiring/wiring_mini_cathode.png)
- Using N channel mosfet
![Wiring2](wiring/wiring_nano_n_fet.png)
	> If using N channel FET, you need to set it to common cathode.  
	> CODE : `#define COMMON_ANODE false`
	


## License and credits
My code under MIT license, other libraries follow their own license.
- Timer.h
- capacitive.h
- etc...

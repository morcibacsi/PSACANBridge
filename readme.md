# PSA CAN Bridge

### What is it ?

Around 2010 PSA Group (Peugeot/Citroen) made breaking changes in their CAN bus protocol. This means that new peripherals like the RCC, NAC or color matrix displays (MATT) which were designed to work with the new protocol (AEE200) will not work in older cars (with AEE2004/2007 protocol)
The goal of this project is to have the new peripherals (mainly the NAC/RCC and MATT) using the new protocol working inside older cars.

### Why ?

Because it is fun. But other than that I wanted to replace the old matrix display in my 5008 to the newer version from a 5008. And since the project was born I could also replace my RD45 to a NAC.

### But what about the others?

There are a few projects like this out there. For example:
- https://github.com/kurkpitaine/stellantis-can-adapter
- https://github.com/ludwig-v/arduino-psa-comfort-can-adapter
- https://github.com/fmiglior/psa_can2004_to_can2010
- https://github.com/npioterczak/rcc_nac_adapter_T4
- https://github.com/V3nn3tj3/arduino-psa-nac-interface-public

They might be good, but I wanted my own version which I can easily modify whenever I want. They might have more features, but for me the goal isn't to replicate every possible use-case, just to have those which I mostly use. Of course I might implement some of the features they have, but that isn't the main goal.

### Components

The components needed for the project to work in your car depends on what you want to replace, but you can think of these:
- Head unit (RCC/NAC)
  - Quadlock-NAC/RCC/MRN adapter
  - GPS antenne
  - USB converter cable
  - AM/FM antenne converter
  - LVDS cable
  - NAC/RCC
  - Display
- Matrix display (MATT)
  - Connector adapter
  - Matrix display (MATT2010)
- Common
  - PSA CAN protocol bridge hardware
  - JST XH6 conversion cable (see more info in the wiki)

Regarding the cables for the headunit I recommend to search on Aliexpress or similar sites for "Peugeot RCC NAC MRN adapter cable". You can buy everything in one package, save yourself the hussle to hunt down everything one by one. The connectors are proprietary and building the patch lead would be a pain. Because of this I won't provide a pinout for these.

![components](https://github.com/morcibacsi/PSACanBridge/raw/main/images/components.jpg)

You still need to build one cable anyways: a conversion cable which contains the JST XH6 connector for the bridge hardware the MATT connector adapter and a quadlock connector (see the [Installation](#Installation) section).

### PSA CAN protocol bridge hardware

I built a 'shield' for the ESP32 dev board where I integrated the CAN bus transceivers and the MCP2515 CAN controller onto one board. The KiCad project files can be found in [this repository][psacanbridgehw].

### Liability

I used the software in my car for months, without any sign of errors. However I cannot take any responsibility if something goes wrong if you build it and install it in your car. So use the software and hardware at your own risk.

### Functionality

 - Different VIN code support for the head unit (in case you can't modify it in the unit)
 - Show door status message when it isn't supported by default (like in the 5008)
 - Web page to setup the board
 - OTA support

[![WATCH IT ON YOUTUBE](https://img.youtube.com/vi/Qk7EEoJt94w/0.jpg)](https://www.youtube.com/watch?v=Qk7EEoJt94w "Watch it on YouTube")

### Compatibility

The software was tested on a Peugeot 5008 made in 2012 however most probably it is compatible with all of the cars with a CAN2004/2007 protocol made by the PSA group. Here is a short list about the cars which should be compatible:

- Peugeot 3008
- Peugeot 5008
- Peugeot 307 (2005-)
- Peugeot 407
- etc...

### Installation

Regarding the installation you are a bit on your own as the easiest place might be different across models.
For me the easiest was to connect the 4 wires (12V, GND, CAN High, CAN Low) at the original matrix connector. You might be luckier and can find a CAN+ (12V for the CAN peripherals) at an easier accessible location.

Below you can see the schematics of a patch lead which has the 6 pin connector of the MATT and it's socket, and a quadlock connector. The CAN lines are exposed to a JST XH 6 connector where you can connect the PSA CAN bridge hardware.

Take note that I left out the quadlock socket (only the plug is there) as it has straight wires, except the CAN wires, which I drawn. For this I just bought a quadlock extension cable and removed the CAN wires.

![matt_quadlock_bridge](https://github.com/morcibacsi/PSACANBridge/raw/main/images/matt_quadlock_bridge.png)

![matt_quadlock_bridge_patch_lead](https://github.com/morcibacsi/PSACANBridge/raw/main/images/matt_quadlock_bridge_patch.jpg)

### Project structure

The PSACanBridgeMain.cpp is the main entry point where everyting is wired together. It is the Can\CanDataConverter.cpp file which parses the incoming CAN messages and converts the data to the appropriate member of the Helpers\DataBroker.h class. In the Can\Handlers you can find the classes which are responsible for sending a particular message on the CAN bus. This is where the DataBroker members are used.
The Can\CanMessageHandlerContainer2010 is also a notable class, it contains the reference to the available handlers. It is responsible to send the messages at the required period.

In the Helper directory you can find the following:
 - Service class for the webpage
 - RTC provider for the real time clock
 - MessageDecider (which CAN messages should be processed)
 - Service to save the configuration

These are just the basics to get started, if you are interested further then head to the discussion section.

### Configuration page

The software contains an access point named PSACANBridge. If you connect to it it will present a captive portal (or you can point your browser to http://192.168.100.1). You can set some aspects of the software. The content of the webpage can be found in the Helpers\WWWData.h file (as a gzipped byte array). It is written in Preact using TypeScript. The source code can be found in [this repository][psacanbridgesetup].

### Building the project

#### From PlatformIO
The project can be built from PlatformIO. It will download the necessary libraries so you don't have to worry about them.

#### From Arduino
In theory it is possible to compile the project from the Arduino IDE as well, but I can't support that scenario. The project takes enough time to maintain the codebase itself, I don't really have the time to support every use-case. Give yourself a favour and take a look on how PlatformIO works, it is much more easier to use that once you figured out the basics.

### Used libraries

The list of the used libraries can be found in the platformio.ini file under the **lib_deps** node.

[psacanbridgehw]: https://github.com/morcibacsi/PSACanBridgeHW
[psacanbridgesetup]: https://github.com/morcibacsi/PSACanBridgeSetup

# COD-themed-human-radar

## What is it
So basically its a,

radar with a LD2410C which can detect humans from a 6m range even when they are still, as the module also detects micromovements.

It also has a 2.8 inch TFT for displaying the fancy ui for the COD themed radar.

Its equipped with a ESP32 S3 DEVKIT C1 N16R8 for delivering fast and powerful performance.

## BOM

| Item | Qty | Supplier | Cost/item | Total Cost | Link |
| :--- | :---: | :--- | :---: | :---: | :--- |
| ESP32 S3 | 1 | Robocraze | $10.00 | $10.00 | [Link](https://robocraze.com/products/7semi-esp32-s3-dev-boardc-1-n8r8-wifi-bluetooth-dual-usb-c-rgb-led?variant=48519036272864) |
| 2.8 inch TFT | 1 | Robu | $7.00 | $7.00 | [Link](https://robu.in/product/2-8-inch-tft-lcd-non-touch-display-modulesd-card-support/) |
| The LD2410C | 1 | Robu | $7.00 | $7.00 | [Link](https://robu.in/product/hi-link-hlk-ld2410c-p-smd22x16mm-sensor-modules-rohs/) |
| The enclosure CAD | 1 | Robu | $4.00 | $4.00 | [Link](https://robu.in/product/3d-printing-service1/) |
| The PCB | 5 | Robu | $1.80 | $9.00 | [Link](https://robu.in/product/online-pcb-manufacturing-service/) |
| CAD lid | 1 | Robu | $2.00 | $2.00 | [Link](https://robu.in/product/3d-printing-service1/) |
| Estimated total shipping | 1 | - | $4.00 | $4.00 | - |
| **Total** | **11** | | | **$43.00** | |

# Guide
###### This course will guide you for making the PROJECT.


## Kicad

### Schematic 
So the schematic looks like this:
<img width="1121" height="706" alt="sche" src="https://github.com/user-attachments/assets/d315d5a2-2b7b-4d6e-b438-7f49199457cb" />
You can do the connections according to this.
#### Symbols and footprints 
For the symbols and footprints you can check out the folder named Symbols and footprints (It has already been made its just a copy), execept for esp 32, it can be natively downloaded from kicad itself.


### Footprint assignment

So you can assign the footprints according to  this image:
<img width="1260" height="711" alt="foooot" src="https://github.com/user-attachments/assets/f77ef123-e165-41e8-9e55-2965204258bb" />

### PCB
So the initial start is like this:
<img width="1321" height="866" alt="hhhhhhhhhhhhhhhhh" src="https://github.com/user-attachments/assets/5c1a2eb3-057a-49fb-9c09-6619a27e2e4e" />


Now we need to arrange it in a user friendly manner, so either you can arrange it like this or any other way which suites you:

<img width="687" height="552" alt="ggg" src="https://github.com/user-attachments/assets/c13f520f-afae-468c-9b5d-9d963c50eb82" />

Now comes the fun part routing, this has no restrictions, you can route it any way you want.

I have routed it like this:

<img width="587" height="701" alt="bbbbbb" src="https://github.com/user-attachments/assets/3df0e5da-1d81-4a5c-8faa-acfe0166e3e7" />




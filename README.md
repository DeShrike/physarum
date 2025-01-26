# Pi Kiosk Lite

Kiosk/Slideshow for Raspberry Pi.

Lightweight, runs on the server version of Raspberry OS, no desktop needed.


## Quick start

Clone this repo.

Install dependencies, see below.

Then do:

```console
cd pi-kiosk-lite
make
```

Place images in the _images_ folder.

Edit _kiosk.ini_ to suit your needs:
 - WIDTH and HEIGHT: 800, 600 or 1024, 768 or 1920, 1080, etc.
 - Adjust the path to the images if needed
 - Add passwords to exit the kiosk. For keyboard password, use small letters only.
   For mouse password, use a sequence of *L* and *R*. 
   Pressing the left and right mouse buttons in that order will exit.
   For joystick password, use a sequence of *A*, *B*, *X* and *Y*.
   Pressing the buttons in that order will exit.
 - Edit the slide duration: in milliseconds. 
   This is the time each slide will remain on screen.
 - Background color, in hex format: *FFFFFF* is white, *FF0000* is red, etc.

To see you monitors current resolution, use this command:

```console
bfset
```



## Full installation

Write a Raspberry Pi OS images to a new SD card. 
Use a server version, without a desktop.

...
TODO

## Install dependencies

```console
sudo apt-get install libsdl2-dev
sudo apt-get install libsdl2-image-dev
sudo apt-get install libsdl2-ttf-dev
```

## TODO

- handle the passwords
- transparent PNG support
- select easing function in INI

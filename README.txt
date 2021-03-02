To connect to network:
sudo route add default gw 192.168.1.1


# Serial MIDI
If serial midi doesn't work and you're using Raspberry PI 3, then maybe bluetooth is getting in the way.

Try following the instructions from https://www.raspberrypi.org/forums/viewtopic.php?t=183724

```
# Add these lines to /boot/config.txt
enable_uart=1
dtoverlay=pi3-miniuart-bt
dtoverlay=midi-uart0
```

## /boot/cmdline.txt may be important as well...
Read https://sinneb.github.io/p736/

```
Disable bluetooth: “sudo systemctl disable bluetooth.service”
Add “enable_uart=1” and “dtoverlay=pi3-disable-bt” to /boot/config.txt
Remove “console=serial0,115200” from /boot/cmdline.txt
```

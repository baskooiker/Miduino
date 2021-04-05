# Miduino

- Install Zynthian OS
- Install openFrameworks
- Add ofxMidi addon
  - Add JACK target to addon_config.mk
```
linuxarmv6l:
	ADDON_PKG_CONFIG_LIBRARIES = alsa jack
```
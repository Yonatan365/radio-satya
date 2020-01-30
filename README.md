# radio-satya
Arduino based MP3 simple and rugged mp3 player

## BOM
- Arduino Uno.
- Sparkfun mono audio amplifier https://www.sparkfun.com/products/11044
- VS1053 VS1053B Stereo Audio MP3 Player Shield https://www.aliexpress.com/item/32873666286.html?spm=a2g0s.9042311.0.0.35404c4dybrPQw
-  3x4 Matrix Keyboard Keypad Module https://www.aliexpress.com/item/32903146554.html?spm=a2g0s.9042311.0.0.35404c4dybrPQw
-  Portable 2600mAh Power Bank with keychain For iPhone 8 7 6 https://www.aliexpress.com/item/33009052845.html?spm=a2g0o.productlist.0.0.7dcf42d30M8N2u&algo_pvid=2f5a49fc-2b79-4b66-a0fa-31a4ad5142ac&algo_expid=2f5a49fc-2b79-4b66-a0fa-31a4ad5142ac-2&btsid=fef4ac22-44a8-40c3-b552-29214b277376&ws_ab_test=searchweb0_0,searchweb201602_4,searchweb201603_53
- Any 8 ohm speaker.
- Any suitable box.
- Volume controller knob (2K-10K potentiometer).
- on/off switch.

## Connections
- keypad to arduino analog and some digitals.
- Arduino Vout and gnd to amplifier power.
- Mp3 output (select one channel and ground\[the one closest to the end\]) to amplifier in.
- Amplifier out to speaker.
- volume potentiometer to amplifier volume.
- on / off between power bank and arduino usb (split usb cable to wires, only interested in the red and black wires).

## SD card folder structure
- 10 folders named 0-9.
- in each folders files named 1-99 with extension of ".mp3".
- Keypad numbers: select the matching directory. Consecutive presses will cycle the files in the directory.
- Keypad #: pauses.
- Keypad *: once stop, twice stop and rewind.

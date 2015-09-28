# PI-Relay

This is for the Raspberry Pi.
It is program is a service that monitors the directory /var/relay.  When it is empty it turns off GPIO 1.  When a file or directory is created there it turns on GPIO 1.  When the directory is empty again it turns off the relay.

I use this as a relay to turn on/off speakers to my Raspberry pi.  My PI is an airplay client for my sound system. The idea is to save power when there isn't music playing.

But what if two programs access the sound port simulateously?  Each one writes a separate files to /var/relay.  For instance I have my doorbell connected to this also.  That is a separate program that initiates a sound.

# Installation

Run make all
Then make install


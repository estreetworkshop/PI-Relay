CC = gcc
TARGET = relay

all: relay.c
	gcc -Wall -o relay relay.c -lwiringPi
	
clean:
	$(RM) $(TARGET)

install:
	install -m 0755 $(TARGET) /usr/bin
	mkdir -p /var/relay
	touch /var/run/relay.pid
	touch /var/log/relay.log
	cp relay.sh /etc/init.d/relay
	update-rc.d relay defaults
	service relay start
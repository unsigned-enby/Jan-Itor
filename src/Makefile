#SCANNER := wayland-scanner

#PREFIX=/usr/local
#BINDIR=$(PREFIX)/bin
#MANDIR=$(PREFIX)/share/man

###Examples
##Cflags
#$(shell pkg-config --cflags pixman-1)
##Libs
#$(shell pkg-config --libs pixman-1)

CXXFLAGS=-Wall
#CFLAGS=-Wall -Werror -Wextra -Wpedantic -Wno-unused-parameter -Wconversion -Wformat-security -Wformat -Wsign-conversion -Wfloat-conversion -Wunused-result
#LIBS=
LDFLAGS=-lftxui-component -lftxui-screen -lftxui-dom
OBJ=DelimDropDown.o InputContainer.o IpFormMenu.o IpForm.o main.o MyCSVMenu.o MyCSV.o SerialMenu.o Serial.o 
CC=color-gcc
CXX=color-g++

main: $(OBJ)
	$(CC) $(LDFLAGS) -o $@ $(OBJ) $(LIBS)

clean:
	$(RM) main $(OBJ) #$(GEN)

.PHONY: clean install


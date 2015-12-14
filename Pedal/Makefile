DOXYFILE = Doxyfile
DOC_DIR = doc

all: compile

.PHONY: all

compile:
	platformio run --target upload

.PHONY: compile

serial:
	picocom -b 115200 /dev/tty.usbmodem1411

.PHONY: serial

doc:
	doxygen $(DOXYFILE)

.PHONY: doc

clean:
	rm -r .pioenvs $(DOC_DIR)

.PHONY: clean

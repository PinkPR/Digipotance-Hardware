DOXYFILE = Doxyfile
DOC_DIR = doc

all: compile doc

compile:
	platformio run --target upload

doc:
	doxygen $(DOXYFILE)

.PHONY: doc

clean:
	rm -r .pioenvs $(DOC_DIR)

.PHONY: clean

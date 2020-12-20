PIO := platformio
BOARD := d1_mini

all: upload

build:
	$(PIO) run

run:
	$(PIO) run --target upload --target monitor --environment d1_mini

clean:
	$(PIO) run --target clean --environment d1_mini

compile_commands.json:
	$(PIO) run -t compiledb
	mv .pio/build/d1_mini/compile_commands.json compile_commands.json

init:
	$(PIO) init --ide emacs --board $(BOARD)
	-rm -f compile_commands.json
	$(MAKE) compile_commands.json

monitor:
	$(PIO) device monitor

upload:
	$(PIO) run --target upload

PIO   := platformio
BOARD := d1_mini
IDE   := vim
CDB   := compile_commands.json

all: upload

build:
	$(PIO) run

run:
	$(PIO) run --target upload --target monitor --environment $(BOARD)

clean:
	$(PIO) run --target clean --environment $(BOARD)
	-rm -f $(CDB)
	-rm -fr .cache
 
$(CDB):  
	$(PIO) run -t compiledb 
	mv .pio/build/$(BOARD)/$(CDB) $(CDB)

init: 
	$(PIO) init --ide $(IDE) --board $(BOARD)
	-rm -f $(CDB)  
	$(MAKE) $(CDB)  

monitor:
	$(PIO) device monitor

upload:
	$(PIO) run --target upload

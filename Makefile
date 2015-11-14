.PHONY: all
all: imaudio sndinfo sndcat sndmix sndgen

.PHONY: install
install: all
	sudo cp bin/* /usr/bin/

.PHONY: uinstall
uinstall:
	sudo rm -rf /usr/bin/sndinfo
	sudo rm -rf /usr/bin/sndcat
	sudo rm -rf /usr/bin/sndmix
	sudo rm -rf /usr/bin/sndgen

.PHONY: imaudio
imaudio:
	make -C ./imaudio/

.PHONY: sndinfo
sndinfo:
	make -C ./sndinfo/

.PHONY: sndcat
sndcat:
	make -C ./sndcat/

.PHONY: sndmix
sndmix:
	make -C ./sndmix/

.PHONY: sndgen
sndgen:
	make -C ./sndgen/

.PHONY: docs
docs:
	rm -rf docs/
	doxygen .Doxyfile

.PHONY: zip
zip:
	# TODO

.PHONY: clean
clean:
	make -C ./imaudio/ clean
	make -C ./sndinfo/ clean
	make -C ./sndcat/ clean
	make -C ./sndmix/ clean
	make -C ./sndgen/ clean
	rm -rf bin/
	rm -rf lib/
	rm -rf docs/

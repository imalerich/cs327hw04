.PHONY: all
all: imaudio sndinfo sndcat sndmix sndgen

.PHONY: imaudio
imaudio:
	make -C ./imaudio/ -j4

.PHONY: sndinfo
sndinfo: imaudio
	make -C ./sndinfo/ -j4

.PHONY: sndcat
sndcat: imaudio
	make -C ./sndcat/ -j4

.PHONY: sndmix
sndmix: imaudio
	make -C ./sndmix/ -j4

.PHONY: sndgen
sndgen: imaudio
	make -C ./sndgen/ -j4

.PHONY: docs
docs:
	rm -rf docs/
	doxygen .Doxyfile

.PHONY: zip
zip: clean docs
	zip immhw04 -r .Doxyfile imaudio Makefile README.md sndcat/ sndgen/ sndinfo/ sndmix/ docs/

.PHONY: zip-nodoc
zip-nodoc: clean
	zip immhw04 -r .Doxyfile imaudio Makefile README.md sndcat/ sndgen/ sndinfo/ sndmix/

.PHONY: install
install: all
	sudo cp bin/* /usr/bin/

.PHONY: uinstall
uinstall:
	sudo rm -rf /usr/bin/sndinfo
	sudo rm -rf /usr/bin/sndcat
	sudo rm -rf /usr/bin/sndmix
	sudo rm -rf /usr/bin/sndgen

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

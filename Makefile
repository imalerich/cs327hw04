all: lib sndinfo sndcat sndmix sndgen
	echo "Binaries available in bin/"

lib:
	make -f ./lib/Makefile

sndinfo:
	make -f ./sndinfo/Makefile

sndcat:
	make -f ./sndcat/Makefile

sndmix:
	make -f ./sndmix/Makefile

sndgen:
	make -f ./sndgen/Makefile

zip:
	echo "Generating .zip for submission"

clean:
	echo "Cleaning All Projects"
	make -f ./lib/Makefile clean
	make -f ./sndinfo/Makefile clean
	make -f ./sndcat/Makefile clean
	make -f ./sndmix/Makefile clean
	make -f ./sndgen/Makefile clean

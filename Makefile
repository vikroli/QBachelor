all: compile clean

compile:
	qmake -makefile -o Makefile_Qt
	make -f Makefile_Qt all
	lrelease src/src.pro
	mv src/*.qm bin/

clean:
	make -f Makefile_Qt clean
	rm -f Makefile_Qt src/Makefile_Qt

distclean: clean
	rm -rf bin

.PHONY: all compile clean distclean

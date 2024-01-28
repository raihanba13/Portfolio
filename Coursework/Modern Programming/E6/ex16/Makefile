FILEDIR=/home/modprog/Exercises/16/

all:
	cd FILEDIR;

%.pdf: %.svg
	convert -resize 300*100 $.svg $.png
	montage -label %t -pointsize 10 -border 3 -tile 8*10 -page A4 $(sort $(wildcard *.svg)) flags.pdf

.PHONY: clean
clean:
    rm -f *


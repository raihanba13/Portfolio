FILEDIR=/home/modprog/Exercises/15/

all:
	cd FILEDIR;

output.pdf: data.tex exercise15.tex data.pdf
	xelatex data.tex exercise15.tex data.pdf

data.tex: data.gp data.dat
	gnuplot data.gp


.PHONY: clean
clean:
    rm -f *.aux *.dvi *.idx *.ilg *.ind *.log *.nav *.out *.snm *.xdv *.toc *~


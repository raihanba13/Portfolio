FILEDIR=/home/modprog/Exercises/14/

all:
	cd FILEDIR;

sum.fst: sum.lisp tst.fas
	clisp -q -c sum.lisp

tst.fas: tst.lisp
	tail -n +2 -c tmp_tst.lisp > tmp_tst.lisp
	clisp -q -c tmp_list.lisp 
	(head -n 1 tst.lisp; cat tmp_tst.fas) > tst.fas 
	rm -f tmp_tst*
	chmod --reference=tst.lisp tst.fas
FILEDIR=/home/modprog/Exercises/13/
GCOM=g++ -Wall -std=c++17 
CCOM=clang++ -Wall -std=c++2a 
OCOM=/usr/local/sw/OracleDeveloperStudio-12.6/developerstudio12.6/bin/CC +W std=c++14

%-gcc: %-gcc-o
	$(GCOM) $*-gcc-o -o $*-gcc

%-gcc-o: %.cc
	$(GCOM) $*-gcc-o -c0 $(FILEDIR)$*.cc

%-clang: %-clang-o
	$(CCOM) $*-clang-o -o $*-clang

%-clang-o: %.cc
	$(CCOM) $*-clang-o -c $(FILEDIR)$*.cc

%-ods: %-ods-o
	$(OCOM) $*-ods-o -o %-ods

%-ods-o: %.cc
	$(OCOM) $*-ods-o -c $(FILEDIR)$*.cc
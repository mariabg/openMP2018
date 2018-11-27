all: nasteroids-par.cpp
	g++ -std=c++14 nasteroids-par.cpp -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -fopenmp -o salida-par.out
clean:
	$(RM) salida-par.out
	$(RM) init_conf.txt
	$(RM) out.txt

# name: Truong-Giang Pham
# Usage:
#    make: compile the program to produce the executable
#    make tar: tar the Makefile and the program
#    make clean: remove the executable
shell: shell.cpp
	g++ shell.cpp -o shell -std=c++11

tar:
	tar -cvf Makefile shell.cpp

clean:
	rm -f shell 

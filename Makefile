shell: shell.cpp
	g++ shell.cpp -o shell -std=c++11

tar:
	tar -cvf Makefile shell.cpp

clean:
	rm -f shell 

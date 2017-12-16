nn: main.o Matrix.o
	g++ -o nn main.o Matrix.o
test_matrix: test_matrix.o Matrix.o
	g++ -o test_matrix test_matrix.o Matrix.o
test_matrix.o: test_matrix.cpp
	g++ -g -Wall -std=c++11 -c test_matrix.cpp
main.o: main.cpp
	g++ -g -Wall -std=c++11 -c main.cpp
Matrix.o: Matrix.cpp
	g++ -g -Wall -std=c++11 -c Matrix.h Matrix.cpp
clean:
	rm nn main.o Matrix.o test_matrix.o
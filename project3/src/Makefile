proj3: main.o bitmap.o Naive_Bayes_Classifier.o
	g++ -o proj3 main.o bitmap.o Naive_Bayes_Classifier.o
main.o: main.cpp
	g++ -g -Wall -std=c++11 -c main.cpp
bitmap.o: bitmap.cpp
	g++ -g -Wall -std=c++11 -c bitmap.cpp
Naive_Bayes_Classifier.o: Naive_Bayes_Classifier.cpp
	g++ -g -Wall -std=c++11 -c Naive_Bayes_Classifier.h Naive_Bayes_Classifier.cpp
clean:
	rm proj3 main.o bitmap.o Naive_Bayes_Classifier.o

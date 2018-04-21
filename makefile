all :
	g++ -g *.cpp -o main -Wall -Werror -std=c++11 

main1 : 
	g++ -g TER.cpp -o main -Wall -Werror -std=c++11 
main2 :
	g++ -g TER2.cpp -o main -Wall -Werror -std=c++11 
 
clean: 
	rm -rf main 
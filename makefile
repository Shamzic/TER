main1 : 
	g++ -g TER.cpp -o main -Wall -Werror -std=c++11 
main2 :
	g++ -g TER2.cpp -o main -Wall -Werror -std=c++11 
couleur :
	g++ -g TER_Couleur.cpp -o main -std=c++11 
roc :
	g++ -g Courbe_ROC.cpp -o main -Wall -Werror -std=c++11 
div :
	g++ -g Division_patchs.cpp -o main -Wall -Werror -std=c++11  

cleanhistos:
	rm -rf histos/*
clean: 
	rm -rf main 

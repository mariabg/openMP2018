#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <fstream>
#include <math.h>
#include <stdio.h>
#include <time.h>


using namespace std;

int main (int argc, char** argv) {
  int nomesirves = argc;
  nomesirves++;
  int format = atoi(argv[1]);
  int tam = atoi(argv[2]);
  int vueltas = atoi(argv[3]);
  vector<double> medias(vueltas);


  if(format==1){
            for(int j =0; j<vueltas;++j){
      double start = clock();
	vector<double> planetas(tam);

      for(unsigned int i =0; i<planetas.size(); ++i){
      	planetas[i]=i;
	      cout << planetas[i] << endl;
      }


    double diff = clock();
    medias[j]=(double)(diff-start)/CLOCKS_PER_SEC;
  }
}
  else{

    for(int j=0;j<vueltas;++j){

	 double start = clock();
	 double *asteroides = new double[tam];

    for( int i =0; i<tam; ++i) {
	asteroides[i]=i;
      cout << asteroides[i] << endl;
    }
	delete[] asteroides;
  double diff = clock();
  medias[j]=(double)(-start+diff)/CLOCKS_PER_SEC;
  }
  }
  double result=0.0;
  for(int i =0; i<vueltas;++i){
    result +=medias[i];
  }
  result= result/vueltas;
  cout << "El programa ha tardado " << result << " segundos\n";

  return 0;
}

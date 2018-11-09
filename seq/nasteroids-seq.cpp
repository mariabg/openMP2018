#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <fstream>
#include <math.h>

using namespace std;

const int WIDTH = 200;
const int HEIGHT = 200;
const int MASS = 1000;
const int SDM = 50;
const double GRAVITY = 6.674*pow(10,-5);
const double TIMEINTERVAL = 0.1;
const double DMIN = 2.0;

struct asteroide {
  double x;
  double y;
  double masa;
  asteroide (){}
  asteroide (double x1, double y1,double m1): x(x1), y(y1), masa(m1) {}
};

struct planeta {
  double x;
  double y;
  double masa;
  planeta (){}
  planeta (double x2, double y2, double m2): x(x2), y(y2), masa(m2) {}
};


// Genera la distribución de asteroides por el plano
void distribucion (int nAsteroides, int nPlanetas, int  semilla, asteroide *listaAsteroides, planeta *listaPlanetas) {

  uniform_real_distribution<double> xdist(0.0, nextafter(WIDTH, numeric_limits<double>::max()));
  uniform_real_distribution<double> ydist(0.0, nextafter(HEIGHT, numeric_limits<double>::max()));
  normal_distribution<double> mdist(MASS, SDM);
  default_random_engine re(semilla);
  for (int i=0; i<nAsteroides; ++i) {
    asteroide aux= asteroide(xdist(re),ydist(re),mdist(re));
    listaAsteroides[i]=aux;
  }
  default_random_engine res(semilla);
  for (int i=0; i<nPlanetas ; ++i) {
    planeta aux;
    if(i%4==0) aux= planeta(0.0,ydist(res),mdist(re)*10);
    else if(i%4==1) aux= planeta(xdist(re),0.0,mdist(res)*10);
    else if(i%4==2) aux= planeta((double)WIDTH,ydist(res),mdist(re)*10);
    else  aux= planeta(xdist(res),(double)HEIGHT,mdist(res)*10);
    listaPlanetas[i]=aux;
  }
}

void archivoInicial (planeta *listaPlanetas, asteroide *listaAsteroides, int nAsteroides, int nIteraciones, int nPlanetas, int semilla) {
  ofstream fs ("init_conf.txt");
  fs.precision(3);
  if (fs.is_open()) {
    fs << fixed << nAsteroides << " "<< nIteraciones << " "<< nPlanetas << " "<< semilla << "\n";
    for (int i=0; i<nAsteroides; i++)
      fs  << fixed << listaAsteroides[i].x << " " <<listaAsteroides[i].y << " " <<listaAsteroides[i].masa << "\n";
    for (int i=0; i<nPlanetas; i++) {
      if (i==nPlanetas-1)
        fs  << fixed << listaPlanetas[i].x << " " <<listaPlanetas[i].y << " " <<listaPlanetas[i].masa;
      else
        fs  << fixed << listaPlanetas[i].x << " " <<listaPlanetas[i].y << " " <<listaPlanetas[i].masa << "\n";
    }
    fs.close();
  } else cout << "Unable to open file";
}

int main (int argc, char** argv) {
  //cout.precision(3);
  cout << argc << endl;
  // Comprobamos que los parámetros se hayan introducido correctamente
  if (argc != 5 || atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0 || atoi(argv[3]) <= 0 || atoi(argv[4]) < 0) {
    cout << "nasteroids-seq: Wrong arguments.\n Correct use:\n nasteroids-seq num_asteroides num_iteraciones num_planetas semilla" << endl;
    return -1;
  } else {
    // Damos nombre a los parámetros de entrada
    int nAsteroides = atoi(argv[1]);
    int nIteraciones = atoi(argv[2]);
    int nPlanetas = atoi(argv[3]);
    unsigned int semilla = atoi(argv[4]);
    cout << "nAsteroides: " << nAsteroides << "\nnIteraciones: " << nIteraciones << "\nnPlanetas: " << nPlanetas << "\nsemilla: " << semilla << endl;

    // 1. Creación de todos los asteroides, y para cada uno obtener su pos X, pos Y y masa.
    asteroide listaAsteroides [nAsteroides];

    // 2. Creación de todos los planetas, y para cada uno obtener su pos X, pos Y y masa
    planeta listaPlanetas [nPlanetas];
    distribucion(nAsteroides,nPlanetas, semilla, listaAsteroides, listaPlanetas);

    // Escribimos el fichero con los valores iniciales
    archivoInicial(listaPlanetas, listaAsteroides, nAsteroides, nIteraciones, nPlanetas, semilla);

    // 3. Bucle de iteraciones:
      // - Calculo de todas las fuerzas que afectan a todos los asteroides (calcular primero las fuerzas del asteroide "i" con el resto de asteroides y luego con el resto de planetas).
        // 1. Distancias
        double distanciasAsteroides [nAsteroides][nAsteroides];
        double pendiente [nAsteroides][nAsteroides];
        for(unsigned int i= 0; i < nAsteroides; i++) {
          distanciasAsteroides[i][i] = 0.0;
          for(unsigned int j= i+1; j < nAsteroides; j++) {
              distanciasAsteroides[i][j] = pow(pow(listaAsteroides[i].x - listaAsteroides[j].x, 2) + pow(listaAsteroides[i].x - listaAsteroides[j].x, 2), 0.5);
              distanciasAsteroides[j][i] = distanciasAsteroides[i][j];
              // 2. Movimiento normal
              if (distanciasAsteroides[i][j]>2) {
                  // Calcular pendiente
                  pendiente[i][j] = (distanciasAsteroides[i].y - distanciasAsteroides[j].y) / (distanciasAsteroides[i].x - distanciasAsteroides[j].x);
                  pendiente[j][i] = pendiente[i][j];
                  if(pendiente[i][j] > 1 || pendiente[i][j] < -1){

                  }
              } else {
                pendiente[i][j] = 0.0;
                pendiente[j][i] = 0.0;
              }
              //a. Ángulo de influencia
              if (pendiente[i][j]>) {

              }
          }
        }
          //b. Fuerza de atracción
          //c. Aplicar fuerzas a asteroides

      // - Evaluar rebote con los bordes del espacio.
      // - Evaluar rebote entre asteroides.
  }

  return 0;
}

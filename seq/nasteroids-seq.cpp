#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <fstream>

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
  default_random_engine re(semilla);
  uniform_real_distribution<double> xdist(0.0, std::nextafter(WIDTH, std :: numeric_limits<double>::max()));
  uniform_real_distribution<double> ydist(0.0, std::nextafter(HEIGHT, std :: numeric_limits<double>::max()));
  normal_distribution<double> mdist(MASS, SDM);
  for (int i=0; i<nAsteroides; ++i) {
    asteroide aux= asteroide(xdist(re),ydist(re),mdist(re));
    listaAsteroides[i]=aux;
  }
  for (int i=0; i<nPlanetas ; ++i) {
    planeta aux;
    if(i%4==0) aux= planeta(0.0,ydist(re),mdist(re)*10);
    else if(i%4==1) aux= planeta(xdist(re),0.0,mdist(re)*10);
    else if(i%4==2) aux= planeta((double)WIDTH,ydist(re),mdist(re)*10);
    else  aux= planeta(xdist(re),(double)HEIGHT,mdist(re)*10);
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
    int semilla = atoi(argv[4]);
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
        //for()
        // 2. Movimiento normal
          //a. Ángulo de influencia
          //b. Fuerza de atracción
          //c. Aplicar fuerzas a asteroides

      // - Evaluar rebote con los bordes del espacio.
      // - Evaluar rebote entre asteroides.
  }

  return 0;
}

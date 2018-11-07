#include <iostream>
#include <chrono>
#include <vector>
#include <random>

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

  asteroide (double x1, double y1,double m1): x(x1), y(y1), masa(m1) {}
};

struct planeta {
  double x;
  double y;
  double masa;

  planeta (double x2, double y2, double m2): x(x2), y(y2), masa(m2) {}
};

// Genera la distribución de asteroides por el plano
asteroide * distribucionAsteroides (int nAsteroides, int seed) {
  default_random_engine re(seed);
  uniform_real_distribution<double> xdist(0.0, std::nextafter(WIDTH, std :: numeric_limits<double>::max()));
  uniform_real_distribution<double> ydist(0.0, std::nextafter(HEIGHT, std :: numeric_limits<double>::max()));
  normal_distribution<double> mdist(MASS, SDM);
  asteroide * listaAsteroides;
  for (int i=0; i<nAsteroides ; ++i) {
    asteroide aux= asteroide(xdist(re),ydist(re),mdist(re));
    listaAsteroides[i]=aux;

  }
  for (int i=0; i<nAsteroides; i++){
    cout << "45, it, " << i << &listaAsteroides[i] << endl;
  }
  cout << endl;

  return listaAsteroides;
}

// Genera la distribución de planetas por el plano
planeta * distribucionPlanetas (int nPlanetas, int seed) {
  //Los planetas se deben situar en los bordes del espacio generado, comenzando en el eje izquierdo
  //(x = 0), el segundo en el superior (y = 0), el tercero en el borde derecho, el cuarto en el inferior,
  //etc. La segunda coordenada seguirá, al igual que los asteroides, una distribución aleatoria con la
  //misma semilla. En cuanto a su masa, se calcula igual que la de los asteroides, pero multiplicada
  //por un valor constante de 10.
  default_random_engine re(seed);
  uniform_real_distribution<double> xdist(0.0, std::nextafter(WIDTH, std :: numeric_limits<double>::max()));
  uniform_real_distribution<double> ydist(0.0, std::nextafter(HEIGHT, std :: numeric_limits<double>::max()));
  normal_distribution<double> mdist(MASS*10, SDM);
  planeta * listaPlanetas;
  for (int i=0; i<nPlanetas ; ++i) {
    planeta aux= planeta(0,0,mdist(re));
    listaPlanetas[i]=aux;
    cout << "67, it:" << i << endl;
  }
  for (int i=0; i<nPlanetas; i++){
    cout << "70, it:" << i << &listaPlanetas[i] << endl;
  }
  cout << endl;

  return listaPlanetas;
}

int guardarArchivoInicial () {
  return 0;
}

int main (int argc, char** argv) {
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
    asteroide * listaAsteroides = nullptr;
    listaAsteroides = distribucionAsteroides(nAsteroides, semilla);

    for (int i=0; i<nAsteroides; i++){
      cout << "98, it:" << i << &listaAsteroides[i] << endl;
    }
    cout << endl;
    // 2. Creación de todos los planetas, y para cada uno obtener su pos X, pos Y y masa
    planeta * listaPlanetas = nullptr;
    listaPlanetas = distribucionPlanetas(nPlanetas, semilla);

    for (int i=0; i<nPlanetas; i++){
      cout << "106, it:" << i << &listaPlanetas[i] << endl;
    }

    // 3. Bucle de iteraciones:
      // - Calculo de todas las fuerzas que afectan a todos los asteroides (calcular primero las fuerzas del asteroide "i" con el resto de asteroides y luego con el resto de planetas).
      // - Movimiento de todos los asteroides: partiendo de la fuerza total, calcular la nueva posición.
      // - Evaluar rebote con los bordes del espacio.
      // - Evaluar rebote entre asteroides.
  }

  return 0;
}

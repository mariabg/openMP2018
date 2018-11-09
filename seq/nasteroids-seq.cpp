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
  double velocidad;
  double aceleracion;
  asteroide (){}
  asteroide (double x1, double y1,double m1): x(x1), y(y1), masa(m1), velocidad(v), aceleracion(a) {}
};

struct planeta {
  double x;
  double y;
  double masa;
  planeta (){}
  planeta (double x2, double y2, double m2): x(x2), y(y2), masa(m2) {}
};


// Genera la distribución de asteroides por el plano
void distribucion (int nAsteroides, int nPlanetas, unsigned int  semilla, asteroide *listaAsteroides, planeta *listaPlanetas) {

  uniform_real_distribution<double> xdist{0.0, nextafter(WIDTH, numeric_limits<double>::max())};
  uniform_real_distribution<double> ydist{0.0, nextafter(HEIGHT, numeric_limits<double>::max())};
  normal_distribution<double> mdist{MASS, SDM};
  default_random_engine re{semilla};
  for (unsigned int i=0; i<nAsteroides; ++i) {
    asteroide aux= asteroide(xdist(re), ydist(re), mdist(re), 0, 0);
    listaAsteroides[i]=aux;
  }
  for (unsigned int i=0; i<nPlanetas ; ++i) {
    planeta aux;
    if(i%4==0) aux= planeta(0.0,ydist(re),mdist(re)*10);
    else if(i%4==1) aux= planeta(xdist(re),0.0,mdist(re)*10);
    else if(i%4==2) aux= planeta(WIDTH,ydist(re),mdist(re)*10);
    else  aux= planeta(xdist(re),HEIGHT,mdist(re)*10);
    listaPlanetas[i]=aux;
  }
}

void archivoInicial (planeta *listaPlanetas, asteroide *listaAsteroides, int nAsteroides, int nIteraciones, int nPlanetas, int semilla) {
  ofstream fs ("init_conf.txt");
  fs.precision(3);
  if (fs.is_open()) {
    fs << fixed << nAsteroides << " "<< nIteraciones << " "<< nPlanetas << " "<< semilla << "\n";
    for (unsigned int i=0; i<nAsteroides; i++)
      fs  << fixed << listaAsteroides[i].x << " " <<listaAsteroides[i].y << " " <<listaAsteroides[i].masa << "\n";
    for (unsigned int i=0; i<nPlanetas; i++) {
      if (i==nPlanetas-1)
        fs  << fixed << listaPlanetas[i].x << " " <<listaPlanetas[i].y << " " <<listaPlanetas[i].masa;
      else
        fs  << fixed << listaPlanetas[i].x << " " <<listaPlanetas[i].y << " " <<listaPlanetas[i].masa << "\n";
    }
    fs.close();
  } else cout << "Unable to open file";
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
    unsigned int semilla = atoi(argv[4]);
    cout << "nAsteroides: " << nAsteroides << "\nnIteraciones: " << nIteraciones << "\nnPlanetas: " << nPlanetas << "\nsemilla: " << semilla << endl;

    // I. Creación de todos los asteroides, y para cada uno obtener su pos X, pos Y y masa.
    asteroide listaAsteroides [nAsteroides];

    // II. Creación de todos los planetas, y para cada uno obtener su pos X, pos Y y masa
    planeta listaPlanetas [nPlanetas];
    distribucion(nAsteroides,nPlanetas, semilla, listaAsteroides, listaPlanetas);

    // Escribimos el fichero con los valores iniciales
    archivoInicial(listaPlanetas, listaAsteroides, nAsteroides, nIteraciones, nPlanetas, semilla);

    // Guardar todas las posiciones al step by step

    // III. Bucle de iteraciones:
    for (unsigned int i = 0; i < nIteraciones; ++i) {
      // 0. Calculo de todas las fuerzas que afectan a todos los asteroides (calcular primero las fuerzas del asteroide "i" con el resto de asteroides y luego con el resto de planetas).
      double distanciasAsteroides [nAsteroides][nAsteroides];
      double pendiente [nAsteroides][nAsteroides];
      double angulos [nAsteroides][nAsteroides];
      double fuerzasX [nAsteroides];
      double fuerzasY [nAsteroides];

      for (unsigned int i=0; i < nAsteroides; ++i) {
        for (unsigned int j=i+1; j < nAsteroides; ++j) {
          // 1. Distancias
          distanciasAsteroides[i][j] = pow(pow(listaAsteroides[i].x - listaAsteroides[j].x, 2) + pow(listaAsteroides[i].x - listaAsteroides[j].x, 2), 0.5);
          distanciasAsteroides[j][i] = distanciasAsteroides[i][j];
          // 2. Movimiento normal
          // 2.1. Ángulo de influencia
          // 2.1.a. Pendiente
          if (distanciasAsteroides[i][j]>2) {
              pendiente[i][j] = (listaAsteroides[i].y - listaAsteroides[j].y) / (listaAsteroides[i].x - listaAsteroides[j].x);
              pendiente[j][i] = pendiente[i][j];
          } else {
            pendiente[i][j] = 0.0;
            pendiente[j][i] = 0.0;
          }
          // 2.1.b. Correción de la pendiente
          if (pendiente[i][j] > 1 || pendiente[i][j] < -1) {
            pendiente[i][j] = pendiente[i][j] − ((int)(pendiente[i][j])/1);
            pendiente[j][i] = pendiente[i][j];
          }
          // 2.1.c. Cálculo del ángulo
          angulos[i][j] = atan(pendiente[i][j]);
          angulos[j][i] = angulos[i][j];

          // 2.2. Fuerzas de atracción
          fx = ((GRAVITY * listaAsteroides[i].masa * listaAsteroides[j].masa)/ pow(distanciasAsteroides[i][j], 2)) * cos(angulo[i][j]);
          fy = ((GRAVITY * listaAsteroides[i].masa * listaAsteroides[j].masa)/ pow(distanciasAsteroides[i][j], 2)) * sin(angulo[i][j]);
          fx = ((fx > 200) ? 200 : fx);
          fy = ((fy > 200) ? 200 : fy);
          fuerzasX[i] += fx;
          fuerzasY[i] += fy;
          fuerzasX[j] -= fx;
          fuerzasY[j] -= fy;

          // Guardar todas las fuerzas al step by step
        }
        for (unsigned int j=0; j < nPlanetas; ++j) {

        }
        // Guardar todas las fuerzas al step by step
      }

      for (usigned int i = 0; i < nAsteroides; ++i) {
    // A PARTIR DE AQUI ESTA TODO MAL :3
        aceleracionFuerzaX = sum(fx/masa)
        aceleracionFuerzaY = sum(fy/masa)
        if (i == 0) {
          vx = 0;
          vy = 0;
        } else {
          vx = velocidadX[i-1][j-1] listaAsteroides[i].aceleracion;
          vy = velocidadY[i-1][j-1] listaAsteroides[i].aceleracion;
        }
        velocidadX[i][j] = v + aceleracionFuerzaX[i][j] * ¿incremento de T?;
        velocidadY[i][j] = v + aceleracionFuerzaY[i][j] * ¿incremento de T?;
         = velocidadX[i][j];
         = velocidadY[i][j];
        listaAsteroides[i].x += vx * ¿incremento de T?;
        listaAsteroides[i].y += vy * ¿incremento de T?;
        listaAsteroides[j].x -= vx * ¿incremento de T?;
        listaAsteroides[j].y -= vy * ¿incremento de T?;

        // 3. Evaluar rebote
         // 3.1. Rebote con los bordes del espacio.
         // 3.2. Rebote entre asteroides.
      }

    }
    }
  }
  return 0;
}

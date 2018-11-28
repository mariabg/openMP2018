#include <iostream>
// #include <chrono>
#include <vector>
#include <random>
#include <fstream>
#include <math.h>
#include <stdio.h>
using namespace std;

// Constantes predefinidas en el enunciado del problema
const int WIDTH = 200;
const int HEIGHT = 200;
const int MASS = 1000;
const int SDM = 50;
const double GRAVITY = 6.674*pow(10,-5);
const double TIMEINTERVAL = 0.1;
const int DMIN = 2;

struct asteroide {
  double x;
  double y;
  double masa;
  double velocidad[2] = {0, 0};
  double aceleracion[2] = {0, 0};
  asteroide (){}
  asteroide (double x1, double y1, double m1): x(x1), y(y1), masa(m1) {}
};

struct planeta {
  double x;
  double y;
  double masa;
  planeta (){}
  planeta (double x2, double y2, double m2): x(x2), y(y2), masa(m2) {}
};


// Genera la distribución de asteroides y planetas por el plano
void distribucion (int nAsteroides, int nPlanetas, int semilla, asteroide *listaAsteroides, planeta *listaPlanetas) {
  uniform_real_distribution<double> xdist{0.0, nextafter(WIDTH, numeric_limits<double>::max())};
  uniform_real_distribution<double> ydist{0.0, nextafter(HEIGHT, numeric_limits<double>::max())};
  normal_distribution<double> mdist{MASS, SDM};
  default_random_engine re(semilla);

  for (int i = 0; i < nAsteroides; ++i) {
    double x = xdist(re);
    double y = ydist(re);
    double m = mdist(re);
    asteroide aux = asteroide(x, y, m);
    listaAsteroides[i] = aux;
  }

  // Establecemos cada planeta en uno de los ejes del espacio de manera ordenada
  for (int i = 0; i < nPlanetas ; ++i) {
    planeta aux;
    if (i % 4 == 0) {
      double y = ydist(re);
      double m = mdist(re);
      aux = planeta(0.0, y, m*10);
    }
    else if (i % 4 == 1) {
      double x = xdist(re);
      double m = mdist(re);
      aux = planeta(x, 0.0, m*10);
    }
    else if (i % 4 == 2) {
      double y = ydist(re);
      double m = mdist(re);
      aux = planeta(WIDTH, y, m*10);
    }
    else {
      double x = xdist(re);
      double m = mdist(re);
      aux = planeta(x, HEIGHT, m*10);
    }
    listaPlanetas[i] = aux;
  }
}

//Generador del archivo de inicialización
void archivoInicial (planeta *listaPlanetas, asteroide *listaAsteroides, int nAsteroides, int nIteraciones, int nPlanetas, unsigned int semilla) {
  ofstream fs ("init_conf.txt");
  fs.precision(3);
  if (fs.is_open()) {
    fs << fixed << nAsteroides << " " << nIteraciones << " " << nPlanetas << " " << semilla << "\n";
    for (int i = 0; i < nAsteroides; i++) {
      fs << fixed << listaAsteroides[i].x << " " << listaAsteroides[i].y << " " << listaAsteroides[i].masa << "\n";
    }
    for (int i = 0; i < nPlanetas; i++) {
      fs << fixed << listaPlanetas[i].x << " " << listaPlanetas[i].y << " " << listaPlanetas[i].masa << "\n";
    }
    fs.close();
  }
  else {
    cout << "No se ha podido abrir el archivo";
  }
}

//Generador del archivo de resultados
void archivoFinal (asteroide *listaAsteroides, int nAsteroides) {
  ofstream fs ("out.txt");
  fs.precision(3);
  if (fs.is_open()) {
    for (int i = 0; i < nAsteroides; i++) {
      fs << fixed << listaAsteroides[i].x << " " << listaAsteroides[i].y << " " << listaAsteroides[i].velocidad[0] << " " << listaAsteroides[i].velocidad[1] << " " << listaAsteroides[i].masa << "\n";
    }
    fs.close();
  }
  else {
    cout << "No se ha podido abrir el archivo";
  }
}

int main (int argc, char** argv) {
  // Las variables que cronometran el tiempo de ejecución están comentadas en todos los ficheros
  // auto start = std::chrono::system_clock::now();

  // Comprobación de los parámetros introducidos
  if (argc != 5 || atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0 || atoi(argv[3]) <= 0 || atoi(argv[4]) < 0) {
    cout << "nasteroids-seq: Argumentos incorrectos.\n El uso correcto es:\n nasteroids-seq num_asteroides num_iteraciones num_planetas semilla" << endl;
    return -1;
  }
  // Asignación de nombre a los parámetros de entrada
  int nAsteroides = atoi(argv[1]);
  int nIteraciones = atoi(argv[2]);
  int nPlanetas = atoi(argv[3]);
  unsigned int semilla = atoi(argv[4]);

  // I. Creación de todos los asteroides, y para cada uno obtención su posX, posY y masa
  asteroide *listaAsteroides = new asteroide[nAsteroides];

  // II. Creación de todos los planetas, y para cada uno obtención su posX, posY y masa
  planeta *listaPlanetas = new planeta[nPlanetas];
  distribucion(nAsteroides,nPlanetas, semilla, listaAsteroides, listaPlanetas);

  // Escritura en el fichero de los valores iniciales
  archivoInicial(listaPlanetas, listaAsteroides, nAsteroides, nIteraciones, nPlanetas, semilla);

  // Declaración de los arrays dinámicos
  double **distanciasAsteroides = new double*[nAsteroides];
  double **distanciasAstPlanetas = new double*[nAsteroides];
  double **pendienteAsteroides = new double*[nAsteroides];
  double **pendienteAstPlanetas = new double*[nAsteroides];
  double **angulosAsteroides = new double*[nAsteroides];
  double **angulosAstPlanetas = new double*[nAsteroides];

  // Inicialización de arrays dinámicos
  for (int i = 0; i < nAsteroides; ++i){
    distanciasAsteroides[i] = new double[nAsteroides];
    distanciasAstPlanetas[i] = new double[nPlanetas];
    pendienteAsteroides[i] = new double[nAsteroides];
    pendienteAstPlanetas[i]= new double[nPlanetas];
    angulosAsteroides[i] = new double[nAsteroides];
    angulosAstPlanetas[i] = new double[nPlanetas];
  }

  // III. Bucle de iteraciones
  for (int t = 0; t < nIteraciones; ++t) {
    // 0. Calculo de todas las fuerzas que afectan a todos los asteroides (calcular primero las fuerzas del asteroide "i" con el resto de asteroides y luego con el resto de planetas).
    double *fuerzasX = new double[nAsteroides];
    double *fuerzasY = new double[nAsteroides];
    for (int i = 0; i < nAsteroides; ++i) {
      double fx;
      double fy;

      // Calculamos fuerzas asteroides-asteroidess
      for (int j = i+1; j < nAsteroides; ++j) {
        // 1. Distancias
        distanciasAsteroides[i][j] = pow(pow(listaAsteroides[i].x - listaAsteroides[j].x, 2) + pow(listaAsteroides[i].y - listaAsteroides[j].y, 2), 0.5);
        // 2. Movimiento normal
        // 2.1. Ángulo de influencia
        // 2.1.a. pendienteAsteroides
        if (distanciasAsteroides[i][j] > DMIN) {
          pendienteAsteroides[i][j] = (listaAsteroides[i].y - listaAsteroides[j].y) / (listaAsteroides[i].x - listaAsteroides[j].x);
          // 2.1.b. Corrección de la pendienteAsteroides
          if (pendienteAsteroides[i][j] > 1 || pendienteAsteroides[i][j] < -1) {
            pendienteAsteroides[i][j] = pendienteAsteroides[i][j] - ((int)(pendienteAsteroides[i][j])/1);
          }
          // 2.1.c. Cálculo del ángulo
          angulosAsteroides[i][j] = atan(pendienteAsteroides[i][j]);
          // 2.2. Fuerzas de atracción
          double f = ((GRAVITY * listaAsteroides[i].masa * listaAsteroides[j].masa)/ pow(distanciasAsteroides[i][j], 2));
          f = ((f > 200) ? 200 : f);
          fx = f*cos(angulosAsteroides[i][j]);
          fy = f* sin(angulosAsteroides[i][j]);

          //Almacenamos las fuerzas que ejercen unos asteroides sobre otros para poder sumarlos posteriormente
          fuerzasX[i] += fx;
          fuerzasY[i] += fy;
          fuerzasX[j] -= fx;
          fuerzasY[j] -= fy;
        }
      }

      // Cálculo de las fuerzas asteroides-planetas
      for (int j = 0; j < nPlanetas; ++j) {
        // 1. Distancias
        distanciasAstPlanetas[i][j] = pow(pow(listaAsteroides[i].x - listaPlanetas[j].x, 2) + pow(listaAsteroides[i].y - listaPlanetas[j].y, 2), 0.5);
        // 2. Movimiento normal
        // 2.1. Ángulo de influencia
        // 2.1.a. pendienteAsteroides
        if (distanciasAstPlanetas[i][j] > DMIN) {
            pendienteAstPlanetas[i][j] = (listaAsteroides[i].y - listaPlanetas[j].y) / (listaAsteroides[i].x - listaPlanetas[j].x);
            // 2.1.b. Corrección de la pendienteAsteroides
            if (pendienteAstPlanetas[i][j] > 1 || pendienteAstPlanetas[i][j] < -1) {
              pendienteAstPlanetas[i][j] = pendienteAstPlanetas[i][j] - ((int)(pendienteAstPlanetas[i][j])/1);
            }
            // 2.1.c. Cálculo del ángulo
            angulosAstPlanetas[i][j] = atan(pendienteAstPlanetas[i][j]);

            // 2.2. Fuerzas de atracción
            double f = ((GRAVITY * listaAsteroides[i].masa * listaPlanetas[j].masa)/ pow(distanciasAstPlanetas[i][j], 2));
            f = ((f > 200) ? 200 : f);
            fx = f*cos(angulosAstPlanetas[i][j]);
            fy = f* sin(angulosAstPlanetas[i][j]);
        }
        else {
          fx = 0;
          fy = 0;
        }
        fuerzasX[i] += fx;
        fuerzasY[i] += fy;
      }
    }

    // Cálculo de las nuevas velocidades, aceleraciones, rebotes y choques
    for (int i = 0; i < nAsteroides; ++i) {
      listaAsteroides[i].aceleracion[0] = fuerzasX[i]/listaAsteroides[i].masa;
      listaAsteroides[i].aceleracion[1] = fuerzasY[i]/listaAsteroides[i].masa;
      listaAsteroides[i].velocidad[0] = listaAsteroides[i].velocidad[0] + listaAsteroides[i].aceleracion[0]*TIMEINTERVAL;
      listaAsteroides[i].velocidad[1] = listaAsteroides[i].velocidad[1] + listaAsteroides[i].aceleracion[1]*TIMEINTERVAL;

      listaAsteroides[i].x = listaAsteroides[i].x + listaAsteroides[i].velocidad[0]*TIMEINTERVAL;
      listaAsteroides[i].y = listaAsteroides[i].y + listaAsteroides[i].velocidad[1]*TIMEINTERVAL;

      // 3. Evaluar rebote
      // 3.1. Rebote con los bordes del espacio.
      if (listaAsteroides[i].x <= 0) {
        listaAsteroides[i].x = 2;
        listaAsteroides[i].velocidad[0] = -listaAsteroides[i].velocidad[0];
      }
      if (listaAsteroides[i].y <= 0) {
        listaAsteroides[i].y = 2;
        listaAsteroides[i].velocidad[1] = -listaAsteroides[i].velocidad[1];
      }
      if (listaAsteroides[i].x >= WIDTH) {
        listaAsteroides[i].x = WIDTH - 2;
        listaAsteroides[i].velocidad[0] = -listaAsteroides[i].velocidad[0];
      }
      if (listaAsteroides[i].y >= HEIGHT) {
        listaAsteroides[i].y = HEIGHT - 2;
        listaAsteroides[i].velocidad[1] = -listaAsteroides[i].velocidad[1];
      }

      // 3.2. Rebote entre asteroides.
      int parar = 0;
      for (int j = i+1; j < nAsteroides; ++j) {
        if (parar == 0 && distanciasAsteroides[i][j] <= DMIN) {
          double swap = listaAsteroides[i].velocidad[0];
          listaAsteroides[i].velocidad[0] = listaAsteroides[j].velocidad[0];
          listaAsteroides[j].velocidad[0] = swap;
          swap = listaAsteroides[i].velocidad[1];
          listaAsteroides[i].velocidad[1] = listaAsteroides[j].velocidad[1];
          listaAsteroides[j].velocidad[1] = swap;
          parar = 1;
        }
      }
    }
  }

  archivoFinal(listaAsteroides,nAsteroides);
  // auto end = chrono::system_clock::now();
  // auto diff = chrono::duration_cast<chrono::microseconds>(end-start);
  //cout << "El programa ha tardado " << diff.count() << " microsegundos\n";

  // Eliminamos memoria
  for (int i = 0; i < nAsteroides; ++i) {
    delete [] distanciasAsteroides[i];
    delete [] distanciasAstPlanetas[i];
    delete [] pendienteAsteroides[i];
    delete [] pendienteAstPlanetas[i];
    delete [] angulosAsteroides[i];
    delete [] angulosAstPlanetas[i];
  }
  delete [] distanciasAsteroides;
  delete [] distanciasAstPlanetas;
  delete [] pendienteAsteroides;
  delete [] pendienteAstPlanetas;
  delete [] angulosAsteroides;
  delete [] angulosAstPlanetas;

  return 0;
}

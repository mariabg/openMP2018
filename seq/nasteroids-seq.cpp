#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <fstream>
#include <math.h>
#include <stdio.h>
using namespace std;

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
  double  velocidad[2]={0, 0};
  double  aceleracion[2]={0, 0};
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
void distribucion (int nAsteroides, int nPlanetas, int semilla, asteroide *listaAsteroides, planeta *listaPlanetas) {
  uniform_real_distribution<double> xdist{0.0, nextafter(WIDTH, numeric_limits<double>::max())};
  uniform_real_distribution<double> ydist{0.0, nextafter(HEIGHT, numeric_limits<double>::max())};
  normal_distribution<double> mdist{MASS, SDM};
  default_random_engine re(semilla);
  for (int i=0; i<nAsteroides; ++i) {
      double x = xdist(re);
      double y = ydist(re);
      double m = mdist(re);
      //cout<<"X: "<< x << " Y:  " <<y <<" M: "<< m<<endl;

    asteroide aux= asteroide(x, y, m);
    listaAsteroides[i]=aux;
  }
  for (int i=0; i<nPlanetas ; ++i) {
    planeta aux;
    if(i%4==0) {
      double y = ydist(re);
      double m = mdist(re);
      aux= planeta(0.0,y,m*10);

    }
    else if(i%4==1) {
      double x = xdist(re);
      double m = mdist(re);
      aux= planeta(x,0.0,m*10);
    }
    else if(i%4==2) {
      double y = ydist(re);
      double m = mdist(re);
      aux= planeta(WIDTH,y,m*10);
    }
    else {
      double x = xdist(re);
      double m = mdist(re);
      aux= planeta(x,HEIGHT,m*10);
    }
    listaPlanetas[i]=aux;
  }
}

void archivoInicial (planeta *listaPlanetas, asteroide *listaAsteroides, int nAsteroides, int nIteraciones, int nPlanetas, unsigned int semilla) {
  ofstream fs ("init_conf.txt");
  fs.precision(3);
  if (fs.is_open()) {
    fs << fixed << nAsteroides << " "<< nIteraciones << " "<< nPlanetas << " "<< semilla << "\n";
    for (int i=0; i<nAsteroides; i++)
      fs  << fixed << listaAsteroides[i].x << " " <<listaAsteroides[i].y << " " <<listaAsteroides[i].masa << "\n";
    for (int i=0; i<nPlanetas; i++) {
        fs  << fixed << listaPlanetas[i].x << " " <<listaPlanetas[i].y << " " <<listaPlanetas[i].masa << "\n";
    }
    fs.close();
  } else cout << "Unable to open file";
}
//TO DO:Cambiar nombre del archivo
void archivoFinal (asteroide *listaAsteroides, int nAsteroides) {
  ofstream fs ("nuestroOut.txt");
  fs.precision(3);
  if (fs.is_open()) {
    for (int i=0; i<nAsteroides; i++)
      fs  << fixed << listaAsteroides[i].x << " " <<listaAsteroides[i].y << " " << listaAsteroides[i].velocidad[0] << " " <<listaAsteroides[i].velocidad[1] << " " << listaAsteroides[i].masa << "\n";
    fs.close();
  } else cout << "Unable to open file";
}

int main (int argc, char** argv) {
  auto start = std::chrono::system_clock::now();
  // Comprobamos que los parámetros se hayan introducido correctamente
  if (argc != 5 || atoi(argv[1]) <= 0 || atoi(argv[2]) <= 0 || atoi(argv[3]) <= 0 || atoi(argv[4]) < 0) {
    cout << "nasteroids-seq: Wrong arguments.\n Correct use:\n nasteroids-seq num_asteroides num_iteraciones num_planetas semilla" << endl;
    return -1;
  }
  // Damos nombre a los parámetros de entrada
  int nAsteroides = atoi(argv[1]);
  int nIteraciones = atoi(argv[2]);
  int nPlanetas = atoi(argv[3]);
  unsigned int semilla = atoi(argv[4]);
  cout << "nAsteroides: " << nAsteroides << "\nnIteraciones: " << nIteraciones << "\nnPlanetas: " << nPlanetas << "\nsemilla: " << semilla << endl;

  // I. Creación de todos los asteroides, y para cada uno obtener su pos X, pos Y y masa.
  // asteroide listaAsteroides [];
  asteroide *listaAsteroides = new asteroide[nAsteroides];

  // II. Creación de todos los planetas, y para cada uno obtener su pos X, pos Y y masa
  planeta *listaPlanetas = new planeta[nPlanetas];
  distribucion(nAsteroides,nPlanetas, semilla, listaAsteroides, listaPlanetas);

  // Escribimos el fichero con los valores iniciales
  archivoInicial(listaPlanetas, listaAsteroides, nAsteroides, nIteraciones, nPlanetas, semilla);

  // Declarar los arrays dinamicos
  double **distanciasAsteroides = new double*[nAsteroides];
  double **distanciasAstPlanetas = new double*[nAsteroides];
  double **pendienteAsteroides = new double*[nAsteroides];
  double **pendienteAstPlanetas = new double*[nAsteroides];
  double **angulosAsteroides = new double*[nAsteroides];
  double **angulosAstPlanetas = new double*[nAsteroides];

  for(int i = 0; i<nAsteroides; ++i){
    distanciasAsteroides[i] = new double[nAsteroides];
    distanciasAstPlanetas[i] = new double[nPlanetas];
    pendienteAsteroides[i] = new double[nAsteroides];
    pendienteAstPlanetas[i]= new double[nPlanetas];
    angulosAsteroides[i] = new double[nAsteroides];
    angulosAstPlanetas[i] = new double[nPlanetas];
  }

  // III. Bucle de iteraciones:
  for (int t = 0; t < nIteraciones; ++t) {
    // 0. Calculo de todas las fuerzas que afectan a todos los asteroides (calcular primero las fuerzas del asteroide "i" con el resto de asteroides y luego con el resto de planetas).
    // cout << "\n\nITERACION " << t << endl;
    // cout << "\n Asteroides vs Asteroides"<<endl;
    double *fuerzasX = new double[nAsteroides];
    double *fuerzasY = new double[nAsteroides];
    for (int i=0; i < nAsteroides; ++i) {
      double fx;
      double fy;
      // Calculamos fuertzas asteroides-asteroidess
      for (int j=i+1; j < nAsteroides; ++j) {
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
          //fx = ((GRAVITY * listaAsteroides[i].masa * listaAsteroides[j].masa)/ pow(distanciasAsteroides[i][j], 2)) * cos(angulosAsteroides[i][j]);
          //fy = ((GRAVITY * listaAsteroides[i].masa * listaAsteroides[j].masa)/ pow(distanciasAsteroides[i][j], 2)) * sin(angulosAsteroides[i][j]);
          double f = ((GRAVITY * listaAsteroides[i].masa * listaAsteroides[j].masa)/ pow(distanciasAsteroides[i][j], 2));

          f = ((f > 200) ? 200 : f);
          fx = f*cos(angulosAsteroides[i][j]);
          fy = f* sin(angulosAsteroides[i][j]);

          //cout << i << " "<<j<<" "<<pow(pow(fx,2)+pow(fy,2),0.5)<<" "<<angulosAsteroides[i][j]<<endl;
          fuerzasX[i] += fx;
          fuerzasY[i] += fy;
          fuerzasX[j] -= fx;
          fuerzasY[j] -= fy;
        }
      }

      // Calculamos fuertzas asteroides-planetas
      // cout << "\n Asteroides vs Planetas"<<endl;

      for (int j=0; j < nPlanetas; ++j) {
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
            
        } else {
          fx=0;
          fy=0;
        }
        // cout << i << " " << j << " " << pow(pow(fx,2)+pow(fy,2),0.5) << " " << angulosAstPlanetas[i][j] << endl;
        fuerzasX[i] += fx;
        fuerzasY[i] += fy;
      }
    }

    // CÁLCULO DE COLISIONES
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
       // TO DO: Verificar que esto funciona y que se puedan usar breaks
       int parar= 0;
       for (int j=i+1; j < nAsteroides; ++j) {

         if(parar==0 && distanciasAsteroides[i][j] <= DMIN) {
           //TO DO quitar cout
           // cout <<"\nHa chocado " <<i <<" con " << j <<" con distancia   " << distanciasAsteroides[i][j] << endl;
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
  auto end = chrono::system_clock::now();
  auto diff = chrono::duration_cast<chrono::microseconds>(end-start);
  cout << "El programa ha tardado " << diff.count() << " microsegundos\n";
  return 0;
}

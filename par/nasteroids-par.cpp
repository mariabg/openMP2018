#include <iostream>
#include <chrono>
#include <vector>
#include <random>
#include <fstream>
#include <math.h>
#include <omp.h>

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
void distribucion (int nAsteroides, int nPlanetas, int  semilla, asteroide *listaAsteroides, planeta *listaPlanetas) {

  uniform_real_distribution<double> xdist{0.0, nextafter(WIDTH, numeric_limits<double>::max())};
  uniform_real_distribution<double> ydist{0.0, nextafter(HEIGHT, numeric_limits<double>::max())};
  normal_distribution<double> mdist{MASS, SDM};
  default_random_engine re{semilla};
  for (int i=0; i<nAsteroides; ++i) {
    asteroide aux= asteroide(xdist(re), ydist(re), mdist(re));
    listaAsteroides[i]=aux;
  }
  for (int i=0; i<nPlanetas ; ++i) {
    planeta aux;
    if(i%4==0) aux= planeta(0.0,ydist(re),mdist(re)*10);
    else if(i%4==1) aux= planeta(xdist(re),0.0,mdist(re)*10);
    else if(i%4==2) aux= planeta(WIDTH,ydist(re),mdist(re)*10);
    else  aux= planeta(xdist(re),HEIGHT,mdist(re)*10);
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

int main (int argc, char** argv) {
  auto start = std::chrono::system_clock::now();

  cout << argc << endl;
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

  // TO DO: guardar el output
  // TO DO: guardar un step by step?

  // Declarar los arrays dinamicos
  double **distanciasAsteroides = new double*[nAsteroides];
  double **distanciasAstPlanetas = new double*[nAsteroides];
  double **pendienteAsteroides = new double*[nAsteroides];
  double **pendienteAstPlanetas = new double*[nAsteroides];
  double **angulosAsteroides = new double*[nAsteroides];
  double **angulosAstPlanetas = new double*[nAsteroides];

  #pragma omp parallel for
  for (int i = 0; i<nAsteroides; ++i) {
    distanciasAsteroides[i] = new double[nAsteroides];
    distanciasAstPlanetas[i] = new double[nPlanetas];
    pendienteAsteroides[i] = new double[nAsteroides];
    pendienteAstPlanetas[i]= new double[nPlanetas];
    angulosAsteroides[i] = new double[nAsteroides];
    angulosAstPlanetas[i] = new double[nPlanetas];
  }

  double *fuerzasX = new double[nAsteroides];
  double *fuerzasY = new double[nAsteroides];

  // III. Bucle de iteraciones:
  for ( int t = 0; t < nIteraciones; ++t) {
    // 0. Calculo de todas las fuerzas que afectan a todos los asteroides (calcular primero las fuerzas del asteroide "i" con el resto de asteroides y luego con el resto de planetas).
    cout << "\n\nITERACION " << t << endl;

    // #pragma omp parallel for collapse(8)
    // TO DO declarar la i publica si queremos hacer el collapse para que el segundo bucle acceda a ella
    #pragma omp parallel
    for (int i=0; i < nAsteroides; ++i) {
      double fx;
      double fy;
      for (int j=i+1; j < nAsteroides; ++j) {
        int id = omp_get_thread_num();
        cout << "thread" << id;
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
            fx = ((GRAVITY * listaAsteroides[i].masa * listaAsteroides[j].masa)/ pow(distanciasAsteroides[i][j], 2)) * cos(angulosAsteroides[i][j]);
            fy = ((GRAVITY * listaAsteroides[i].masa * listaAsteroides[j].masa)/ pow(distanciasAsteroides[i][j], 2)) * sin(angulosAsteroides[i][j]);
            fx = ((fx > 200) ? 200 : fx);
            fy = ((fy > 200) ? 200 : fy);
            fuerzasX[i] += fx;
            fuerzasY[i] += fy;
            fuerzasX[j] -= fx;
            fuerzasY[j] -= fy;
        }
        // TO DO: BORRAR COMENTARIOS POR PANTALLA
        cout << "\ncomparando asteroide " << i << " con asteroide " << j << " tienen fX " << fx << endl;
        cout << "comparando asteroide " << i << " con asteroide " << j << "tienen fY " << fy << endl;
        cout << "comparando asteroide " << j << " con asteroide " << i << "tienen fX " << -fx << endl;
        cout << "comparando asteroide " << j << " con asteroide " << i << "tienen fY " << -fy << endl;
      }
      cout << "\nla suma de las fuerzas X creadas por los asteroides al asteroide " << i <<" es "<<fuerzasX[i]<< endl;
      cout << "la suma de las fuerzas Y creadas por los asteroides al asteroide " << i <<" es "<<fuerzasY[i]<< endl;

      for (int j=0; j < nPlanetas; ++j) {
        // 1. Distancias
        distanciasAstPlanetas[i][j] = pow(pow(listaAsteroides[i].x - listaPlanetas[j].x, 2) + pow(listaAsteroides[i].y - listaAsteroides[j].y, 2), 0.5);
        // 2. Movimiento normal
        // 2.1. Ángulo de influencia
        // 2.1.a. pendienteAsteroides
        // TO DO ¿se ejercen fuerzas si los planetas están más cerca de 2?
        if (distanciasAstPlanetas[i][j] > DMIN) {
            pendienteAstPlanetas[i][j] = (listaAsteroides[i].y - listaPlanetas[j].y) / (listaAsteroides[i].x - listaPlanetas[j].x);
        } else {
          pendienteAstPlanetas[i][j] = 0.0;
        }
        // 2.1.b. Corrección de la pendienteAsteroides
        if (pendienteAstPlanetas[i][j] > 1 || pendienteAstPlanetas[i][j] < -1) {
          pendienteAstPlanetas[i][j] = pendienteAstPlanetas[i][j] - ((int)(pendienteAstPlanetas[i][j])/1);
        }
        // 2.1.c. Cálculo del ángulo
        angulosAstPlanetas[i][j] = atan(pendienteAstPlanetas[i][j]);

        // 2.2. Fuerzas de atracción
        fx = ((GRAVITY * listaAsteroides[i].masa * listaPlanetas[j].masa)/ pow(distanciasAstPlanetas[i][j], 2)) * cos(angulosAstPlanetas[i][j]);
        fy = ((GRAVITY * listaAsteroides[i].masa * listaPlanetas[j].masa)/ pow(distanciasAstPlanetas[i][j], 2)) * sin(angulosAstPlanetas[i][j]);
        fx = ((fx > 200) ? 200 : fx);
        fy = ((fy > 200) ? 200 : fy);
        fuerzasX[i] += fx;
        fuerzasY[i] += fy;

        cout << "comparando asteroide " << i << " con planeta " << j << " tienen fuerzasX[i]: " << fx << endl;
        cout << "comparando asteroide " << i << " con planeta " << j << " tienen fuerzasY[i]: " << fy << endl;
      }
      cout << "\nla suma de las fuerzas totales X creadas por los asteroides y los planetas al asteroide " << i <<" es "<<fuerzasX[i]<< endl;
      cout << "la suma de las fuerzas totales Y creadas por los asteroides y los planetas al asteroide " << i <<" es "<<fuerzasY[i]<< endl;

    }

    // #pragma omp parallel for collapse(8)
    // no añadir hasta que pongamos un critical en el choque entre asteroides
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

      cout << "\nvelocidad x del asteroide " << i << ": " << listaAsteroides[i].velocidad[0] << endl;
      cout << "velocidad y del asteroide " << i << ": " << listaAsteroides[i].velocidad[1] << endl;
      cout << "\naceleracion x del asteroide " << i << ": " << listaAsteroides[i].aceleracion[0] << endl;
      cout << "aceleracion y del asteroide " << i << ": " << listaAsteroides[i].aceleracion[1] << endl;
       // 3.2. Rebote entre asteroides.
       // TO DO: Buffer de intercambio circular
       for (int j=i+1; j < nAsteroides; ++j) {
         if (distanciasAsteroides[i][j] <= DMIN) {
           double swap = listaAsteroides[i].velocidad[0];
           listaAsteroides[i].velocidad[0] = listaAsteroides[j].velocidad[0];
           listaAsteroides[j].velocidad[0] = swap;
           swap = listaAsteroides[i].velocidad[1];
           listaAsteroides[i].velocidad[1] = listaAsteroides[j].velocidad[1];
           listaAsteroides[j].velocidad[1] = swap;
         }
       }
    }
  }
  auto end = chrono::system_clock::now();
  auto diff = chrono::duration_cast<chrono::microseconds>(end-start);
  cout << "El programa ha tardado " << diff.count() << "segundos en ejecutarse\n";
  return 0;
}

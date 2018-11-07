#include <iostream>

int main (int argc, char** argv) {
  // recibir parametros:
    // num_asteroides
    // num_iteraciones
    // num_planetas
    // semilla
    if () {
      cout << "nasteroids-seq: Wrong arguments.\n Correct use:\n nasteroids-seq num_asteroides num_iteraciones num_planetas semilla" << endl;
      return -1;
    } else {
      for (int i = 0; i < argc; ++i)
          cout << argv[i] << "\n";
      cout << "You have entered " << argc
           << " arguments:" << "\n";
    }





    // else:
      // write it to init_conf.txt (un dato por linea)

    return 0;
}

#### Guía Guernika

- Enviar local --> Guernika: ```scp archivoQueQueraisMandar a0XXXXXX@guernika.lab.inf.uc3m.es:~/```
- Enviar Guernika --> local: ```scp a0XXXXXX@guernika.lab.inf.uc3m.es:~/archivoQueQueraisMandar ./ ```
- Entrar en Guernika para ejecutar algo ```ssh -p 22 a0XXXXXX@guernika.lab.inf.uc3m.es'```


#### Comandos compilación
- ```g++ -std=c++14 nasteroids-seq.cpp -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -o salida-seq.out```
- ```g++ -std=c++14 nasteroids-par.cpp -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -fopenmp -o salida-par.out```
- ```python3 stats.py 2 3 1 200 10 seq "cosa que estamos probando"```: los cuatro primeros argumentos son los que se quieren pasar al programa. Después se utiliza el fichero ```seq``` o ```par```, y por último se pasa un string con el comentario que identifique la prueba.

#### ¡Importante!
Hay dos ficheros de Python para las estadísticas:
  - ```stats.py```, que ejecuta n veces el código y guarda los resultados en el fichero ```stats.txt```.

  - ```textToExcel.py```, que coge os datos del fichero ```stats.txt```, los pasa a un Excel  (```stats.xlsx```) y elimina el fichero ```.txt``` para evitar duplicidades más adelante cuando se vuelva a repetir este proceso. Tuve que partirlo en dos ficheros porque la librería que se necesita para modificar Excels con Python no está disponible en Guernika. El primer script es el que se ejecuta en Guernika, después se trae a local el fichero de salida con todas las estadísticas y se vuelcan los datos al Excel con ```textToExcel.py```

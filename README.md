#### Guía Guernika

- Enviar local --> Guernika: ```scp archivoQueQueraisMandar a0XXXXXX@guernika.lab.inf.uc3m.es:~/```
- Enviar Guernika --> local: ```scp a0XXXXXX@guernika.lab.inf.uc3m.es:~/archivoQueQueraisMandar ./ ```
- Entrar en Guernika para ejecutar algo ```ssh -p 22 a0XXXXXX@guernika.lab.inf.uc3m.es'```


#### Comandos compilación
- ```g++ -std=c++14 nasteroids-seq.cpp -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -o salida.out```
- ```g++ -std=c++14 nasteroids-par.cpp -Wall -Wextra -Wno-deprecated -Werror -pedantic -pedantic-errors -o salida.out```

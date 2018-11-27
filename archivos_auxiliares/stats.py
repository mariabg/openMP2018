import sys
import subprocess

def main ():
    if len(sys.argv) is not 8:
        print ("Se ha llamado incorrectamente al programa. Ejemplo: python stats.py 10 seq \"doble pragma en creacion asteroides\".")
        return -1

    # nombramos los parámetros
    nAsteroides = sys.argv[1]
    nIteraciones = sys.argv[2]
    nPlanetas = sys.argv[3]
    semilla = sys.argv[4]
    nIter = int(sys.argv[5])
    eType = sys.argv[6]
    modif = sys.argv[7]

    # escogemos el modo de compilación requerido
    if eType == 'seq':
        compileLine = ['g++', '-std=c++14', 'seq/nasteroids-seq.cpp', '-Wall', '-Wextra', '-Wno-deprecated', '-Werror', '-pedantic', '-pedantic-errors', "-oseq/salida-seq.out"]
    elif eType == 'par':
        compileLine = ['g++', '-std=c++14', 'par/nasteroids-par.cpp', '-Wall', '-Wextra', '-Wno-deprecated', '-Werror', '-pedantic', '-pedantic-errors', '-fopenmp', '-opar/salida-par.out']
    else:
        print ("No se ha introducido \"par\" o \"seq\" como parámetro.")
        return -1

    # compilamos el código con sus parámetros correspondientes
    compilation = subprocess.Popen(compileLine, stdout=subprocess.PIPE)
    stdout, stderr = compilation.communicate()

    # ejecutamos el código nIter-veces, y hallamos la media del tiempo de ejecución
    sum = 0
    for i in range(0, nIter):
        try:
            # ejecutamos el código compilado y guardamos su impresión por pantalla para procesarlo a continuación
            execution = subprocess.Popen([eType + "/salida-" + eType + ".out", nAsteroides, nIteraciones, nPlanetas, semilla], stdout=subprocess.PIPE)
            stdout, stderr = execution.communicate()
            out = stdout.decode('UTF-8').split()
            # obtenemos los segundos que ha tardado en ejecutarse
            sum += float(out[len(out)-2])
        except Exception as e:
            print("Se ha encontrado el siguiente problema:", e)
            return -1
    # Realizamos la media entre todos los tiempos obtenidos
    mean = sum/nIter

    # Escribimos la salida en un archivo de texto (todos los parámetros de entrada junto con el tiempo medio de ejecución)
    f = open("stats.txt", "a+")
    f.write("\n" + nAsteroides + ", " + nIteraciones + ", " +  nPlanetas + ", " +  semilla + ", " +  str(nIter) + ", " +  eType + ", " +  modif + ", " +  str(mean))

    return 0

main()

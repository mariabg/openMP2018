import sys
import subprocess
# import openpyxl

def main ():
    if len(sys.argv) is not 8:
        print ("Wrong arguments, use python stats.py 10 seq \"doble pragma en creacion asteroides\".")
        return -1

    # get all the parameters
    nAsteroides = sys.argv[1]
    nIteraciones = sys.argv[2]
    nPlanetas = sys.argv[3]
    semilla = sys.argv[4]
    nIter = int(sys.argv[5])
    eType = sys.argv[6]
    modif = sys.argv[7]

    # different compiling parameters por sequencial and parallel compilation
    if eType == 'seq':
        compileLine = ['g++', '-std=c++14', 'seq/nasteroids-seq.cpp', '-Wall', '-Wextra', '-Wno-deprecated', '-Werror', '-pedantic', '-pedantic-errors', "-oseq/salida-seq.out"]
    elif eType == 'par':
        compileLine = ['g++', '-std=c++14', 'par/nasteroids-par.cpp', '-Wall', '-Wextra', '-Wno-deprecated', '-Werror', '-pedantic', '-pedantic-errors', '-fopenmp', '-opar/salida-par.out']
    else:
        print ("Not recognized \"par\" or \"seq\".")
        return -1

    # compile de code with the corresponding parameters
    compilation = subprocess.Popen(compileLine, stdout=subprocess.PIPE)
    stdout, stderr = compilation.communicate()

    # Run nIter times the compiled file and get the average runtime
    sum = 0
    for i in range(0, nIter):
        try:
            # try to execute the compiled code and save its output for later value retrieval
            execution = subprocess.Popen([eType + "/salida-" + eType + ".out", nAsteroides, nIteraciones, nPlanetas, semilla], stdout=subprocess.PIPE)
            stdout, stderr = execution.communicate()
            out = stdout.decode('UTF-8').split()
            # get the number of seconds out of the full program's output
            sum += float(out[len(out)-2])
        except Exception as e:
            print("There was the following error:", e)
            return -1
    mean = sum/nIter

    f = open("stats.txt", "a+")
    f.write("\n" + nAsteroides + ", " + nIteraciones + ", " +  nPlanetas + ", " +  semilla + ", " +  str(nIter) + ", " +  eType + ", " +  modif + ", " +  str(mean))

    # Write the values to the Excel file
    # book = openpyxl.load_workbook('stats.xlsx')
    # sheet = book.active
    # sheet.append((int(nAsteroides), int(nIteraciones), int(nPlanetas), int(semilla), int(nIter), eType, modif, mean))
    # book.save('stats.xlsx')

    return 0

main()

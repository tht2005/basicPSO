import sys
import os
import numpy as np

def run(prog, inp, out):
    os.system('./{} ./{} ./{}'.format(prog, inp, out))

def prepareInput(f, inputFile):
    if os.path.isfile(f):
        file = open(f, "r")

        name = ''
        n = 0
        rem = 0
        w = []

        for line in file:
            tokens = line.split()

            if len(tokens) == 0:
               continue

            if tokens[0] == 'EOF':
                break
            elif rem > 0:
                w.append(line)
            elif tokens[0] == 'NAME:':
                name = tokens[1]
            elif tokens[0] == 'DIMENSION:':
                n = int(tokens[1])
            elif tokens[0] == 'EDGE_WEIGHT_SECTION':
                rem = 1

        file.close()

        out = open(inputFile, "w")
        out.write(str(n) + '\n')
        for a in w:
            out.write(a)
        out.close()

        return [ name, n ]


def checkValid(inputFile, psoOFile):
    # get input
    file = open(inputFile, "r")
    content = file.readlines()
    file.close()

    if len(content) == 0:
        print('input file empty!')
        exit()

    n = int(content[0])

    if len(content) != n + 1:
        print('number of lines of file content != n + 1')
        exit()

    C = []
    for i in range(1, n + 1):
        C.append(content[i].split())

    # get solution and check
    file = open(psoOFile, "r")
    content = file.readlines()
    file.close()

    if len(content) != 2:
        print('number of lines of pso_solution != 2')
        exit()

    res = int(content[0])
    order = content[1].split()

    calc_res = 0
    for i in range(n - 1):
        calc_res += int(C[int(order[i])][int(order[i + 1])])

    return res == calc_res


# my random generator
SEED = np.random.random_integers(23, 32439909)

def changeSEED():
    global SEED
    SEED = SEED ^ (SEED >> 10) ^ (SEED << 10)
    SEED *= 3293902
    SEED *= 20932809
    SEED = SEED ^ (SEED >> 10) ^ (SEED << 10)
    SEED *= 3122005
    # make SEED unsigned
    if SEED < 0:
        SEED += (1 << 32)

def randMax(maxx):
    changeSEED()
    return int(SEED) % maxx

def rand(l, r):
    return l + randMax(r - l + 1)

##########################################################


if __name__ == "__main__":
    # compile c++ files
    os.system('g++ -std=c++17 ./src/tsp_n_small.cpp -o tsp_n_small')
    os.system('g++ -std=c++17 ./src/tsp_pso.cpp -o tsp_pso')

    # generate random test
    if len(sys.argv) == 1:
        n = int(input('Input n: '))
        ntest = int(input('Input number of tests: '))

        inputFile = 'a.inp'
        psoOutputFile = 'pso.out'
        optOutputFile = 'opt.out'

        for itest in range(ntest):
            # generate a testcase
            f = open(inputFile, 'w')
            f.write(str(n) + '\n')
            for i in range(n):
                for j in range(n):
                    if randMax(3) == 0:
                        f.write(str(rand(1, 10000)) + ' ')
                    else:
                        f.write(str(rand(1, 1000000000)) + ' ')
                f.write('\n')
            f.close()

            print('Start running test ' + str(itest + 1) + ':')

            print('Start pso solution...')
            # run pso solution
            run('tsp_pso', inputFile, psoOutputFile)
            if checkValid(inputFile, psoOutputFile) == False:
                print('pso prog output invalid solution!')
                exit()

            if n <= 22:
                print('Start dp solution...')
                # run dp solution
                run('tsp_n_small', inputFile, optOutputFile)
            else:
                print('n is too large (n > 22) => not run dp solution')

            print('\n\n\n\n')



    # use the strong test
    # python3 ./.../main.py (directory of tests)
    else:
        test_directory = sys.argv[1]

        for filename in os.listdir(test_directory):
            f = os.path.join(test_directory, filename)
            [ name, n ] = prepareInput(f, "./a.inp")
            print(name, n)


import sys
import os
import time
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


psoWeight = 0
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
    calc_res += int(C[int(order[-1])][0])

    global psoWeight
    psoWeight = res

    if res != calc_res:
        print('user output: {}, expected: {}'.format(res, calc_res))

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


def genTest(n):
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


if __name__ == "__main__":
    # compile c++ files
    os.system('g++ -std=c++17 ./src/tsp_n_small.cpp -o tsp_n_small')
    os.system('g++ -std=c++17 ./src/tsp_pso.cpp -o tsp_pso')

    # generate random test
    n = int(input('Input n: '))
    ntest = int(input('Input number of tests: '))

    inputFile = 'a.inp'
    psoOutputFile = 'pso.out'
    optOutputFile = 'opt.out'

    sumPercent = 0
    sumTime = 0

    for itest in range(ntest):
        # generate a testcase
        genTest(n)

        print('Start running test ' + str(itest + 1) + ':')

        print('Start pso solution...')
        # run pso solution
        begin = time.time()
        run('tsp_pso', inputFile, psoOutputFile)
        end = time.time()
        
        sumTime += end - begin

        if checkValid(inputFile, psoOutputFile) == False:
            print('pso prog output invalid solution!')
            exit()

        opt = -1

        if n <= 22:
            print('Start dp solution...')
            # run dp solution
            run('tsp_n_small', inputFile, optOutputFile)

            file = open(optOutputFile, "r")
            opt = int(file.readline().split()[0])
            file.close()

        else:
            print('n is too large (n > 22) => not run dp solution')

        if opt != -1:
            print('Pso solution: {}'.format(psoWeight))
            print('Dp solution: {}'.format(opt))
            print('Percent: {}%'.format(opt * 100. / psoWeight))
            sumPercent += opt * 100. / psoWeight

        print('\n\n\n\n')

    print('Average percent: {}%'.format(sumPercent / ntest))
    print('Average Time: {}s'.format(sumTime / ntest))




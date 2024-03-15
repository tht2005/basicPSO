import sys
import os

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




if __name__ == "__main__":

    # compile c++ files
    os.system('g++ -std=c++17 ./src/tsp_n_small.cpp -o tsp_n_small')

    test_directory = sys.argv[1]

    for filename in os.listdir(test_directory):
        f = os.path.join(test_directory, filename)
        [ name, n ] = prepareInput(f, "./a.inp")
        print(name, n)


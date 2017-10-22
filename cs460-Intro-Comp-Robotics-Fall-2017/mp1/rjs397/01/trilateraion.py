import sys
from scipy.optimize import fsolve
import math

def trilaterate3D(distances):

    return fsolve(eqs, (0, 0, 0, 0), args=distances)

def eqs(p, d):

    x, y, z, _ = p

    p1 = math.pow((x - d[0][0]), 2) + math.pow((y - d[0][1]), 2) + math.pow((z - d[0][2]), 2) - math.pow(d[0][3], 2)
    p2 = math.pow((x - d[1][0]), 2) + math.pow((y - d[1][1]), 2) + math.pow((z - d[1][2]), 2) - math.pow(d[1][3], 2)
    p3 = math.pow((x - d[2][0]), 2) + math.pow((y - d[2][1]), 2) + math.pow((z - d[2][2]), 2) - math.pow(d[2][3], 2)
    p4 = math.pow((x - d[3][0]), 2) + math.pow((y - d[3][1]), 2) + math.pow((z - d[3][2]), 2) - math.pow(d[3][3], 2)

    return (p1, p2, p3, p4)


if __name__ == "__main__":

    # Retrive file name for input data
    if(len(sys.argv) == 1):
        print "Please enter data file name."
        exit()

    filename = sys.argv[1]

    # Read data
    lines = [line.rstrip('\n') for line in open(filename)]
    distances = []
    for line in range(0, len(lines)):
        distances.append(map(float, lines[line].split(' ')))

    # Print out the data
    print "The input four points and distances, in the format of [x, y, z, d], are:"
    for p in range(0, len(distances)):
        print distances[p]

    # Call the function and compute the location
    location = trilaterate3D(distances)
    print
    print "The location of the point is: " + str(location)

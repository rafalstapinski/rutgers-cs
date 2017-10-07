import math
import matplotlib.pyplot as plot
import random

def phi(x):

    if x < 0:
        sign = -1
    elif x > 0:
        sign = 1
    else:
        sign = 0

    exponent = -1 * 2 * math.pow(x, 2) / math.pi

    # print 1 - math.pow(math.e, exponent)
    # return .5 + sign/2 * math.sqrt(1 - math.pow(math.e, exponent))

    x = x - .5
    x = x * 2
    x = x * x
    x = x - 1
    x = x * -1
    x = math.log(x)
    x = x * math.pi
    x = x / -2
    x = math.sqrt(x)
    return x

x = []

for i in range(500):
    x.append(phi(random.random()))

plot.hist(x, bins=50, range=[-5, 5])

plot.show()

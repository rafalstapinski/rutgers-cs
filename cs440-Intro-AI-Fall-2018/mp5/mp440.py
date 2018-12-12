import inspect
import sys
import matplotlib.pyplot as plt

'''
Raise a "not defined" exception as a reminder 
'''


def _raise_not_defined():
    print "Method not implemented: %s" % inspect.stack()[1][3]
    sys.exit(1)


'''
Kalman 2D
'''


def kalman2d(data):
    estimated = []
    # Your code starts here
    # You should remove _raise_not_defined() after you complete your code
    # Your code ends here

    for d in data:
        estimated.append([d[2], d[3]])

    # _raise_not_defined()
    return estimated


'''
Plotting
'''


def plot(data, output):
    # Your code starts here
    # You should remove _raise_not_defined() after you complete your code
    # Your code ends here

    observed_x = []
    observed_y = []

    for d in data:
        observed_x.append(d[2])
        observed_y.append(d[3])

    plt.plot(observed_x, observed_y, 'b-')

    estimate_x = []
    estimate_y = []

    for d in output:
        estimate_x.append(d[0])
        estimate_y.append(d[1])

    plt.plot(estimate_x, estimate_y, 'r-')

    plt.show()


'''
Kalman 2D 
'''


def kalman2d_shoot(ux, uy, ox, oy, reset=False):
    decision = (0, 0, False)
    # Your code starts here
    # Your code ends here
    _raise_not_defined()
    return decision


'''
Kalman 2D 
'''


def kalman2d_adv_shoot(ux, uy, ox, oy, reset=False):
    decision = (0, 0, False)
    # Your code starts here
    # Your code ends here
    _raise_not_defined()
    return decision

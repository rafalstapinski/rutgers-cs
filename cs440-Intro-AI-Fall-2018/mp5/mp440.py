import inspect
import sys
import matplotlib.pyplot as plt
import numpy as np
import math

'''
Raise a "not defined" exception as a reminder
'''


def _raise_not_defined():
    print "Method not implemented: %s" % inspect.stack()[1][3]
    sys.exit(1)


'''
Kalman 2D
'''

Q = np.matrix([[math.pow(10, -4), 2 * math.pow(10, -5)],
               [2 * math.pow(10, -5), math.pow(10, -4)]])

R = np.matrix([[math.pow(10, -2), 5 * math.pow(10, -3)],
               [5 * math.pow(10, -3), 2 * math.pow(10, -2)]])


def kalman2d(data):
    estimated = []
    # Your code starts here
    # You should remove _raise_not_defined() after you complete your code
    # Your code ends here

    global Q, R

    P_k = np.eye(2) * .1
    A = np.eye(2)
    B = np.eye(2)
    H = np.eye(2)
    x_k = np.matrix([0, 0]).T
    I = np.eye(2)

    for i in range(len(data)):

        if i == 0:
            u_k_1 = np.matrix([0, 0]).T
        else:
            u_k_1 = np.matrix([data[i][0], data[i][1]]).T

        x_k_ = A * x_k + B * u_k_1

        P_k_ = A * P_k * A.T + Q

        K_k = (P_k_ * H.T) / (H * P_k_ * H.T + R)

        if i == 0:
            z_k = np.matrix([0, 0]).T
        else:
            z_k = np.matrix([data[i][2], data[i][3]]).T

        x_k = x_k_ + K_k * (z_k - H * x_k_)

        P_k = (I - K_k * H) * P_k_

        estimated.append([x_k[0, 0], x_k[1, 0]])

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

    plt.plot(observed_x, observed_y, 'b-o')

    estimate_x = []
    estimate_y = []

    for d in output:
        estimate_x.append(d[0])
        estimate_y.append(d[1])

    plt.plot(estimate_x, estimate_y, 'r-o')

    plt.show()


'''
Kalman 2D
'''

data = []


def kalman2d_shoot(ux, uy, ox, oy, reset=False):

    global Q, R, data
    Q = np.matrix([[2, 0.5], [0.5, 2]])
    R = np.matrix([[200, 50], [50, 300]])

    if reset:
        data = [[ux, uy, ox, oy]]
    else:
        data.append([ux, uy, ox, oy])

    estimated = kalman2d(data)

    ex = estimated[-1][0]
    ey = estimated[-1][1]

    if len(estimated) > 2 and len(data) > 2 and math.sqrt(math.pow(estimated[-1][0] - data[-1][2], 2) + math.pow(estimated[-1][1] - data[-1][3], 2)) < 5:
        return (ex, ey, True)
    else:
        return (ex, ey, False)

    if len(estimated) > 150:
        return (ex, ey, True)


'''
Kalman 2D
'''


def kalman2d_adv_shoot(ux, uy, ox, oy, reset=False):
    decision = (0, 0, False)
    # Your code starts here
    # Your code ends here

    return kalman2d_shoot(ux, ui, ox, oy, reset)

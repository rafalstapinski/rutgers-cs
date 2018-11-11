import numpy as np
import matplotlib
import sys

matplotlib.use("TkAgg")
import matplotlib.pyplot as plt

N = int(sys.argv[-1])
plt.hist(np.random.normal(size=N), bins=50, range=(-5, 5))
plt.title("N = {}".format(N))
plt.show()

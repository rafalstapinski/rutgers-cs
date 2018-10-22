import math

"""
GCD algorithm
"""


def gcd(a, b):
    while b:
        a, b = b, a % b
    return a


"""
Rectangles on a rubik's cube
"""


def rubiks(n):
    return int(6 * math.pow(math.factorial(n + 1) / (2 * math.factorial(n - 1)), 2))


"""
Guessing a number
"""


def guess_unlimited(n, is_this_it):
    for i in range(1, n + 1):
        if is_this_it(i):
            return i

    return -1


"""
Guessing a number where you can only make two guesses that are larger
"""


def guess_limited(n, is_this_smaller):

    div = int(math.ceil(math.sqrt(n)))
    print "div", div

    for i in range(1, div):
        if not is_this_smaller(i * div):
            start = (i - 1) * div
            for j in range(0, div):
                guess = start + j
                if not is_this_smaller(guess):
                    return guess

"""
Guessing a number, bonus problem
"""


def guess_limited_plus(n, is_this_smaller):
    return -1


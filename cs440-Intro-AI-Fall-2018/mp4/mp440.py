import inspect
import sys
import numpy as np
import math
from collections import Counter
import progressbar

lf_pairs = {}
legal_labels = [0, 1, 2, 3, 4, 5, 6, 7, 8, 9]
priors = {label: float(0) for label in legal_labels}
k = .0000001


def gaussian_kernel(size, sigma):

    ax = np.arange(-size // 2 + 1., size // 2 + 1.)
    xx, yy = np.meshgrid(ax, ax)

    kernel = np.exp(-(xx ** 2 + yy ** 2) / (2. * sigma ** 2))

    gauss_kernel = kernel / np.sum(kernel)

    return np.round_(np.matrix(gauss_kernel), 3)


# kernel_15 = gaussian_kernel(15, 1)

kernel_15 = np.matrix([[1] * 15] * 15)

"""
Raise a "not defined" exception as a reminder 
"""


def _raise_not_defined():
    print "Method not implemented: %s" % inspect.stack()[1][3]
    sys.exit(1)


"""
Extract 'basic' features, i.e., whether a pixel is background or
forground (part of the digit) 
"""


def extract_basic_features(digit_data, width, height):

    features = []

    for i in range(width):
        for j in range(height):
            features.append(digit_data[i][j] > 0)

    return features


"""
Applies a kernel to a rectangular 2d array of data, data is multiplies by an offset
"""


def kernelize(data, kernel, power_offset):

    height = len(data)
    width = len(data[0])
    data = np.matrix(data)
    r_s = int(len(kernel) / 2)

    conv = []

    for i in range(r_s, height - r_s):

        conv.append([])

        for j in range(r_s, width - r_s):

            data_slice = data[
                np.ix_(range(i - r_s, i + r_s + 1), range(j - r_s, j + r_s + 1))
            ]
            kernelized = np.multiply(data_slice, kernel)
            conv[-1].append(int(power_offset * round(kernelized.sum(), 1)))

    conv = np.matrix(conv)

    return conv


"""
Extract advanced features that you will come up with 
"""


def extract_advanced_features(digit_data, width, height):

    edge_features = []
    center_features = []
    blank_features = []

    conv_1 = kernelize(digit_data, kernel_15, 100)

    for i in range(width):
        for j in range(height):
            edge_features.append(int(digit_data[i][j] > 1) + 2)
            center_features.append(int(digit_data[i][j] == 1) + 4)
            blank_features.append(int(digit_data[i][j] == 0) + 6)

    side_features = []

    matrix = np.matrix(digit_data)

    mid_w = int(width / 2)
    mid_h = int(height / 2)

    left_sum = matrix[np.ix_(range(0, mid_w), range(0, height))].sum()
    right_sum = matrix[np.ix_(range(mid_w, width), range(0, height))].sum()

    top_sum = matrix[np.ix_(range(0, width), range(0, mid_h))].sum()
    bottom_sum = matrix[np.ix_(range(0, width), range(mid_h, height))].sum()

    if left_sum > right_sum:
        side_features.append("L")
    elif left_sum < right_sum:
        side_features.append("R")
    else:
        side_features.append("LR")

    if top_sum > bottom_sum:
        side_features.append("T")
    elif top_sum < bottom_sum:
        side_features.append("B")
    else:
        side_features.append("TB")

    return (
        conv_1.flatten().tolist()[0]
        + edge_features
        + side_features
        + center_features
        + blank_features
    )


"""
Extract the final features that you would like to use
"""


def extract_final_features(digit_data, width, height):

    return extract_basic_features(
        digit_data, width, height
    ) + extract_advanced_features(digit_data, width, height)


"""
Compupte the parameters including the prior and and all the P(x_i|y). Note
that the features to be used must be computed using the passed in method
feature_extractor, which takes in a single digit data along with the width
and height of the image. For example, the method extract_basic_features
defined above is a function than be passed in as a feature_extractor
implementation.

The percentage parameter controls what percentage of the example data
should be used for training. 
"""


def compute_statistics(
    data, labels, width, height, feature_extractor, percentage=100.0
):

    global lf_pairs
    feature_count = len(feature_extractor(data[0], width, height))
    lf_pairs = {
        (label, feature_i): {}
        for feature_i in range(feature_count)
        for label in legal_labels
    }

    data_len = int(len(data) * .01 * percentage)
    feature_vals = set()

    for i in range(data_len):

        features = feature_extractor(data[i], width, height)
        label = labels[i]
        priors[label] += 1

        for feature_i in range(len(features)):
            feature_val = features[feature_i]
            if feature_val in lf_pairs[(label, feature_i)]:
                lf_pairs[(label, feature_i)][feature_val] += 1
            else:
                lf_pairs[(label, feature_i)][feature_val] = 1

            feature_vals.add(feature_val)

    # normalize priors
    for label in priors:
        priors[label] /= float(data_len)

    for pair in lf_pairs:
        for feature_val in lf_pairs[pair]:
            lf_pairs[pair][feature_val] += k
        for feature_val in feature_vals:
            if feature_val not in lf_pairs[pair]:
                lf_pairs[pair][feature_val] = k
        count = sum(lf_pairs[pair][feature_val] for feature_val in lf_pairs[pair])
        for feature_val in lf_pairs[pair]:
            lf_pairs[pair][feature_val] = float(lf_pairs[pair][feature_val]) / float(
                count
            )


"""
For the given features for a single digit image, compute the class 
"""


def compute_class(features):

    prediction = -1
    max_log_joint = float("-inf")

    for label in legal_labels:

        log_joint = math.log(priors[label])

        for feature_i in range(len(features)):

            feature_val = features[feature_i]
            log_joint += math.log(lf_pairs[(label, feature_i)][feature_val])

        if log_joint > max_log_joint:
            max_log_joint = log_joint
            prediction = label

    return prediction


"""
Compute joint probaility for all the classes and make predictions for a list
of data
"""


def classify(data, width, height, feature_extractor):

    predicted = []

    for char in data:
        predicted.append(compute_class(feature_extractor(char, width, height)))

    return predicted

import sys
import copy
from collections import Iterable

import matplotlib.pyplot as plt
from matplotlib.path import Path
from matplotlib import collections as mc
import matplotlib.patches as patches

import numpy as np

import random

'''
Set up matplotlib to create a plot with an empty square
'''
def setupPlot():
    fig = plt.figure(num=None, figsize=(5, 5), dpi=120, facecolor='w', edgecolor='k')
    plt.autoscale(False)
    plt.axis('off')
    ax = fig.add_subplot(1,1,1)
    ax.set_axis_off()
    ax.add_patch(patches.Rectangle(
        (0,0),   # (x,y)
        1,          # width
        1,          # height
        fill=False
        ))
    return fig, ax

'''
Make a patch for a single pology
'''
def createPolygonPatch(polygon, color):
    verts = []
    codes= []
    for v in range(0, len(polygon)):
        xy = polygon[v]
        verts.append((xy[0]/10., xy[1]/10.))
        if v == 0:
            codes.append(Path.MOVETO)
        else:
            codes.append(Path.LINETO)
    verts.append(verts[0])
    codes.append(Path.CLOSEPOLY)
    path = Path(verts, codes)
    patch = patches.PathPatch(path, facecolor=color, lw=1)

    return patch


'''
Render the problem
'''
def drawProblem(robotStart, robotGoal, polygons):
    fig, ax = setupPlot()
    patch = createPolygonPatch(robotStart, 'green')
    ax.add_patch(patch)
    patch = createPolygonPatch(robotGoal, 'red')
    ax.add_patch(patch)
    for p in range(0, len(polygons)):
        patch = createPolygonPatch(polygons[p], 'gray')
        ax.add_patch(patch)
    plt.show()

def two_closest_points(points, point):

    pt1_dist = sys.maxint
    pt2_dist = sys.maxint

    pt1 = point
    pt2 = point

    for i in points:

        if points[i] == point:
            continue

        pti_dist = np.sqrt(
            (point[0] - points[i][0])** 2
          + (point[1] - points[i][1])** 2
        )

        if pti_dist < pt1_dist and pt2 != points[i] and point != points[i]:

            pt1 = points[i]
            pt1_dist = pti_dist

        elif pti_dist < pt2_dist and pt1 != points[i] and point != points[i]:

            pt2 = points[i]
            pt2_dist = pti_dist

    return pt1, pt2

def closest_point(points, point):


    closest_dist = sys.maxint
    closest_pt = point

    for i in points:

        if points[i] == point:
            continue

        pti_dist = np.sqrt(
            (point[0] - points[i][0])** 2
          + (point[1] - points[i][1])** 2
        )

        if pti_dist < closest_dist:

            closest_pt = points[i]
            closest_dist = pti_dist

    return closest_pt

def closest_pt_on_segment(p, a, b):

    a_to_p = (p[0] - a[0], p[1] - a[1])
    a_to_b = (b[0] - a[0], b[1] - a[1])

    a_to_b_2 = a_to_b[0] ** 2 + a_to_b[1] ** 2

    a_to_b_dot_a_to_p = a_to_p[0] * a_to_b[0] + a_to_p[1] * a_to_b[1]

    if a_to_b_2 == 0:
        t = 1
    else:


        t = a_to_b_dot_a_to_p / a_to_b_2

        if t < 0:
            t = 0
        elif t > 1:
            t = 1

    return (a[0] + a_to_b[0] * t, a[1] + a_to_b[1] * t)

def growSimpleRRT_helper(points):

    adjListMap = {}
    tree = {}

    pt1, pt2 = two_closest_points(points, points[1])

    pt1_i = points.keys()[points.values().index(pt1)]
    # pt2_i = points.keys()[points.values().index(pt2)]

    tree[1] = points[1]
    tree[pt1_i] = pt1
    # tree[pt2_i] = pt2

    adjListMap[1] = [pt1_i]

    adjListMap[pt1_i] = [1]
    # adjListMap[pt2_i] = [1]

    new_index = len(points) + 1

    if len(points) <= 2:
        return tree, adjListMap

    points_keys = sorted(points.keys())

    for i in points_keys:

        pt2_list = {}

        # displayRRTandPath(tree, adjListMap, None)

        if i in tree:
            continue

        adjListMap[i] = []

        pt1 = closest_point(tree, points[i])
        pt1_i = tree.keys()[tree.values().index(pt1)]

        for adj in adjListMap[pt1_i]:
            pt2_list[adj] = tree[adj]

        pt2 = closest_point(pt2_list, points[i])
        pt2_i = tree.keys()[tree.values().index(pt2)]

        closest_pt = closest_pt_on_segment(points[i], pt1, pt2)

        closest_pt_i = new_index
        new_index += 1

        tree[i] = points[i]

        if closest_pt == pt1:

            adjListMap[pt1_i].append(i)
            adjListMap[i].append(pt1_i)

        elif closest_pt == pt2:

            adjListMap[pt2_i].append(i)
            adjListMap[i].append(pt2_i)

        else:

            tree[closest_pt_i] = closest_pt

            adjListMap[pt1_i].append(closest_pt_i)
            adjListMap[pt2_i].append(closest_pt_i)

            # adjListMap[pt1_i].remove(pt2_i)
            # adjListMap[pt2_i].remove(pt1_i)

            adjListMap[i] = [closest_pt_i]

            adjListMap[closest_pt_i] = [i, pt1_i, pt2_i]

    return tree, adjListMap


'''
Grow a simple RRT
'''
def growSimpleRRT(points):
    newPoints = dict()
    adjListMap = dict()

    # Your code goes here

    # newPoints, adjListMap = growSimpleRRT_helper(points)
    #
    # print newPoints
    # print adjListMap

    return growSimpleRRT_helper(points)


def bfs(tree, start, goal):

    explored = []
    queue = [[start]]

    if start == goal:
        return goal

    while queue:

        path = queue.pop(0)
        pt = path[-1]

        if pt not in explored:

            adjacentList = tree[pt]

            for adjacent in adjacentList:

                new_path = [path]
                new_path.append(adjacent)
                queue.append(new_path)

                if adjacent == goal:
                    return new_path

            explored.append(pt)

def flatten(l):
    for i in l:
        if isinstance(i, Iterable):
            for x in flatten(i):
                yield x
        else:
            yield i

'''
Perform basic search
'''
def basicSearch(tree, start, goal):
    path = []

    # Your code goes here. As the result, the function should
    # return a list of vertex labels, e.g.
    #
    # path = [23, 15, 9, ..., 37]
    #
    # in which 23 would be the label for the start and 37 the
    # label for the goal.


    return list(flatten(bfs(tree, start, goal)))


'''
Display the RRT and Path
'''
def displayRRTandPath(points, tree, path, robotStart = None, robotGoal = None, polygons = None):

    # Your code goes here
    # You could start by copying code from the function
    # drawProblem and modify it to do what you need.
    # You should draw the problem when applicable.

    fig, ax = setupPlot()

    if robotStart is not None:

        patch = createPolygonPatch(robotStart, 'green')
        ax.add_patch(patch)

    if robotGoal is not None:

        patch = createPolygonPatch(robotGoal, 'red')
        ax.add_patch(patch)

    if polygons is not None:

        for p in range(0, len(polygons)):
            patch = createPolygonPatch(polygons[p], 'gray')
            ax.add_patch(patch)


    tree_list = []
    path_list = []

    for i in tree:

        for adj in tree[i]:

            tree_list.append([
                np.asarray(points[i], dtype=np.float) / 10,
                np.asarray(points[adj], dtype=np.float) / 10
            ])

        ax.add_collection(mc.LineCollection(tree_list, color = '#000000'))

    if len(path) > 0:

        for i in range(len(path) - 1):

            path_list.append([
                np.asarray(points[path[i]], dtype=np.float) / 10,
                np.asarray(points[path[i + 1]], dtype=np.float) / 10,
            ])

        ax.add_collection(mc.LineCollection(path_list, color = '#f97306'))

    plt.show()

    return

def ccw(a, b, c):
    return (c[1] - a[1]) * (b[0] - a[0]) > (b[1] - a[1]) * (c[0] - a[0])


def intersect(a, b, c, d):

    # share a point - consider intersection
    if a == c or a == d or b == c or b == d:
        return True

    # otherwise intersect
    return ccw(a, c, d) != ccw(b, c, d) and ccw(a, b, c) != ccw(a, b, d)

def point_in_poly(point, poly):

    n = len(poly)

    inside = False

    pt1_x, pt1_y = poly[0]

    for i in range(n+1):

        pt2_x, pt2_y = poly[i % n]

        if point[0] > min(pt1_y,pt2_y):

            if point[0] <= max(pt1_y,pt2_y):

                if point[0] <= max(pt1_x,pt2_x):

                    if pt1_y != pt2_y:
                        xinters = (point[0] - pt1_y) * (pt2_x - pt1_x) / (pt2_y - pt1_y) + pt1_x

                    if pt1_x == pt2_x or point[0] <= xinters:
                        inside = not inside

        pt1_x, pt1_y = pt2_x, pt2_y

    return inside

'''
Collision checking
'''
def isCollisionFree(robot, point, obstacles):

    print robot
    print point
    print obstacles

    robot_t = []

    for r in robot:

        robot_t.append((r[0] + point[0], r[1] + point[1]))

        if robot_t[-1][0] < 0 or robot_t[-1][1] < 0 or robot_t[-1][0] > 10 or robot_t[-1][1] > 10:
            return False

    for obstacle in obstacles:

        for i in range(-1, len(obstacle) - 1):

            obstacle_a = obstacle[i]
            obstacle_b = obstacle[i + 1]

            for j in range(-1, len(robot_t) - 1):

                robot_c = robot_t[j]
                robot_d = robot_t[j + 1]

                if intersect(obstacle_a, obstacle_b, robot_c, robot_d):
                    return False

                if point_in_poly(robot_c, obstacle):
                    return False

    return True

def line_works(pt1, pt2, obstacles):

    for obstacle in obstacles:

        for i in range(-1, len(obstacle) - 1):

            obstacle_a = obstacle[i]
            obstacle_b = obstacle[i + 1]

            if intersect(obstacle_a, obstacle_b, pt1, pt2):
                return False

    return True

'''
The full RRT algorithm
'''
def RRT(robot, obstacles, startPoint, goalPoint):

    points = dict()
    tree = dict()
    path = []

    points[1] = startPoint

    k = 2

    # if point works in current tree, add it
    # then also
    # if new point can connect to goal, search on that tree

    while True:

        line_works = False

        new_x = random.uniform(0, 10)
        new_y = random.uniform(0, 10)

        points[k] = (new_x, new_y)

        points, adjListMap = growSimpleRRT(points)

        adj_pt = adjListMap[k][0]

        if not line_works(points[k], adj_pt, obstacles):
            continue

        k += 1

        if line_works(points[k], goalPoint, obstacles):
            break



    return points, tree, path

if __name__ == "__main__":

    # Retrive file name for input data
    if(len(sys.argv) < 6):
        print "Five arguments required: python spr.py [env-file] [x1] [y1] [x2] [y2]"
        exit()

    filename = sys.argv[1]
    x1 = float(sys.argv[2])
    y1 = float(sys.argv[3])
    x2 = float(sys.argv[4])
    y2 = float(sys.argv[5])

    # Read data and parse polygons
    lines = [line.rstrip('\n') for line in open(filename)]
    robot = []
    obstacles = []
    for line in range(0, len(lines)):
        xys = lines[line].split(';')
        polygon = []
        for p in range(0, len(xys)):
            xy = xys[p].split(',')
            polygon.append((float(xy[0]), float(xy[1])))
        if line == 0 :
            robot = polygon
        else:
            obstacles.append(polygon)

    # Print out the data
    print "Robot:"
    print str(robot)
    print "Pologonal obstacles:"
    for p in range(0, len(obstacles)):
        print str(obstacles[p])
    print ""

    # Visualize
    robotStart = []
    robotGoal = []

    def start((x,y)):
        return (x+x1, y+y1)
    def goal((x,y)):
        return (x+x2, y+y2)
    robotStart = map(start, robot)
    robotGoal = map(goal, robot)
    drawProblem(robotStart, robotGoal, obstacles)

    # Example points for calling growSimpleRRT
    # You should expect many mroe points, e.g., 200-500
    points = dict()
    points[1] = (5, 5)
    points[2] = (7, 8.2)
    points[3] = (6.5, 5.2)
    points[4] = (0.3, 4)
    points[5] = (6, 3.7)
    points[6] = (9.7, 6.4)
    points[7] = (4.4, 2.8)
    points[8] = (9.1, 3.1)
    points[9] = (8.1, 6.5)
    points[10] = (0.7, 5.4)
    points[11] = (5.1, 3.9)
    points[12] = (2, 6)
    points[13] = (0.5, 6.7)
    points[14] = (8.3, 2.1)
    points[15] = (7.7, 6.3)
    points[16] = (7.9, 5)
    points[17] = (4.8, 6.1)
    points[18] = (3.2, 9.3)
    points[19] = (7.3, 5.8)
    points[20] = (9, 0.6)

    # Printing the points
    print ""
    print "The input points are:"
    print str(points)
    print ""

    # points, adjListMap = growSimpleRRT(points)

    # Search for a solution
    # path = basicSearch(adjListMap, 1, 20)

    # Your visualization code
    # displayRRTandPath(points, adjListMap, path)

    # Solve a real RRT problem
    RRT(robot, obstacles, (x1, y1), (x2, y2))

    # Your visualization code
    # displayRRTandPath(points, adjListMap, path, robotStart, robotGoal, obstacles)

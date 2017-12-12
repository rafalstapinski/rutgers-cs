import sys

import matplotlib.pyplot as plt
from matplotlib.path import Path
import matplotlib.patches as patches
import numpy as np

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

'''
Returns closest neighbour
'''
def closest_point(points, point):

    res_dist = sys.maxint

    for i in points:

        if points[i] == point:
            continue

        pti_dist = np.sqrt(
            (point[0] - points[i][0])** 2
          + (point[1] - points[i][1])** 2
        )

        if pti_dist < res_dist and points[i] != point:
            res = i
            res_dist = pti_dist

    return points[res]

def closest_pt_on_segment(a, b, p):

    a_to_p = (p[0] - a[0], p[1] - a[1])
    a_to_b = (b[0] - a[0], b[1] - a[1])

    a_to_b_2 = a_to_b[0] ** 2 + a_to_b[1] ** 2

    a_to_b_dot_a_to_p = a_to_p[0] * a_to_b[0] + a_to_p[1] * a_to_b[1]

    t = a_to_b_dot_a_to_p / a_to_b_2

    if t < 0:
        t = 0
    elif t > 1:
        t = 1

    return (a[0] + a_to_b[0] * t, a[1] + a_to_b[1] * t)

def growSimpleRRT_helper(points):

    new_points = []
    adjListMap = {}

    for i in points:

        pt1 = closest_point(points, points[i])
        pt2 = closest_point({j:points[j] for j in points if points[j] != pt1}, points[i])

        print points[i], pt1, pt2
        # closest_pt = closest_pt_on_segment(points[i], pt1, pt2)

        # if closest_pt not in (pt1, pt2):
        #     new_points.append(closest_pt)

    for pt in new_points:
        points[len(points) + 1] = pt

    iter_points = points



    return points, adjListMap


'''
Grow a simple RRT
'''
def growSimpleRRT(points):
    newPoints = dict()
    adjListMap = dict()

    # Your code goes here

    newPoints, adjListMap = growSimpleRRT_helper(points)

    print newPoints
    print adjListMap

    return newPoints, adjListMap


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

    print bfs(tree, 1, 19)
    return bfs(tree, start, goal)

'''
Display the RRT and Path
'''
def displayRRTandPath(points, tree, path, robotStart = None, robotGoal = None, polygons = None):

    # Your code goes here
    # You could start by copying code from the function
    # drawProblem and modify it to do what you need.
    # You should draw the problem when applicable.
    return

'''
Collision checking
'''
def isCollisionFree(robot, point, obstacles):

    # Your code goes here.

    return False

'''
The full RRT algorithm
'''
def RRT(robot, obstacles, startPoint, goalPoint):

    points = dict()
    tree = dict()
    path = []
    # Your code goes here.

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

    points, adjListMap = growSimpleRRT(points)

    # Search for a solution
    path = basicSearch(adjListMap, 1, 20)

    # Your visualization code
    displayRRTandPath(points, adjListMap, path)

    # Solve a real RRT problem
    RRT(robot, obstacles, (x1, y1), (x2, y2))

    # Your visualization code
    displayRRTandPath(points, adjListMap, path, robotStart, robotGoal, obstacles)

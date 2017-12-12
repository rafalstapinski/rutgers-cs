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

def two_closest_points(points, point):

    if point == points[1]:
        pt1 = points[2]
        pt2 = points[3]

    elif point == points[2]:
        pt1 = points[1]
        pt2 = points[3]

    else:
        pt1 = points[1]
        pt2 = points[2]

    pt1_dist = np.sqrt((point[0] - pt1[0]) ** 2 + (point[1] - pt1[1]) ** 2)
    pt2_dist = np.sqrt((point[0] - pt2[0]) ** 2 + (point[1] - pt2[1]) ** 2)

    for i in points:

        if points[i] == point:
            continue

        pti_dist = np.sqrt(
            (point[0] - points[i][0])** 2
          + (point[1] - points[i][1])** 2
        )

        if pti_dist < pt1_dist and pt1 != points[i]:
            pt1 = points[i]
        elif pti_dist < pt2_dist and pt2 != points[i]:
            pt2 = points[i]

    return pt1, pt2

def closest_pt_on_segment(ax, ay, bx, by, px, py):

    pass

def growSimpleRRT_helper(points, adjListMap):

    for i in points:

        pt1, pt2 = two_closest_points(points, points[i])

        print points[i], pt1, pt2


'''
Grow a simple RRT
'''
def growSimpleRRT(points):
    newPoints = dict()
    adjListMap = dict()

    # Your code goes here

    newPoints = points

    for i in points:

        newPoints, adjListMap = growSimpleRRT_helper(newPoints, adjListMap)

    return newPoints, adjListMap

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

    return path

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
    # points[6] = (9.7, 6.4)
    # points[7] = (4.4, 2.8)
    # points[8] = (9.1, 3.1)
    # points[9] = (8.1, 6.5)
    # points[10] = (0.7, 5.4)
    # points[11] = (5.1, 3.9)
    # points[12] = (2, 6)
    # points[13] = (0.5, 6.7)
    # points[14] = (8.3, 2.1)
    # points[15] = (7.7, 6.3)
    # points[16] = (7.9, 5)
    # points[17] = (4.8, 6.1)
    # points[18] = (3.2, 9.3)
    # points[19] = (7.3, 5.8)
    # points[20] = (9, 0.6)

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
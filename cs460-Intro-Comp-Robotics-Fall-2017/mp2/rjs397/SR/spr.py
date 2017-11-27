import sys
import math
from numpy import ones, vstack
from numpy.linalg import lstsq

'''
Report reflexive vertices
'''
def findReflexiveVertices(polygons):
    vertices=[]

    # Your code goes here
    # You should return a list of (x,y) values as lists, i.e.
    # vertices = [[x1,y1],[x2,y2],...]

    for polygon in polygons:

        x_center = sum(v[0] for v in polygon) / len(polygon)
        y_center = sum(v[1] for v in polygon) / len(polygon)

        for vertex in polygon:

            if polygon.index(vertex) == 0:

                prev_v = polygon[-1]
                next_v = polygon[1]

            elif polygon.index(vertex) == len(polygon) - 1:

                prev_v = polygon[-2]
                next_v = polygon[0]


            if  abs(x_center - vertex[0]) < abs(x_center - prev_v[0]) \
            and abs(y_center - vertex[1]) < abs(y_center - prev_v[1]):

                continue

            vertices.append(vertex)

    return vertices

'''
Compute the roadmap graph
'''

def ccw(a, b, c):
    return (c[1] - a[1]) * (b[0] - a[0]) > (b[1] - a[1]) * (c[0] - a[0])

def intersect(a, b, c, d):
    return ccw(a, c, d) != ccw(b, c, d) and ccw(a, b, c) != ccw(a, b, d)

def distance(a, b):

    return math.hypot(b[0] - a[0], b[1] - a[1])

def has_to_intersect(a, b, poly_a, poly_b):

    for vertex_1 in poly_a:

        if vertex_1 == a:
            continue

        for vertex_2 in poly_a:

            if vertex_2 == a:
                continue

        if intersect(a, b, vertex_1, vertex_2):
            return True

    for vertex_1 in poly_b:

        if vertex_1 == b:
            continue

        for vertex_2 in poly_b:

            if vertex_2 == b:
                continue

        if intersect(a, b, vertex_1, vertex_2):
            return True

    return False

def find_left_neighbor(v, polygon, reflexVertices):

    current_polygon = polygon * 2

    for neighbor_vertex in current_polygon[current_polygon.index(v) + 1:]:

        if neighbor_vertex in reflexVertices:

            return neighbor_vertex

def find_right_neighbor(v, polygon, reflexVertices):

    return find_left_neighbor(v, list(reversed(polygon)), reflexVertices)

def above(v, m, b):

    return v[1] > m * v[0] + b

def computeSPRoadmap(polygons, reflexVertices):
    vertexMap = dict()
    adjacencyListMap = dict()

    i = 1

    for reflex_vertex in reflexVertices:

        vertexMap[i] = reflex_vertex
        adjacencyListMap[i] = []

        i += 1

    for reflex_vertex in reflexVertices:

        vertex_label = [key for key, val in vertexMap.iteritems() if reflex_vertex == val][0]

        for polygon in polygons:
            if reflex_vertex in polygon:
                current_polygon = polygon
                break

        left_neighbor = find_left_neighbor(reflex_vertex, current_polygon, reflexVertices)
        left_neighbor_label = [key for key, val in vertexMap.iteritems() if left_neighbor == val][0]
        left_neighbor_dist = distance(reflex_vertex, left_neighbor)

        adjacencyListMap[vertex_label].append([left_neighbor_label, left_neighbor_dist])


        right_neighbor = find_right_neighbor(reflex_vertex, current_polygon, reflexVertices)
        right_neighbor_label = [key for key, val in vertexMap.iteritems() if right_neighbor == val][0]
        right_neighbor_dist = distance(reflex_vertex, right_neighbor)

        adjacencyListMap[vertex_label].append([right_neighbor_label, right_neighbor_dist])

        # find bitangents, by connecting to other polygons

        for other_polygon in polygons:

            if other_polygon == current_polygon:

                continue

            for other_vertex in other_polygon:

                if other_vertex not in reflexVertices:
                    continue

                # if visible to other vertex, check if bitangent

                if not has_to_intersect(reflex_vertex, other_vertex, current_polygon, other_polygon):

                    # if bitangent, add to adjancencyListMap

                    x_coors, y_coors = zip(*[reflex_vertex, other_vertex])
                    A = vstack([x_coors, ones(len(x_coors))]).T
                    m, b = lstsq(A, y_coors)[0]

                    a_left_neighbor = find_left_neighbor(reflex_vertex, current_polygon, reflexVertices)
                    a_right_neighbor = find_right_neighbor(reflex_vertex, current_polygon, reflexVertices)

                    b_left_neighbor = find_left_neighbor(other_vertex, other_polygon, reflexVertices)
                    b_right_neighbor = find_right_neighbor(other_vertex, other_polygon, reflexVertices)

                    if  above(a_left_neighbor, m, b) == above(a_right_neighbor, m, b) \
                    and above(b_left_neighbor, m, b) == above(b_right_neighbor, m, b):

                        other_label = [key for key, val in vertexMap.iteritems() if other_vertex == val][0]
                        other_dist = distance(reflex_vertex, other_vertex)

                        adjacencyListMap[vertex_label].append([other_label, other_dist])

    print adjacencyListMap
        #

    # Your code goes here
    # You should check for each pair of vertices whether the
    # edge between them should belong to the shortest path
    # roadmap.
    #
    # Your vertexMap should look like
    # {1: [5.2,6.7], 2: [9.2,2.3], ... }
    #
    # and your adjacencyListMap should look like
    # {1: [[2, 5.95], [3, 4.72]], 2: [[1, 5.95], [5,3.52]], ... }
    #
    # The vertex labels used here should start from 1

    return vertexMap, adjacencyListMap

'''
Perform uniform cost search
'''
def uniformCostSearch(adjListMap, start, goal):
    path = []
    pathLength = 0

    # Your code goes here. As the result, the function should
    # return a list of vertex labels, e.g.
    #
    # path = [23, 15, 9, ..., 37]
    #
    # in which 23 would be the label for the start and 37 the
    # label for the goal.

    return path, pathLength

'''
Agument roadmap to include start and goal
'''
def updateRoadmap(polygons, vertexMap, adjListMap, x1, y1, x2, y2):
    updatedALMap = dict()
    startLabel = 0
    goalLabel = -1

    # Your code goes here. Note that for convenience, we
    # let start and goal have vertex labels 0 and -1,
    # respectively. Make sure you use these as your labels
    # for the start and goal vertices in the shortest path
    # roadmap. Note that what you do here is similar to
    # when you construct the roadmap.

    updatedALMap = adjListMap

    return startLabel, goalLabel, updatedALMap

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
    polygons = []
    for line in range(0, len(lines)):
        xys = lines[line].split(';')
        polygon = []
        for p in range(0, len(xys)):
            polygon.append(map(float, xys[p].split(',')))
        polygons.append(polygon)

    # Print out the data
    print "Pologonal obstacles:"
    for p in range(0, len(polygons)):
        print str(polygons[p])
    print ""

    # Compute reflex vertices
    reflexVertices = findReflexiveVertices(polygons)
    print "Reflexive vertices:"
    print str(reflexVertices)
    print ""

    # Compute the roadmap
    vertexMap, adjListMap = computeSPRoadmap(polygons, reflexVertices)
    print "Vertex map:"
    print str(vertexMap)
    print ""
    print "Base roadmap:"
    print str(adjListMap)
    print ""

    # Update roadmap
    start, goal, updatedALMap = updateRoadmap(polygons, vertexMap, adjListMap, x1, y1, x2, y2)
    print "Updated roadmap:"
    print str(updatedALMap)
    print ""

    # Search for a solution
    path, length = uniformCostSearch(updatedALMap, start, goal)
    print "Final path:"
    print str(path)
    print "Final path length:" + str(length)


    # Extra visualization elements goes here

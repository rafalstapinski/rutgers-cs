import random
import copy
from collections import deque
''' ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ '''
'''
                For Search Algorithms
'''
''' ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ '''
'''
BFS add to queue
'''

queue = None


def add_to_queue_BFS(node_id, parent_node_id, cost, initialize=False):
    # Your code here
    global queue

    if initialize:
        queue = deque([])

    queue.append((node_id, parent_node_id))


'''
BFS add to queue
'''


def is_queue_empty_BFS():
    # Your code here
    global queue
    return len(queue) == 0


'''
BFS pop from queue
'''


def pop_front_BFS():
    # Your code here
    global queue
    return queue.popleft()


'''
DFS add to queue
'''


def add_to_queue_DFS(node_id, parent_node_id, cost, initialize=False):
    # Your code here
    global queue
    if initialize:
        queue = deque([])

    queue.append((node_id, parent_node_id))


'''
DFS add to queue
'''


def is_queue_empty_DFS():
    # Your code here
    global queue
    return len(queue) == 0


'''
DFS pop from queue
'''


def pop_front_DFS():
    # Your code here
    global queue
    return queue.pop()


'''
UC add to queue
'''


def add_to_queue_UC(node_id, parent_node_id, cost, initialize=False):
    # Your code here
    global queue
    if initialize:
        queue = []

    queue.append((node_id, parent_node_id, cost))
    queue = sorted(queue, key=lambda x: x[2])


'''
UC add to queue
'''


def is_queue_empty_UC():
    # Your code here
    global queue
    return len(queue) == 0


'''
UC pop from queue
'''


def pop_front_UC():
    # Your code here

    global queue
    (node_id, parent_node_id, cost) = queue.pop(0)

    return (node_id, parent_node_id)


'''
A* add to queue
'''


def add_to_queue_ASTAR(node_id, parent_node_id, cost, initialize=False):
    # Your code here
    add_to_queue_UC(node_id, parent_node_id, cost, initialize)


'''
A* add to queue
'''


def is_queue_empty_ASTAR():
    # Your code here
    return is_queue_empty_UC()


'''
A* pop from queue
'''


def pop_front_ASTAR():
    # Your code here
    return pop_front_UC()


''' ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ '''
'''
                For n-queens problem
'''
''' ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ '''
'''
Compute a random state
'''


def print_state(state):

    checkerboard_size = len(state)

    for i in range(checkerboard_size + 1):
        line = ""
        for j in range(checkerboard_size):
            if state[j] == i:
                line += "x "
            else:
                line += "- "
        print line


def get_random_state(n):
    state = []
    # Your code here

    state = [random.randint(0, n) for i in range(n)]
    return state


'''
Compute pairs of queens in conflict
'''


def compute_attacking_pairs(state):
    number_attacking_pairs = 0
    # Your code here

    checkerboard_size = len(state)

    for column in range(checkerboard_size):

        horizontal_attack = False
        top_diagonal_attack = False
        bottom_diagonal_attack = False

        for right_column in range(column + 1, checkerboard_size):
            if state[column] == state[right_column] and not horizontal_attack:
                number_attacking_pairs += 1
                horizontal_attack = True
            elif right_column - column == state[right_column] - state[
                    column] and not bottom_diagonal_attack:
                bottom_diagonal_attack = True
                number_attacking_pairs += 1
            elif right_column - column == state[column] - state[
                    right_column] and not top_diagonal_attack:
                top_diagonal_attack = True
                number_attacking_pairs += 1

    return number_attacking_pairs


'''
The basic hill-climing algorithm for n queens
'''


def hill_desending_n_queens(state, comp_att_pairs):
    final_state = []
    # Your code here

    if comp_att_pairs(state) == 0:
        return state

    checkerboard_size = len(state)
    attacking_columns = set()

    for column in range(checkerboard_size):
        for right_column in range(column + 1, checkerboard_size):
            if state[column] == state[right_column] or right_column - column == state[
                    column] - state[right_column] or right_column - column == state[column] - state[
                        right_column]:
                attacking_columns.add(column)
                attacking_columns.add(right_column)

    potential_states = []

    for column in attacking_columns:
        for new_value in range(checkerboard_size):
            potential_state = copy.deepcopy(state)
            potential_state[column] = new_value
            potential_states.append((potential_state, comp_att_pairs(potential_state)))

    try:
        final_state = min(potential_states, key=lambda x: x[1])[0]
    except ValueError:  # no potential states here
        final_state = state

    return final_state


'''
Hill-climing algorithm for n queens with restart
'''


def n_queens(n, get_rand_st, comp_att_pairs, hill_descending):
    final_state = []
    # Your code here

    state = get_rand_st(n)

    counter = 0
    shoulder = 0
    while True:
        counter += 1

        previous_cost = comp_att_pairs(state)
        state = hill_descending(state, comp_att_pairs)
        new_cost = comp_att_pairs(state)

        if comp_att_pairs(state) == 0:
            final_state = state
            break

        if previous_cost == new_cost:
            shoulder += 1

        if shoulder > 4:
            state = get_rand_st(n)
            shoulder = 0

    return final_state

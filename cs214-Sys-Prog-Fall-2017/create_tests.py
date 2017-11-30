import os
from shutil import copyfile
from subprocess import call

def create_shallow(amount):

    os.mkdir("Tests/shallow%d" % amount)
    os.mkdir("Tests/shallow-output-p%d" % amount)
    os.mkdir("Tests/shallow-output-t%d" % amount)

    base_dir = os.path.dirname(os.path.realpath(__file__))

    for i in range(amount):

        # os.symlink(
        #     '%s/movie_metadata.csv' % (base_dir),
        #     'shallow%d/%d.csv' % (amount, i)
        # )

        copyfile(
            '%s/medium.csv' % (base_dir),
            'Tests/shallow%d/%d.csv' % (amount, i)
        )

def create_deep(amount):

    working_dir = "Tests/deep%d/" % amount

    os.mkdir(working_dir)
    os.mkdir("Tests/deep-output-p%d" % amount)
    os.mkdir("Tests/deep-output-t%d" % amount)

    base_dir = os.path.dirname(os.path.realpath(__file__))

    for i in range(amount):

        working_dir += '%d/' % i

        os.mkdir(working_dir)
        copyfile(
            '%s/medium.csv' % (base_dir),
            '%s/%d.csv' % (working_dir, i)
        )
        # os.symlink(
        #     '%s/movie_metadata.csv' % (base_dir),
        #     '%s/%d.csv' % (working_dir, i)
        # )

def test_time(test_type, amount):

    print('Multiprocess time - %s %d' % (test_type, amount))
    print

    call([
        'time',
        'Project 1: Basic Data Sorter - multiproc/sorter',
        '-c director_name',
        '-d Tests/%s%d' % (test_type, amount),
        # '-o Tests/%s-output-p%d' % (test_type, amount)
    ])

    print
    print

    print('Multithreaded time - %s %d' % (test_type, amount))
    print

    call([
        'time',
        'Project 2: Basic Data Sorter - multithreaded/sorter',
        '-c director_name',
        '-d Tests/%s%d' % (test_type, amount),
        # '-o Tests/%s-output-p%d' % (test_type, amount)
    ])

call([
    'rm',
    '-rf',
    'Tests'
])

os.mkdir('Tests')

# create_shallow(1)
# create_shallow(10)
# create_shallow(1000)
create_shallow(200)
# #
# # create_deep(1)
# # create_deep(10)
# create_deep(1000)
create_deep(200)
#
# test_time('deep', 1000)


'''

    deep 200 results:

        threaded: 3 - 4 seconds, 401 threads

'''

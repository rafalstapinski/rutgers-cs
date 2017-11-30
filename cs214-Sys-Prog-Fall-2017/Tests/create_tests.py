import os

def create_shallow(amount):

    os.mkdir("shallow%d" % amount)
    os.mkdir("shallow-output-p%d" % amount)
    os.mkdir("shallow-output-t%d" % amount)

    base_dir = os.path.dirname(os.path.realpath(__file__))

    for i in range(amount):

        os.symlink(
            '%s/movie_metadata.csv' % (base_dir),
            'shallow%d/%d.csv' % (amount, i)
        )

def create_deep(amount):

    working_dir = "deep%d/" % amount

    os.mkdir(working_dir)
    os.mkdir("deep-output-p%d" % amount)
    os.mkdir("deep-output-t%d" % amount)

    base_dir = os.path.dirname(os.path.realpath(__file__))

    for i in range(amount):

        working_dir += '%d/' % i

        os.mkdir(working_dir)
        os.symlink(
            '%s/movie_metadata.csv' % (base_dir),
            '%s/%d.csv' % (working_dir, i)
        )


# create_shallow(1)
# create_shallow(10)
# create_shallow(100)
# create_shallow(1000)

create_deep(1)
create_deep(10)

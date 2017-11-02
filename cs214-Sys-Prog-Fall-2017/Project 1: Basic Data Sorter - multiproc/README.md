# Multiprocess sorter

## Design

The program revolves around a recursive walk on the specified input path. At
each directory it encounters, it forks a new process to walk that directory,
while it continues to walk the current directory, where it then waits for all
children to finish. If it encounters a file, it forks a new process to sort that
file. That child will exit once it finishes sorting.

## Assumptions

One of the assumptions made here were that CSV files were properly formatted
assuming that their top column was formatted according to the specified movie
metadata specification.

## Difficulties

There were many difficulties encountered during this, as many were beyond the
scope of normal memory management and programming logic. Learning to use
shared memory and forking were some of these.


## How to use


```
make clean all
./sorter -c <field> [-d <search_dir>] [-o <output_dir>]
```

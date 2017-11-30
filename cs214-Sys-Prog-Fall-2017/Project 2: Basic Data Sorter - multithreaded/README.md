# Multithreaded sorter

## Design

The program revolves around a recursive walk on the specified input path. At
each directory it encounters, it creates a new process to walk that directory,
while it continues to walk the current directory. For all new files it
encounters, it starts a new process to sort that file. For each file sort,
the file is sorted by itself into a local heap variable. After sorting, the
thread locks the global list, and then merges the file into the global list.
After unlocking, the thread joins the parents. After all files/directories
have been processed and all threads are joined, the main thread outputs the
global list contents into the final file.

## Assumptions

One of the assumptions made here were that CSV files were properly formatted
assuming that their top column was formatted according to the specified movie
metadata specification.

## Difficulties

There were several difficulties in making this project. Luckily, a lot of the
concepts were able to be reused from the previous multiprocess sorter. One
challenge was making sure that the merge_list() function (reused from the
mergesort file) was not creating new lists but integrating two lists together.
This meant that I needed to move each specific instance of head into the
new thread, and take it out from the global scope. After this was done, things
worked smoothly.

## How to use

```
make clean all
./sorter -c <field> [-d <search_dir>] [-o <output_dir>]
```

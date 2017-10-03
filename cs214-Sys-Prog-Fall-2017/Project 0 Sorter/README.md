# Design

The sorter works by first taking in the entire CSV file as a string into STDIN.
It then tokenizes the string, line by line, to be added to each row. Each line
is tokenized to populate the columns of a row struct. As each row struct is
created, it is added to the beginning of a linked list of row structs. To
perform sorting, the sorter implements a mergesort algorithm on the linked list
and uses the key provided in STDIN to perform comparisons on the specified
column.

# Assumptions

One of the biggest assumptions made here was that each entry would be cleanly
separated by commas. Unfortunately, several movie titles had commas in the, and
were therefore encompassed by double quotes. This was fixed by flagging each of
these entries, and for the purpose of tokenization, the title commas become
temporarily replaced with another character, until they are reprinted.

# Difficulties

A major difficulty was remembering all the ins and outs of pointers, especially
in and of structs. Also, dealing with offsets inside each struct became
difficult which lead to long chains of if statements instead to deal with
different column names, as apparently, C does not possess the capability for
reflection.

# Running

Run `make all` to run
and `make clean` to clean

Then run as normal:
`cat input.file | ./sorter -c  movie_title > output.csv`

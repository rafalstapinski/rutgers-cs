# Multithreaded sorter

## Design

The program revolves around a client and server.

The client performs a recursive walk over the specified input domain and creates
a thread for every new file found. That thread then connects to the a socket
on a server running on the specified host/port. It then sends over the column
to be sorter, as well as the file contents. When the client has fully traversed
the directory and received confirmation that all files have been sorted, it
sends one more request for the joined sorted files, and writes it to a file.

The server listens on a specified port. For every new connection, it spawns a
thread tied to the opened socket. It waits to receive a request. If the request
is for a file sort, it waits to receive the contents of the file and then
stores it temporarily. It then reads the whole contents of the file, and sorts
it into memory. For every new file sorted, it gets merged into the in memory
list of rows. Once it receives a request to dump all the files, it goes through
the in memory list and writes it to the client via the socket as a CSV formatted
string.


## Assumptions

One of the assumptions made here were that CSV files were properly formatted
assuming that their top column was formatted according to the specified movie
metadata specification. Another assumption was that the client was able to
connect to the server: i.e. no firewall, no problems connecting locally, or DNS
lookup. Another assumption was that files stored between writing from the client
and reading into memory on the server would not be deleted in the interim.

## Difficulties

There were several difficulties in making this project. One was making sure that
the server and client were able to keep connections open and wait for specific
requests despite having to stream large files. Since most multithreading work
was figured out in the previous project, the only headache with threads was on
the server when receiving multiple files. That was solved quickly by not joining
threads immediately, and instead killing them once they were done.

## How to use

```
make clean all
./sorter_server -p <port>
./sorter_client -c <field> -h <host> -p <port> [-d <search_dir>] [-o <output_dir>]
```

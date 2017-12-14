all: sorter_client sorter_server

sorter_client:
	gcc -pthread -o sorter_client sorter_client.c -Wall 

sorter_server:
	gcc -pthread -o sorter_server sorter_server.c -Wall

clean:
	rm -rf sorter_client
	rm -rf sorter_server

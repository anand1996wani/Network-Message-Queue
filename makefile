./driver : driver.c nmb.c local_server
	gcc -o driver driver.c nmb.c 

./local_server : 
	gcc -o local_server local_server.c nmb.c

# Network-Message-Queue
Network Message Queue

## Objective  
*	To implement a mechanism called Network Message Bus (NMB) on a LAN using UDP sockets and System V Message Queues.
*	Similar to Message Queue on single host, this API allows two processes on different hosts to exchange messages.

## Design
`local_server.c`
* creates the data store that stores the state of each connection being processed
* array based implementation implies fixed capacity and linear search

`nmb.c`
*	msgsnd_nmb() : Client uses this function to send message to receiver. Internally this request first goes to Local TCP server which processes it further ahead.
*	msgrcv_nmb() : Client uses this function to receive message sent to it. Internally this request first goes to Local TCP server which picks up the message from system queue.
*	msgget_nmb() : This function is to create the system queue.

`driver.c`
*	To test network message queue API.

## Results
*	Network Message Queue API Created.


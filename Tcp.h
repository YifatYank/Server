/************************************************************
* File description: TCP header file. the class inherit from	*
* socket. methods of tcp socket type						*
************************************************************/

#ifndef TCP_H_
#define TCP_H_

#include "Socket.h"
#include <map>

class Tcp : public Socket {
private:
    int descriptorCommunicateClient;
    map<int, int> *soketMapping;

    /***********************************************************************
* function name: Tcp												   *
* The Input: Boolean, true - if server, false if client and port number*
* The output: none										               *
* The Function operation: creating new Tcp						       *
***********************************************************************/
    Tcp(bool isServers, int port_num);

public:
    /***********************************************************************
	* function name: ~Tcp												   *
	* The Input: none													   *
	* The output: none										               *
	* The Function operation: default destructor					       *
	***********************************************************************/
    virtual ~Tcp();

    /***********************************************************************
    * function name: initialize											   *
    * The Input: none              										   *
    * The output: int number representing the return status		           *
    * The Function operation: initialize the Socket object and getting a   *
    * socket descriptor.												   *
    ***********************************************************************/
    int initialize();

    /***********************************************************************
    * function name: sendData											   *
    * The Input: string representing the data to send		               *
    * The output: int number representing the return status		           *
    * The Function operation: sending the input data to the socket         *
    * who connect to this socket. 										   *
    ***********************************************************************/
    int sendData(string data, int clientDescriptor);

    int sendDataByMapping(string data, int clientDescriptor);

    /***********************************************************************
    * function name: recive	`											   *
    * The Input: none										               *
    * The output: int number representing the return status	               *
    * The Function operation: getting data from the other socket and print *
    * the data															   *
    ***********************************************************************/
    int reciveData(char *buffer, int size, int socketDescriptor);

    int reciveDataByMapping(char *buffer, int size, int socketDescriptor);

    int acceptOneClient();

    void addMapping(int soketId, int descriptor);

    static Tcp *getTcp(bool isServers, int port_num);


};

#endif /* TCP_H_ */
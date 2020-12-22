#include <iostream>
#include <string>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <sys/uio.h>
#include <sys/time.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <fstream>

// Testmodul: Client um zu gucken, ob man sich mit einem bestehenden Server verbinden kann.

#define PORT 34567

using namespace std;

int main()
{
    string ip;
    cout<<"Enter server ip: ";
    cin>>ip;
    char msg[12] = {0};
    char data[2] = {0};

    struct hostent* host = gethostbyname(ip.c_str());
    sockaddr_in send_socket_address;
    bzero((char*)&send_socket_address, sizeof(send_socket_address));  

    send_socket_address.sin_family = AF_INET; 
    send_socket_address.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    send_socket_address.sin_port = htons(PORT);

    int client_socket = socket(AF_INET, SOCK_STREAM, 0);
    int status = connect(client_socket,(sockaddr*) &send_socket_address, sizeof(send_socket_address));

    int option = 1;
    setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    while(1)
    {
        usleep(30000);

        send(client_socket, data, sizeof(data), 0);
        memset(&data, 0, sizeof(data));

        memset(&msg, 0, sizeof(msg));
        recv(client_socket, (char*)&msg, sizeof(msg), 0);
        if(msg[8] + msg[9] < 127)
        {
            int p2_y = msg[2] + msg[3] ;
            int b_y = msg[4] + msg[5] ;
            int v_y = msg[10] + msg[11];

            if(p2_y > 127){p2_y = 256 - p2_y;}
            if(b_y > 127){b_y = 256 - b_y;}
            if(v_y > 127){v_y = 256 - v_y;}

            if(p2_y >= b_y + v_y)
            {
                data[0] = 'U';
            }

            if(p2_y + 12 <= b_y + v_y)
            {
                data[0] = 'D';
            }
        }

        if(msg[3] + msg[4] > 124)
        {
            data[0] = 'O';
            send(client_socket, data, sizeof(data), 0);
            break;
        }
        for(size_t i=0; i < sizeof(msg); ++i)printf("%d ", msg[i]);
        cout<<endl;
    }

    close(client_socket);
    cout<<"exit"<<endl;

    return 0;
}
#include <iostream>
#include "pong.h"
#include <errno.h>
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
#include <pthread.h>

// Testmodul: Generiert ein Testserver der auf Verbindung mit Client wartet.

using namespace std;


int main()
{
    cout<<"hello"<<endl;
    Pong pong;
    sockaddr_in server_address;
    sockaddr_in client_address;
    socklen_t client_address_size;

    int server_socket;
    int bind_status;
    int client_socket;

    bzero((char*)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(34567);

    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    
    int option = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));

    listen(server_socket, 10);

    client_address_size = sizeof(client_address);

    if(server_socket < 0)
    {
        cerr << "Error establishing the server socket" << endl;
        exit(0);
    }

    if(bind_status < 0)
    {
        cerr << "Error binding socket to local address" << endl;
        exit(0);
    }

    client_socket = accept(server_socket, (sockaddr *)&client_address, &client_address_size);
    if(client_socket < 0)
    {
        cerr << "Error accepting request from client!" << endl;
        exit(1);
    }

    cout << "Connected with client!" << endl;
    while(1)
    {
        //usleep(30000);
        memset(&msg, 0, sizeof(msg));
        int x= (recv(client_socket, (char*)&msg, sizeof(msg), 0));
        cout<<x<< " ";
        if(x)
        {
            cout<<msg<<endl;
            pong.move_ball();
            if(msg[0] == 'U')
            {
                pong.move_player(2, -PLAYER_SPEED);
            }

            if(msg[0] == 'D')
            {
                pong.move_player(2, PLAYER_SPEED);
            }

            if(msg[0] == 'O')
            {
                break;
            }
            
            if(pong.ball_velocity[0] < 0)
            {
                if(pong.p1_position[1] > pong.ball_position[1] + pong.ball_velocity[1])
                {
                    pong.move_player(1, -PLAYER_SPEED);
                }

                if(pong.p1_position[1] + BAT_LENGTH < pong.ball_position[1] + pong.ball_velocity[1]) 
                {
                    pong.move_player(1, PLAYER_SPEED);
                }
            }
            pong.get_data(data);
            send(client_socket, data, sizeof(data), 0);
        }

        else
        {
            cout<<strerror(errno)<<endl;
            break;
        }
        
    }
    close(client_socket);
    close(server_socket);
    return 0;
}

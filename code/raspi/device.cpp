#include "device.h"
#include <boost/algorithm/string.hpp>

Display Device::display = Display();
char Device::rcv_msg[MAX_LINE] = {0};

void Device::enter_ip(){
    int pointer = 8;
    int positions[15] = {20, 25, 30, 35, 36, 41, 46, 51, 52, 57, 62, 67, 68, 73, 78};
    while(true){
        string ip(host_ip);
        display.clear_display();
        display.draw_text(20, 20, ip);
        display.draw_horizontal_line(positions[pointer], 25, 4);
	    display.draw_text(80, 60, "connect");

        // Steuern des Zeigers
        if(button_pressed(LEFT) && pointer > 0){
            pointer -= 1;
            if(host_ip[pointer] == '.'){
                usleep(30000);
                pointer -= 1;
            }
        }

        if(button_pressed(RIGHT) && pointer < 15){
            pointer += 1;
            if(host_ip[pointer] == '.'){
                usleep(30000);
                pointer += 1;
            }
        }

        if(button_pressed(UP)){
            host_ip[pointer] += 1;
            if(host_ip[pointer] > '9'){
                usleep(30000);
                host_ip[pointer] = '0';
            }
        }

        if(button_pressed(DOWN)){
            host_ip[pointer] -= 1;
            if(host_ip[pointer] < '0')
            {
                usleep(30000);
                host_ip[pointer] = '9';
            }
        }

        if(button_pressed(ENTER)){
            sleep(1);
            break;
        }

        display.display();
    }
}

Device::Device(){
    if(map_peripheral(&gpio) == -1){
        error("error gpio");
  	}
    strcpy(host_ip,"192.168.002.214");
    setup_gpio();
}

void Device::exit(){
	beep(c_node, 500);
    display.clear_display();
    display.display();
}

void Device::error(std::string s){
	display.clear_display();
	display.draw_text(30, 30, s);
	display.display();
	sleep(2);
}

bool Device::single(Game *game, int player){
    game->reset();
    game->is_multi = false;
    while(!game->get_winner()){
        usleep(30000);
        display.clear_display();
        if(game->main_loop()){
            beep(c_node, 20);
        }
        game->control(player);
        game->bot(2);
        game->draw(display, player);
        display.display();
    }
    return player == game->get_winner();
}

void Device::get_ip(){
    display.clear_display();
    system("ifconfig > inegame.txt");
    system("grep \'inet \' inegame.txt > ip.txt");
    string line;
    ifstream file("ip.txt");
    int pos_y = 0;
    int pos_x;
    if(file.is_open()){
        while(getline(file, line)){
            pos_x = 30;
            for(int i = 13; line[i] != ' ';++i){
                pos_x += display.draw_char(pos_x, pos_y, line[i]) + 2;
		    }
            pos_y += 10;
        }
        file.close();
    }
    system("rm -rf inegame.txt ip.txt");
    display.display();
}

int Device::menu(vector<string> texts, bool music){
    pthread_t thread_id;
	if(music){
	    pthread_create(&thread_id, NULL, play_music, NULL);
    }
    int max_page = (texts.size()-1)%3 == 0 ? (texts.size()-1)/3 - 1 : (texts.size()-1)/3;
    int current_page = 0;
    int pointer = 18;
    size_t index = 1;
    while(true){
        int max_mode = 0;
        usleep(30000);
        display.clear_display();
        display.fill_rect(25, pointer, 2, 2);
        int y = 5;
        display.draw_text(40, y, texts[0]);
        y += 15;
        size_t current_item = current_page * 3 + 1;
        for(int i=0; i<3; ++i){
            if(i){
                current_item += 1;
            }
            if(current_item < texts.size()){
                display.draw_text(30, y, texts[current_item]);
                y += 15;
            }
        }
        if(button_pressed(RIGHT) && current_page < max_page){
            usleep(30000);
            current_page += 1;
            pointer = 18;
            index = current_page * 3 + 1;
        }
        if(button_pressed(LEFT) && current_page > 0){
            usleep(30000);
            current_page -= 1;
            pointer = 18;
            index = current_page * 3 + 1;
        }
        if(!(current_page || max_page)){
            max_mode = texts.size() - 1;
        }
        else if(current_page < max_page){
            max_mode = 3;
        }
        else{
            max_mode = texts.size()%4; 
        }
        if(button_pressed(DOWN) && pointer < 18 + 16 * (max_mode-1)){
            usleep(30000);
            pointer += 16;
            index += 1;
        }
        if(button_pressed(UP) && pointer > 18){
            usleep(30000);
            pointer -= 16;
            index -= 1;
        }
        if(button_pressed(ENTER))
        {
            beep(c_node, 20);
            if(music){
                pthread_cancel(thread_id);
            }
            usleep(500000);
            return index == texts.size() - 1 ? 0 : index;
        }
        display.display();
    }
}

string Device::ip_to_number(char a, char b, char c){
    int x = (a - '0') * 100 + (b - '0') * 10 + c - '0';
    return to_string(x);
}

bool Device::join(Game *game, int player){
    enter_ip();
    game->reset();
    game->is_multi = true;
    char send_msg[2] = {0};
    std::string ip = ip_to_number(host_ip[0], host_ip[1], host_ip[2]) + "."
                        + ip_to_number(host_ip[4], host_ip[5], host_ip[6]) + "."
                        + ip_to_number(host_ip[8], host_ip[9], host_ip[10]) + "."
                        + ip_to_number(host_ip[12], host_ip[13], host_ip[14]);
    
    struct hostent* host = gethostbyname(ip.c_str());
    sockaddr_in send_socket_address;
    bzero((char*)&send_socket_address, sizeof(send_socket_address));  

    send_socket_address.sin_family = AF_INET; 
    send_socket_address.sin_addr.s_addr = inet_addr(inet_ntoa(*(struct in_addr*)*host->h_addr_list));
    send_socket_address.sin_port = htons(PORT);

    int option;
    int status;
    int client_socket = socket(AF_INET, SOCK_STREAM, 0);

    if(client_socket < 0){
        goto error;
    }
    status = connect(client_socket,(sockaddr*) &send_socket_address, sizeof(send_socket_address));
    if(status < 0){
        goto error;
    }

    option = 1;
    setsockopt(client_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if(game->is_mmo){
        recv(client_socket, (char*)&rcv_msg, MAX_LINE, 0);
        player = stoi(string(rcv_msg));
        cout<<player<<endl;
    }
    send_msg[0] = 'H';
    send_msg[1] = player + '0';

    while(1)
    {
        display.clear_display();
        if(send(client_socket, send_msg, sizeof(send_msg), 0) < 0){
            goto error;
        }
        memset(&send_msg, 0, sizeof(send_msg));

        memset(&rcv_msg, 0, sizeof(rcv_msg));
        if(recv(client_socket, (char*)&rcv_msg, MAX_LINE, 0) < 0){
            goto error;
        }
        send_msg[0] = 'H';
        send_msg[1] = player + '0';
        game->set_data(rcv_msg);
        game->draw(display, player);
        if(button_pressed(UP)){
            send_msg[0] = 'U';
        }
        if(button_pressed(DOWN)){
            send_msg[0] = 'D';
        }
        if(button_pressed(LEFT)){
            send_msg[0] = 'L';
        }
        if(button_pressed(RIGHT)){
            send_msg[0] = 'R';
        }
        if(button_pressed(ENTER)){
            send_msg[0] = 'E';
            if(game->is_mmo){
                send_msg[0] = '0';
                send(client_socket, send_msg, sizeof(send_msg), 0);
                return false;
            }
        }
        if(game->get_winner()){
            send_msg[0] = 'O';
            send(client_socket, send_msg, sizeof(send_msg), 0);
            return player == game->get_winner();
        }
        display.display(false);
    }
    error:
        error(strerror(errno));
        close(client_socket);
        throw -1;
        return false;
}

bool Device::host(Game *game, int player){
    game->reset();
    game->is_multi = true;
    bool connected = false;
    bool leave = false;
    get_ip();
    display.draw_text(10, 40, "waiting for player");
    display.draw_text(100, 60, "exit");
    display.display();

    int server_socket;
    server_socket = socket(AF_INET, SOCK_STREAM, 0);
    int option = 1;
    setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &option, sizeof(option));

    if(server_socket < 0)
    {
        error(strerror(errno));
        throw -1;
    }
    thread t(server, game, ref(server_socket), ref(connected), ref(leave), player);

    while(!connected)
    {
        if(button_pressed(ENTER))
        {
            usleep(300000);
            beep(c_node, 20);
            leave = true;
            shutdown(server_socket, SHUT_RD);
            close(server_socket);
            t.detach();
            throw -1;
        }
    }

    t.join();

    return player == game->get_winner();
}

void Device::server(Game *game, int &server_socket, bool &connected, bool &leave, int player){
    sockaddr_in server_address;
    sockaddr_in client_address;
    socklen_t client_address_size;

    bzero((char*)&server_address, sizeof(server_address));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = htonl(INADDR_ANY);
    server_address.sin_port = htons(PORT);

    int bind_status = bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
    if(bind_status < 0)
    {
        goto exit;
    }

    listen(server_socket, game->max_players-1);

    int client_socket;

    client_address_size = sizeof(client_address);
    if(!leave)
    {
        client_socket = accept(server_socket, (sockaddr *)&client_address, &client_address_size);
    }
    else 
    {
        goto exit;
    }

    if(client_socket < 0)
    {
        goto exit;
    }
    connected = true;

    while(1)
    {
        memset(&rcv_msg, 0, sizeof(rcv_msg));
        if(recv(client_socket, (char*)&rcv_msg, sizeof(rcv_msg), 0)){
            game->control(rcv_msg[1] - '0', rcv_msg[0]);
            if(rcv_msg[0] == 'O')goto exit;
            if(game->main_loop())
            {
                beep(c_node, 20);
            }
            game->control();
            string data = game->get_data();      
            if(send(client_socket, data.c_str(), data.size(), 0) < 0)
            {
                error(strerror(errno));
                goto exit;
            }
            display.clear_display();
            game->draw(display, player);
            display.display();
        }
        else{
            error("lost connection");
            goto exit;
        }        
    }
    exit:
        close(client_socket);
        close(server_socket);
    return ;
}
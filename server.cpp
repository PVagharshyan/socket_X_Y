#include <iostream>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define PORT 8888
#define CLIENT_COUNT 2
#define BUFFER_SIZE 1024
#define X 3
#define Y 3

#define SIG_LENGTH 2

std::string matrix_to_string(int** matrix, int x, int y) {
    std::string result = "";
    for (int i {}; i < y; ++i) {
        for (int j {}; j < x; ++j) {
            result += (char)((int)matrix[i][j] + 48); 
        }
    }
    result = "m" + result;
    return result;
}

bool check_matrix_for_player(int** matrix, int x, int y, int player_key) {
    for (int i {}; i < y; ++i) {
        bool flag = true;
        for (int j {}; j < x; ++j) {
            if (matrix[i][j] != player_key) {
                flag = false;
            }
        }
        if (flag) {
            std::cout << "horizonakan" << std::endl;
            return true;
        }
    }
    for (int j {}; j < x; ++j) {
        bool flag = true;
        for (int i {}; i < y; ++i) {
            if (matrix[i][j] != player_key) {
                flag = false;  
            }
        }
        if (flag) {
            std::cout << "vertikal" << std::endl;
            return true;
        }
    }
    int count = 0;
    for (int i {}; i < y; ++i) {
        for (int j {}; j < x; ++j) {
            if (i == j && matrix[i][j] == player_key) {
                ++count;
            }
        }
    }
    if (count == 3) {
        return true;
    }
    count = 0;
    for (int i {}; i < y; ++i) {
        for (int j {}; j < x; ++j) {
            if (i + j == X && matrix[i][j] == player_key) {
                ++count;
            }
        }
    }
    if (count == 3) {
        return true;
    }
    return false;
}

int main() {

    int** matrix = new int*[Y];
    for (int i {}; i < Y; ++i) {
        matrix[i] = new int[X];
        for (int j {}; j < X; ++j) {
            matrix[i][j] = 0;
        }
    }
//server_criation-------------------------------------------

    struct sockaddr_in addr;
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    int server = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t size = sizeof(addr);

    if (bind(server, (struct sockaddr*)&addr,sizeof(addr)) < 0) {
        std::cout << "Error:with binding!" << std::endl;
        return -1;
    }

    if (listen(server, 3) < 0) {
        std::cout << "Error:with listening" << std::endl;
        return -1;
    }
    else {
        std::cout << "Server start listen: port " << PORT << std::endl;
    }

//clients ------------------------------------------------------

    std::string player_1_name = "";
    std::string player_2_name = "";
    
    int client_1, client_2;

    client_1 = accept(server, (struct sockaddr*)&addr, &size);
    std::cout << "spasum enq erkrodi mianalun" << std::endl;
    client_2 = accept(server, (struct sockaddr*)&addr, &size);
    
    if (client_1 < 0 || client_2 < 0) {
        std::cout << "Error: with connection!" << std::endl;
        return -1;
    }
    else {
        std::cout << "Player #1 connected!" << std::endl;
        char buffer[BUFFER_SIZE];
        send(client_1, "start", 10, 0);
        recv(client_1, buffer, BUFFER_SIZE, 0);
        std::cout << "Player #1 name: " << buffer << std::endl;
        player_1_name = buffer;
    
        std::cout << "Player #2 connected!" << std::endl;
        send(client_2, "start", 10, 0);
        recv(client_2, buffer, BUFFER_SIZE, 0);
        std::cout << "Player #2 name: " << buffer << std::endl;
        player_2_name = buffer;
    }

//--------------------------------------------------------------

//yntacq---------------------------------------------------------

    bool flag = true;
    while (true) {
        if (flag) {
            if (check_matrix_for_player(matrix, X, Y, 2)) {
                send(client_2, "#", 2, 0);
                send(client_1, "*", 2, 0);
                break;
            }

            send(client_1, "k1", 4, 0);
            
            //send matrix----------------------------------

            char buffer_matrix[BUFFER_SIZE];
            std::string str_matrix = matrix_to_string(matrix, X, Y);
            int i = 0;
            for (; i < str_matrix.length(); ++i) {
                buffer_matrix[i] = str_matrix[i];
            }
            buffer_matrix[i] = '\0';
            
            send(client_1, buffer_matrix, BUFFER_SIZE, 0);

            //-----------------------------------------------


            std::cout << player_1_name << std::endl;
            send(client_1, "Hello_player_1", BUFFER_SIZE, 0);
            std::cout << "spasum enq player_1-i patasxanin" << std::endl;
            char buffer[BUFFER_SIZE];
            send(client_1, "start", 10, 0);
            recv(client_1, buffer, BUFFER_SIZE, 0);
            std::cout << "player_1 patasxanec: " << buffer << std::endl;
            
            //update-----------------------------------------

            int k = 0;
            for (int i {}; i < Y; ++i) {
                for (int j {}; j < X; ++j) {
                    matrix[i][j] = (int)buffer[++k] - (int)'0';
                }
            }
            
            str_matrix = matrix_to_string(matrix, X, Y);
            i = 0;
            for (; i < str_matrix.length(); ++i) {
                buffer_matrix[i] = str_matrix[i];
            }
            buffer_matrix[i] = '\0';
            
            std::cout << buffer_matrix << std::endl; 
            send(client_2, buffer_matrix, BUFFER_SIZE, 0);

            //--------------------------------------------------
        }
        else {
            if (check_matrix_for_player(matrix, X, Y, 1)) {
                send(client_1, "#", 2, 0);
                send(client_2, "*", 2, 0);
                break;
            }
            
            send(client_2, "k2", 4, 0);
            
            //send matrix----------------------------------

            char buffer_matrix[BUFFER_SIZE];
            std::string str_matrix = matrix_to_string(matrix, X, Y);
            int i = 0;
            for (; i < str_matrix.length(); ++i) {
                buffer_matrix[i] = str_matrix[i];
            }
            buffer_matrix[i] = '\0';
            
            send(client_2, buffer_matrix, BUFFER_SIZE, 0);

            //-----------------------------------------------
            
            std::cout << player_2_name << std::endl;
            send(client_2, "Hello_player_2", BUFFER_SIZE, 0);
            std::cout << "spasum enq player_2-i patasxanin" << std::endl;
            char buffer[BUFFER_SIZE];
            send(client_2, "start", 10, 0);
            recv(client_2, buffer, BUFFER_SIZE, 0);
            std::cout << "player_2 patasxanec: " << buffer << std::endl;
            
            //update-----------------------------------------

            int k = 0;
            for (int i {}; i < Y; ++i) {
                for (int j {}; j < X; ++j) {
                    matrix[i][j] = (int)buffer[++k] - (int)'0';
                }
            }
            
            str_matrix = matrix_to_string(matrix, X, Y);
            i = 0;
            for (; i < str_matrix.length(); ++i) {
                buffer_matrix[i] = str_matrix[i];
            }
            buffer_matrix[i] = '\0';
            std::cout << buffer_matrix << std::endl; 
            send(client_1, buffer_matrix, BUFFER_SIZE, 0);

            //--------------------------------------------------
        }
        flag = !flag;
    }

//--------------------------------------------------------------------

    close(client_1);
    close(client_2);
    close(server);
    return 0;
}

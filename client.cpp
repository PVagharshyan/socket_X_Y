#include <iostream>
#include <cstring>
#include <string>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>

#define BUFFER_SIZE 1024
#define PORT 8888
#define CLIENT_COUNT 2
#define X 3
#define Y 3

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

void print_matrix(int** matrix, int x, int y) {
    for (int i {}; i < y; ++i) {
        for (int j {}; j < x; ++j) {
            std::cout << matrix[i][j] << " ";
        }
        std::cout << std::endl;
    }
}

int main() {
    int player_key = 0;

    int** matrix = new int*[Y];
    for (int i {}; i < Y; ++i) {
        matrix[i] = new int[X];
        for (int j {}; j < X; ++j) {
            matrix[i][j] = 0;
        }
    }

//server-i stexcum -----------------------------------------------

    struct sockaddr_in addr;
    
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = INADDR_ANY;
    addr.sin_port = htons(PORT);

    int client = socket(AF_INET, SOCK_STREAM, 0);
    socklen_t size = sizeof(addr);

    if (connect(client, (struct sockaddr*)&addr, size) < 0) {
        std::cout << "Error! with client side connection!" << std::endl;
        return -1;
    }
    std::cout << "Client connected!" << std::endl;
//----------------------------------------------------------------

//kardum e anun ev uxarkum server ------------------------------

    while (true) {
        char buffer[BUFFER_SIZE];
        recv(client, buffer, BUFFER_SIZE, 0);
        break;
    }
    std::string Name = "";
    std::cout << "input yout name: ";
    std::cin >> Name;
    char buffer_name[BUFFER_SIZE] {0};
    for (int i {}; i < Name.length(); ++i) {
        buffer_name[i] = Name[i];
    }
    send(client, buffer_name, BUFFER_SIZE, 0);

//-----------------------------------------------------------------

//xaxi yntcq------------------------------------------------------
  
    while(true){
        char buffer[BUFFER_SIZE];
        int server_ms = recv(client, buffer, BUFFER_SIZE, 0);
        if (server_ms <= 0){
            continue;
        }
        else if (buffer[0] == '#') {
            std::cout << "duq haxteciq!!" << std::endl;
            break;
        }
        else if (buffer[0] == '*') {
            std::cout << "duq partvel eq!!" << std::endl;
            break;
        }
        else if (buffer[0] == 'k') {
            player_key = (int)buffer[1] - (int)'0'; 
        }
        else if (buffer[0] == 'm') {
            int k = 0;
            for (int i {}; i < Y; ++i) {
                for (int j {}; j < X; ++j) {
                    matrix[i][j] = (int)buffer[++k] - (int)'0';
                }
            }
            print_matrix(matrix, X, Y);
        }
        else {
            std::cout << "hraver-y ekav serveric" << std::endl;
            std::cout << buffer << std::endl;
            std::cout << "spasum enq myusi patasxanin" << std::endl;
            while (true) {
                char buffer_start[BUFFER_SIZE];
                recv(client, buffer_start, BUFFER_SIZE, 0);
                if (buffer_start[0] == 's')
                break;
                else continue;
            }

            int x = -1;
            int y = -1;
            while (true){
                std::cout << "X : ";
                std::cin >> x;
                std::cout << "Y : ";
                std::cin >> y;
                if (matrix[y][x] != 0) {
                    std::cout << "vandaky azat che!" << std::endl;
                    continue;
                }
                if (x >= 0 && x < X && y >= 0 && y < Y) break;
            }
            
            matrix[y][x] = player_key;

            char buffer_matrix[BUFFER_SIZE];
            std::string str_matrix = matrix_to_string(matrix, X, Y);
            int i = 0;
            for (; i < str_matrix.length(); ++i) {
                buffer_matrix[i] = str_matrix[i];
            }
            buffer_matrix[i] = '\0';
            
            send(client, buffer_matrix, BUFFER_SIZE, 0);
        }
    }

//-------------------------------------------------------------

    close(client);

    return 0;
}

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <cstring> 
#include <unistd.h> 
#include <arpa/inet.h>
#include <string>
#include "mylib.h"

std::mutex mtx;
std::condition_variable cv;
std::string shared_buffer;
bool data_available = false;
volatile bool stop_threads = false;

const char SERVER_IP[] = "127.0.0.1";
const int SERVER_PORT = 12345;

void print_without_service_info(const std::string str){
    int i = 0;
    while (str[i] != '[')
    {
        std::cout << str[i++];
    }
}

void send_to_program2(const std::string& data) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1) {
        std::cerr << ">Error: Could not create socket." << std::endl;
        return;
    }

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(SERVER_PORT);

    if (inet_pton(AF_INET, SERVER_IP, &server.sin_addr) <= 0) {
        std::cerr << ">Error: Invalid address." << std::endl;
        close(sock);
        return;
    }

    if (connect(sock, (struct sockaddr*)&server, sizeof(server)) < 0) {
        std::cerr << ">Error: Connection failed." << std::endl;
        close(sock);
        return;
    }

    if (send(sock, data.c_str(), data.size(), 0) < 0) {
        std::cerr << ">Error: Send failed." << std::endl;
        close(sock);
        return;
    }

    std::cout << ">The data was sent successfully!" << std::endl;
    close(sock);
}
void thread2() {
    while (!stop_threads) {
        std::unique_lock<std::mutex> ulock(mtx);
        cv.wait(ulock, [] { return data_available || stop_threads; });

        if (stop_threads) break;

        std::string data = shared_buffer;
        shared_buffer.clear();
        data_available = false;
        ulock.unlock();

        std::cout << ">Received data = " << data << std::endl;
        data = std::to_string(my_func2(data));

        send_to_program2(data);
    }
}

int main() {
    std::setlocale(LC_ALL, "");
    const int MAX_INPUT_LENGTH = 64;
    std::thread th(thread2);

    std::cout << ">Please enter a numeric value up to "<< MAX_INPUT_LENGTH << " characters." << std::endl;

    while (true) {
        std::string input;
        
        std::cin >> input;

        if (input.size() > MAX_INPUT_LENGTH || input.find_first_not_of("0123456789") != std::string::npos) {
            std::cerr << ">Error: incorect data, try again (numeric value up to " << MAX_INPUT_LENGTH << " characters.)" << std:: endl;
            continue;
        }

        my_func1(input);

        while (data_available) {
            std::this_thread::yield();
        }
        
        std::unique_lock<std::mutex>ulock(mtx);
        shared_buffer = input;
        data_available = true;
        ulock.unlock();

        cv.notify_one();
    }

    cv.notify_all();
    stop_threads = true;
    th.join();

    return 0;
}

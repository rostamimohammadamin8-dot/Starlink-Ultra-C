#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <chrono>
#include <fstream>
#include <netinet/in.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>

using namespace std;

// Starlink Ultra-Low Latency Target (ms)
const int PING_THRESHOLD = 100; 

bool check_latency(string ip) {
    struct sockaddr_in server_addr;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) return false;

    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(443);
    inet_pton(AF_INET, ip.c_str(), &server_addr.sin_addr);

    auto start = chrono::high_resolution_clock::now();
    struct timeval tv;
    tv.tv_sec = 1; tv.tv_usec = 0;
    setsockopt(sock, SOL_SOCKET, SO_RCVTIMEO, (const char*)&tv, sizeof tv);

    int result = connect(sock, (struct sockaddr *)&server_addr, sizeof(server_addr));
    auto end = chrono::high_resolution_clock::now();
    close(sock);

    if (result == 0) {
        auto duration = chrono::duration_cast<chrono::milliseconds>(end - start).count();
        return (duration < PING_THRESHOLD);
    }
    return false;
}

int main() {
    // Cloudflare US IP Ranges for Starlink Simulation
    vector<string> ip_range = {"104.16.80.1", "104.17.210.5", "172.67.150.10", "104.18.2.10"};
    ofstream result_file("clean_ips.txt");

    for (const auto& ip : ip_range) {
        if (check_latency(ip)) {
            result_file << ip << endl;
        }
    }

    result_file.close();
    return 0;
}

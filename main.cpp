#include <iostream>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

string base64_encode(const string &in) {
    static const string lookup = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    string out; int val = 0, valb = -6;
    for (unsigned char c : in) {
        val = (val << 8) + c; valb += 8;
        while (valb >= 0) { out.push_back(lookup[(val >> valb) & 0x3F]); valb -= 6; }
    }
    if (valb > -6) out.push_back(lookup[((val << 8) >> (valb + 8)) & 0x3F]);
    while (out.size() % 4) out.push_back('=');
    return out;
}

int main() {
    // Starlink Ultra-Advanced Logic
    string uuid = "77777777-7777-7777-7777-777777777777"; // You can replace with your own UUID
    string sni = "amin-starlink.workers.dev";
    string target_ip = "104.16.80.1"; // Will be updated by scanner

    stringstream config;
    config << "vless://" << uuid << "@" << target_ip << ":443"
           << "?encryption=none&security=tls&sni=" << sni 
           << "&fp=chrome&type=ws&host=" << sni 
           << "&path=%2F%3Fed%3D2048#Starlink-Ultra-C-Engine";

    ofstream subFile("sub.txt");
    if (subFile.is_open()) {
        subFile << base64_encode(config.str());
        subFile.close();
    }
    return 0;
}

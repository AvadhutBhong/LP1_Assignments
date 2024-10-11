#include <iostream>
#include <cstdlib> // for system()
#include <ctime>

void sync_ntp_time(const std::string& server) {
    std::string command = "ntpdate -q " + server;
    int result = system(command.c_str());

    if (result == 0) {
        std::time_t now = std::time(nullptr);
        std::cout << "NTP Server: " << server << std::endl;
        std::cout << "Local Time synchronized: " << std::ctime(&now);
    } else {
        std::cerr << "Error synchronizing time with NTP server." << std::endl;
    }
}

int main() {
    std::string ntp_server = "pool.ntp.org";  // Replace with your preferred NTP server
    sync_ntp_time(ntp_server);
    return 0;
}

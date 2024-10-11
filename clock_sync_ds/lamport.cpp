#include <iostream>

class LamportClock {
private:
    int clock;

public:
    LamportClock() : clock(0) {}

    int get_time() const {
        return clock;
    }

    void tick() {
        clock++;
    }
};

int main() {
    // Create two Lamport clocks
    LamportClock clock1;
    LamportClock clock2;

    // Simulate events and clock updates
    std::cout << "Initial Clock 1: " << clock1.get_time() << std::endl;
    std::cout << "Initial Clock 2: " << clock2.get_time() << std::endl;

    clock1.tick();
    std::cout << "Clock 1 after tick: " << clock1.get_time() << std::endl;

    clock2.tick();
    std::cout << "Clock 2 after tick: " << clock2.get_time() << std::endl;

    // Compare clock values
    if (clock1.get_time() < clock2.get_time()) {
        std::cout << "Clock 1 is behind Clock 2" << std::endl;
    } else if (clock1.get_time() > clock2.get_time()) {
        std::cout << "Clock 2 is behind Clock 1" << std::endl;
    } else {
        std::cout << "Clock 1 and Clock 2 are synchronized" << std::endl;
    }

    return 0;
}

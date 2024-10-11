#include <iostream>
#include <vector>
#include <iomanip> // for formatted output
using namespace std;

class OptimalPageReplacement {
private:
    int noOfPages, capacity, hit, fault;
    vector<int> pages, frame;
    vector<vector<int>> table;
    bool isFull;

public:
    OptimalPageReplacement(int noOfPages, int capacity, vector<int> pages)
        : noOfPages(noOfPages), capacity(capacity), pages(pages), hit(0), fault(0), isFull(false) {
        frame.assign(capacity, -1);
        table.assign(noOfPages, vector<int>(capacity, -1));
    }

    void simulate() {
        int ptr = 0;

        for (int i = 0; i < noOfPages; i++) {
            int page = pages[i];

            // Check if page is already in the frame (hit or miss)
            int search = searchFrame(page);

            if (search == -1) { // Page fault
                if (isFull) {
                    ptr = getOptimalIndex(i);
                }
                frame[ptr] = page;
                fault++;
                cout << setw(4) << "F";

                if (!isFull) {
                    ptr++;
                    if (ptr == capacity) {
                        ptr = 0;
                        isFull = true;
                    }
                }
            } else { // Page hit
                hit++;
                cout << setw(4) << "H";
            }

            // Copy current frame status into the table for display
            table[i] = frame;
        }
    }

    void displayResults() {
        cout << "\n----------------------------------------------------------------------\n";
        for (int i = 0; i < capacity; i++) {
            for (int j = 0; j < noOfPages; j++) {
                cout << setw(3) << table[j][i] << " ";
            }
            cout << endl;
        }

        cout << "----------------------------------------------------------------------\n";
        double hitRatio = ((double)hit / noOfPages) * 100;
        double faultRatio = ((double)fault / noOfPages) * 100;

        cout << "Page Faults: " << fault << "\nPage Hits: " << hit << endl;
        cout << "Hit Ratio: " << fixed << setprecision(2) << hitRatio << "%" << endl;
        cout << "Fault Ratio: " << fixed << setprecision(2) << faultRatio << "%" << endl;
    }

private:
    int searchFrame(int page) {
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == page) {
                return j; // Page found in frame
            }
        }
        return -1; // Page not found (page fault)
    }

    int getOptimalIndex(int currentIndex) {
        vector<int> index(capacity, 200);  // 200 is used as a placeholder for pages that will not be used
        vector<bool> indexFlag(capacity, false);

        for (int j = currentIndex + 1; j < noOfPages; j++) {
            for (int k = 0; k < capacity; k++) {
                if (pages[j] == frame[k] && !indexFlag[k]) {
                    index[k] = j; // Set future index for page
                    indexFlag[k] = true;
                    break;
                }
            }
        }

        // Find the page that will not be used for the longest time (or not used at all)
        int maxIndex = index[0];
        int optimalPtr = 0;
        for (int j = 1; j < capacity; j++) {
            if (index[j] > maxIndex) {
                maxIndex = index[j];
                optimalPtr = j;
            }
        }

        return optimalPtr;
    }
};

int main() {
    int noOfPages, capacity;
    cout << "Enter the number of pages: ";
    cin >> noOfPages;

    vector<int> pages(noOfPages);
    cout << "Enter the pages: ";
    for (int i = 0; i < noOfPages; i++) {
        cin >> pages[i];
    }

    cout << "Enter the capacity of the frame: ";
    cin >> capacity;

    // Create an object of OptimalPageReplacement
    OptimalPageReplacement optimal(noOfPages, capacity, pages);

    cout << "\n----------------------------------------------------------------------\n";
    optimal.simulate(); // Simulate the optimal page replacement
    optimal.displayResults(); // Display the results

    return 0;
}
#include <iostream>
#include <vector>
#include <iomanip> // for formatted output
using namespace std;

class OptimalPageReplacement {
private:
    int noOfPages, capacity, hit, fault;
    vector<int> pages, frame;
    vector<vector<int>> table;
    bool isFull;

public:
    OptimalPageReplacement(int noOfPages, int capacity, vector<int> pages)
        : noOfPages(noOfPages), capacity(capacity), pages(pages), hit(0), fault(0), isFull(false) {
        frame.assign(capacity, -1);
        table.assign(noOfPages, vector<int>(capacity, -1));
    }

    void simulate() {
        int ptr = 0;

        for (int i = 0; i < noOfPages; i++) {
            int page = pages[i];

            // Check if page is already in the frame (hit or miss)
            int search = searchFrame(page);

            if (search == -1) { // Page fault
                if (isFull) {
                    ptr = getOptimalIndex(i);
                }
                frame[ptr] = page;
                fault++;
                cout << setw(4) << "F";

                if (!isFull) {
                    ptr++;
                    if (ptr == capacity) {
                        ptr = 0;
                        isFull = true;
                    }
                }
            } else { // Page hit
                hit++;
                cout << setw(4) << "H";
            }

            // Copy current frame status into the table for display
            table[i] = frame;
        }
    }

    void displayResults() {
        cout << "\n----------------------------------------------------------------------\n";
        for (int i = 0; i < capacity; i++) {
            for (int j = 0; j < noOfPages; j++) {
                cout << setw(3) << table[j][i] << " ";
            }
            cout << endl;
        }

        cout << "----------------------------------------------------------------------\n";
        double hitRatio = ((double)hit / noOfPages) * 100;
        double faultRatio = ((double)fault / noOfPages) * 100;

        cout << "Page Faults: " << fault << "\nPage Hits: " << hit << endl;
        cout << "Hit Ratio: " << fixed << setprecision(2) << hitRatio << "%" << endl;
        cout << "Fault Ratio: " << fixed << setprecision(2) << faultRatio << "%" << endl;
    }

private:
    int searchFrame(int page) {
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == page) {
                return j; // Page found in frame
            }
        }
        return -1; // Page not found (page fault)
    }

    int getOptimalIndex(int currentIndex) {
        vector<int> index(capacity, 200);  // 200 is used as a placeholder for pages that will not be used
        vector<bool> indexFlag(capacity, false);

        for (int j = currentIndex + 1; j < noOfPages; j++) {
            for (int k = 0; k < capacity; k++) {
                if (pages[j] == frame[k] && !indexFlag[k]) {
                    index[k] = j; // Set future index for page
                    indexFlag[k] = true;
                    break;
                }
            }
        }

        // Find the page that will not be used for the longest time (or not used at all)
        int maxIndex = index[0];
        int optimalPtr = 0;
        for (int j = 1; j < capacity; j++) {
            if (index[j] > maxIndex) {
                maxIndex = index[j];
                optimalPtr = j;
            }
        }

        return optimalPtr;
    }
};

int main() {
    int noOfPages, capacity;
    cout << "Enter the number of pages: ";
    cin >> noOfPages;

    vector<int> pages(noOfPages);
    cout << "Enter the pages: ";
    for (int i = 0; i < noOfPages; i++) {
        cin >> pages[i];
    }

    cout << "Enter the capacity of the frame: ";
    cin >> capacity;

    // Create an object of OptimalPageReplacement
    OptimalPageReplacement optimal(noOfPages, capacity, pages);

    cout << "\n----------------------------------------------------------------------\n";
    optimal.simulate(); // Simulate the optimal page replacement
    optimal.displayResults(); // Display the results

    return 0;
}

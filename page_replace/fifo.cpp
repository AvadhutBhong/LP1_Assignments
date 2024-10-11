#include <iostream>
#include <vector>
#include <iomanip> // for formatting output

using namespace std;

class FIFOPageReplacement {
private:
    int noOfPages, capacity, hit, fault, index;
    vector<int> pages, frame;
    vector<vector<int>> table;

public:
    FIFOPageReplacement(int noOfPages, int capacity, vector<int> pages)
        : noOfPages(noOfPages), capacity(capacity), pages(pages), hit(0), fault(0), index(0) {
        frame.assign(capacity, -1);
        table.assign(noOfPages, vector<int>(capacity, -1));
    }

    void simulate() {
        for (int i = 0; i < noOfPages; i++) {
            int search = searchFrame(pages[i]);

            if (search == -1) {
                replaceFrame(i);
                cout << setw(4) << "F";
            } else {
                hit++;
                cout << setw(4) << "H";
            }

            // Copy the current frame status into the table for output
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
        double faultRatio = ((double) fault / noOfPages) * 100;
        double hitRatio = ((double) hit / noOfPages) * 100;

        cout << "Page Faults: " << fault << "\nPage Hits: " << hit << endl;
        cout << "Hit Ratio: " << fixed << setprecision(2) << hitRatio << "%" << endl;
        cout << "Fault Ratio: " << fixed << setprecision(2) << faultRatio << "%" << endl;
    }

private:
    int searchFrame(int page) {
        for (int j = 0; j < capacity; j++) {
            if (frame[j] == page) {
                return j; // page found in frame
            }
        }
        return -1; // page not found
    }

    void replaceFrame(int currentPage) {
        frame[index] = pages[currentPage];
        fault++;
        index = (index + 1) % capacity; // FIFO index rotation
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

    // Create an object of FIFOPageReplacement
    FIFOPageReplacement fifo(noOfPages, capacity, pages);

    cout << "\n----------------------------------------------------------------------\n";
    fifo.simulate(); // Simulate the FIFO page replacement
    fifo.displayResults(); // Display the final results

    return 0;
}

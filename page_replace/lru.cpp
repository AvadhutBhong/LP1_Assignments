#include <iostream>
#include <vector>
#include <queue>
#include <algorithm> // for find
#include <iomanip>   // for formatting output

using namespace std;

class LRUPageReplacement {
private:
    int noOfPages, capacity, hit, fault;
    vector<int> pages, frame;
    vector<vector<int>> table;
    queue<int> recentQueue;

public:
    LRUPageReplacement(int noOfPages, int capacity, vector<int> pages)
        : noOfPages(noOfPages), capacity(capacity), pages(pages), hit(0), fault(0) {
        frame.assign(capacity, -1);
        table.assign(noOfPages, vector<int>(capacity, -1));
    }

    void simulate() {
        for (int i = 0; i < noOfPages; i++) {
            int page = pages[i];

            // Check if page is already in the frame
            int search = searchFrame(page);

            if (search == -1) { // Page fault occurs
                replaceFrame(page);
                cout << setw(4) << "F";
            } else { // Page hit
                hit++;
                cout << setw(4) << "H";
                // Remove the page from the queue and add it again to update its recent status
                removeAndPushToBack(page);
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
                return j; // Page found in frame (hit)
            }
        }
        return -1; // Page not found (fault)
    }

    void replaceFrame(int page) {
        if (recentQueue.size() == capacity) {
            // Remove the least recently used page (front of the queue)
            int lruPage = recentQueue.front();
            recentQueue.pop();

            // Find the frame index of the LRU page and replace it
            auto it = find(frame.begin(), frame.end(), lruPage);
            if (it != frame.end()) {
                int index = distance(frame.begin(), it);
                frame[index] = page;
            }
        } else {
            // Place the new page into the next available frame
            for (int j = 0; j < capacity; j++) {
                if (frame[j] == -1) {
                    frame[j] = page;
                    break;
                }
            }
        }
        fault++;
        recentQueue.push(page); // Add the new page to the back of the queue
    }

    void removeAndPushToBack(int page) {
        queue<int> tempQueue;

        // Remove the current page from the queue
        while (!recentQueue.empty()) {
            int frontPage = recentQueue.front();
            recentQueue.pop();
            if (frontPage != page) {
                tempQueue.push(frontPage);
            }
        }
        // Add the page back as the most recently used
        recentQueue = tempQueue;
        recentQueue.push(page);
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

    // Create an object of LRUPageReplacement
    LRUPageReplacement lru(noOfPages, capacity, pages);

    cout << "\n----------------------------------------------------------------------\n";
    lru.simulate(); // Simulate the LRU page replacement
    lru.displayResults(); // Display the final results

    return 0;
}

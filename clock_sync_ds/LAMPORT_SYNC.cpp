// C++ program to illustrate Lamport's Logical Clock

#include <iostream>
#include <vector>
#include <algorithm> // For std::max

using namespace std;

// Function to display the logical timestamps of events in both processes
void displayTimestamps(int numEventsP1, int numEventsP2, const vector<int>& timestampsP1, const vector<int>& timestampsP2) {
    cout << "\nTimestamps of events in Process P1:\n";
    for (int time : timestampsP1) {
        cout << time << " ";
    }

    cout << "\nTimestamps of events in Process P2:\n";
    for (int time : timestampsP2) {
        cout << time << " ";
    }
    cout << endl;
}

// Function to calculate timestamps using Lamport's Logical Clock algorithm
void calculateLamportLogicalClock(int numEventsP1, int numEventsP2, const vector<vector<int>>& messageDependencies) {
    // Initialize timestamps for both processes
    vector<int> timestampsP1(numEventsP1);
    vector<int> timestampsP2(numEventsP2);

    // Set initial timestamps for events in both processes
    for (int i = 0; i < numEventsP1; ++i) {
        timestampsP1[i] = i + 1;
    }
    for (int i = 0; i < numEventsP2; ++i) {
        timestampsP2[i] = i + 1;
    }

    // Display message dependencies in a tabular format
    cout << "\nMessage Dependency Matrix:\n\t";
    for (int i = 0; i < numEventsP2; ++i) {
        cout << "e2" << i + 1 << "\t";
    }
    for (int i = 0; i < numEventsP1; ++i) {
        cout << "\ne1" << i + 1 << "\t";
        for (int j = 0; j < numEventsP2; ++j) {
            cout << messageDependencies[i][j] << "\t";
        }
    }

    // Update timestamps based on message dependencies
    for (int i = 0; i < numEventsP1; ++i) {
        for (int j = 0; j < numEventsP2; ++j) {
            // Message sent from P1 to P2
            if (messageDependencies[i][j] == 1) {
                timestampsP2[j] = max(timestampsP2[j], timestampsP1[i] + 1);
                for (int k = j + 1; k < numEventsP2; ++k) {
                    timestampsP2[k] = timestampsP2[k - 1] + 1; // Increment timestamps
                }
            }
            // Message received by P1 from P2
            else if (messageDependencies[i][j] == -1) {
                timestampsP1[i] = max(timestampsP1[i], timestampsP2[j] + 1);
                for (int k = i + 1; k < numEventsP1; ++k) {
                    timestampsP1[k] = timestampsP1[k - 1] + 1; // Increment timestamps
                }
            }
        }
    }

    // Display the final timestamps
    displayTimestamps(numEventsP1, numEventsP2, timestampsP1, timestampsP2);
}

// Driver code
int main() {
    int numEventsP1 = 5, numEventsP2 = 3;

    // Message dependency matrix initialization
    vector<vector<int>> messageDependencies = {
        {0, 0, 0},  // e1_1 does not interact with P2 events
        {0, 0, 1},  // e1_2 sends a message to e2_3
        {0, 0, 0},  // e1_3 does not interact with P2 events
        {0, 0, 0},  // e1_4 does not interact with P2 events
        {0, -1, 0}  // e1_5 receives a message from e2_2
    };

    // Calculate and display timestamps
    calculateLamportLogicalClock(numEventsP1, numEventsP2, messageDependencies);

    return 0;
}

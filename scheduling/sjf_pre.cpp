#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int remainingTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;
    bool isCompleted = false; // Track if process has completed
};

bool compareByArrival(const Process &a, const Process &b) {
    return a.arrivalTime < b.arrivalTime;
}

void sjfPreemptive(vector<Process> &processes) {
    int n = processes.size();
    int completed = 0, currentTime = 0;

    while (completed != n) {
        int minIndex = -1;
        int minBurst = INT32_MAX;

        // Find the process with the shortest remaining burst time that has arrived
        for (int i = 0; i < n; ++i) {
            if (processes[i].arrivalTime <= currentTime &&
                !processes[i].isCompleted &&
                processes[i].remainingTime < minBurst) {
                minBurst = processes[i].remainingTime;
                minIndex = i;
            }
        }

        if (minIndex == -1) {
            // No process available, advance time
            ++currentTime;
            continue;
        }

        // Execute the selected process for 1 time unit
        processes[minIndex].remainingTime--;
        currentTime++;

        // If the process is completed, update its metrics
        if (processes[minIndex].remainingTime == 0) {
            processes[minIndex].completionTime = currentTime;
            processes[minIndex].turnAroundTime = processes[minIndex].completionTime - processes[minIndex].arrivalTime;
            processes[minIndex].waitingTime = processes[minIndex].turnAroundTime - processes[minIndex].burstTime;
            processes[minIndex].isCompleted = true;
            completed++;
        }
    }
}

void displayResults(const vector<Process> &processes) {
    float avgTAT = 0, avgWT = 0;
    int n = processes.size();

    cout << "\n*** Shortest Job First Scheduling (Preemptive) ***\n";
    cout << "Process\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n";
    cout << "-------------------------------------------------------------------------\n";

    for (const auto &p : processes) {
        cout << "P" << p.id << "\t\t" << p.arrivalTime << "ms\t\t" << p.burstTime << "ms\t\t"
             << p.completionTime << "ms\t\t" << p.turnAroundTime << "ms\t\t" << p.waitingTime << "ms\n";

        avgTAT += p.turnAroundTime;
        avgWT += p.waitingTime;
    }

    avgTAT /= n;
    avgWT /= n;

    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << avgTAT << "ms\n";
    cout << "Average Waiting Time: " << fixed << setprecision(2) << avgWT << "ms\n";
}

int main() {
    int n;
    cout << "*** Shortest Job First Scheduling (Preemptive) ***\n";
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; ++i) {
        processes[i].id = i + 1;
        cout << "\nEnter Arrival Time for Process " << processes[i].id << ": ";
        cin >> processes[i].arrivalTime;
        cout << "Enter Burst Time for Process " << processes[i].id << ": ";
        cin >> processes[i].burstTime;
        processes[i].remainingTime = processes[i].burstTime;
    }

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), compareByArrival);

    // Execute SJF preemptive scheduling
    sjfPreemptive(processes);

    // Display the results
    displayResults(processes);

    return 0;
}

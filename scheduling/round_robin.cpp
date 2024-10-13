#include <iostream>
#include <iomanip>
#include <queue>
#include <vector>
#include <algorithm>
using namespace std;

struct Process {
    int processID;
    int arrival, burst, waiting, turnAround, remainingTime;
    int completionTime, finish;
};

// Function to input process data
void inputProcesses(vector<Process>& processes) {
    int n = processes.size();
    for (int i = 0; i < n; ++i) {
        processes[i].processID = i + 1;
        cout << "\nEnter the arrival time for P" << processes[i].processID << ": ";
        cin >> processes[i].arrival;
        cout << "Enter the burst time for P" << processes[i].processID << ": ";
        cin >> processes[i].burst;
        processes[i].remainingTime = processes[i].burst;
        processes[i].finish = 0;
    }
}

// Function to sort processes by arrival time
void sortProcessesByArrival(vector<Process>& processes) {
    sort(processes.begin(), processes.end(), [](const Process& a, const Process& b) {
        return a.arrival < b.arrival;
    });
}

// Function to execute Round Robin scheduling
void roundRobinScheduling(vector<Process>& processes, int quantum) {
    int n = processes.size();
    int time = 0, completedProcesses = 0;
    queue<int> readyQueue;            // Queue to manage process execution
    vector<bool> inQueue(n, false);   // Track if a process is in the queue

    // Add the first process to the queue
    readyQueue.push(0);
    inQueue[0] = true;

    while (completedProcesses < n) {
        int i = readyQueue.front();
        readyQueue.pop();
        inQueue[i] = false;

        // Execute process for 'quantum' or remaining time
        if (processes[i].remainingTime <= quantum) {
            time += processes[i].remainingTime;
            processes[i].remainingTime = 0;
            processes[i].completionTime = time;
            processes[i].turnAround = time - processes[i].arrival;
            processes[i].waiting = processes[i].turnAround - processes[i].burst;
            processes[i].finish = 1;
            ++completedProcesses;
        } else {
            time += quantum;
            processes[i].remainingTime -= quantum;
        }

        // Add new processes to the queue that have arrived
        for (int j = 0; j < n; ++j) {
            if (processes[j].arrival <= time && processes[j].finish == 0 && !inQueue[j]) {
                readyQueue.push(j);
                inQueue[j] = true;
            }
        }

        // Re-add the current process to the queue if it hasn't finished
        if (processes[i].remainingTime > 0) {
            readyQueue.push(i);
            inQueue[i] = true;
        }
    }
}

// Function to display the results
void displayResults(const vector<Process>& processes) {
    double totalWaitingTime = 0, totalTurnAroundTime = 0;
    int n = processes.size();

    cout << "\n*** RR Scheduling (Preemptive) ***\n";
    cout << "Process\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n";
    cout << "------------------------------------------------------------------------------------\n";

    for (const auto& p : processes) {
        cout << "P" << p.processID << "\t\t" << p.arrival << "ms\t\t" << p.burst << "ms\t\t"
             << p.completionTime << "ms\t\t" << p.turnAround << "ms\t\t" << p.waiting << "ms\n";
        totalWaitingTime += p.waiting;
        totalTurnAroundTime += p.turnAround;
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage Turnaround Time: " << (totalTurnAroundTime / n) << " ms\n";
    cout << "Average Waiting Time: " << (totalWaitingTime / n) << " ms\n";
}

int main() {
    int n, quantum;

    cout << "*** RR Scheduling (Preemptive) ***\n";
    cout << "Enter Number of Processes: ";
    cin >> n;

    vector<Process> processes(n);

    // Input process details
    inputProcesses(processes);

    cout << "\nEnter Time Quantum: ";
    cin >> quantum;

    // Sort processes by arrival time
    sortProcessesByArrival(processes);

    // Perform Round Robin Scheduling
    roundRobinScheduling(processes, quantum);

    // Display the results
    displayResults(processes);

    return 0;
}

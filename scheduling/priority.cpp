#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm> // For sorting
using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int priority;
    int completionTime;
    int waitingTime;
    int turnAroundTime;
};

// Sort processes by arrival time and priority
bool compareArrivalAndPriority(const Process &a, const Process &b) {
    if (a.arrivalTime == b.arrivalTime)
        return a.priority < b.priority;  // Higher priority = lower number
    return a.arrivalTime < b.arrivalTime;
}

void nonPreemptivePriorityScheduling(vector<Process> &processes) {
    int n = processes.size();
    int currentTime = 0;

    // Keep track of which processes are completed
    vector<bool> completed(n, false);
    int completedCount = 0;

    while (completedCount < n) {
        int minPriorityIndex = -1;

        // Find the highest priority process that has arrived
        for (int i = 0; i < n; ++i) {
            if (!completed[i] && processes[i].arrivalTime <= currentTime) {
                if (minPriorityIndex == -1 || processes[i].priority < processes[minPriorityIndex].priority) {
                    minPriorityIndex = i;
                }
            }
        }

        if (minPriorityIndex == -1) {
            ++currentTime;  // No process available, advance time
        } else {
            // Process the selected process
            currentTime += processes[minPriorityIndex].burstTime;
            processes[minPriorityIndex].completionTime = currentTime;
            processes[minPriorityIndex].turnAroundTime = 
                processes[minPriorityIndex].completionTime - processes[minPriorityIndex].arrivalTime;
            processes[minPriorityIndex].waitingTime = 
                processes[minPriorityIndex].turnAroundTime - processes[minPriorityIndex].burstTime;

            completed[minPriorityIndex] = true;
            completedCount++;
        }
    }
}

void displayResults(const vector<Process> &processes) {
    float avgTAT = 0, avgWT = 0;
    int n = processes.size();

    cout << "\n*** Non-Preemptive Priority Scheduling ***" << endl;
    cout << "Process\tArrival Time\tBurst Time\tPriority\tCompletion Time\tTurnaround Time\tWaiting Time\n";
    cout << "------------------------------------------------------------------------------------------\n";

    for (const auto &p : processes) {
        cout << "P" << p.id << "\t\t" << p.arrivalTime << "\t\t" << p.burstTime << "\t\t"
             << p.priority << "\t\t" << p.completionTime << "\t\t" 
             << p.turnAroundTime << "\t\t" << p.waitingTime << "\n";
        avgTAT += p.turnAroundTime;
        avgWT += p.waitingTime;
    }

    avgTAT /= n;
    avgWT /= n;

    cout << "\nAverage Waiting Time: " << fixed << setprecision(2) << avgWT << " ms\n";
    cout << "Average Turnaround Time: " << fixed << setprecision(2) << avgTAT << " ms\n";
}

int main() {
    int n;
    cout << "*** Non-Preemptive Priority Scheduling ***" << endl;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; ++i) {
        processes[i].id = i + 1;
        cout << "\nEnter Arrival Time for Process " << processes[i].id << ": ";
        cin >> processes[i].arrivalTime;
        cout << "Enter Burst Time for Process " << processes[i].id << ": ";
        cin >> processes[i].burstTime;
        cout << "Enter Priority for Process " << processes[i].id << ": ";
        cin >> processes[i].priority;
    }

    // Sort processes by arrival time and priority
    sort(processes.begin(), processes.end(), compareArrivalAndPriority);

    // Perform non-preemptive priority scheduling
    nonPreemptivePriorityScheduling(processes);

    // Display the results
    displayResults(processes);

    return 0;
}

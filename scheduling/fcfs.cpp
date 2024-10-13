#include <iostream>
#include <iomanip>
#include <vector>
#include <algorithm> // for sorting
using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnAroundTime;
    int waitingTime;
};

// Function to sort processes by arrival time
bool compareArrival(const Process &a, const Process &b) {
    return a.arrivalTime < b.arrivalTime;
}

void calculateCompletionTime(vector<Process> &processes) {
    int n = processes.size();
    processes[0].completionTime = processes[0].arrivalTime + processes[0].burstTime;

    for (int i = 1; i < n; ++i) {
        if (processes[i].arrivalTime > processes[i - 1].completionTime) {
            processes[i].completionTime = processes[i].arrivalTime + processes[i].burstTime;
        } else {
            processes[i].completionTime = processes[i - 1].completionTime + processes[i].burstTime;
        }
    }
}

void calculateTurnAroundAndWaitingTime(vector<Process> &processes) {
    for (auto &p : processes) {
        p.turnAroundTime = p.completionTime - p.arrivalTime;
        p.waitingTime = p.turnAroundTime - p.burstTime;
    }
}

void displayResults(const vector<Process> &processes) {
    float totalTAT = 0, totalWT = 0;
    int n = processes.size();

    cout << "\n*** First Come First Serve Scheduling ***" << endl;
    cout << "Process\tArrival Time\tBurst Time\tCompletion Time\tTurnaround Time\tWaiting Time\n";
    cout << "-------------------------------------------------------------------------------\n";

    for (const auto &p : processes) {
        cout << "P" << p.id << "\t\t" << p.arrivalTime << "ms\t\t" << p.burstTime << "ms\t\t"
             << p.completionTime << "ms\t\t" << p.turnAroundTime << "ms\t\t" << p.waitingTime << "ms\n";

        totalTAT += p.turnAroundTime;
        totalWT += p.waitingTime;
    }

    cout << "\nAverage Turnaround Time: " << fixed << setprecision(2) << (totalTAT / n) << "ms\n";
    cout << "Average Waiting Time: " << fixed << setprecision(2) << (totalWT / n) << "ms\n";
}

int main() {
    int n;
    cout << "*** First Come First Serve Scheduling ***" << endl;
    cout << "Enter the number of processes: ";
    cin >> n;

    vector<Process> processes(n);

    for (int i = 0; i < n; ++i) {
        processes[i].id = i + 1;
        cout << "\nEnter Arrival Time for Process " << processes[i].id << ": ";
        cin >> processes[i].arrivalTime;
        cout << "Enter Burst Time for Process " << processes[i].id << ": ";
        cin >> processes[i].burstTime;
    }

    // Sort processes by arrival time
    sort(processes.begin(), processes.end(), compareArrival);

    // Calculate completion, turnaround, and waiting times
    calculateCompletionTime(processes);
    calculateTurnAroundAndWaitingTime(processes);

    // Display the results
    displayResults(processes);

    return 0;
}

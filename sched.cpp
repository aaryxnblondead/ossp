#include <iostream>
#include <queue>
#include <vector>
#include <string>

enum Priority { EMERGENCY = 1, URGENT, ROUTINE };

struct Patient {
    std::string name;
    Priority prio;
    int time_needed, arrival, remaining;
    Patient(std::string n, Priority p, int t, int a)
        : name(n), prio(p), time_needed(t), arrival(a), remaining(t) {}
    bool operator<(const Patient& o) const {
        return prio != o.prio ? prio > o.prio : arrival > o.arrival;
    }
};

int main() {
    std::priority_queue<Patient> pq;
    int now = 0, served = 0;
    Patient* current = nullptr;
    std::vector<std::string> logs;
    std::string cmd;

    auto add_patient = [&](int custom_now = -1) {
        std::string name; int pr, need;
        std::cout << "Name: "; std::getline(std::cin, name);
        std::cout << "Priority (1=EME,2=URG,3=ROU): "; std::cin >> pr;
        std::cout << "Consult Time: "; std::cin >> need; std::cin.ignore();
        pq.emplace(name, Priority(pr), need, custom_now == -1 ? now : custom_now);
    };

    std::cout << "Clinic Preemptive Scheduler | ENTER=next min | add | report\n";
    while (true) {
        std::cout << "\nTime " << now << " | ";
        std::cout << (current ? "With: " + current->name + ", " + std::to_string(current->remaining) + "min left\n" : "Available\n");
        std::cout << "Action: ";
        std::getline(std::cin, cmd);

        if (cmd.empty()) {
            ++now;
            if (current && --current->remaining == 0) {
                logs.push_back("Done: " + current->name + " at t=" + std::to_string(now));
                delete current; current = nullptr; ++served;
            }
            if (!current && !pq.empty()) {
                current = new Patient(pq.top()); pq.pop();
                logs.push_back("Start: " + current->name + " at t=" + std::to_string(now));
            }
            if (current && !pq.empty() && pq.top().prio < current->prio) {
                logs.push_back("Preempt: " + current->name + " at t=" + std::to_string(now));
                pq.push(*current); delete current; current = nullptr;
            }
            if (!current && !pq.empty()) {
                current = new Patient(pq.top()); pq.pop();
                logs.push_back("Start: " + current->name + " at t=" + std::to_string(now));
            }
        } else if (cmd == "add") add_patient();
        else if (cmd == "report") {
            std::cout << "\n--- REPORT ---\n";
            for (auto& s : logs) std::cout << s << "\n";
            std::cout << "Patients served: " << served << "\n";
            break;
        }
    }
}

#include <chrono>
#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;
using namespace std::chrono;

struct ClassInfo {
    string courseCode;
    string courseName;
    string lecturerName;
    string venue;
    string day;
    string startTime;
    string endTime;
    string programme;
};

// Benchmark function
void Benchmark(int n) {
    unordered_map<string, ClassInfo> schedules;
    vector<ClassInfo> records;
    records.reserve(n);

    // Generate synthetic data
    for (int i = 0; i < n; i++) {
        records.push_back({"CS" + to_string(i), "Course" + to_string(i),
                           "Lecturer" + to_string(i % 200), "Room" + to_string(i % 50),
                           "Monday", to_string(i % 12 + 8) + "AM", to_string(i % 12 + 9) + "AM", "CS"});
    }

    // 1) Insertion
    auto start = chrono::high_resolution_clock::now();
    for (auto &record : records) {
        string key = record.day + "_" + record.startTime + "_" + record.courseCode;
        schedules[key] = record;
    }
    auto end = chrono::high_resolution_clock::now();
    cout << "[n=" << n << "] Insert: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    // 2) Lookup
    start = chrono::high_resolution_clock::now();
    for (int i = 0; i < n; i++) {
        string key = "Monday_" + to_string(i % 12 + 8) + "AM_CS" + to_string(i);
        auto it = schedules.find(key);
        if (it == schedules.end()) cout << ""; // prevent optimization
    }
    end = chrono::high_resolution_clock::now();
    cout << "[n=" << n << "] Lookup: "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    // 3) Filter scan
    start = chrono::high_resolution_clock::now();
    string targetLecturer = "Lecturer" + to_string((n / 2) % 200);
    int matches = 0;
    for (auto &entry : schedules) {
        if (entry.second.lecturerName == targetLecturer) matches++;
    }
    end = chrono::high_resolution_clock::now();
    cout << "[n=" << n << "] Filter scan (matches=" << matches << "): "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";

    // 4) Sort day schedule
    vector<ClassInfo> tempList;
    for (auto &entry : schedules) {
        if (entry.second.day == "Monday") tempList.push_back(entry.second);
    }
    start = chrono::high_resolution_clock::now();
    sort(tempList.begin(), tempList.end(),
         [](const ClassInfo &a, const ClassInfo &b) { return a.startTime < b.startTime; });
    end = chrono::high_resolution_clock::now();
    cout << "[n=" << n << "] Sort day schedule (size=" << tempList.size() << "): "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count() << " ms\n";
}

unordered_map<string, ClassInfo> schedules;

// Student info mapping
struct Student {
    string id;
    string name;
    string programme;
};

Student currentStudent;

// ---------------------------------------------------
// Fetch Schedule into Hash Map for a given student
// ---------------------------------------------------
void FetchSchedule(string studentID) {
    vector<ClassInfo> records;

    if (studentID == "24000085") { // CS student
        currentStudent = {"24000085", "Muhammad Faizzuddin", "CS"};
        records = {
            {"CS101", "Intro to Programming", "Dr. Lee", "Lab 1", "Monday", "08AM", "10AM", "CS"},
            {"CS201", "Data Structures", "Prof. Tan", "Room 202", "Monday", "11AM", "12PM", "CS"},
            {"CS301", "Algorithms", "Dr. Smith", "Room 303", "Wednesday", "14PM", "16PM", "CS"},
            {"CS401", "Operating Systems", "Dr. Lee", "Lab 4", "Friday", "14PM", "16PM", "CS"}
        };
    } else if (studentID == "24000087") { // IT student
        currentStudent = {"24000087", "Luqman Hakim", "IT"};
        records = {
            {"IT101", "Networking Basics", "Dr. Wong", "Lab 3", "Tuesday", "10AM", "12PM", "IT"},
            {"IT102", "Computer Hardware", "Dr. Tan", "Room 201", "Tuesday", "14PM", "16PM", "IT"},
            {"IT201", "Database Systems", "Dr. Lim", "Lab 2", "Thursday", "13PM", "15PM", "IT"},
            {"IT202", "Web Development", "Dr. Kumar", "Lab 6", "Thursday", "16PM", "18PM", "IT"},
            {"IT301", "Cybersecurity", "Dr. Wong", "Room 210", "Friday", "16PM", "18PM", "IT"}
        };
    } else if (studentID == "24000270") { // IS student
        currentStudent = {"24000270", "Lew Wei Cheng", "IS"};
        records = {
            {"IS101", "Information Systems", "Dr. Kumar", "Room 105", "Wednesday", "09AM", "11AM", "IS"},
            {"IS102", "Business Process Management", "Dr. Tan", "Room 106", "Wednesday", "13PM", "15PM", "IS"},
            {"IS201", "System Analysis", "Dr. Tan", "Room 106", "Friday", "09AM", "11AM", "IS"},
            {"IS202", "Enterprise Architecture", "Dr. Wong", "Room 108", "Friday", "13PM", "15PM", "IS"}
        };
    } else {
        cout << "Invalid student ID.\n";
        return;
    }

    schedules.clear();
    for (auto &record : records) {
        string key = record.day + "_" + record.startTime;
        schedules[key] = record;
    }
}

// ---------------------------------------------------
// Retrieve Classes by Day
// ---------------------------------------------------
void GetClassesByDay(string day) {
    vector<ClassInfo> tempList;
    for (auto &entry : schedules) {
        if (entry.second.day == day) {
            tempList.push_back(entry.second);
        }
    }

    if (tempList.empty()) {
        cout << "No classes scheduled for " << day << ".\n";
        return;
    }

    sort(tempList.begin(), tempList.end(),
         [](ClassInfo a, ClassInfo b) { return a.startTime < b.startTime; });

    cout << "\n--- " << day << " Schedule ---\n";
    for (auto &info : tempList) {
        cout << info.startTime << "-" << info.endTime << ": "
             << info.courseCode << " - " << info.courseName
             << " | Lecturer: " << info.lecturerName
             << " | Venue: " << info.venue << "\n";
    }
}

// ---------------------------------------------------
// Display Weekly Timetable in Grid Style (08AM–20PM)
// ---------------------------------------------------
void DisplayWeeklyGrid() {
    vector<string> days = {"Monday", "Tuesday", "Wednesday", "Thursday", "Friday"};
    vector<string> times = {"08AM","09AM","10AM","11AM","12PM","13PM","14PM","15PM","16PM","17PM","18PM","19PM","20PM"};

    cout << "\n===== Weekly Timetable (Grid View) =====\n\n";
    cout << "Student: " << currentStudent.name << " | Programme: " << currentStudent.programme << "\n\n";

    cout << setw(8) << "Time";
    for (string day : days) {
        cout << setw(25) << day;
    }
    cout << "\n";

    for (string time : times) {
        cout << setw(8) << time;
        for (string day : days) {
            string key = day + "_" + time;
            if (schedules.find(key) != schedules.end()) {
                ClassInfo info = schedules[key];
                cout << setw(25) << info.courseCode + "@" + info.venue;
            } else {
                cout << setw(25) << "-";
            }
        }
        cout << "\n";
    }
}

// ---------------------------------------------------
// Filter Schedule by Attribute (key press for attribute & value)
// ---------------------------------------------------
void FilterScheduleMenu() {
    int attrChoice;

    cout << "\n===== Filter by Attribute =====\n";
    cout << "1. Course Code\n";
    cout << "2. Course Name\n";
    cout << "3. Lecturer Name\n";
    cout << "4. Venue\n";
    cout << "Choose an attribute (1-4): ";
    cin >> attrChoice;

    string attribute;
    switch (attrChoice) {
        case 1: attribute = "courseCode"; break;
        case 2: attribute = "courseName"; break;
        case 3: attribute = "lecturerName"; break;
        case 4: attribute = "venue"; break;
        default:
            cout << "Invalid choice.\n";
            return;
    }

    // Collect unique values for the chosen attribute
    vector<string> values;
    for (auto &entry : schedules) {
        string val;
        if (attribute == "courseCode") val = entry.second.courseCode;
        else if (attribute == "courseName") val = entry.second.courseName;
        else if (attribute == "lecturerName") val = entry.second.lecturerName;
        else if (attribute == "venue") val = entry.second.venue;

        if (find(values.begin(), values.end(), val) == values.end()) {
            values.push_back(val);
        }
    }

    // Show values as numbered options
    cout << "\nAvailable " << attribute << "s:\n";
    for (size_t i = 0; i < values.size(); i++) {
        cout << i+1 << ". " << values[i] << "\n";
    }
    cout << "Choose a value number: ";
    int valChoice;
    cin >> valChoice;

    if (valChoice < 1 || valChoice > (int)values.size()) {
        cout << "Invalid choice.\n";
        return;
    }

    string selectedValue = values[valChoice-1];

    // Filter results
    cout << "\nFiltered results for " << attribute << " = " << selectedValue << ":\n";
    bool found = false;
    for (auto &entry : schedules) {
        ClassInfo info = entry.second;
        if ((attribute == "courseCode" && info.courseCode == selectedValue) ||
            (attribute == "courseName" && info.courseName == selectedValue) ||
            (attribute == "lecturerName" && info.lecturerName == selectedValue) ||
            (attribute == "venue" && info.venue == selectedValue)) {
            cout << info.day << " " << info.startTime << "-" << info.endTime << ": "
                 << info.courseCode << " - " << info.courseName
                 << " | Lecturer: " << info.lecturerName
                 << " | Venue: " << info.venue << "\n";
            found = true;
        }
    }
    if (!found) cout << "No matching classes found.\n";
}

// ---------------------------------------------------
// Menu System (returns to Student ID menu on Exit)
// ---------------------------------------------------
void Menu() {
    int choice;
    string day;

    do {
        cout << "\n===== Uschedule Menu =====\n";
        cout << "Student: " << currentStudent.name << " | Programme: " << currentStudent.programme << "\n";
        cout << "1. Display Weekly Timetable (Grid View)\n";
        cout << "2. Retrieve Classes by Day\n";
        cout << "3. Filter Schedule by Attribute\n";
        cout << "4. Exit to Student ID Menu\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice) {
            case 1:
                DisplayWeeklyGrid();
                break;
            case 2:
                cout << "Enter Day: "; cin >> day;
                GetClassesByDay(day);
                break;
            case 3:
                FilterScheduleMenu();
                break;
            case 4:
                cout << "Returning to Student ID menu...\n";
                return; // go back to student ID menu
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    } while (true);
}

// ---------------------------------------------------
// Main Function (loops back to Student ID menu)
// ---------------------------------------------------
int main() {
    while (true) {
        string studentID;
        cout << "\n===== Student Login =====\n";
        cout << "Enter Student ID (or type 'quit' to exit program, or 'bench' to run benchmark): ";
        cin >> studentID;

        if (studentID == "quit") {
            cout << "Program terminated.\n";
            break;
        }
        if (studentID == "bench") {
            // Run benchmarks for multiple sizes
            vector<int> sizes = {100, 1000, 10000, 50000};
            for (int n : sizes) {
                // Repeat each test 3 times for accuracy
                for (int r = 0; r < 3; r++) {
                    Benchmark(n);
                }
                cout << "-------------------------\n";
            }
            continue;
        }

        FetchSchedule(studentID);
        if (!schedules.empty()) {
            Menu();
        }
    }
    return 0;
}

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <algorithm>
#include <iomanip>
using namespace std;

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
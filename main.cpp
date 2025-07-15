#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>
#include <algorithm>

using namespace std;

// ======= Class Definitions =======
class Course {
public:
    string id, name, instructor;
    Course() {}
    Course(string cid, string cname, string inst)
        : id(cid), name(cname), instructor(inst) {}
    string toString() const {
        return id + "," + name + "," + instructor;
    }
    static Course fromString(const string& line) {
        stringstream ss(line);
        string id, name, instructor;
        getline(ss, id, ',');
        getline(ss, name, ',');
        getline(ss, instructor);
        return Course(id, name, instructor);
    }
};

class Student {
public:
    string id, name;
    Student() {}
    Student(string sid, string sname) : id(sid), name(sname) {}
    string toString() const {
        return id + "," + name;
    }
};

// ======= File Constants =======
const string COURSE_FILE = "courses.txt";
const string ENROLL_FILE = "enrollments.txt";
const string STUDENT_FILE = "students.txt";

// ======= Utility Functions =======
bool courseExists(const string& cid) {
    ifstream in(COURSE_FILE);
    string line;
    while (getline(in, line)) {
        Course c = Course::fromString(line);
        if (c.id == cid) return true;
    }
    return false;
}

bool isAlreadyEnrolled(const string& sid, const string& cid) {
    ifstream in(ENROLL_FILE);
    string line;
    while (getline(in, line)) {
        if (line.find(sid) != string::npos && line.find(cid) != string::npos)
            return true;
    }
    return false;
}

string getCurrentTime() {
    time_t now = time(0);
    string dt = ctime(&now);
    dt.erase(remove(dt.begin(), dt.end(), '\n'), dt.end());
    return dt;
}

vector<Course> loadCourses() {
    vector<Course> courses;
    ifstream in(COURSE_FILE);
    string line;
    while (getline(in, line)) {
        courses.push_back(Course::fromString(line));
    }
    return courses;
}

// ======= Admin Functions =======
void addCourse() {
    Course course;
    cout << "Enter Course ID: "; cin >> course.id;
    if (courseExists(course.id)) {
        cout << "❌ Course ID already exists!\n"; return;
    }
    cin.ignore();
    cout << "Enter Course Name: "; getline(cin, course.name);
    cout << "Enter Instructor Name: "; getline(cin, course.instructor);
    ofstream out(COURSE_FILE, ios::app);
    out << course.toString() << endl;
    cout << "✅ Course added successfully.\n";
}

void viewCourses() {
    auto courses = loadCourses();
    if (courses.empty()) {
        cout << "⚠️ No courses found.\n"; return;
    }
    cout << "\n📚 Available Courses:\n";
    for (auto& c : courses) {
        cout << "ID: " << c.id << ", Name: " << c.name << ", Instructor: " << c.instructor << endl;
    }
}

void viewEnrollments() {
    ifstream in(ENROLL_FILE);
    string line;
    cout << "\n📒 All Enrollments:\n";
    while (getline(in, line)) cout << line << endl;
}

// ======= Student Functions =======
void registerStudent(Student& s) {
    cout << "Enter Student ID: "; cin >> s.id;
    cin.ignore();
    cout << "Enter Student Name: "; getline(cin, s.name);
    ofstream out(STUDENT_FILE, ios::app);
    out << s.toString() << endl;
    cout << "✅ Student registered successfully.\n";
}

void enrollCourses(const Student& s) {
    auto courses = loadCourses();
    if (courses.empty()) {
        cout << "⚠️ No courses available.\n"; return;
    }

    cout << "\n📚 Course List:\n";
    for (size_t i = 0; i < courses.size(); ++i) {
        cout << i + 1 << ". " << courses[i].name << " (" << courses[i].id << ")\n";
    }

    vector<string> enrolled;
    cout << "\nEnter course numbers to enroll (0 to finish):\n";
    while (true) {
        int choice; cin >> choice;
        if (choice == 0) break;
        if (choice >= 1 && choice <= (int)courses.size()) {
            string cid = courses[choice - 1].id;
            if (!isAlreadyEnrolled(s.id, cid)) {
                enrolled.push_back(cid);
            } else {
                cout << "⚠️ Already enrolled in " << cid << "\n";
            }
        } else cout << "❌ Invalid choice.\n";
    }

    ofstream out(ENROLL_FILE, ios::app);
    for (auto& cid : enrolled) {
        out << s.id << " (" << s.name << ") → " << cid << " @ " << getCurrentTime() << endl;
    }
    cout << "✅ Enrollments saved.\n";
}

void viewStudentEnrollments(const string& sid) {
    ifstream in(ENROLL_FILE);
    string line;
    bool found = false;
    cout << "\n📖 Your Enrollments:\n";
    while (getline(in, line)) {
        if (line.find(sid) != string::npos) {
            cout << line << endl;
            found = true;
        }
    }
    if (!found) cout << "⚠️ No enrollments found.\n";
}

// ======= Menus =======
void adminMenu() {
    int ch;
    do {
        cout << "\n=== 👩‍💼 Admin Menu ===\n"
             << "1. Add Course\n"
             << "2. View All Courses\n"
             << "3. View All Enrollments\n"
             << "0. Back to Main Menu\n"
             << "Choice: ";
        cin >> ch;
        switch (ch) {
            case 1: addCourse(); break;
            case 2: viewCourses(); break;
            case 3: viewEnrollments(); break;
        }
    } while (ch != 0);
}

void studentMenu() {
    Student s;
    registerStudent(s);
    int ch;
    do {
        cout << "\n=== 👨‍🎓 Student Menu ===\n"
             << "1. View Courses\n"
             << "2. Enroll in Courses\n"
             << "3. View My Enrollments\n"
             << "0. Back to Main Menu\n"
             << "Choice: ";
        cin >> ch;
        switch (ch) {
            case 1: viewCourses(); break;
            case 2: enrollCourses(s); break;
            case 3: viewStudentEnrollments(s.id); break;
        }
    } while (ch != 0);
}

int main() {
    int choice;
    do {
        cout << "\n=== 🎓 Course Enrollment System ===\n"
             << "1. Admin Panel\n"
             << "2. Student Panel\n"
             << "0. Exit\n"
             << "Choice: ";
        cin >> choice;
        switch (choice) {
            case 1: adminMenu(); break;
            case 2: studentMenu(); break;
            case 0: cout << "👋 Exiting. Goodbye!\n"; break;
            default: cout << "❌ Invalid option. Try again.\n";
        }
    } while (choice != 0);
    return 0;
}

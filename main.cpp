#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// ======== Class Definitions ========
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

// ======== File Constants ========
const string COURSE_FILE = "courses.txt";

// ======== Utility Functions ========
bool courseExists(const string& cid) {
    ifstream in(COURSE_FILE);
    string line;
    while (getline(in, line)) {
        Course c = Course::fromString(line);
        if (c.id == cid) return true;
    }
    return false;
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

// ======== Admin Functions ========
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

// ======== Admin Menu ========
void adminMenu() {
    int ch;
    do {
        cout << "\n=== 👩‍💼 Admin Menu ===\n"
             << "1. Add Course\n"
             << "2. View All Courses\n"
             << "0. Exit\n"
             << "Choice: ";
        cin >> ch;
        switch (ch) {
            case 1: addCourse(); break;
            case 2: viewCourses(); break;
        }
    } while (ch != 0);
}

// ======== Main Function ========
int main() {
    cout << "\n=== 🎓 Course Enrollment System ===\n";
    adminMenu();
    cout << "👋 Exiting. Goodbye!\n";
    return 0;
}

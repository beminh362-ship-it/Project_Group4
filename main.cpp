#include <iostream>
#include <vector>
#include <string>
#include <limits>
#include <sstream>

#include "Utils.h"
#include "CSVHelper.h"

#include "Member.h"
#include "Student.h"
#include "Lecturer.h"

#include "Class.h"
#include "AttendanceSession.h"

using namespace std;

//  HAM PHU TRO
// Ham tach chuoi dua tren dau phay (dung cho danh sach SV)
vector<string> splitByComma(const string& s) {
    vector<string> result;
    stringstream ss(s);
    string item;
    while (getline(ss, item, ',')) {
        if (!item.empty()) result.push_back(item);
    }
    return result;
}

// LOGIN MENU 
void showLoginMenu() {
    cout << "\n===== STUDENT ATTENDANCE SYSTEM =====\n";
    cout << "1. Login\n";
    cout << "0. Exit\n";
    cout << "Selection: ";
}

// LECTURER DASHBOARD 
void showLecturerMenu() {
    cout << "\n===== LECTURER DASHBOARD =====\n";
    cout << "1. View list of assigned classes\n";
    cout << "2. Search / Filter classes\n";
    cout << "3. View student list in a class\n";
    cout << "4. Create attendance session\n";
    cout << "5. Take manual attendance (View Missing Students)\n";
    cout << "6. Review and edit attendance\n";
    cout << "7. View attendance history\n";
    cout << "8. Delete attendance session\n";
    cout << "0. Logout\n";
    cout << "Selection: ";
}

//  STUDENT DASHBOARD 
void showStudentMenu() {
    cout << "\n===== STUDENT DASHBOARD =====\n";
    cout << "1. View registered classes\n";
    cout << "2. Search classes\n";
    cout << "3. View notifications\n";
    cout << "4. Mark attendance\n";
    cout << "5. View attendance history\n";
    cout << "0. Logout\n";
    cout << "Selection: ";
}

// XỬ LÝ LOGIN 
Member* handleLogin() {
    string email, password;

    cout << "Email: ";
    cin >> email;

    cout << "Password: ";
    cin >> password;

    if (!Utils::isValidUniversityEmail(email)) {
        cout << "Email or password is incorrect. Please try again.\n";
        return nullptr;
    }

    // Kiểm tra sinh viên trước
    Student* student = CSVHelper::findStudentByEmailAndPassword(email, password);
    if (student != nullptr) {
        cout << "System access successful.\n";
        return student;
    }

    // Nếu không phải sinh viên, kiểm tra giảng viên
    Lecturer* lecturer = CSVHelper::findLecturerByEmailAndPassword(email, password);
    if (lecturer != nullptr) {
        cout << "System access successful.\n";
        return lecturer;
    }

    cout << "Email or password is incorrect. Please try again.\n";
    return nullptr;
}

// LUỒNG GIẢNG VIÊN 

void lecturerDashboard(Lecturer* lecturer) {
    int choice;
    string classId, keyword;

    do {
        showLecturerMenu();
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            lecturer->viewAssignedClasses();
            break;

        case 2:
            cout << "Enter course name / class ID: ";
            cin.ignore();
            getline(cin, keyword);
            lecturer->searchOrFilterClasses(keyword);
            break;

        case 3:
            cout << "Enter class ID: ";
            cin >> classId;
            lecturer->viewStudentList(classId);
            break;

        case 4:
            cout << "Enter class ID: ";
            cin >> classId;
            lecturer->createAttendanceSession(classId);
            break;

        case 5:
            //LOGIC DA DUOC CHUYEN SANG LECTURER.CPP
            cout << "Enter class ID: ";
            cin >> classId;
            lecturer->takeManualAttendance(classId);
            break;

        case 6:
            cout << "Enter class ID: ";
            cin >> classId;
            lecturer->reviewAndEditAttendance(classId);
            break;

        case 7:
            cout << "Enter class ID: ";
            cin >> classId;
            lecturer->viewAttendanceHistory(classId);
            break;

        case 8:
            cout << "Enter class ID: ";
            cin >> classId;
            lecturer->deleteAttendanceSession(classId);
            break;

        case 0:
            cout << "Logout successful.\n";
            break;

        default:
            cout << "Invalid selection.\n";
        }

    } while (choice != 0);
}

//  LUỒNG SINH VIÊN

void studentDashboard(Student* student) {
    int choice;
    string classId, keyword;

    do {
        showStudentMenu();
        if (!(cin >> choice)) {
            cout << "Invalid input.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
        case 1:
            student->viewRegisteredClasses();
            break;

        case 2:
            cout << "Enter course name or class ID: ";
            cin.ignore();
            getline(cin, keyword);
            student->searchClasses(keyword);
            break;

        case 3:
            cout << "Enter class ID: ";
            cin >> classId;
            student->viewNotifications(classId);
            break;

        case 4:
            cout << "Enter class ID: ";
            cin >> classId;
            student->takeAttendance(classId);
            break;

        case 5:
            cout << "Enter class ID: ";
            cin >> classId;
            student->viewAttendanceHistory(classId);
            break;

        case 0:
            cout << "Logout successful.\n";
            break;

        default:
            cout << "Invalid selection.\n";
        }

    } while (choice != 0);
}

// MAIN

int main() {
    int choice;

    do {
        showLoginMenu();
        if (!(cin >> choice)) {
            cout << "Invalid input. Please enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        if (choice == 1) {
            Member* user = handleLogin();

            if (user != nullptr) {
                if (user->getRole() == "Student") {
                    studentDashboard(dynamic_cast<Student*>(user));
                }
                else if (user->getRole() == "Lecturer") {
                    lecturerDashboard(dynamic_cast<Lecturer*>(user));
                }

                delete user;
            }
        }
        else if (choice == 0) {
            cout << "System terminated.\n";
        }
        else {
            cout << "Invalid selection.\n";
        }

    } while (choice != 0);

    return 0;
}
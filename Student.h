
#ifndef STUDENT_H
#define STUDENT_H

#include <string>
#include <vector>
#include "Member.h"

using namespace std;

class Student : public Member {
private:
    string studentId;
    vector<string> registeredClasses;

public:
    Student(string email, string studentId, vector<string> classes);

    // Override từ Member
    string getRole() const override { return "Student"; }

    // Getters
    string getStudentId() const;
    vector<string> getRegisteredClasses() const;

    // Student functions
    void viewRegisteredClasses();
    void searchClasses(string keyword);
    void viewNotifications(string classId);
    void takeAttendance(string classId);
    void viewAttendanceHistory(string classId);
};

#endif
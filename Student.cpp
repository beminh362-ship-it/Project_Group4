#include "Student.h"
#include "CSVHelper.h"
#include "Utils.h"
#include <iostream>
#include <algorithm>
#include <sstream> 

using namespace std;

//CONSTRUCTOR
Student::Student(string email, string studentId, vector<string> classes)
    : Member(email, studentId) {
    this->studentId = studentId;
    this->registeredClasses = classes;
}
//GETTERS
string Student::getStudentId() const {
    return studentId;
}
vector<string> Student::getRegisteredClasses() const {
    return registeredClasses;
}
//1.VIEW REGISTERED CLASSES
void Student::viewRegisteredClasses() {
    cout<<"\n=== REGISTERED CLASSES ===\n";
    if (registeredClasses.empty()) {
        cout<<"You have not registered for any classes.\n";
        return;
    }
    for (size_t i = 0; i < registeredClasses.size(); i++) {
        string classId = registeredClasses[i];
        vector<string> classInfo = CSVHelper::getClassInfo(classId);

        if (!classInfo.empty()) {
            cout<<"Class "<<(i + 1)<<endl;
            cout<<"Class ID: "<<classId<<endl;
            cout<<"Course name: "<<classInfo[0]<<endl;
            if (classInfo.size() > 1) {
                cout<<"Lecturer: "<<classInfo[1]<<endl;
            }
            cout<<"------------------------\n";
        }
    }
}
//2.SEARCH CLASSES
void Student::searchClasses(string keyword) {
    cout<<"\n=== SEARCH RESULTS ===\n";
    bool found = false;
    for (const string& classId : registeredClasses) {
        vector<string> classInfo = CSVHelper::getClassInfo(classId);
        if (classInfo.empty()) continue;
        string courseName = classInfo[0];
        // Tìm kiếm chính xác hoặc tìm gần đúng
        if (classId == keyword || Utils::caseInsensitiveContains(courseName, keyword)) {
            cout<<"Class ID: "<<classId<<endl;
            cout<<"Course name: "<<courseName<<endl;
            if (classInfo.size() > 1) {
                cout<<"Lecturer: "<<classInfo[1]<<endl;
            }
            cout<<"------------------------\n";
            found = true;
        }
    }
    if (!found) {
        cout<<"No matching classes found.\n";
    }
}
//3.VIEW NOTIFICATIONS
void Student::viewNotifications(string classId) {
    cout<<"\n=== ATTENDANCE NOTIFICATION ===\n";
    //Kiểm tra đăng ký
    bool isRegistered = false;
    for (const string& id : registeredClasses) {
        if (id == classId) {
            isRegistered = true;
            break;
        }
    }
    if (!isRegistered) {
        cout<<"You are not registered for this class.\n";
        return;
    }
    //Thông tin phiên hoạt động
    vector<string> activeSession = CSVHelper::getActiveSession(classId);
    // CSV Structure: [0]ClassID, [1]Start, [2]End, [3]Pass, [4]List
    if (activeSession.empty() || activeSession.size() < 3) {
        cout << "No active attendance session.\n";
        return;
    }
    string startTime = activeSession[1];
    string endTime = activeSession[2];
    time_t endTimeT = Utils::parseDateTime(endTime);

    if (!Utils::isExpired(endTimeT)) {
        cout<<"Attendance session is OPEN!\n";
        cout<<"Start time: "<<startTime<<"\n";
        cout<<"End time: "<<endTime<<"\n";
    }
    else {
        cout<<"Attendance has ended.\n";
    }
}
//4.TAKE ATTENDANCE
void Student::takeAttendance(string classId) {
    cout<<"\n=== MARK ATTENDANCE ===\n";
    //Kiểm tra đăng ký
    bool isRegistered = false;
    for (const string& id : registeredClasses) {
        if (id == classId) {
            isRegistered = true;
            break;
        }
    }
    if (!isRegistered) {
        cout<<"You are not registered for this class.\n";
        return;
    }
    //Thông tin phiên hoạt động 
    vector<string> activeSession = CSVHelper::getActiveSession(classId);
    // CSV Structure: [0]ClassID, [1]Start, [2]End, [3]Pass, [4]List
    if (activeSession.empty() || activeSession.size() < 3) {
        cout<<"No active attendance session.\n";
        return;
    }
    string endTime = activeSession[2];
    string password = (activeSession.size() > 3) ? activeSession[3] : "";
    //Kiểm tra hạn
    time_t endTimeT = Utils::parseDateTime(endTime);
    if (Utils::isExpired(endTimeT)) {
        cout<<"Attendance has ended."<endl;
        return;
    }
    //Nếu sinh viên có mặt
    if (CSVHelper::hasStudentAttended(classId, studentId)) {
        cout<<"You have already taken attendance for this session.\n";
        return;
    }
    //Kiểm tra trực tiếp dữ liệu phiên
    if (activeSession.size() >= 5) {
        stringstream ss(activeSession[4]);
        string id;
        while (getline(ss, id, ',')) {
            if (id == studentId) {
                cout<<"You have already taken attendance for this session.\n";
                return;
            }
        }
    }
    //Xác nhận mật khẩu 
    if (!password.empty()) {
        cout<<"Enter attendance password: ";
        string inputPassword;
        cin>>inputPassword;

        if (inputPassword != password) {
            cout<<"Incorrect password. Please try again."<<endl;
            return;
        }
    }
    //Xác nhận điểm danh
    cout<<"Enter 'present' to confirm attendance: ";
    string confirmation;
    cin>>confirmation;
    //Chuyển về chữ thường
    transform(confirmation.begin(), confirmation.end(),
        confirmation.begin(), ::tolower);
    if (confirmation != "present") {
        cout<<"Attendance cancelled.\n";
        return;
    }
    //Lưu điểm danh
    bool success = CSVHelper::markAttendance(classId, studentId);

    if (success) {
        cout<<"Attendance marked successfully!"<<endl;
    }
    else {
        cout<<"Failed to mark attendance (System Error)."<<endl;
    }
}
//5.VIEW HISTORY
void Student::viewAttendanceHistory(string classId) {
    cout<<"\n=== ATTENDANCE HISTORY ===\n";
    //Kiểm tra đăng ký
    bool isRegistered = false;
    for (const string& id : registeredClasses) {
        if (id == classId) {
            isRegistered = true;
            break;
        }
    }
    if (!isRegistered) {
        cout<<"You are not registered for this class.\n";
        return;
    }
    //Thông tin lớp
    vector<string> classInfo = CSVHelper::getClassInfo(classId);
    if (!classInfo.empty()) {
        cout<<"Course: "<<classInfo[0]<<"\n";
    }
    cout<<"Class ID: "<<classId<<"\n\n";
    //Lấy tất cả phiên điểm danh
    vector<vector<string>> allSessions = CSVHelper::getAllSessions(classId);

    if (allSessions.empty()) {
        cout<<"No attendance sessions found.\n";
        return;
    }
    int attendedCount = 0;
    int totalSessions = 0;
    //Duyệt các buổi điểm danh và kiểm tra trạng thái
    for (size_t i = 0; i < allSessions.size(); i++) {
        vector<string> session = allSessions[i];
        // CSV Structure: [0]ClassID, [1]Start, [2]End, [3]Pass, [4]List
        if (session.size() < 3) continue;
        string startTime = session[1]; 
        string endTime = session[2];   
        string status = "Absent";
        //Kiểm tra danh sách sinh viên ở Index 4
        if (session.size() >= 5) {
            string studentList = session[4];

            //Dùng stringstream để tách dấu phẩy và so sánh chính xác
            stringstream ss(studentList);
            string id;
            while (getline(ss, id, ',')) {
                if (id == this->studentId) {
                    status = "Present";
                    attendedCount++;
                    break;
                }
            }
        }
        cout<<"Session "<<(totalSessions + 1)<<": ";
        cout<<startTime<<" - "<<status<<endl;
        totalSessions++;
    }
    cout<<"\nSUMMARY\n";
    cout<<"Total: "<<attendedCount<<"/"<<totalSessions<<" sessions attended.\n";
}
#include "Lecturer.h"
#include "CSVHelper.h"
#include "Utils.h"
#include <iostream>
#include <iomanip> 
#include <sstream>
#include <limits>  
#include <vector>

using namespace std;

// HAM HO TRO CHUYEN DOI NGAY GIO
string formatDateTimeISO(string dateDMY, string timeHM) {
    stringstream ss(dateDMY);
    string d, m, y;
    char sep;
    if (dateDMY.find('/') != string::npos) sep = '/';
    else sep = '-';
    getline(ss, d, sep);
    getline(ss, m, sep);
    getline(ss, y, sep);
    if (d.length() == 1) d = "0" + d;
    if (m.length() == 1) m = "0" + m;
    return y + "-" + m + "-" + d + " " + timeHM + ":00";
}

// HAM HO TRO LAY LICH HOC
string getClassScheduleDetails(string classId) {
    vector<vector<string>> schedules = CSVHelper::docFile("class_schedule.csv");
    string dates = "";
    string timeRange = "";
    bool found = false;

    for (const auto& row : schedules) {
        if (row.size() >= 4 && row[0] == classId) {
            string d = row[1];
            string t = row[2] + " - " + row[3];
            if (!found) {
                timeRange = t;
                dates = d;
                found = true;
            }
            else {
                dates += ", " + d;
            }
        }
    }
    if (!found) return "No schedule";
    return dates + " (" + timeRange + ")";
}

// Constructor
Lecturer::Lecturer(string email, string cccd) : Member(email, cccd) {
    this->email = email;
    this->cccd = cccd;
}

bool Lecturer::dangNhap(string emailNhap, string cccdNhap, vector<pair<string, string>> dsGV) {
    for (auto gv : dsGV) {
        if (gv.first == emailNhap && gv.second == cccdNhap) {
            email = emailNhap;
            cccd = cccdNhap;
            return true;
        }
    }
    return false;
}

vector<Class> Lecturer::xemDanhSachLop(string emailGV, vector<Class> dsLop) {
    vector<Class> ketQua;
    for (Class lop : dsLop) {
        if (lop.getEmailGV() == emailGV) {
            ketQua.push_back(lop);
        }
    }
    return ketQua;
}

vector<Class> Lecturer::locLopTheoNgay(vector<Class> dsLopGV, string ngay) {
    vector<Class> ketQua;
    for (Class lop : dsLopGV) {
        if (lop.getNgayHoc() == ngay) {
            ketQua.push_back(lop);
        }
    }
    return ketQua;
}

vector<Class> Lecturer::timLop(vector<Class> dsLopGV, string tuKhoa) {
    vector<Class> ketQua;
    for (Class lop : dsLopGV) {
        if (lop.getMaLop() == tuKhoa ||
            lop.getTenMon().find(tuKhoa) != string::npos) {
            ketQua.push_back(lop);
        }
    }
    return ketQua;
}

vector<vector<string>> Lecturer::xemDanhSachSinhVien(vector<vector<string>> dsSV) {
    return dsSV;
}

vector<vector<string>> Lecturer::xemLichSuSession(vector<vector<string>> dsSession) {
    return dsSession;
}


//  IMPLEMENTATIONS CHO MAIN 

// 1. Xem danh sách lớp được phân công
void Lecturer::viewAssignedClasses() {
    cout << "\n================ ASSIGNED CLASSES LIST ================\n";
    vector<vector<string>> allClasses = CSVHelper::docFile("classes.csv");

    cout << left << setw(15) << "Class ID"
        << setw(30) << "Subject Name"
        << setw(60) << "Schedule (Dates & Time)"
        << setw(15) << "Status" << endl;
    cout << string(120, '-') << endl;

    bool found = false;
    for (const auto& row : allClasses) {
        if (row.size() >= 3 && row[2] == this->email) {
            string status = (row.size() >= 4 && !row[3].empty()) ? "Has Students" : "Empty";
            string schedule = getClassScheduleDetails(row[0]);
            cout << left << setw(15) << row[0]
                << setw(30) << row[1]
                << setw(60) << schedule
                << setw(15) << status << endl;
            found = true;
        }
    }
    if (!found) cout << "You are not currently assigned to any classes.\n";
    cout << "==============================================================\n";
}

// 2. Tìm kiếm lớp học theo từ khóa
void Lecturer::searchOrFilterClasses(string keyword) {
    if (keyword.empty()) {
        cout << "Please enter a keyword.\n";
        return;
    }
    cout << "\n=== SEARCH CLASSES WITH KEYWORD: " << keyword << " ===\n";
    vector<vector<string>> allClasses = CSVHelper::docFile("classes.csv");

    cout << left << setw(15) << "Class ID"
        << setw(30) << "Subject Name"
        << setw(60) << "Schedule" << endl;
    cout << string(105, '-') << endl;

    bool found = false;
    for (const auto& row : allClasses) {
        if (row.size() >= 3 && row[2] == this->email) {
            if (row[0].find(keyword) != string::npos || row[1].find(keyword) != string::npos) {
                string schedule = getClassScheduleDetails(row[0]);
                cout << left << setw(15) << row[0]
                    << setw(30) << row[1]
                    << setw(60) << schedule << endl;
                found = true;
            }
        }
    }
    if (!found) cout << "No matching classes found.\n";
}

// 3. Xem danh sách sinh viên của một lớp
void Lecturer::viewStudentList(string classId) {
    vector<string> classInfo = CSVHelper::getClassInfo(classId);
    if (classInfo.empty()) {
        cout << "Class information not found.\n";
        return;
    }
    if (classInfo.size() >= 2 && classInfo[1] != this->email) {
        cout << "You do not have permission to view this class list.\n";
        return;
    }
    string schedule = getClassScheduleDetails(classId);

    cout << "\n============================================================\n";
    cout << "   STUDENT LIST FOR CLASS: " << classId << "\n";
    cout << "   Subject : " << classInfo[0] << "\n";
    cout << "   Schedule: " << schedule << "\n";
    cout << "============================================================\n";

    if (classInfo.size() < 3 || classInfo[2].empty()) {
        cout << "The class has no students.\n";
        return;
    }
    string studentListRaw = classInfo[2];
    stringstream ss(studentListRaw);
    string studentId;
    int count = 1;
    cout << left << setw(10) << "No."
        << setw(20) << "Student ID"
        << setw(30) << "Student Name" << endl;
    cout << string(60, '-') << endl;
    while (getline(ss, studentId, ',')) {
        string name = CSVHelper::getStudentNameById(studentId);
        cout << left << setw(10) << count++
            << setw(20) << studentId
            << setw(30) << name << endl;
    }
    cout << endl;
}

// 4. Tạo phiên điểm danh
string convertDateToISO(string dateDMY) {
    stringstream ss(dateDMY);
    string d, m, y;
    char delimiter = (dateDMY.find('/') != string::npos) ? '/' : '-';
    getline(ss, d, delimiter);
    getline(ss, m, delimiter);
    getline(ss, y, delimiter);
    if (d.length() == 1) d = "0" + d;
    if (m.length() == 1) m = "0" + m;
    return y + "-" + m + "-" + d;
}

void Lecturer::createAttendanceSession(string classId) {
    vector<string> classInfo = CSVHelper::getClassInfo(classId);
    if (classInfo.empty() || (classInfo.size() >= 2 && classInfo[1] != this->email)) {
        cout << "Error: You are not in charge of this class or the class does not exist.\n";
        return;
    }
    if (CSVHelper::hasOpenSession(classId)) {
        cout << "Error: Another attendance session is currently active for this class.\n";
        return;
    }
    string dateInput, timeStart, timeEnd, password;
    cout << "--- Create New Attendance Session ---\n";
    cout << "1. Enter Date (DD/MM/YYYY, e.g., 23/01/2026): ";
    cin >> ws;
    getline(cin, dateInput);
    cout << "2. Enter Start Time (HH:MM, e.g., 08:00): ";
    getline(cin, timeStart);
    cout << "3. Enter End Time (HH:MM, e.g., 09:30): ";
    getline(cin, timeEnd);
    cout << "4. Enter Attendance Password: ";
    getline(cin, password);
    string isoDate = convertDateToISO(dateInput);
    string finalStartTime = isoDate + " " + timeStart + ":00";
    string finalEndTime = isoDate + " " + timeEnd + ":00";
    CSVHelper::addAttendanceSession(classId, finalStartTime, finalEndTime, password);
    cout << "\n>>> Attendance session created successfully!\n";
    cout << "Time: " << finalStartTime << " -> " << finalEndTime << endl;
    cout << "Password: " << password << endl;
}

// 5. Điểm danh thủ công 
void Lecturer::takeManualAttendance(string classId) {
    // 1. Kiểm tra session
    vector<string> activeSession = CSVHelper::getActiveSession(classId);
    if (activeSession.empty()) {
        cout << "Error: No active attendance session for class " << classId << ".\n";
        cout << "Please create a session first (Option 4).\n";
        return;
    }

    // 2. Lấy thông tin lớp
    vector<string> classInfo = CSVHelper::getClassInfo(classId);
    if (classInfo.empty() || classInfo.size() < 3) {
        cout << "Error: Class information not found.\n";
        return;
    }

    // 3. Tách danh sách sinh viên lớp
    vector<string> allStudents;
    string studentListRaw = classInfo[2];
    stringstream ssClass(studentListRaw);
    string sId;
    while (getline(ssClass, sId, ',')) {
        if (!sId.empty()) allStudents.push_back(sId);
    }

    // 4. Tách danh sách sinh viên đã điểm danh
    vector<string> presentStudents;
    string presentListRaw = (activeSession.size() >= 4) ? activeSession[3] : "";
    stringstream ssPresent(presentListRaw);
    string pId;
    while (getline(ssPresent, pId, ',')) {
        if (!pId.empty()) presentStudents.push_back(pId);
    }

    // 5. Hiển thị sinh viên trong lop
    cout << "\n=== LIST OF STUDENTS ===\n";
    cout << "Subject: " << classInfo[0] << endl;

    int missingCount = 0;
    for (const string& studentId : allStudents) {
        bool isPresent = false;
        for (const string& presentId : presentStudents) {
            if (studentId == presentId) {
                isPresent = true;
                break;
            }
        }
        if (!isPresent) {
            string name = CSVHelper::getStudentNameById(studentId);
            cout << studentId << " - " << name << endl;
            missingCount++;
        }
    }

    if (missingCount == 0) {
        cout << "(All students have attended)\n";
    }
    else {
        cout << "------------------------------------------\n";
    }

    // 6. VÒNG LẶP NHẬP LIỆU LIÊN TỤC
    string inputId;
    while (true) {
        cout << "\n=== MANUAL ATTENDANCE ===" << endl;
        cout << "Enter Student ID to mark attendance (or enter 0 to finish): ";
        cin >> inputId;

        // Điều kiện thoát vòng lặp
        if (inputId == "0") {
            cout << "Exiting manual attendance mode.\n";
            break;
        }

        //  CHECK: Sinh viên có trong lớp không? 
        bool isStudentInClass = false;
        for (const string& id : allStudents) {
            if (id == inputId) {
                isStudentInClass = true;
                break;
            }
        }

        if (!isStudentInClass) {
            cout << "Error: Student " << inputId << " does not exist in this class. Attendance failed.\n";
        }
        else {
            // Gọi hàm điểm danh
            if (CSVHelper::markAttendance(classId, inputId)) {
                cout << "Attendance marked successfully for student " << inputId << ".\n";
            }
            else {
                cout << "Error: Attendance failed. Student might have already been marked present.\n";
            }
        }
    }
}

// 6. Xem và Sửa phiên điểm danh
// ĐÃ CẬP NHẬT: Thêm tùy chọn nhập 0 để thoát
void Lecturer::reviewAndEditAttendance(string classId) {
    cout << "\n=== ATTENDANCE SESSION LIST ===\n";
    vector<vector<string>> sessions = CSVHelper::getAllSessions(classId);
    if (sessions.empty()) {
        cout << "No attendance sessions available.\n";
        return;
    }
    for (size_t i = 0; i < sessions.size(); i++) {
        if (sessions[i].size() >= 2)
            cout << i + 1 << ". Start: " << sessions[i][1] << " | End: " << sessions[i][2] << endl;
    }

    int choice;
    // Cập nhật câu thông báo
    cout << "Select session to edit (enter No.) or enter 0 to exit: ";
    cin >> choice;

    // Nhập 0 để thoát 
    if (choice == 0) {
        return;
    }

    if (choice < 1 || choice >(int)sessions.size()) {
        cout << "Invalid selection.\n";
        return;
    }

    vector<string> selectedSession = sessions[choice - 1];
    string startTime = selectedSession[1];
    vector<string> currentStudents = CSVHelper::getPresentStudentsOfSession(classId, startTime);
    string dateInput, timeEnd, newPassword;

    cout << "\n--- Update Session Information ---\n";
    cout << "1. Enter End Date (DD/MM/YYYY, e.g., 23/01/2026): ";
    cin >> ws;
    getline(cin, dateInput);
    cout << "2. Enter New End Time (HH:MM, e.g., 09:30): ";
    getline(cin, timeEnd);
    cout << "3. Enter New Password: ";
    getline(cin, newPassword);
    string isoDate = convertDateToISO(dateInput);
    string newEndTime = isoDate + " " + timeEnd + ":00";
    CSVHelper::updateAttendanceSession(classId, startTime, newEndTime, newPassword, currentStudents);
    cout << "\n>>> Attendance session updated successfully.\n";
    cout << "New end time: " << newEndTime << endl;
}

// 7. Xem lịch sử điểm danh
void Lecturer::viewAttendanceHistory(string classId) {
    cout << "\n=== ATTENDANCE HISTORY FOR CLASS " << classId << " ===\n";
    vector<vector<string>> sessions = CSVHelper::getAllSessions(classId);
    if (sessions.empty()) {
        cout << "No attendance data available.\n";
        return;
    }
    cout << left << setw(25) << "Start Time"
        << setw(25) << "End Time"
        << setw(15) << "Count" << endl;
    cout << string(65, '-') << endl;
    for (const auto& sess : sessions) {
        if (sess.size() >= 3) {
            int count = 0;
            if (sess.size() >= 5 && !sess[4].empty()) {
                string list = sess[4];
                count = 1;
                for (char c : list) {
                    if (c == ',') count++;
                }
            }
            cout << left << setw(25) << sess[1]
                << setw(25) << sess[2]
                << setw(15) << count << endl;
        }
    }
}

// 8. Xóa một phiên điểm danh
void Lecturer::deleteAttendanceSession(string classId) {
    cout << "\n=== DELETE ATTENDANCE SESSION ===\n";
    vector<vector<string>> sessions = CSVHelper::getAllSessions(classId);
    if (sessions.empty()) {
        cout << "No sessions available to delete.\n";
        return;
    }
    for (size_t i = 0; i < sessions.size(); i++) {
        if (sessions[i].size() >= 2)
            cout << i + 1 << ". " << sessions[i][1] << endl;
    }
    int choice;
    cout << "Select session No. to delete: ";
    cin >> choice;
    if (choice < 1 || choice >(int)sessions.size()) {
        cout << "Invalid selection.\n";
        return;
    }
    string startTime = sessions[choice - 1][1];
    char confirm;
    cout << "Are you sure you want to delete session " << startTime << "? (y/n): ";
    cin >> confirm;
    if (confirm == 'y' || confirm == 'Y') {
        if (CSVHelper::removeAttendanceSession(classId, startTime)) {
            cout << "Deleted successfully.\n";
        }
        else {
            cout << "Delete failed.\n";
        }
    }
    else {
        cout << "Operation cancelled.\n";
    }
}
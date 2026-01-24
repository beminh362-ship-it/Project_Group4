#define _CRT_SECURE_NO_WARNINGS 

#include "CSVHelper.h"
#include "Student.h"
#include "Lecturer.h"
#include "Utils.h"
#include <sstream>
#include <fstream>
#include <ctime>

// ==================== 1. DOC FILE CSV ====================
vector<vector<string>> CSVHelper::docFile(const string& tenFile) {
    vector<vector<string>> duLieu;
    ifstream file(tenFile);

    if (!file.is_open()) {
        return duLieu;
    }

    string dong;
    while (getline(file, dong)) {
        vector<string> cacCot = tachDong(dong);
        duLieu.push_back(cacCot);
    }

    file.close();
    return duLieu;
}

// ==================== 2. GHI FILE CSV ====================
void CSVHelper::ghiFile(const string& tenFile, const vector<vector<string>>& duLieu) {
    ofstream file(tenFile);

    if (!file.is_open()) {
        return;
    }

    for (size_t i = 0; i < duLieu.size(); i++) {
        file << ghepDong(duLieu[i]);
        if (i != duLieu.size() - 1) {
            file << endl;
        }
    }

    file.close();
}

// ==================== 3. CAP NHAP DONG ====================
void CSVHelper::capNhatDong(const string& tenFile, int viTri, const vector<string>& dongMoi) {
    vector<vector<string>> duLieu = docFile(tenFile);

    if (viTri < 0 || viTri >= static_cast<int>(duLieu.size())) {
        return;
    }

    duLieu[viTri] = dongMoi;
    ghiFile(tenFile, duLieu);
}

// ==================== 4. TIM KIEM ====================
vector<vector<string>> CSVHelper::timKiem(const string& tenFile, int cotCanTim, const string& giaTri) {
    vector<vector<string>> ketQua;
    vector<vector<string>> duLieu = docFile(tenFile);

    for (size_t i = 0; i < duLieu.size(); i++) {
        if (cotCanTim < static_cast<int>(duLieu[i].size())) {
            if (duLieu[i][cotCanTim] == giaTri) {
                ketQua.push_back(duLieu[i]);
            }
        }
    }

    return ketQua;
}

int CSVHelper::timViTri(const string& tenFile, int cotCanTim, const string& giaTri) {
    vector<vector<string>> duLieu = docFile(tenFile);

    for (size_t i = 0; i < duLieu.size(); i++) {
        if (cotCanTim < static_cast<int>(duLieu[i].size())) {
            if (duLieu[i][cotCanTim] == giaTri) {
                return static_cast<int>(i);
            }
        }
    }

    return -1;
}

// ==================== HAM PHU TRO ====================
vector<string> CSVHelper::tachDong(const string& dong) {
    vector<string> ketQua;
    string tam;
    stringstream ss(dong);

    while (getline(ss, tam, ';')) {
        ketQua.push_back(tam);
    }

    return ketQua;
}

string CSVHelper::ghepDong(const vector<string>& cot) {
    string dong = "";

    for (size_t i = 0; i < cot.size(); i++) {
        dong += cot[i];
        if (i != cot.size() - 1) {
            dong += ";";
        }
    }

    return dong;
}

// ==================== STUDENT LOGIN ====================
Student* CSVHelper::findStudentByEmailAndPassword(const string& email, const string& password) {
    vector<vector<string>> students = docFile("students.csv");

    // Format CSV: TenSinhVien;Email;MaSinhVien;DanhSachLopHoc
    // PASSWORD = MaSinhVien (MSSV)

    for (const auto& row : students) {
        if (row.size() < 3) continue;

        string csvEmail = row[1];
        string csvMSSV = row[2];

        if (csvEmail == email && csvMSSV == password) {
            string studentId = csvMSSV;

            vector<string> classes;
            if (row.size() >= 4 && !row[3].empty()) {
                stringstream ss(row[3]);
                string classId;
                while (getline(ss, classId, ',')) {
                    classes.push_back(classId);
                }
            }

            return new Student(email, studentId, classes);
        }
    }

    return nullptr;
}

// ==================== LECTURER LOGIN ====================
Lecturer* CSVHelper::findLecturerByEmailAndPassword(const string& email, const string& password) {
    vector<vector<string>> lecturers = docFile("lecturers.csv");

    // Format CSV: TenGiangVien;Email;SoCCCD;DanhSachLopHoc
    // PASSWORD = SoCCCD

    for (const auto& row : lecturers) {
        if (row.size() < 3) continue;

        string csvEmail = row[1];
        string csvCCCD = row[2];

        if (csvEmail == email && csvCCCD == password) {
            return new Lecturer(email, csvCCCD);
        }
    }

    return nullptr;
}

// ==================== CLASS INFO ====================
vector<string> CSVHelper::getClassInfo(const string& classId) {
    vector<vector<string>> classes = docFile("classes.csv");

    // Format: MaLop;TenMonHoc;EmailGiangVien;DanhSachSinhVien

    for (const auto& row : classes) {
        if (row.size() >= 3 && row[0] == classId) {
            vector<string> info;
            info.push_back(row[1]); // Tên môn học
            info.push_back(row[2]); // Email giảng viên
            if (row.size() >= 4) {
                info.push_back(row[3]); // Danh sách sinh viên
            }
            return info;
        }
    }

    return vector<string>();
}

// ==================== ACTIVE SESSION ====================
vector<string> CSVHelper::getActiveSession(const string& classId) {
    string filename = "class_" + classId + "_sessions.csv";
    vector<vector<string>> sessions = docFile(filename);

    time_t now = time(nullptr);

    for (const auto& row : sessions) {
        if (row.size() < 3) continue;

        string startTime = row[1];
        string endTime = row[2];

        time_t start = Utils::parseDateTime(startTime);
        time_t end = Utils::parseDateTime(endTime);

        if (now >= start && now <= end) {
            vector<string> sessionInfo;
            sessionInfo.push_back(startTime);
            sessionInfo.push_back(endTime);
            sessionInfo.push_back(row.size() >= 4 ? row[3] : "");
            sessionInfo.push_back(row.size() >= 5 ? row[4] : "");
            return sessionInfo;
        }
    }

    return vector<string>();
}

// ==================== CHECK ATTENDANCE ====================
bool CSVHelper::hasStudentAttended(const string& classId, const string& studentId) {
    vector<string> activeSession = getActiveSession(classId);

    if (activeSession.empty() || activeSession.size() < 4) {
        return false;
    }

    string studentList = activeSession[3];

    if (studentList.empty()) return false;

    stringstream ss(studentList);
    string id;
    while (getline(ss, id, ',')) {
        if (id == studentId) {
            return true;
        }
    }

    return false;
}

// ==================== MARK ATTENDANCE ====================
bool CSVHelper::markAttendance(const string& classId, const string& studentId) {
    string filename = "class_" + classId + "_sessions.csv";
    vector<vector<string>> sessions = docFile(filename);

    time_t now = time(nullptr);
    bool found = false;
    int sessionIndex = -1;

    for (size_t i = 0; i < sessions.size(); i++) {
        if (sessions[i].size() < 3) continue;

        time_t start = Utils::parseDateTime(sessions[i][1]);
        time_t end = Utils::parseDateTime(sessions[i][2]);

        if (now >= start && now <= end) {
            sessionIndex = static_cast<int>(i);
            found = true;
            break;
        }
    }

    if (!found) return false;

    if (sessions[sessionIndex].size() < 5) {
        while (sessions[sessionIndex].size() < 4) {
            sessions[sessionIndex].push_back("");
        }
        sessions[sessionIndex].push_back(studentId);
    }
    else {
        string& studentList = sessions[sessionIndex][4];
        if (!studentList.empty()) {
            studentList += ",";
        }
        studentList += studentId;
    }

    ghiFile(filename, sessions);
    updateStudentAttendanceCount(classId, studentId, 1);

    return true;
}

// ==================== GET ALL SESSIONS ====================
vector<vector<string>> CSVHelper::getAllSessions(const string& classId) {
    string filename = "class_" + classId + "_sessions.csv";
    return docFile(filename);
}

// ==================== HAS OPEN SESSION ====================
bool CSVHelper::hasOpenSession(const string& classId) {
    vector<string> activeSession = getActiveSession(classId);
    return !activeSession.empty();
}

// ==================== ADD SESSION ====================
void CSVHelper::addAttendanceSession(const string& classId, const string& startTime,
    const string& endTime, const string& password) {
    string filename = "class_" + classId + "_sessions.csv";
    vector<vector<string>> sessions = docFile(filename);

    vector<string> newSession;
    newSession.push_back(classId);
    newSession.push_back(startTime);
    newSession.push_back(endTime);
    newSession.push_back(password);
    newSession.push_back("");

    sessions.push_back(newSession);
    ghiFile(filename, sessions);
}

// ==================== UPDATE SESSION ====================
void CSVHelper::updateAttendanceSession(const string& classId, const string& startTime,
    const string& endTime, const string& password,
    const vector<string>& presentStudents) {
    string filename = "class_" + classId + "_sessions.csv";
    vector<vector<string>> sessions = docFile(filename);

    for (auto& session : sessions) {
        if (session.size() >= 2 && session[0] == classId && session[1] == startTime) {
            session[2] = endTime;
            if (session.size() >= 4) {
                session[3] = password;
            }
            else {
                session.push_back(password);
            }

            string studentList = "";
            for (size_t i = 0; i < presentStudents.size(); i++) {
                studentList += presentStudents[i];
                if (i < presentStudents.size() - 1) {
                    studentList += ",";
                }
            }

            if (session.size() >= 5) {
                session[4] = studentList;
            }
            else {
                session.push_back(studentList);
            }

            break;
        }
    }

    ghiFile(filename, sessions);
}

// ==================== GET PRESENT STUDENTS ====================
vector<string> CSVHelper::getPresentStudentsOfSession(const string& classId, const string& startTime) {
    string filename = "class_" + classId + "_sessions.csv";
    vector<vector<string>> sessions = docFile(filename);

    for (const auto& session : sessions) {
        if (session.size() >= 5 && session[0] == classId && session[1] == startTime) {
            vector<string> students;
            if (!session[4].empty()) {
                stringstream ss(session[4]);
                string studentId;

                while (getline(ss, studentId, ',')) {
                    students.push_back(studentId);
                }
            }
            return students;
        }
    }

    return vector<string>();
}

// ==================== GET SESSION END TIME ====================
string CSVHelper::getSessionEndTime(const string& classId, const string& startTime) {
    string filename = "class_" + classId + "_sessions.csv";
    vector<vector<string>> sessions = docFile(filename);

    for (const auto& session : sessions) {
        if (session.size() >= 3 && session[0] == classId && session[1] == startTime) {
            return session[2];
        }
    }

    return "";
}

// ==================== REMOVE SESSION ====================
bool CSVHelper::removeAttendanceSession(const string& classId, const string& startTime) {
    string filename = "class_" + classId + "_sessions.csv";
    vector<vector<string>> sessions = docFile(filename);
    vector<vector<string>> newSessions;

    bool removed = false;

    for (const auto& session : sessions) {
        if (session.size() >= 2 && session[0] == classId && session[1] == startTime) {
            removed = true;
            continue;
        }
        newSessions.push_back(session);
    }

    if (removed) {
        ghiFile(filename, newSessions);
    }

    return removed;
}

// ==================== UPDATE ATTENDANCE COUNT ====================
void CSVHelper::updateStudentAttendanceCount(const string& classId, const string& studentId, int delta) {
    string filename = "class_" + classId + "_students.csv";
    vector<vector<string>> students = docFile(filename);

    bool found = false;

    for (auto& row : students) {
        if (row.size() >= 3 && row[1] == studentId) {
            int count = stoi(row[2]);
            count += delta;
            if (count < 0) count = 0;
            row[2] = to_string(count);
            found = true;
            break;
        }
    }

    if (!found && delta > 0) {
        vector<vector<string>> allStudents = docFile("students.csv");
        for (const auto& student : allStudents) {
            if (student.size() >= 3 && student[2] == studentId) {
                vector<string> newRow;
                newRow.push_back(student[0]);
                newRow.push_back(studentId);
                newRow.push_back(to_string(delta));
                students.push_back(newRow);
                found = true;
                break;
            }
        }
    }

    if (found) {
        ghiFile(filename, students);
    }
}

// ==================== LOOKUP STUDENT NAME ====================
string CSVHelper::getStudentNameById(const string& studentId) {
    vector<vector<string>> students = docFile("students.csv");
    for (const auto& row : students) {
        if (row.size() >= 3 && row[2] == studentId) {
            return row[0]; // Trả về Tên sinh viên
        }
    }
    return "Unknown";
}

// ==================== HAM PHU TRO NGAY THANG ====================
// Ham nay duoc dat o day de phuc vu cho getClassSchedule
static string getDayOfWeek(string dateDMY) {
    int d, m, y;
    char sep;
    stringstream ss(dateDMY);
    ss >> d >> sep >> m >> sep >> y;

    tm time_in = { 0 };
    time_in.tm_mday = d;
    time_in.tm_mon = m - 1;
    time_in.tm_year = y - 1900;
    time_in.tm_isdst = -1;

    time_t time_temp = mktime(&time_in);

    // Nho #define o dau file, dong nay se khong bao loi nua
    const tm* time_out = localtime(&time_temp);

    if (time_out == nullptr) return "";

    string days[] = { "Chu Nhat", "Thu 2", "Thu 3", "Thu 4", "Thu 5", "Thu 6", "Thu 7" };
    return days[time_out->tm_wday];
}

// ==================== GET CLASS SCHEDULE ====================
string CSVHelper::getClassSchedule(const string& classId) {
    vector<vector<string>> schedules = docFile("class_schedule.csv");

    for (const auto& row : schedules) {
        if (row.size() >= 4 && row[0] == classId) {
            string dateStr = row[1];
            string start = row[2];
            string end = row[3];

            string dayName = getDayOfWeek(dateStr);

            return dayName + " (" + start + " - " + end + ")";
        }
    }
    return "Chua co lich";
}
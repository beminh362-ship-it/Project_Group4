#ifndef CSVHELPER_H
#define CSVHELPER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

class CSVHelper {
public:
    // Ham moi them de lay lich hoc
    static string getClassSchedule(const string& classId);

    // ===== 1. DOC & GHI FILE =====
    static vector<vector<string>> docFile(const string& tenFile);
    static void ghiFile(const string& tenFile, const vector<vector<string>>& duLieu);
    static string getStudentNameById(const string& studentId);

    // ===== 2. CAP NHAT DONG =====
    static void capNhatDong(const string& tenFile, int viTri, const vector<string>& dongMoi);

    // ===== 3. TIM KIEM =====
    static vector<vector<string>> timKiem(const string& tenFile, int cotCanTim, const string& giaTri);
    static int timViTri(const string& tenFile, int cotCanTim, const string& giaTri);

    // ===== 4. ATTENDANCE SESSION FUNCTIONS =====
    static bool hasOpenSession(const string& classId);
    static void addAttendanceSession(const string& classId, const string& startTime,
        const string& endTime, const string& password);
    static void updateAttendanceSession(const string& classId, const string& startTime,
        const string& endTime, const string& password,
        const vector<string>& presentStudents);
    static vector<string> getPresentStudentsOfSession(const string& classId, const string& startTime);
    static string getSessionEndTime(const string& classId, const string& startTime);
    static bool removeAttendanceSession(const string& classId, const string& startTime);
    static void updateStudentAttendanceCount(const string& classId, const string& studentId, int delta);

    // ===== 5. STUDENT/LECTURER LOOKUP =====
    static class Student* findStudentByEmailAndPassword(const string& email, const string& password);
    static class Lecturer* findLecturerByEmailAndPassword(const string& email, const string& password);

    // ===== 6. CLASS INFO =====
    static vector<string> getClassInfo(const string& classId);
    static vector<string> getActiveSession(const string& classId);
    static bool hasStudentAttended(const string& classId, const string& studentId);
    static bool markAttendance(const string& classId, const string& studentId);
    static vector<vector<string>> getAllSessions(const string& classId);

    // ===== 7. HAM PHU TRO =====
    static vector<string> tachDong(const string& dong);
    static string ghepDong(const vector<string>& cot);
};

#endif
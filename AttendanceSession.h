#ifndef ATTENDANCESESSION_H
#define ATTENDANCESESSION_H
#include <string>
#include <vector>
using namespace std;
class AttendanceSession {
private:
    string classId;                  // Mã lớp
    string startTime;                // DD/MM/YYYY HH:MM
    string endTime;                  // DD/MM/YYYY HH:MM
    string password;                 // Mk (có thể rỗng)
    vector<string> presentStudents;  // Danh sách MSSV có mặt (svcomat)

public:
    // Constructor 
    AttendanceSession();
    AttendanceSession(
        const string& classId,
        const string& startTime,
        const string& endTime,
        const string& password
    );

    // Getter
    string getClassId() const;
    string getStartTime() const;
    string getEndTime() const;
    string getPassword() const;
    vector<string> getPresentStudents() const;

    //TẠO PHIÊN ĐIỂM DANH 
    // Ghi 1 session mới vào file CSV
    bool createSession();

    //Trạng thái session 
    bool isOpen() const;
    bool isClosed() const;

    // Attendance logic 
    bool hasStudentCheckedIn(const string& studentId) const;

    bool studentCheckIn(
        const string& studentId,
        const string& inputPassword
    );

    bool lecturerMarkPresent(const string& studentId);

    // Sửa trạng thái (sau khi đóng) 
    bool addPresentStudent(const string& studentId);
    bool removePresentStudent(const string& studentId);

    // File handling 
    void saveToSessionFile() const;
    void updateStudentAttendanceCount(
        const string& studentId,
        int delta
    ) const;

    //  Xóa session
    static bool deleteSession(
        const string& classId,
        const string& startTime
    );
};

#endif

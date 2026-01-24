#include "AttendanceSession.h"
#include "CSVHelper.h"
#include "Utils.h"
#include <algorithm>
#include <ctime>

//  Constructor 
AttendanceSession::AttendanceSession() {}
AttendanceSession::AttendanceSession(
    const string& classId,
    const string& startTime,
    const string& endTime,
    const string& password
) {
    this->classId = classId;
    this->startTime = startTime;
    this->endTime = endTime;
    this->password = password;
}

// Getter 
string AttendanceSession::getClassId() const { return classId; }
string AttendanceSession::getStartTime() const { return startTime; }
string AttendanceSession::getEndTime() const { return endTime; }
string AttendanceSession::getPassword() const { return password; }

vector<string> AttendanceSession::getPresentStudents() const {
    return presentStudents;
}

//  TẠO PHIÊN ĐIỂM DANH 
bool AttendanceSession::createSession() {
    // Không cho tạo nếu đã có session đang mở
    if (CSVHelper::hasOpenSession(classId))
        return false;

    presentStudents.clear();

    CSVHelper::addAttendanceSession(
        classId,
        startTime,
        endTime,
        password
    );

    return true;
}

// Trạng thái session
bool AttendanceSession::isOpen() const {
    time_t now = time(nullptr);
    return now >= Utils::parseDateTime(startTime)
        && now <= Utils::parseDateTime(endTime);
}

bool AttendanceSession::isClosed() const {
    time_t now = time(nullptr);
    return now > Utils::parseDateTime(endTime);
}

//  Attendance logic 
bool AttendanceSession::hasStudentCheckedIn(const string& studentId) const {
    return find(
        presentStudents.begin(),
        presentStudents.end(),
        studentId
    ) != presentStudents.end();
}
bool AttendanceSession::studentCheckIn(
    const string& studentId,
    const string& inputPassword
) {
    if (!isOpen()) return false;
    if (!password.empty() && password != inputPassword) return false;
    if (hasStudentCheckedIn(studentId)) return false;

    presentStudents.push_back(studentId);
    saveToSessionFile();
    updateStudentAttendanceCount(studentId, +1);
    return true;
}

bool AttendanceSession::lecturerMarkPresent(const string& studentId) {
    if (!isOpen()) return false;
    if (hasStudentCheckedIn(studentId)) return false;

    presentStudents.push_back(studentId);
    saveToSessionFile();
    updateStudentAttendanceCount(studentId, +1);
    return true;
}

//  Sửa trạng thái (sau khi đóng) 
bool AttendanceSession::addPresentStudent(const string& studentId) {
    if (!isClosed()) return false;
    if (hasStudentCheckedIn(studentId)) return false;

    presentStudents.push_back(studentId);
    saveToSessionFile();
    updateStudentAttendanceCount(studentId, +1);
    return true;
}

bool AttendanceSession::removePresentStudent(const string& studentId) {
    if (!isClosed()) return false;

    auto it = find(
        presentStudents.begin(),
        presentStudents.end(),
        studentId
    );
    if (it == presentStudents.end()) return false;

    presentStudents.erase(it);
    saveToSessionFile();
    updateStudentAttendanceCount(studentId, -1);
    return true;
}

//  CSV handling 
void AttendanceSession::saveToSessionFile() const {
    CSVHelper::updateAttendanceSession(
        classId,
        startTime,
        endTime,
        password,
        presentStudents
    );
}

void AttendanceSession::updateStudentAttendanceCount(
    const string& studentId,
    int delta
) const {
    CSVHelper::updateStudentAttendanceCount(
        classId,
        studentId,
        delta
    );
}

//  Xóa session 

bool AttendanceSession::deleteSession(
    const string& classId,
    const string& startTime
) {
    vector<string> attended =
        CSVHelper::getPresentStudentsOfSession(classId, startTime);

    string endTime =
        CSVHelper::getSessionEndTime(classId, startTime);

    if (time(nullptr) > Utils::parseDateTime(endTime)) {
        for (const string& id : attended) {
            CSVHelper::updateStudentAttendanceCount(
                classId, id, -1
            );
        }
    }

    return CSVHelper::removeAttendanceSession(
        classId, startTime
    );
}





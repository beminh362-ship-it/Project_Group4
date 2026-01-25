// ==================== Lecturer.h ====================
#ifndef LECTURER_H
#define LECTURER_H

#include <string>
#include <vector>
#include "Class.h"
#include "Member.h"

using namespace std;

class Lecturer : public Member {
private:
    string cccd;

public:
    Lecturer(string email, string cccd);

    // Override từ Member
    string getRole() const override { return "Lecturer"; }

    // 1. ĐĂNG NHẬP
    bool dangNhap(string emailNhap, string cccdNhap,
        vector<pair<string, string>> dsGV);

    // 2. QUẢN LÝ LỚP HỌC 
    vector<Class> xemDanhSachLop(string emailGV, vector<Class> dsLop);
    vector<Class> locLopTheoNgay(vector<Class> dsLopGV, string ngay);
    vector<Class> timLop(vector<Class> dsLopGV, string tuKhoa);

    // 3. XEM DANH SÁCH & LỊCH SỬ 
    vector<vector<string>> xemDanhSachSinhVien(vector<vector<string>> dsSV);
    vector<vector<string>> xemLichSuSession(vector<vector<string>> dsSession);

    //  4. FUNCTIONS ĐƯỢC GỌI TỪ MAIN 
    void viewAssignedClasses();
    void searchOrFilterClasses(string keyword);
    void viewStudentList(string classId);
    void createAttendanceSession(string classId);
    void takeManualAttendance(string classId);
    void reviewAndEditAttendance(string classId);
    void viewAttendanceHistory(string classId);
    void deleteAttendanceSession(string classId);
};

#endif

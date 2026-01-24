#include "Lecturer.h"
#include "CSVHelper.h"
#include "Utils.h"
#include <iostream>
#include <iomanip> // Thư viện để format bảng (setw)
#include <sstream>
#include <limits>  // Thêm thư viện này để xử lý buffer


using namespace std;


// ================= HAM HO TRO CHUYEN DOI NGAY GIO =================
// Hàm này giúp chuyển đổi (Ngày + Giờ) thành chuẩn ISO (YYYY-MM-DD HH:MM:SS)
string formatDateTimeISO(string dateDMY, string timeHM) {
   // Input: dateDMY = "23/01/2026", timeHM = "08:00"
   // Output: "2026-01-23 08:00:00"


   stringstream ss(dateDMY);
   string d, m, y;
   char sep;


   // Hỗ trợ đọc cả dấu / hoặc -
   // Đoạn này parse ngày tháng năm
   if (dateDMY.find('/') != string::npos) sep = '/';
   else sep = '-';


   getline(ss, d, sep);
   getline(ss, m, sep);
   getline(ss, y, sep);


   // Đảm bảo d và m có 2 chữ số (ví dụ 1 -> 01)
   if (d.length() == 1) d = "0" + d;
   if (m.length() == 1) m = "0" + m;


   return y + "-" + m + "-" + d + " " + timeHM + ":00";
}


// Constructor
Lecturer::Lecturer(string email, string cccd) : Member(email, cccd) {
   this->email = email;
   this->cccd = cccd;
}


// Giữ lại các hàm cũ
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




// ========== IMPLEMENTATIONS CHO MAIN (DA CAP NHAT) =================


// 1. Xem danh sách lớp được phân công
void Lecturer::viewAssignedClasses() {
   cout << "DANH SÁCH CÁC LỚP ĐƯỢC PHÂN CÔNG: \n";
   vector<vector<string>> allClasses = CSVHelper::docFile("classes.csv");


   // Header bảng
   cout << left << setw(15) << "Ma Lop"
       << setw(30) << "Ten Mon Hoc"
       << setw(15) << "Trang Thai" << endl;
   cout << string(60, '-') << endl;


   bool found = false;
   // Format classes.csv: MaLop;TenMon;EmailGV;DanhSachSV
   for (const auto& row : allClasses) {
       if (row.size() >= 3 && row[2] == this->email) {
           string status = (row.size() >= 4 && !row[3].empty()) ? "Co SV" : "Trong";
           cout << left << setw(15) << row[0]
               << setw(30) << row[1]
               << setw(15) << status << endl;
           found = true;
       }
   }


   if (!found) {
       cout << "Ban hien chua duoc phan cong lop hoc nao.\n";
   }
   cout << "-------------------\n";
}


// 2. Tìm kiếm lớp học theo từ khóa
// Thay thế hàm searchOrFilterClasses cũ bằng hàm này
void Lecturer::searchOrFilterClasses(string keyword) {
   if (keyword.empty()) {
       cout << "Vui long nhap tu khoa.\n";
       return;
   }


   cout << "TIM KIEM LOP HOC VOI TU KHOA: " << keyword << " ===\n";
   vector<vector<string>> allClasses = CSVHelper::docFile("classes.csv");
 // --- CẬP NHẬT HEADER BẢNG ---
 cout << left << setw(15) << "Ma Lop"
 << setw(30) << "Ten Mon Hoc"
 << setw(25) << "Thoi Gian" << endl; // Thêm cột Thời Gian
cout << string(70, '-') << endl;


bool found = false;
for (const auto& row : allClasses) {
 // Chỉ tìm trong lớp của giảng viên này
 if (row.size() >= 3 && row[2] == this->email) {
     // Tìm trong Mã lớp (row[0]) hoặc Tên môn (row[1])
     if (row[0].find(keyword) != string::npos || row[1].find(keyword) != string::npos) {


         // --- CẬP NHẬT: Lấy lịch học từ CSVHelper ---
         string schedule = CSVHelper::getClassSchedule(row[0]);


         cout << left << setw(15) << row[0]
             << setw(30) << row[1]
             << setw(25) << schedule << endl; // In thêm lịch học
         found = true;
     }
 }
}


if (!found) cout << "Khong tim thay lop nao phu hop.\n";
}


// 3. Xem danh sách sinh viên của một lớp (DA CAP NHAT HIEN TEN)
// --- CẬP NHẬT HÀM SỐ 3: XEM DANH SÁCH SINH VIÊN ---
void Lecturer::viewStudentList(string classId) {
    // 1. Lấy thông tin cơ bản của lớp
    vector<string> classInfo = CSVHelper::getClassInfo(classId);
 
 
    // Check lỗi / quyền hạn
    if (classInfo.empty()) {
        cout << "Khong tim thay thong tin lop hoc.\n";
        return;
    }
    if (classInfo.size() >= 2 && classInfo[1] != this->email) {
        cout << "Ban khong co quyen xem danh sach lop nay.\n";
        return;
    }
 
 
    // 2. Lấy thêm thông tin Lịch Học (gọi hàm từ CSVHelper)
    string schedule = CSVHelper::getClassSchedule(classId);
 
 
    // 3. Hiển thị Header với thông tin Lịch Học
    cout << "\n============================================================\n";
    cout << "   DANH SACH SINH VIEN LOP: " << classId << "\n";
    cout << "   Mon Hoc : " << classInfo[0] << "\n"; // classInfo[0] là tên môn
    cout << "   Lich Hoc: " << schedule << "\n";     // <--- DÒNG MỚI THÊM VÀO ĐÂY
    cout << "============================================================\n";
 
 
    // 4. Kiểm tra danh sách rỗng
    if (classInfo.size() < 3 || classInfo[2].empty()) {
        cout << "Lop chua co sinh vien nao.\n";
        return;
    }
 
 
    // 5. Vẽ bảng sinh viên
    string studentListRaw = classInfo[2];
    stringstream ss(studentListRaw);
    string studentId;
    int count = 1;
 
 
    cout << left << setw(10) << "STT"
        << setw(20) << "Ma Sinh Vien"
        << setw(30) << "Ten Sinh Vien" << endl;
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
 // Hàm hỗ trợ chuyển đổi định dạng ngày (Thêm vào đầu file hoặc bên trên hàm createAttendanceSession)
 string convertDateToISO(string dateDMY) {
    // Input: 23/01/2026 -> Output: 2026-01-23
    // Mục đích: Để hệ thống có thể so sánh thời gian chính xác
    stringstream ss(dateDMY);
    string d, m, y;
 
 
   // Tách chuỗi dựa trên dấu gạch chéo '/' hoặc dấu gạch ngang '-'
   char delimiter = (dateDMY.find('/') != string::npos) ? '/' : '-';


   getline(ss, d, delimiter);
   getline(ss, m, delimiter);
   getline(ss, y, delimiter);


   // Đảm bảo d và m có 2 chữ số (ví dụ 1 -> 01)
   if (d.length() == 1) d = "0" + d;
   if (m.length() == 1) m = "0" + m;


   return y + "-" + m + "-" + d;
}


// --- CẬP NHẬT HÀM SỐ 4: TẠO PHIÊN ĐIỂM DANH ---
void Lecturer::createAttendanceSession(string classId) {
   // 1. Kiểm tra quyền và trạng thái
   vector<string> classInfo = CSVHelper::getClassInfo(classId);
   if (classInfo.empty() || (classInfo.size() >= 2 && classInfo[1] != this->email)) {
       cout << "Loi: Ban khong phu trach lop nay hoac lop khong ton tai.\n";
       return;
   }


   if (CSVHelper::hasOpenSession(classId)) {
       cout << "Loi: Dang co mot phien diem danh khac dang dien ra cho lop nay.\n";
       return;
   }


   string dateInput, timeStart, timeEnd, password;


   // 2. Nhập liệu thân thiện (Tách riêng Ngày và Giờ)
   cout << "--- Tao Phien Diem Danh Moi ---\n";


   cout << "1. Nhap Ngay (DD/MM/YYYY, vd: 23/01/2026): ";
   cin >> ws;
   getline(cin, dateInput);


   cout << "2. Nhap Gio Bat Dau (HH:MM, vd: 08:00): ";
   getline(cin, timeStart);


   cout << "3. Nhap Gio Ket Thuc (HH:MM, vd: 09:30): ";
   getline(cin, timeEnd);


   cout << "4. Nhap Mat Khau Diem Danh: ";
   getline(cin, password);


   // 3. Xử lý dữ liệu về định dạng chuẩn (YYYY-MM-DD HH:MM:SS) để lưu file
   // Lưu ý: Hệ thống cần format YYYY-MM-DD để so sánh ngày tháng chính xác
   string isoDate = convertDateToISO(dateInput);


   string finalStartTime = isoDate + " " + timeStart + ":00";
   string finalEndTime = isoDate + " " + timeEnd + ":00";


   // 4. Lưu vào CSV
   CSVHelper::addAttendanceSession(classId, finalStartTime, finalEndTime, password);


   cout << "\n>>> Tao phien diem danh thanh cong!\n";
   cout << "Thoi gian: " << finalStartTime << " -> " << finalEndTime << endl;
   cout << "Mat khau: " << password << endl;
}
// 5. Điểm danh thủ công cho sinh viên
void Lecturer::takeManualAttendance(string classId) {
   cout << "\n=== DIEM DANH THU CONG ===\n";
   string studentId;
   cout << "Nhap Ma Sinh Vien can diem danh: ";
   cin >> studentId;


   // Check xem sinh viên có trong lớp không (Optional)


   bool success = CSVHelper::markAttendance(classId, studentId);
   if (success) {
       cout << "Diem danh thanh cong cho sinh vien " << studentId << ".\n";
   }
   else {
       cout << "Loi: Khong tim thay phien diem danh dang hoat dong hoac sinh vien da duoc diem danh.\n";
       cout << "Vui long kiem tra xem co phien diem danh nao dang dien ra trong khung gio nay khong.\n";
   }
}


// 6. Xem và Sửa phiên điểm danh
void Lecturer::reviewAndEditAttendance(string classId) {
   cout << " DANH SACH PHIEN DIEM DANH: \n";
   vector<vector<string>> sessions = CSVHelper::getAllSessions(classId);


   if (sessions.empty()) {
       cout << "Chua co phien diem danh nao.\n";
       return;
    }
 
 
    for (size_t i = 0; i < sessions.size(); i++) {
        if (sessions[i].size() >= 2)
            cout << i + 1 << ". Bat dau: " << sessions[i][1] << " | Ket thuc: " << sessions[i][2] << endl;
    }
 
 
    int choice;
    cout << "Chon phien muon sua (nhap STT): ";
    cin >> choice;
 
 
    if (choice < 1 || choice >(int)sessions.size()) {
        cout << "Lua chon khong hop le.\n";
        return;
    }
 
 
    vector<string> selectedSession = sessions[choice - 1];
    string startTime = selectedSession[1]; // Key để tìm session
 
 
    // Lấy danh sách SV hiện tại để ghi lại
    vector<string> currentStudents = CSVHelper::getPresentStudentsOfSession(classId, startTime);
 
 
    string newEndTime, newPassword;
    cout << "Nhap Thoi Gian Ket Thuc Moi (YYYY-MM-DD HH:MM:SS): ";
    getline(cin >> ws, newEndTime);
    cout << "Nhap Mat Khau Moi: ";
    getline(cin, newPassword);
 
 
    CSVHelper::updateAttendanceSession(classId, startTime, newEndTime, newPassword, currentStudents);
    cout << "Cap nhat phien diem danh thanh cong.\n";
 }
 
 
 // 7. Xem lịch sử điểm danh
 void Lecturer::viewAttendanceHistory(string classId) {
    cout <<  LICH SU DIEM DANH LOP " << classId << " ===\n";
 
 
    vector<vector<string>> sessions = CSVHelper::getAllSessions(classId);
    if (sessions.empty()) {
        cout << "Chua co du lieu diem danh.\n";
        return;
    }
cout << left << setw(25) << "Thoi Gian Bat Dau"
       << setw(25) << "Thoi Gian Ket Thuc"
       << setw(15) << "So Luong" << endl;
   cout << string(65, '-') << endl;


   for (const auto& sess : sessions) {
       if (sess.size() >= 3) {
           // Đếm số sinh viên
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
   cout << "\XOA PHIEN DIEM DANH: \n";
   // Liệt kê trước để người dùng chọn thời gian
   vector<vector<string>> sessions = CSVHelper::getAllSessions(classId);
   if (sessions.empty()) {
       cout << "Khong co phien nao de xoa.\n";
       return;
   }
for (size_t i = 0; i < sessions.size(); i++) {
       if (sessions[i].size() >= 2)
           cout << i + 1 << ". " << sessions[i][1] << endl;
   }


   int choice;
   cout << "Chon STT phien muon xoa: ";
   cin >> choice;


   if (choice < 1 || choice >(int)sessions.size()) {
       cout << "Lua chon sai.\n";
       return;
   }


   string startTime = sessions[choice - 1][1];


   // Cần xác nhận
   char confirm;
   cout << "Ban co chac chan muon xoa phien " << startTime << "? (y/n): ";
   cin >> confirm;

if (confirm == 'y' || confirm == 'Y') {
       if (CSVHelper::removeAttendanceSession(classId, startTime)) {
           cout << "Da xoa thanh cong.\n";
       }
       else {
           cout << "Xoa that bai.\n";
       }
   }
   else {
       cout << "Da huy thao tac.\n";
   }
}
  

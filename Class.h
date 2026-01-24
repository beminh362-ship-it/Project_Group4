#ifndef CLASS_H
#define CLASS_H
#include <string>
using namespace std;


/*
* Class
* ----------------
* Lớp biểu diễn THỰC THỂ lớp học trong hệ thống
* Dùng cho các Use-case của Giảng viên:
* - View class list
* - Search class
* - Filter class by date
*/
class Class {
private:
   string maLop;        // Mã lớp
   string tenMon;       // Tên môn học
   string ngayHoc;      // Ngày học (DD/MM/YYYY)
   string gioBatDau;    // Giờ bắt đầu
   string gioKetThuc;   // Giờ kết thúc
   string emailGV;      // Giảng viên phụ trách


public:
   Class();
   Class(string ma, string ten, string ngay,
       string bd, string kt, string email);


   // Getter
   string getMaLop() const;
   string getTenMon() const;
   string getNgayHoc() const;
   string getEmailGV() const;
};

#endif

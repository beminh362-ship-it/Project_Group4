#include "Class.h"


Class::Class() {}


Class::Class(string ma, string ten, string ngay,
   string bd, string kt, string email) {
   maLop = ma;
   tenMon = ten;
   ngayHoc = ngay;
   gioBatDau = bd;
   gioKetThuc = kt;
   emailGV = email;
}


string Class::getMaLop() const {
   return maLop;
}




string Class::getTenMon() const {
   return tenMon;
}




string Class::getNgayHoc() const {
   return ngayHoc;
}


string Class::getEmailGV() const {
    return emailGV;
} 
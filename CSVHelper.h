#ifndef CSVHELPER_H
#define CSVHELPER_H

#include <string>
#include <vector>
#include <fstream>
#include <sstream>

using namespace std;

/*
 * CSVHelper
 * Lớp hỗ trợ đọc, ghi và tìm kiếm dữ liệu trong file CSV
 * Dùng cho hệ thống điểm danh sinh viên
 *
 * Quy ước:
 * - File CSV dùng dấu ';' để phân cách
 * - Dữ liệu gốc chỉ đọc
 * - File phát sinh mới được ghi / cập nhật
 */
class CSVHelper {
public:
    // ===== 1. ĐỌC & GHI FILE =====

    // Đọc file CSV, trả về dữ liệu dạng bảng (2 chiều)
    vector<vector<string>> docFile(string tenFile);

    // Ghi dữ liệu vào file CSV
    void ghiFile(string tenFile, vector<vector<string>> duLieu);

    // ===== 2. CẬP NHẬT DÒNG =====

    // Cập nhật 1 dòng theo vị trí (index)
    void capNhatDong(string tenFile, int viTri, vector<string> dongMoi);

    // ===== 3. TÌM KIẾM =====

    // Tìm tất cả dòng có giá trị khớp ở 1 cột
    vector<vector<string>> timKiem(string tenFile, int cotCanTim, string giaTri);

    // Tìm vị trí (index) dòng đầu tiên khớp
    int timViTri(string tenFile, int cotCanTim, string giaTri);

private:
    // Tách 1 dòng CSV thành các cột
    vector<string> tachDong(string dong);

    // Ghép các cột thành 1 dòng CSV
    string ghepDong(vector<string> cot);
};

#endif

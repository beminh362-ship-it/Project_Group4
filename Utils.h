
#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <ctime>

using namespace std;

class Utils {
public:
    // Kiểm tra email có đúng định dạng @ut.edu.vn
    static bool isValidInstitutionEmail(const string& email);
    static bool isValidUniversityEmail(const string& email); // Alias for main.cpp

    // So sánh chuỗi không phân biệt hoa thường
    static bool caseInsensitiveContains(string source, string target);

    // Chuyển chuỗi "DD/MM/YYYY HH:MM" sang time_t
    static time_t parseDateTime(const string& datetime);

    // Kiểm tra thời gian session đã hết hạn chưa
    static bool isExpired(time_t endTime);

    // Tính thời gian còn lại (phút)
    static int calculateRemainingTime(const string& endTime);
};

#endif

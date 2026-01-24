#include "Utils.h"
#include <algorithm>
#include <sstream>
#include <ctime>
#include <iomanip>
#include <iostream>

using namespace std;

// ================= EMAIL VALIDATION =================
bool Utils::isValidInstitutionEmail(const string& email) {
    string suffix = "@ut.edu.vn";

    if (email.length() < suffix.length())
        return false;

    return email.compare(
        email.length() - suffix.length(),
        suffix.length(),
        suffix
    ) == 0;
}

// Alias for main.cpp compatibility
bool Utils::isValidUniversityEmail(const string& email) {
    return isValidInstitutionEmail(email);
}

// ================= STRING HELPER =================
bool Utils::caseInsensitiveContains(string source, string target) {
    transform(source.begin(), source.end(), source.begin(), ::tolower);
    transform(target.begin(), target.end(), target.begin(), ::tolower);

    return source.find(target) != string::npos;
}

// ================= TIME HELPER (UPDATED) =================
time_t Utils::parseDateTime(const string& datetime) {
    if (datetime.empty()) return -1;

    tm t = {};
    int part1, part2, part3, H, M, S = 0;
    char sep1, sep2, sep3, sep4; // Ký tự phân cách (-, /, :, space)

    stringstream ss(datetime);

    // Thu doc 3 thanh phan dau tien (Ngay/Thang/Nam hoac Nam/Thang/Ngay)
    // va Gio:Phut
    // Format chung: A-B-C H:M[:S]
    if (ss >> part1 >> sep1 >> part2 >> sep2 >> part3 >> H >> sep3 >> M) {

        // Doc them giay (neu co)
        if (ss >> sep4 >> S) {}

        // LOGIC PHAN BIET FORMAT:

        // Truong hop 1: YYYY-MM-DD (Format luu trong file CSV)
        // Neu so dau tien > 31, chac chan la Nam
        if (part1 > 31) {
            t.tm_year = part1 - 1900;
            t.tm_mon = part2 - 1;
            t.tm_mday = part3;
        }
        // Truong hop 2: DD/MM/YYYY (Format nguoi dung nhap)
        // Neu so thu 3 > 31, chac chan so cuoi la Nam
        else {
            t.tm_mday = part1;
            t.tm_mon = part2 - 1;
            t.tm_year = part3 - 1900;
        }

        t.tm_hour = H;
        t.tm_min = M;
        t.tm_sec = S;
        t.tm_isdst = -1; // De he thong tu xac dinh gio mua he

        return mktime(&t);
    }

    return -1; // Loi parse
}

bool Utils::isExpired(time_t endTime) {
    // So sanh thoi gian hien tai voi endTime
    return difftime(time(nullptr), endTime) > 0;
}

int Utils::calculateRemainingTime(const string& endTime) {
    time_t endTimeT = parseDateTime(endTime);
    if (endTimeT == -1) return 0;

    time_t now = time(nullptr);
    double diff = difftime(endTimeT, now);

    // Chuyển giây sang phút
    if (diff < 0) return 0;
    return static_cast<int>(diff / 60);
}
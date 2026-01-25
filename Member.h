#ifndef MEMBER_H
#define MEMBER_H


#include <string>
using namespace std;


/*
 * Member:
 * - Là người đã tồn tại trong hệ thống (có trong CSV)
 * - Chưa xác định vai trò cho tới khi đăng nhập
 * - Là lớp cha của Student và Lecturer
 */
class Member {
protected:
    string email;     // institutional email
    string id;        // studentId hoặc CCCD


public:
    Member();
    Member(string email, string id);
    virtual ~Member();


    string getEmail() const;
    string getId() const;


    // Đa hình: sau login mới biết role
    virtual string getRole() const = 0;
};


#endif




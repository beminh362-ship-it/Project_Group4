#include "Member.h"

//Student và Lecturer sẽ kế thừa Member
// Constructor mặc định
Member::Member() {
    email = "";
    id = "";
}

// Constructor có tham số
Member::Member(string email, string id) {
    this->email = email;
    this->id = id;
}

// Destructor ảo để delete đúng đối tượng con
Member::~Member() {}


// Getter
string Member::getEmail() const {
    return email;
}

string Member::getId() const {
    return id;
}




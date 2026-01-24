#include "CSVHelper.h"

// ==================== 1. ĐỌC FILE CSV ====================
vector<vector<string>> CSVHelper::docFile(string tenFile) {
    vector<vector<string>> duLieu;
    ifstream file(tenFile);

    if (!file.is_open()) {
        return duLieu;
    }

    string dong;
    while (getline(file, dong)) {
        vector<string> cacCot = tachDong(dong);
        duLieu.push_back(cacCot);
    }

    file.close();
    return duLieu;
}

// ==================== 2. GHI FILE CSV ====================
void CSVHelper::ghiFile(string tenFile, vector<vector<string>> duLieu) {
    ofstream file(tenFile);

    if (!file.is_open()) {
        return;
    }

    for (int i = 0; i < duLieu.size(); i++) {
        file << ghepDong(duLieu[i]);
        if (i != duLieu.size() - 1) {
            file << endl;
        }
    }

    file.close();
}

// ==================== 3. CAP NHAP DONG ====================
void CSVHelper::capNhatDong(string tenFile, int viTri, vector<string> dongMoi) {
    vector<vector<string>> duLieu = docFile(tenFile);

    if (viTri < 0 || viTri >= duLieu.size()) {
        return;
    }

    duLieu[viTri] = dongMoi;
    ghiFile(tenFile, duLieu);
}

// ==================== 4. TIM KIEM ====================
vector<vector<string>> CSVHelper::timKiem(string tenFile, int cotCanTim, string giaTri) {
    vector<vector<string>> ketQua;
    vector<vector<string>> duLieu = docFile(tenFile);

    for (int i = 0; i < duLieu.size(); i++) {
        if (cotCanTim < duLieu[i].size()) {
            if (duLieu[i][cotCanTim] == giaTri) {
                ketQua.push_back(duLieu[i]);
            }
        }
    }

    return ketQua;
}

int CSVHelper::timViTri(string tenFile, int cotCanTim, string giaTri) {
    vector<vector<string>> duLieu = docFile(tenFile);

    for (int i = 0; i < duLieu.size(); i++) {
        if (cotCanTim < duLieu[i].size()) {
            if (duLieu[i][cotCanTim] == giaTri) {
                return i;
            }
        }
    }

    return -1;
}

// ==================== HAM PHU TRO ====================
vector<string> CSVHelper::tachDong(string dong) {
    vector<string> ketQua;
    string tam;
    stringstream ss(dong);

    while (getline(ss, tam, ';')) {
        ketQua.push_back(tam);
    }

    return ketQua;
}

string CSVHelper::ghepDong(vector<string> cot) {
    string dong = "";

    for (int i = 0; i < cot.size(); i++) {
        dong += cot[i];
        if (i != cot.size() - 1) {
            dong += ";";
        }
    }

    return dong;
}

#include <iostream>
#include <string>
#include <cstdlib>
#include <ctime>
#include <limits>
using namespace std;

const int SO_DO_KHO = 3;
const int SO_KY_NANG = 3;
const int SO_TALENT = 3;
const int SO_TANG = 3;
const int SO_STAGE_MOI_TANG = 10;
const int TONG_STAGE = SO_TANG * SO_STAGE_MOI_TANG;
const int MAX_LEVEL = 1 + TONG_STAGE;
const int HE_SO_BOSS_HP = 300;
const int HE_SO_BOSS_ATK_DEF = 150;

const int BINH_MAU_BAN_DAU = 3;
const int MAX_BINH_MAU = 5;
const int HOI_MAU = 40;
const int GIAM_SUY_YEU = 30;

const int TI_LE_ROI_BINH_MAU = 10;
const int TI_LE_ROI_SUY_YEU = 2;
const int GIA_BINH_MAU = 20;
const int GIA_SUY_YEU = 100;

const int VANG_CO_BAN = 5;
const int HE_SO_VANG_BOSS = 3;
const int TI_LE_JACKPOT = 5;
const int HE_SO_JACKPOT = 5;

struct DoKho {
    string ten;
    int heSoQuai;
    int heSoVang;
    int maxSuyYeu;
};

struct ChiSo {
    int hp;
    int atk;
    int def;
    int critRate;
    int critDmg;
    int evasion;
};

struct KyNang {
    string ten;
    int satThuongGoc;
    int heSoAtk;
    int hoiChieu;
    int levelMo;
};

struct Talent {
    string ten;
    ChiSo thuong;
};

struct NhanVat {
    string ten;
    int hp;
    ChiSo cs;
    int level;
    int binhMau;
    int binhSuyYeu;
    int vang;
    int diemTalent;
    int hoiChieuConLai[SO_KY_NANG];
};

struct QuaiVat {
    string ten;
    int hp;
    int hpToiDa;
    int atk;
    int def;
};

struct KetQuaTran {
    int soLuot;
    int soKyNang;
    int soBinhMau;
    int soSuyYeu;
};

const DoKho DS_DO_KHO[SO_DO_KHO] = {
    {"Easy",      80, 100, 3},
    {"Normal",   100, 100, 3},
    {"Hardcore", 130,  50, 1}
};

const string TEN_QUAI[SO_TANG] = {"Slime", "Skeleton", "Demon"};

const ChiSo THUONG_LEN_CAP = {10, 2, 1, 0, 0, 0};

const Talent DS_TALENT[SO_TALENT] = {
    {"HP +15",  {15, 0, 0, 0, 0, 0}},
    {"ATK +3",  { 0, 3, 0, 0, 0, 0}},
    {"DEF +2",  { 0, 0, 2, 0, 0, 0}}
};

const ChiSo CHI_SO_BAN_DAU = {95, 21, 7, 20, 300, 20};

const KyNang DS_KY_NANG[SO_KY_NANG] = {
    {"Double Shot",    0, 170, 2, 1},
    {"Arrow Rain",    10, 150, 3, 3},
    {"Piercing Shot", 25, 230, 5, 5}
};

// Doi stage (1-30) thanh chuoi "tang-stage", vi du 13 -> "2-3"
string tenStage(int stage) {
    return to_string((stage - 1) / SO_STAGE_MOI_TANG + 1) + "-"
         + to_string((stage - 1) % SO_STAGE_MOI_TANG + 1);
}

// In menu chinh kem do kho va tien do hien tai
void hienThiMenu(const DoKho &doKho, int stage) {
    cout << "\n===== DAU TRUONG MINI RPG =====\n";
    cout << "Do kho: " << doKho.ten;
    if (stage <= TONG_STAGE) cout << " | Stage " << tenStage(stage);
    cout << "\n";
    cout << "1. Tao nhan vat\n";
    cout << "2. Chon do kho\n";
    cout << "3. Xem trang thai\n";
    cout << "4. Talent\n";
    cout << "5. Vao dau truong\n";
    cout << "6. Huong dan\n";
    cout << "7. Cua hang\n";
    cout << "0. Thoat\n";
}

// Nhap so nguyen trong [nhoNhat, lonNhat], nhap sai thi bat nhap lai
int nhapSoNguyen(string loiNhac, int nhoNhat, int lonNhat) {
    int so;
    while (true) {
        cout << loiNhac;
        if (cin >> so && so >= nhoNhat && so <= lonNhat) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return so;
        }
        cout << "Loi: hay nhap so nguyen tu " << nhoNhat << " den " << lonNhat << ".\n";
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
    }
}

// Nhap chuoi khong rong, do dai khong qua doDaiToiDa
string nhapTen(string loiNhac, int doDaiToiDa) {
    string ten;
    while (true) {
        cout << loiNhac;
        getline(cin, ten);
        if (!ten.empty() && (int)ten.size() <= doDaiToiDa) return ten;
        cout << "Loi: ten khong duoc rong va toi da " << doDaiToiDa << " ky tu.\n";
    }
}

// Cong chi so them vao cs
void congChiSo(ChiSo &cs, const ChiSo &them) {
    cs.hp += them.hp;
    cs.atk += them.atk;
    cs.def += them.def;
    cs.critRate += them.critRate;
    cs.critDmg += them.critDmg;
    cs.evasion += them.evasion;
}

// Chon do kho, chi cho doi khi chua danh stage nao
void chonDoKho(DoKho &doKho, int stage) {
    if (stage > 1) {
        cout << "Khong the doi do kho khi da bat dau leo thap!\n";
        return;
    }
    cout << "\n--- CHON DO KHO ---\n";
    for (int i = 0; i < SO_DO_KHO; i++) {
        cout << i + 1 << ". " << DS_DO_KHO[i].ten
             << " | Chi so quai: " << DS_DO_KHO[i].heSoQuai << "%"
             << " | Gold: " << DS_DO_KHO[i].heSoVang << "%"
             << " | Weaken potion toi da: " << DS_DO_KHO[i].maxSuyYeu << "\n";
    }
    int chon = nhapSoNguyen("Chon do kho (1-3): ", 1, SO_DO_KHO);
    doKho = DS_DO_KHO[chon - 1];
    cout << "Da chon do kho: " << doKho.ten << "\n";
}

// Tao nhan vat Ranger, nguoi choi chi nhap ten
void taoNhanVat(NhanVat &nv, bool &daTao) {
    cout << "\n--- TAO NHAN VAT ---\n";
    nv.ten = nhapTen("Nhap ten nhan vat: ", 20);
    nv.cs = CHI_SO_BAN_DAU;
    nv.hp = nv.cs.hp;
    nv.level = 1;
    nv.binhMau = BINH_MAU_BAN_DAU;
    nv.binhSuyYeu = 0;
    nv.vang = 0;
    nv.diemTalent = 0;
    for (int i = 0; i < SO_KY_NANG; i++) nv.hoiChieuConLai[i] = 0;
    daTao = true;
    cout << "Tao thanh cong Ranger " << nv.ten << "!\n";
}

// Sinh quai theo stage va do kho; stage cuoi moi tang la boss King manh hon quai stage truoc
QuaiVat taoQuai(int stage, const DoKho &doKho) {
    QuaiVat quai;
    int tang = (stage - 1) / SO_STAGE_MOI_TANG;
    bool laBoss = (stage % SO_STAGE_MOI_TANG == 0);
    int s = stage;
    int heSoHp = 100;
    int heSoAtkDef = 100;
    if (laBoss) {
        s = stage - 1;
        heSoHp = HE_SO_BOSS_HP;
        heSoAtkDef = HE_SO_BOSS_ATK_DEF;
    }
    quai.hpToiDa = (40 + 12 * s) * heSoHp * doKho.heSoQuai / 10000;
    quai.atk = (10 + 2 * s) * heSoAtkDef * doKho.heSoQuai / 10000;
    quai.def = (2 + s) * heSoAtkDef * doKho.heSoQuai / 10000;
    quai.hp = quai.hpToiDa;
    if (laBoss) quai.ten = TEN_QUAI[tang] + " King";
    else quai.ten = TEN_QUAI[tang];
    return quai;
}

// In trang thai nhan vat
void hienThiTrangThai(const NhanVat &nv, const DoKho &doKho) {
    cout << "\n--- TRANG THAI ---\n";
    cout << "Ten           : " << nv.ten << " (Ranger)\n";
    cout << "Level         : " << nv.level << "/" << MAX_LEVEL << "\n";
    cout << "HP            : " << nv.hp << "/" << nv.cs.hp << "\n";
    cout << "ATK           : " << nv.cs.atk << "\n";
    cout << "DEF           : " << nv.cs.def << "\n";
    cout << "Crit Rate     : " << nv.cs.critRate << "%\n";
    cout << "Crit Dmg      : " << nv.cs.critDmg << "%\n";
    cout << "Evasion       : " << nv.cs.evasion << "%\n";
    cout << "Gold          : " << nv.vang << "\n";
    cout << "Health potion : " << nv.binhMau << "/" << MAX_BINH_MAU << "\n";
    cout << "Weaken potion : " << nv.binhSuyYeu << "/" << doKho.maxSuyYeu << "\n";
    cout << "Diem talent   : " << nv.diemTalent << "\n";
    cout << "Ky nang:\n";
    for (int i = 0; i < SO_KY_NANG; i++) {
        cout << "  " << i + 1 << ". " << DS_KY_NANG[i].ten << " - ";
        if (nv.level < DS_KY_NANG[i].levelMo) cout << "Mo o level " << DS_KY_NANG[i].levelMo << "\n";
        else if (nv.hoiChieuConLai[i] > 0) cout << "Hoi chieu " << nv.hoiChieuConLai[i] << " luot\n";
        else cout << "San sang\n";
    }
}

// In danh sach talent va so diem con lai
void hienThiTalent(const NhanVat &nv) {
    cout << "\n--- TALENT ---\n";
    cout << "Diem talent con lai: " << nv.diemTalent << "\n";
    cout << "Chi so hien tai: HP " << nv.cs.hp << " | ATK " << nv.cs.atk
         << " | DEF " << nv.cs.def << "\n";
    for (int i = 0; i < SO_TALENT; i++) {
        cout << i + 1 << ". " << DS_TALENT[i].ten << "\n";
    }
    cout << "0. Quay lai\n";
}

// Dung diem talent de tang HP, ATK hoac DEF, lap den khi het diem hoac chon 0
void nangTalent(NhanVat &nv) {
    while (true) {
        hienThiTalent(nv);
        if (nv.diemTalent == 0) {
            cout << "Ban khong con diem talent. Len cap de nhan them diem!\n";
            return;
        }
        int chon = nhapSoNguyen("Chon talent (0-3): ", 0, SO_TALENT);
        if (chon == 0) return;
        const Talent &t = DS_TALENT[chon - 1];
        congChiSo(nv.cs, t.thuong);
        nv.hp += t.thuong.hp;
        nv.diemTalent--;
        cout << "Da cong " << t.ten << "!\n";
    }
}

// Cua hang: mua health potion va weaken potion bang gold
void cuaHang(NhanVat &nv, const DoKho &doKho) {
    while (true) {
        cout << "\n--- CUA HANG ---\n";
        cout << "Gold: " << nv.vang << "\n";
        cout << "1. Health potion (hoi " << HOI_MAU << " HP) - " << GIA_BINH_MAU << " gold"
             << " | Dang co " << nv.binhMau << "/" << MAX_BINH_MAU << "\n";
        cout << "2. Weaken potion (quai -" << GIAM_SUY_YEU << "% ATK/DEF) - " << GIA_SUY_YEU << " gold"
             << " | Dang co " << nv.binhSuyYeu << "/" << doKho.maxSuyYeu << "\n";
        cout << "0. Quay lai\n";
        int chon = nhapSoNguyen("Chon mon hang (0-2): ", 0, 2);
        if (chon == 0) return;
        if (chon == 1) {
            if (nv.binhMau >= MAX_BINH_MAU) cout << "Tui health potion da day!\n";
            else if (nv.vang < GIA_BINH_MAU) cout << "Khong du gold!\n";
            else {
                nv.vang -= GIA_BINH_MAU;
                nv.binhMau++;
                cout << "Da mua 1 health potion.\n";
            }
        } else {
            if (nv.binhSuyYeu >= doKho.maxSuyYeu) cout << "Tui weaken potion da day!\n";
            else if (nv.vang < GIA_SUY_YEU) cout << "Khong du gold!\n";
            else {
                nv.vang -= GIA_SUY_YEU;
                nv.binhSuyYeu++;
                cout << "Da mua 1 weaken potion.\n";
            }
        }
    }
}

// In luat choi
void hienThiHuongDan() {
    cout << "\n--- HUONG DAN ---\n";
    cout << "* Muc tieu: leo thap " << SO_TANG << " tang, moi tang " << SO_STAGE_MOI_TANG
         << " stage, ha het " << TONG_STAGE << " quai de chien thang.\n";
    cout << "* Stage cuoi moi tang la Boss King: HP x3, ATK/DEF x1.5.\n";
    cout << "* Do kho: Easy quai 80%, Normal 100%, Hardcore 130% (gold chi 50%, weaken potion toi da 1).\n";
    cout << "* Moi luot chon: Tan cong / Ky nang / Phong thu (giam 1/2 sat thuong) /\n";
    cout << "  Health potion (+" << HOI_MAU << " HP) / Weaken potion (quai -" << GIAM_SUY_YEU
         << "% ATK va DEF den het tran, 1 lan moi tran).\n";
    cout << "* Sat thuong = ATK - DEF doi thu (+/- 3), toi thieu 1.\n";
    cout << "* Ranger co " << CHI_SO_BAN_DAU.critRate << "% chi mang (x"
         << CHI_SO_BAN_DAU.critDmg / 100 << " sat thuong) va "
         << CHI_SO_BAN_DAU.evasion << "% ne don.\n";
    cout << "* Ky nang mo o level ";
    for (int i = 0; i < SO_KY_NANG; i++) {
        cout << DS_KY_NANG[i].levelMo << (i < SO_KY_NANG - 1 ? ", " : "");
    }
    cout << "; dung xong phai cho hoi chieu.\n";
    cout << "* Ha 1 quai: len 1 cap (toi da " << MAX_LEVEL << "), +1 diem talent, hoi day HP cho stage sau.\n";
    cout << "* Quai roi gold (tang theo stage, boss x" << HE_SO_VANG_BOSS << "), "
         << TI_LE_JACKPOT << "% no JACKPOT x" << HE_SO_JACKPOT << " gold.\n";
    cout << "* Quai co " << TI_LE_ROI_BINH_MAU << "% roi health potion, "
         << TI_LE_ROI_SUY_YEU << "% roi weaken potion.\n";
    cout << "* Cua hang (menu 7): health potion " << GIA_BINH_MAU << " gold, weaken potion "
         << GIA_SUY_YEU << " gold.\n";
    cout << "* Dung diem talent o menu 4 de tang HP, ATK hoac DEF.\n";
    cout << "* Chi co 1 mang: HP ve 0 la thua!\n";
}

// Sat thuong nguoi choi gay cho quai, kyNang = -1 la don danh thuong
int satThuongNguoiChoi(const NhanVat &nv, const QuaiVat &quai, int kyNang, bool &chiMang) {
    int satThuong;
    if (kyNang == -1) {
        satThuong = nv.cs.atk;
    } else {
        const KyNang &kn = DS_KY_NANG[kyNang];
        satThuong = kn.satThuongGoc + nv.cs.atk * kn.heSoAtk / 100;
    }
    satThuong = satThuong - quai.def + (rand() % 7 - 3);
    if (satThuong < 1) satThuong = 1;
    chiMang = (rand() % 100 < nv.cs.critRate);
    if (chiMang) satThuong = satThuong * nv.cs.critDmg / 100;
    return satThuong;
}

// Sat thuong quai gay cho nguoi choi, nguoi choi co the ne
int satThuongQuai(const QuaiVat &quai, const NhanVat &nv, bool &ne) {
    ne = (rand() % 100 < nv.cs.evasion);
    if (ne) return 0;
    int satThuong = quai.atk - nv.cs.def + (rand() % 7 - 3);
    if (satThuong < 1) satThuong = 1;
    return satThuong;
}

// Chon va dung 1 ky nang da mo theo level, tra ve false neu khong dung duoc
bool dungKyNang(NhanVat &nv, QuaiVat &quai) {
    cout << "Ky nang:\n";
    for (int i = 0; i < SO_KY_NANG; i++) {
        const KyNang &kn = DS_KY_NANG[i];
        cout << "  " << i + 1 << ". " << kn.ten << " (" << kn.heSoAtk << "% ATK + " << kn.satThuongGoc << ") - ";
        if (nv.level < kn.levelMo) cout << "Mo o level " << kn.levelMo << "\n";
        else if (nv.hoiChieuConLai[i] > 0) cout << "Hoi chieu " << nv.hoiChieuConLai[i] << " luot\n";
        else cout << "San sang\n";
    }
    cout << "  0. Quay lai\n";
    int chon = nhapSoNguyen("Chon ky nang (0-3): ", 0, SO_KY_NANG);
    if (chon == 0) return false;
    int i = chon - 1;
    const KyNang &kn = DS_KY_NANG[i];
    if (nv.level < kn.levelMo) {
        cout << "Chua du level! " << kn.ten << " mo o level " << kn.levelMo << ".\n";
        return false;
    }
    if (nv.hoiChieuConLai[i] > 0) {
        cout << kn.ten << " dang hoi chieu, con " << nv.hoiChieuConLai[i] << " luot.\n";
        return false;
    }
    bool chiMang;
    int satThuong = satThuongNguoiChoi(nv, quai, i, chiMang);
    quai.hp -= satThuong;
    nv.hoiChieuConLai[i] = kn.hoiChieu;
    cout << nv.ten << " dung " << kn.ten << " gay " << satThuong << " sat thuong";
    if (chiMang) cout << " (CHI MANG!)";
    cout << ".\n";
    return true;
}

// Giam hoi chieu cac ky nang di 1 luot
void giamHoiChieu(NhanVat &nv) {
    for (int i = 0; i < SO_KY_NANG; i++) {
        if (nv.hoiChieuConLai[i] > 0) nv.hoiChieuConLai[i]--;
    }
}

// Dung 1 health potion, tra ve false neu khong dung duoc
bool dungBinhMau(NhanVat &nv) {
    if (nv.binhMau == 0) {
        cout << "Ban da het health potion!\n";
        return false;
    }
    if (nv.hp == nv.cs.hp) {
        cout << "HP dang day, khong can dung health potion.\n";
        return false;
    }
    int truoc = nv.hp;
    nv.hp = min(nv.hp + HOI_MAU, nv.cs.hp);
    nv.binhMau--;
    cout << nv.ten << " uong health potion, hoi " << nv.hp - truoc << " HP (con " << nv.binhMau << " binh).\n";
    return true;
}

// Dung 1 weaken potion len quai, moi tran toi da 1 lan, tra ve false neu khong dung duoc
bool dungBinhSuyYeu(NhanVat &nv, QuaiVat &quai, bool &daSuyYeu) {
    if (nv.binhSuyYeu == 0) {
        cout << "Ban khong co weaken potion!\n";
        return false;
    }
    if (daSuyYeu) {
        cout << quai.ten << " da bi suy yeu roi, moi tran chi dung 1 lan.\n";
        return false;
    }
    quai.atk -= quai.atk * GIAM_SUY_YEU / 100;
    quai.def -= quai.def * GIAM_SUY_YEU / 100;
    nv.binhSuyYeu--;
    daSuyYeu = true;
    cout << nv.ten << " nem weaken potion! " << quai.ten << " con ATK " << quai.atk
         << ", DEF " << quai.def << ".\n";
    return true;
}

// Tang cap (toi da MAX_LEVEL), cong diem talent, bao ky nang moi mo; tra ve true neu len cap
bool lenCap(NhanVat &nv) {
    if (nv.level >= MAX_LEVEL) return false;
    nv.level++;
    congChiSo(nv.cs, THUONG_LEN_CAP);
    nv.diemTalent++;
    cout << "*** LEN CAP " << nv.level << "! HP toi da " << nv.cs.hp
         << " | ATK " << nv.cs.atk << " | DEF " << nv.cs.def << " | +1 diem talent ***\n";
    for (int i = 0; i < SO_KY_NANG; i++) {
        if (DS_KY_NANG[i].levelMo == nv.level) {
            cout << "*** Mo ky nang moi: " << DS_KY_NANG[i].ten << "! ***\n";
        }
    }
    return true;
}

// In HP hai ben va cac lua chon o dau moi luot
void hienThiLuot(const NhanVat &nv, const QuaiVat &quai, int luot) {
    cout << "\n--- Luot " << luot << " ---\n";
    cout << nv.ten << ": HP " << nv.hp << "/" << nv.cs.hp
         << " | Health potion " << nv.binhMau << " | Weaken potion " << nv.binhSuyYeu << "\n";
    cout << quai.ten << ": HP " << quai.hp << "/" << quai.hpToiDa
         << " | ATK " << quai.atk << " | DEF " << quai.def << "\n";
    cout << "1. Tan cong  2. Ky nang  3. Phong thu  4. Health potion  5. Weaken potion\n";
}

// Dien bien 1 tran theo luot, ghi thong ke vao kq, tra ve true neu nguoi choi thang
bool chienDau(NhanVat &nv, QuaiVat quai, KetQuaTran &kq) {
    kq.soLuot = 0;
    kq.soKyNang = 0;
    kq.soBinhMau = 0;
    kq.soSuyYeu = 0;
    for (int i = 0; i < SO_KY_NANG; i++) nv.hoiChieuConLai[i] = 0;
    bool daSuyYeu = false;
    int luot = 1;
    while (true) {
        hienThiLuot(nv, quai, luot);
        bool dangPhongThu = false;
        int chon = nhapSoNguyen("Chon hanh dong (1-5): ", 1, 5);
        if (chon == 1) {
            bool chiMang;
            int satThuong = satThuongNguoiChoi(nv, quai, -1, chiMang);
            quai.hp -= satThuong;
            cout << nv.ten << " ban ten gay " << satThuong << " sat thuong";
            if (chiMang) cout << " (CHI MANG!)";
            cout << ".\n";
        } else if (chon == 2) {
            if (!dungKyNang(nv, quai)) continue;
            kq.soKyNang++;
        } else if (chon == 3) {
            dangPhongThu = true;
            cout << nv.ten << " vao the phong thu.\n";
        } else if (chon == 4) {
            if (!dungBinhMau(nv)) continue;
            kq.soBinhMau++;
        } else {
            if (!dungBinhSuyYeu(nv, quai, daSuyYeu)) continue;
            kq.soSuyYeu++;
        }
        kq.soLuot = luot;

        if (quai.hp <= 0) {
            cout << quai.ten << " da bi ha guc!\n";
            return true;
        }

        bool ne;
        int satThuong = satThuongQuai(quai, nv, ne);
        if (ne) {
            cout << nv.ten << " ne duoc don tan cong cua " << quai.ten << "!\n";
        } else {
            if (dangPhongThu) satThuong = max(1, satThuong / 2);
            nv.hp -= satThuong;
            cout << quai.ten << " tan cong gay " << satThuong << " sat thuong";
            if (dangPhongThu) cout << " (da giam nho phong thu)";
            cout << ".\n";
        }
        if (nv.hp <= 0) {
            nv.hp = 0;
            return false;
        }
        giamHoiChieu(nv);
        luot++;
    }
}

// Tinh gold co ban x cua 1 stage: tang theo stage, boss x3, nhan he so gold cua do kho
int tinhVang(int stage, const DoKho &doKho) {
    int vang = VANG_CO_BAN + stage;
    if (stage % SO_STAGE_MOI_TANG == 0) vang *= HE_SO_VANG_BOSS;
    return vang * doKho.heSoVang / 100;
}

// Nhan gold va vat pham roi sau khi ha quai, ghi mo ta vat pham vao chuoi roiDo
void nhanPhanThuong(NhanVat &nv, int stage, const DoKho &doKho, int &vang, bool &jackpot, string &roiDo) {
    vang = tinhVang(stage, doKho);
    jackpot = (rand() % 100 < TI_LE_JACKPOT);
    if (jackpot) vang *= HE_SO_JACKPOT;
    nv.vang += vang;

    roiDo = "";
    if (rand() % 100 < TI_LE_ROI_BINH_MAU) {
        if (nv.binhMau < MAX_BINH_MAU) {
            nv.binhMau++;
            roiDo += "Health potion x1 ";
        } else {
            roiDo += "Health potion x1 (tui day, bo lai) ";
        }
    }
    if (rand() % 100 < TI_LE_ROI_SUY_YEU) {
        if (nv.binhSuyYeu < doKho.maxSuyYeu) {
            nv.binhSuyYeu++;
            roiDo += "Weaken potion x1 ";
        } else {
            roiDo += "Weaken potion x1 (tui day, bo lai) ";
        }
    }
    if (roiDo == "") roiDo = "Khong co";
}

// In bang ket qua sau khi hoan thanh 1 stage
void hienThiKetQuaStage(int stage, const KetQuaTran &kq, int diemTalent, int vang, bool jackpot, string roiDo) {
    cout << "\n+========== KET QUA STAGE ==========+\n";
    cout << "| Ai chien thang        : " << tenStage(stage) << "\n";
    cout << "| So luot hoan thanh    : " << kq.soLuot << "\n";
    cout << "| So ky nang da dung    : " << kq.soKyNang << "\n";
    cout << "| Diem talent nhan duoc : " << diemTalent << "\n";
    cout << "| Health potion da dung : " << kq.soBinhMau << "\n";
    cout << "| Weaken potion da dung : " << kq.soSuyYeu << "\n";
    cout << "| Gold nhan duoc        : " << vang;
    if (jackpot) cout << "  *** JACKPOT! ***";
    cout << "\n";
    cout << "| Vat pham roi          : " << roiDo << "\n";
    cout << "+===================================+\n";
}

// Danh quai o stage hien tai, thang thi nhan thuong, len cap, hoi day HP va sang stage tiep
void vaoDauTruong(NhanVat &nv, bool daTao, int &stage, bool &daKetThuc, const DoKho &doKho) {
    if (!daTao) {
        cout << "Ban chua tao nhan vat!\n";
        return;
    }
    if (daKetThuc) {
        cout << "Hanh trinh da ket thuc. Hay tao nhan vat moi de choi lai!\n";
        return;
    }
    QuaiVat quai = taoQuai(stage, doKho);
    cout << "\n===== STAGE " << tenStage(stage) << " =====\n";
    if (stage % SO_STAGE_MOI_TANG == 0) cout << "!!! BOSS XUAT HIEN !!!\n";
    cout << quai.ten << " | HP " << quai.hpToiDa << " | ATK " << quai.atk << " | DEF " << quai.def << "\n";

    KetQuaTran kq;
    if (!chienDau(nv, quai, kq)) {
        cout << "\n" << nv.ten << " da guc nga o stage " << tenStage(stage) << ". GAME OVER!\n";
        daKetThuc = true;
        return;
    }

    cout << "\nChien thang!\n";
    int vang;
    bool jackpot;
    string roiDo;
    nhanPhanThuong(nv, stage, doKho, vang, jackpot, roiDo);
    int diemTalent = lenCap(nv) ? 1 : 0;
    nv.hp = nv.cs.hp;
    hienThiKetQuaStage(stage, kq, diemTalent, vang, jackpot, roiDo);
    cout << "HP da duoc hoi day (" << nv.hp << "/" << nv.cs.hp << "). Tong gold: " << nv.vang << "\n";

    stage++;
    if (stage > TONG_STAGE) {
        cout << "\n***** CHUC MUNG! " << nv.ten << " DA CHINH PHUC TOAN BO THAP ("
             << doKho.ten << ") *****\n";
        daKetThuc = true;
    } else if (nv.diemTalent > 0) {
        cout << "Ban co " << nv.diemTalent << " diem talent, vao menu 4 de cong.\n";
    }
}

// Dieu phoi menu
int main() {
    srand(time(0));
    NhanVat nguoiChoi;
    DoKho doKho = DS_DO_KHO[1];
    bool daTao = false;
    bool daKetThuc = false;
    int stage = 1;
    int luaChon;

    do {
        hienThiMenu(doKho, stage);
        luaChon = nhapSoNguyen("Chon chuc nang: ", 0, 7);
        switch (luaChon) {
            case 1:
                taoNhanVat(nguoiChoi, daTao);
                stage = 1;
                daKetThuc = false;
                break;
            case 2: chonDoKho(doKho, stage); break;
            case 3:
                if (daTao) hienThiTrangThai(nguoiChoi, doKho);
                else cout << "Ban chua tao nhan vat!\n";
                break;
            case 4:
                if (daTao) nangTalent(nguoiChoi);
                else cout << "Ban chua tao nhan vat!\n";
                break;
            case 5: vaoDauTruong(nguoiChoi, daTao, stage, daKetThuc, doKho); break;
            case 6: hienThiHuongDan(); break;
            case 7:
                if (daTao) cuaHang(nguoiChoi, doKho);
                else cout << "Ban chua tao nhan vat!\n";
                break;
            case 0: cout << "Tam biet!\n"; break;
        }
    } while (luaChon != 0);
    return 0;
}
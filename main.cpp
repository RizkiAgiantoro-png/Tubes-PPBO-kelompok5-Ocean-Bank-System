#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <limits>
using namespace std;

const int MAX = 100;

// ============================================================
// ANSI COLOR CODES (Windows 10+ & VSCode Terminal Compatible)
// ============================================================
#define RESET     "\033[0m"
#define BOLD      "\033[1m"
#define RED       "\033[91m"
#define GREEN     "\033[92m"
#define YELLOW    "\033[93m"
#define BLUE      "\033[94m"
#define MAGENTA   "\033[95m"
#define CYAN      "\033[96m"
#define WHITE     "\033[97m"
#define DIM       "\033[2m"

// ============================================================
// UTILITY: INPUT VALIDATION
// ============================================================
double inputAngka(const string& prompt) {
    double nilai;
    while (true) {
        cout << prompt;
        if (cin >> nilai && nilai >= 0) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return nilai;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "  [!] Input tidak valid. Masukkan angka positif.\n" << RESET;
    }
}

int inputMenu(const string& prompt, int min, int max) {
    int nilai;
    while (true) {
        cout << prompt;
        if (cin >> nilai && nilai >= min && nilai <= max) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return nilai;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "  [!] Pilihan tidak valid. Masukkan " << min << "-" << max << ".\n" << RESET;
    }
}

string inputTeks(const string& prompt) {
    string teks;
    cout << prompt;
    getline(cin, teks);
    while (teks.empty()) {
        cout << RED << "  [!] Input tidak boleh kosong.\n" << RESET;
        cout << prompt;
        getline(cin, teks);
    }
    return teks;
}

// ============================================================
// ABSTRACTION (Pure Virtual / Interface)
// ============================================================
class Rekening {
public:
    virtual void    tampilkanInfo()    const = 0;
    virtual string  getTipe()          const = 0;
    virtual bool    tarik(double jml)        = 0;
    virtual bool    setor(double jml)        = 0;   // <-- setor masuk abstraction
    virtual ~Rekening() {}
};

// ============================================================
// CLASS Nasabah : ENCAPSULATION + INHERITANCE + POLYMORPHISM
// ============================================================
class Nasabah : public Rekening {
protected:
    string nama, spesies, noRek;
    double saldo;

public:
    // CONSTRUCTOR
    Nasabah(const string& nama, const string& spesies,
            const string& noRek, double saldo)
        : nama(nama), spesies(spesies), noRek(noRek), saldo(saldo) {}

    // DESTRUCTOR
    virtual ~Nasabah() {}

    // ENCAPSULATION - Getters
    string getNama()    const { return nama; }
    string getNoRek()   const { return noRek; }
    string getSpesies() const { return spesies; }
    double getSaldo()   const { return saldo; }

    // SETOR (dari abstraction)
    bool setor(double jumlah) override {
        if (jumlah <= 0) return false;
        saldo += jumlah;
        return true;
    }

    // TARIK
    bool tarik(double jumlah) override {
        if (jumlah <= 0 || jumlah > saldo) return false;
        saldo -= jumlah;
        return true;
    }

    // TAMPILKAN INFO
    void tampilkanInfo() const override {
        cout << "\n";
        cout << CYAN  << "  +-----------------------------------------------+\n" << RESET;
        cout << CYAN  << "  |" << BOLD << YELLOW << "        INFORMASI NASABAH REGULER              " << CYAN << "|\n" << RESET;
        cout << CYAN  << "  +-----------------------------------------------+\n" << RESET;
        cout << CYAN  << "  |" << RESET << " Nama Nasabah : " << GREEN  << left << setw(30) << nama    << CYAN << "|\n" << RESET;
        cout << CYAN  << "  |" << RESET << " Spesies Laut : " << WHITE  << left << setw(30) << spesies << CYAN << "|\n" << RESET;
        cout << CYAN  << "  |" << RESET << " No. Rekening : " << YELLOW << left << setw(30) << noRek   << CYAN << "|\n" << RESET;
        cout << CYAN  << "  |" << RESET << " Tipe Akun    : " << BLUE   << left << setw(30) << getTipe()<< CYAN << "|\n" << RESET;
        cout << CYAN  << "  |" << RESET << " Saldo        : " << BOLD   << GREEN
             << left << setw(24) << fixed << setprecision(2) << saldo << " Koin" << RESET << CYAN << "|\n" << RESET;
        cout << CYAN  << "  +-----------------------------------------------+\n" << RESET;
    }

    string getTipe() const override { return "Reguler"; }

    // OPERATOR OVERLOADING
    bool operator>(const Nasabah& o) const { return saldo > o.saldo; }

    friend ostream& operator<<(ostream& os, const Nasabah& n) {
        os << "[" << n.getTipe() << "] " << n.nama << " | " << n.noRek;
        return os;
    }
};

// ============================================================
// CLASS NasabahVIP : INHERITANCE + POLYMORPHISM
// ============================================================
class NasabahVIP : public Nasabah {
    double limitKredit;
public:
    NasabahVIP(const string& nama, const string& spesies,
               const string& noRek, double saldo, double limit = 1000.0)
        : Nasabah(nama, spesies, noRek, saldo), limitKredit(limit) {}

    // POLYMORPHISM - VIP bisa tarik hingga limit kredit
    bool tarik(double jumlah) override {
        if (jumlah <= 0 || jumlah > (saldo + limitKredit)) return false;
        saldo -= jumlah;
        return true;
    }

    void tampilkanInfo() const override {
        cout << "\n";
        cout << MAGENTA << "  +-----------------------------------------------+\n" << RESET;
        cout << MAGENTA << "  |" << BOLD << YELLOW << "       INFORMASI NASABAH VIP [BINTANG]         " << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  +-----------------------------------------------+\n" << RESET;
        cout << MAGENTA << "  |" << RESET << " Nama Nasabah : " << GREEN   << left << setw(30) << nama       << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  |" << RESET << " Spesies Laut : " << WHITE   << left << setw(30) << spesies    << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  |" << RESET << " No. Rekening : " << YELLOW  << left << setw(30) << noRek      << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  |" << RESET << " Tipe Akun    : " << BOLD    << MAGENTA << left << setw(30) << "VIP [BINTANG]" << RESET << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  |" << RESET << " Saldo        : " << BOLD    << GREEN
             << left << setw(24) << fixed << setprecision(2) << saldo << " Koin" << RESET << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  |" << RESET << " Limit Kredit : " << CYAN
             << left << setw(24) << fixed << setprecision(2) << limitKredit << " Koin" << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  +-----------------------------------------------+\n" << RESET;
    }

    string getTipe()       const override { return "VIP"; }
    double getLimitKredit()const          { return limitKredit; }
};

// ============================================================
// FILE HANDLING
// ============================================================
Nasabah* daftar[MAX];
int jumlahNasabah = 0;

string ambilKolom(const string& baris, char delim, int index) {
    int count = 0, start = 0;
    for (int i = 0; i <= (int)baris.size(); i++) {
        if (i == (int)baris.size() || baris[i] == delim) {
            if (count == index)
                return baris.substr(start, i - start);
            count++;
            start = i + 1;
        }
    }
    return "";
}

int hitungKolom(const string& baris, char delim) {
    int count = 1;
    for (char c : baris) if (c == delim) count++;
    return count;
}

void simpanFile() {
    ofstream f("nasabah.txt");
    if (!f.is_open()) {
        cout << RED << "  [!] Gagal menyimpan file.\n" << RESET;
        return;
    }
    for (int i = 0; i < jumlahNasabah; i++) {
        Nasabah* n = daftar[i];
        f << n->getTipe() << "|" << n->getNama() << "|"
          << n->getSpesies() << "|" << n->getNoRek() << "|"
          << fixed << setprecision(2) << n->getSaldo();
        if (n->getTipe() == "VIP")
            f << "|" << fixed << setprecision(2)
              << dynamic_cast<NasabahVIP*>(n)->getLimitKredit();
        f << "\n";
    }
    f.close();
}

void muatFile() {
    ifstream f("nasabah.txt");
    if (!f.is_open()) return;
    string baris;
    while (getline(f, baris) && jumlahNasabah < MAX) {
        if (baris.empty()) continue;
        int    kol   = hitungKolom(baris, '|');
        string tipe  = ambilKolom(baris, '|', 0);
        string nama  = ambilKolom(baris, '|', 1);
        string spes  = ambilKolom(baris, '|', 2);
        string noRek = ambilKolom(baris, '|', 3);
        double saldo = stod(ambilKolom(baris, '|', 4));

        if (tipe == "VIP" && kol >= 6) {
            double limit = stod(ambilKolom(baris, '|', 5));
            daftar[jumlahNasabah++] = new NasabahVIP(nama, spes, noRek, saldo, limit);
        } else {
            daftar[jumlahNasabah++] = new Nasabah(nama, spes, noRek, saldo);
        }
    }
    f.close();
}

// ============================================================
// UTILITY TAMPILAN
// ============================================================
Nasabah* cariRekening(const string& noRek) {
    for (int i = 0; i < jumlahNasabah; i++)
        if (daftar[i]->getNoRek() == noRek) return daftar[i];
    return nullptr;
}

string generateNoRek() {
    return "OCN-" + to_string(100000 + rand() % 900000);
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void garis(const string& warna = CYAN) {
    cout << warna << "  ================================================\n" << RESET;
}

void header(const string& judul) {
    cout << "\n";
    garis(CYAN);
    int sp = max(0, (48 - (int)judul.size()) / 2);
    cout << CYAN << "  " << string(sp, ' ') << BOLD << YELLOW << judul << RESET << "\n";
    garis(CYAN);
}

void struk(const string& jenis, const Nasabah* n, double jumlah) {
    cout << "\n";
    cout << GREEN << "  +-----------------------------------------------+\n" << RESET;
    cout << GREEN << "  |" << BOLD << WHITE << "          TRANSAKSI BERHASIL                   " << GREEN << "|\n" << RESET;
    cout << GREEN << "  +-----------------------------------------------+\n" << RESET;
    cout << GREEN << "  |" << RESET << " Jenis          : " << CYAN   << left << setw(28) << jenis         << GREEN << "|\n" << RESET;
    cout << GREEN << "  |" << RESET << " Nama           : " << YELLOW << left << setw(28) << n->getNama()  << GREEN << "|\n" << RESET;
    cout << GREEN << "  |" << RESET << " No. Rekening   : " << YELLOW << left << setw(28) << n->getNoRek() << GREEN << "|\n" << RESET;
    cout << GREEN << "  |" << RESET << " Jumlah         : " << MAGENTA<< left << setw(22) << fixed << setprecision(2) << jumlah << " Koin" << GREEN << "|\n" << RESET;
    cout << GREEN << "  |" << RESET << " Saldo Akhir    : " << BOLD << GREEN << left << setw(22) << fixed << setprecision(2) << n->getSaldo() << " Koin" << RESET << GREEN << "|\n" << RESET;
    cout << GREEN << "  +-----------------------------------------------+\n" << RESET;
}

void tungguEnter() {
    cout << DIM << YELLOW << "\n  Tekan Enter untuk kembali ke menu..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============================================================
// FITUR-FITUR MENU
// ============================================================
void menuDaftarNasabah() {
    header("DAFTARKAN NASABAH BARU");
    if (jumlahNasabah >= MAX) {
        cout << RED << "  [!] Database penuh. Tidak bisa mendaftar lagi.\n" << RESET;
        return;
    }

    string nama    = inputTeks(CYAN "  Nama Nasabah       : " RESET);
    string spesies = inputTeks(CYAN "  Jenis Spesies Laut : " RESET);
    double saldo   = inputAngka(CYAN "  Saldo Awal (Koin)  : " RESET);

    char vip;
    cout << CYAN << "  Daftar sebagai VIP? (y/n): " << RESET;
    cin >> vip;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string noRek = generateNoRek();
    while (cariRekening(noRek)) noRek = generateNoRek();

    if (vip == 'y' || vip == 'Y')
        daftar[jumlahNasabah++] = new NasabahVIP(nama, spesies, noRek, saldo);
    else
        daftar[jumlahNasabah++] = new Nasabah(nama, spesies, noRek, saldo);

    simpanFile();
    cout << GREEN << "\n  [OK] Nasabah berhasil didaftarkan!\n" << RESET;
    cout << YELLOW << "  Nomor Rekening Anda: " << BOLD << noRek << RESET << "\n";
}

void menuCekSaldo() {
    header("CEK SALDO NASABAH");
    string nr = inputTeks(CYAN "  No. Rekening: " RESET);
    Nasabah* n = cariRekening(nr);
    if (n)
        cout << GREEN << "\n  Saldo " << YELLOW << n->getNama()
             << GREEN << " : " << BOLD << fixed << setprecision(2)
             << n->getSaldo() << " Koin" << RESET << "\n";
    else
        cout << RED << "\n  [!] Rekening tidak ditemukan.\n" << RESET;
}

void menuSetor() {
    header("SETOR SALDO");
    string nr = inputTeks(CYAN "  No. Rekening : " RESET);
    Nasabah* n = cariRekening(nr);
    if (!n) { cout << RED << "\n  [!] Rekening tidak ditemukan.\n" << RESET; return; }

    cout << GREEN << "  Saldo Saat Ini: " << BOLD << fixed << setprecision(2)
         << n->getSaldo() << " Koin\n" << RESET;
    double jml = inputAngka(CYAN "  Jumlah Setor  : " RESET);

    if (n->setor(jml)) {
        simpanFile();
        struk("SETOR SALDO", n, jml);
    } else {
        cout << RED << "\n  [GAGAL] Jumlah setor harus lebih dari 0.\n" << RESET;
    }
}

void menuTarik() {
    header("TARIK SALDO");
    string nr = inputTeks(CYAN "  No. Rekening : " RESET);
    Nasabah* n = cariRekening(nr);
    if (!n) { cout << RED << "\n  [!] Rekening tidak ditemukan.\n" << RESET; return; }

    cout << GREEN << "  Saldo Saat Ini: " << BOLD << fixed << setprecision(2)
         << n->getSaldo() << " Koin\n" << RESET;
    double jml = inputAngka(CYAN "  Jumlah Tarik  : " RESET);

    if (n->tarik(jml)) {
        simpanFile();
        struk("TARIK SALDO", n, jml);
    } else {
        cout << RED << "\n  [GAGAL] Saldo tidak mencukupi atau jumlah tidak valid.\n" << RESET;
    }
}

void menuLaporan() {
    header("LAPORAN KEUANGAN OCEAN BANK");
    double total = 0;
    int reguler = 0, vip = 0;
    for (int i = 0; i < jumlahNasabah; i++) {
        total += daftar[i]->getSaldo();
        if (daftar[i]->getTipe() == "VIP") vip++;
        else reguler++;
    }
    cout << CYAN  << "  Total Nasabah  : " << YELLOW << jumlahNasabah << " orang\n" << RESET;
    cout << CYAN  << "  Nasabah Reguler: " << WHITE  << reguler << "\n" << RESET;
    cout << CYAN  << "  Nasabah VIP    : " << MAGENTA<< vip    << "\n" << RESET;
    cout << CYAN  << "  Total Saldo    : " << BOLD << GREEN << fixed << setprecision(2) << total << " Koin\n" << RESET;
}

void menuInfoLengkap() {
    header("CEK DATA NASABAH LENGKAP");
    string nr = inputTeks(CYAN "  No. Rekening: " RESET);
    Nasabah* n = cariRekening(nr);
    if (n)
        n->tampilkanInfo();   // POLYMORPHISM - otomatis pilih VIP atau Reguler
    else
        cout << RED << "\n  [!] Rekening tidak ditemukan.\n" << RESET;
}

void menuCariNama() {
    header("CARI NASABAH BERDASARKAN NAMA");
    string nm = inputTeks(CYAN "  Nama (sebagian): " RESET);
    bool ketemu = false;
    cout << "\n";
    for (int i = 0; i < jumlahNasabah; i++) {
        if (daftar[i]->getNama().find(nm) != string::npos) {
            cout << GREEN << "  [" << i+1 << "] " << YELLOW << daftar[i]->getNama()
                 << WHITE << " | " << CYAN << daftar[i]->getNoRek()
                 << DIM   << " (" << daftar[i]->getTipe() << ")"
                 << RESET << "\n";
            ketemu = true;
        }
    }
    if (!ketemu) cout << RED << "  [!] Nama tidak ditemukan.\n" << RESET;
}

// ============================================================
// MAIN
// ============================================================
int main() {
    srand((unsigned)time(0));
    muatFile();

    int pilihan;
    do {
        clearScreen();
        cout << BOLD << CYAN << "\n         ~~ SELAMAT DATANG DI OCEAN BANK ~~\n" << RESET;
        header("MENU UTAMA");
        cout << BLUE    << "  [1] " << WHITE << "Daftarkan Nasabah Baru\n"          << RESET;
        cout << CYAN    << "  [2] " << WHITE << "Cek Saldo\n"                       << RESET;
        cout << GREEN   << "  [3] " << WHITE << "Setor Saldo\n"                     << RESET;
        cout << YELLOW  << "  [4] " << WHITE << "Tarik Saldo\n"                     << RESET;
        cout << MAGENTA << "  [5] " << WHITE << "Laporan Keuangan\n"                << RESET;
        cout << CYAN    << "  [6] " << WHITE << "Cek Data Nasabah Lengkap\n"        << RESET;
        cout << BLUE    << "  [7] " << WHITE << "Cari Nasabah Berdasarkan Nama\n"   << RESET;
        cout << RED     << "  [0] " << WHITE << "Keluar & Simpan Data\n\n"          << RESET;

        pilihan = inputMenu(string(YELLOW) + "  Pilihan (0-7): " + RESET, 0, 7);
        clearScreen();

        switch (pilihan) {
            case 1: menuDaftarNasabah(); break;
            case 2: menuCekSaldo();      break;
            case 3: menuSetor();         break;
            case 4: menuTarik();         break;
            case 5: menuLaporan();       break;
            case 6: menuInfoLengkap();   break;
            case 7: menuCariNama();      break;
            case 0: break;
        }

        if (pilihan != 0) tungguEnter();

    } while (pilihan != 0);

    simpanFile();
    cout << GREEN << "\n  [OK] Data disimpan. Terima kasih telah menggunakan Ocean Bank!\n\n" << RESET;

    // DESTRUCTOR - bebaskan memori heap
    for (int i = 0; i < jumlahNasabah; i++) delete daftar[i];
    return 0;
}

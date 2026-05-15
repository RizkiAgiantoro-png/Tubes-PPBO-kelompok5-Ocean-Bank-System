#include <iostream>
#include <fstream>
#include <ctime>
#include <iomanip>
#include <limits>
#include <sstream>
using namespace std;

const int MAX = 100;

// ANSI COLORS
#define RESET     "\033[0m"
#define BOLD      "\033[1m"
#define DIM       "\033[2m"
#define ITALIC    "\033[3m"
#define RED       "\033[91m"
#define GREEN     "\033[92m"
#define YELLOW    "\033[93m"
#define BLUE      "\033[94m"
#define MAGENTA   "\033[95m"
#define CYAN      "\033[96m"
#define WHITE     "\033[97m"
#define BG_DARK   "\033[40m"

// ============================================================
// INPUT VALIDATION
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

int inputMenu(const string& prompt, int mn, int mx) {
    int nilai;
    while (true) {
        cout << prompt;
        if (cin >> nilai && nilai >= mn && nilai <= mx) {
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return nilai;
        }
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << RED << "  [!] Pilihan tidak valid. Masukkan " << mn << "-" << mx << ".\n" << RESET;
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
// ABSTRACTION
// ============================================================
class Rekening {
public:
    virtual void   tampilkanInfo() const = 0;
    virtual string getTipe()       const = 0;
    virtual bool   tarik(double jml)     = 0;
    virtual bool   setor(double jml)     = 0;
    virtual ~Rekening() {}
};

// ============================================================
// NASABAH
// ============================================================
class Nasabah : public Rekening {
protected:
    string nama, spesies, noRek, pin;
    double saldo;

public:
    Nasabah(const string& nama, const string& spesies,
            const string& noRek, double saldo, const string& pin = "0000")
        : nama(nama), spesies(spesies), noRek(noRek), saldo(saldo), pin(pin) {}

    virtual ~Nasabah() {}

    string getNama()    const { return nama; }
    string getNoRek()   const { return noRek; }
    string getSpesies() const { return spesies; }
    string getPin()     const { return pin; }
    double getSaldo()   const { return saldo; }

    bool cekPin(const string& p) const { return pin == p; }

    bool setor(double jumlah) override {
        if (jumlah <= 0) return false;
        saldo += jumlah;
        return true;
    }

    bool tarik(double jumlah) override {
        if (jumlah <= 0 || jumlah > saldo) return false;
        saldo -= jumlah;
        return true;
    }

    void tampilkanInfo() const override {
        cout << "\n";
        cout << CYAN << "  +================================================+\n" << RESET;
        cout << CYAN << "  |" << BOLD << YELLOW << "          INFORMASI AKUN NASABAH                " << CYAN << "|\n" << RESET;
        cout << CYAN << "  +================================================+\n" << RESET;
        cout << CYAN << "  |" << RESET << "  Nama          : " << GREEN  << left << setw(30) << nama     << CYAN << "|\n" << RESET;
        cout << CYAN << "  |" << RESET << "  Spesies Laut  : " << WHITE  << left << setw(30) << spesies  << CYAN << "|\n" << RESET;
        cout << CYAN << "  |" << RESET << "  No. Rekening  : " << YELLOW << left << setw(30) << noRek    << CYAN << "|\n" << RESET;
        cout << CYAN << "  |" << RESET << "  Tipe Akun     : " << BLUE   << left << setw(30) << getTipe()<< CYAN << "|\n" << RESET;
        cout << CYAN << "  |" << RESET << "  Saldo         : " << BOLD   << GREEN
             << left << setw(24) << fixed << setprecision(2) << saldo << " Koin" << RESET << CYAN << "|\n" << RESET;
        cout << CYAN << "  +================================================+\n" << RESET;
    }

    string getTipe() const override { return "Reguler"; }

    bool operator>(const Nasabah& o) const { return saldo > o.saldo; }

    friend ostream& operator<<(ostream& os, const Nasabah& n) {
        os << "[" << n.getTipe() << "] " << n.nama << " | " << n.noRek;
        return os;
    }
};

// ============================================================
// NASABAH VIP
// ============================================================
class NasabahVIP : public Nasabah {
    double limitKredit;
public:
    NasabahVIP(const string& nama, const string& spesies,
               const string& noRek, double saldo,
               double limit = 1000.0, const string& pin = "0000")
        : Nasabah(nama, spesies, noRek, saldo, pin), limitKredit(limit) {}

    bool tarik(double jumlah) override {
        if (jumlah <= 0 || jumlah > (saldo + limitKredit)) return false;
        saldo -= jumlah;
        return true;
    }

    void tampilkanInfo() const override {
        cout << "\n";
        cout << MAGENTA << "  +================================================+\n" << RESET;
        cout << MAGENTA << "  |" << BOLD << YELLOW << "       INFORMASI AKUN NASABAH VIP [★]          " << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  +================================================+\n" << RESET;
        cout << MAGENTA << "  |" << RESET << "  Nama          : " << GREEN   << left << setw(30) << nama       << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  |" << RESET << "  Spesies Laut  : " << WHITE   << left << setw(30) << spesies    << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  |" << RESET << "  No. Rekening  : " << YELLOW  << left << setw(30) << noRek      << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  |" << RESET << "  Tipe Akun     : " << BOLD    << MAGENTA << left << setw(30) << "VIP [★ BINTANG]" << RESET << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  |" << RESET << "  Saldo         : " << BOLD    << GREEN
             << left << setw(24) << fixed << setprecision(2) << saldo << " Koin" << RESET << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  |" << RESET << "  Limit Kredit  : " << CYAN
             << left << setw(24) << fixed << setprecision(2) << limitKredit << " Koin" << MAGENTA << "|\n" << RESET;
        cout << MAGENTA << "  +================================================+\n" << RESET;
    }

    string getTipe()        const override { return "VIP"; }
    double getLimitKredit() const          { return limitKredit; }
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
          << fixed << setprecision(2) << n->getSaldo() << "|"
          << n->getPin();
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
        string pin   = (kol >= 6) ? ambilKolom(baris, '|', 5) : "0000";

        if (tipe == "VIP" && kol >= 7) {
            double limit = stod(ambilKolom(baris, '|', 6));
            daftar[jumlahNasabah++] = new NasabahVIP(nama, spes, noRek, saldo, limit, pin);
        } else {
            daftar[jumlahNasabah++] = new Nasabah(nama, spes, noRek, saldo, pin);
        }
    }
    f.close();
}

// ============================================================
// RIWAYAT TRANSAKSI
// ============================================================
void catatRiwayat(const string& noRek, const string& jenis, double jumlah, double saldoAkhir) {
    ofstream f("riwayat.txt", ios::app);
    if (!f.is_open()) return;

    time_t now = time(0);
    tm* t = localtime(&now);
    char buf[32];
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", t);

    f << noRek << "|" << buf << "|" << jenis << "|"
      << fixed << setprecision(2) << jumlah << "|"
      << fixed << setprecision(2) << saldoAkhir << "\n";
    f.close();
}

void tampilRiwayat(const string& noRek) {
    ifstream f("riwayat.txt");
    if (!f.is_open()) {
        cout << RED << "  [!] Belum ada riwayat transaksi.\n" << RESET;
        return;
    }

    bool ada = false;
    string baris;
    cout << "\n";
    cout << CYAN << "  +------------------------------------------------------------------+\n" << RESET;
    cout << CYAN << "  |" << BOLD << YELLOW << "              RIWAYAT TRANSAKSI                               " << CYAN << "|\n" << RESET;
    cout << CYAN << "  +------------------------------------------------------------------+\n" << RESET;
    cout << CYAN << "  |" << RESET << DIM << "  Tanggal/Waktu       Jenis          Jumlah       Saldo Akhir  " << RESET << CYAN << "|\n" << RESET;
    cout << CYAN << "  +------------------------------------------------------------------+\n" << RESET;

    while (getline(f, baris)) {
        if (baris.empty()) continue;
        int kol = 1;
        for (char c : baris) if (c == '|') kol++;
        if (kol < 5) continue;

        string rek   = ambilKolom(baris, '|', 0);
        if (rek != noRek) continue;

        string tgl   = ambilKolom(baris, '|', 1);
        string jenis = ambilKolom(baris, '|', 2);
        double jml   = stod(ambilKolom(baris, '|', 3));
        double sal   = stod(ambilKolom(baris, '|', 4));

        string warna = WHITE;
        if (jenis == "SETOR")          warna = GREEN;
        else if (jenis == "TARIK")     warna = RED;
        else if (jenis == "TRANSFER")  warna = YELLOW;
        else if (jenis == "TERIMA")    warna = CYAN;

        cout << CYAN << "  |" << RESET;
        cout << "  " << DIM << left << setw(20) << tgl << RESET;
        cout << warna << left << setw(15) << jenis << RESET;
        cout << MAGENTA << right << setw(10) << fixed << setprecision(2) << jml << " Koin  " << RESET;
        cout << GREEN   << right << setw(10) << fixed << setprecision(2) << sal << " Koin  " << RESET;
        cout << CYAN << "|\n" << RESET;
        ada = true;
    }

    if (!ada) {
        cout << CYAN << "  |" << RESET << DIM << "              Belum ada transaksi untuk rekening ini.         " << RESET << CYAN << "|\n" << RESET;
    }

    cout << CYAN << "  +------------------------------------------------------------------+\n" << RESET;
    f.close();
}

// ============================================================
// UTILITY
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
    cout << warna << "  ==================================================\n" << RESET;
}

void header(const string& judul) {
    int pad = (int)(50 - judul.size()) / 2;
    if (pad < 0) pad = 0;
    cout << "\n";
    garis(CYAN);
    cout << CYAN << "  " << string(pad, ' ') << BOLD << YELLOW << judul << RESET << "\n";
    garis(CYAN);
}

void struk(const string& jenis, const Nasabah* n, double jumlah, const string& keterangan = "") {
    time_t now = time(0);
    tm* t = localtime(&now);
    char buf[32];
    strftime(buf, sizeof(buf), "%d/%m/%Y %H:%M:%S", t);

    cout << "\n";
    cout << GREEN << "  +================================================+\n" << RESET;
    cout << GREEN << "  |" << BOLD << WHITE << "            TRANSAKSI BERHASIL                  " << GREEN << "|\n" << RESET;
    cout << GREEN << "  +------------------------------------------------+\n" << RESET;
    cout << GREEN << "  |" << RESET << "  Jenis         : " << CYAN   << left << setw(30) << jenis         << GREEN << "|\n" << RESET;
    cout << GREEN << "  |" << RESET << "  Nama          : " << YELLOW << left << setw(30) << n->getNama()  << GREEN << "|\n" << RESET;
    cout << GREEN << "  |" << RESET << "  No. Rekening  : " << YELLOW << left << setw(30) << n->getNoRek() << GREEN << "|\n" << RESET;
    if (!keterangan.empty())
        cout << GREEN << "  |" << RESET << "  Keterangan    : " << WHITE  << left << setw(30) << keterangan    << GREEN << "|\n" << RESET;
    cout << GREEN << "  |" << RESET << "  Jumlah        : " << MAGENTA<< left << setw(24) << fixed << setprecision(2) << jumlah << " Koin" << GREEN << "|\n" << RESET;
    cout << GREEN << "  |" << RESET << "  Saldo Akhir   : " << BOLD << GREEN << left << setw(24) << fixed << setprecision(2) << n->getSaldo() << " Koin" << RESET << GREEN << "|\n" << RESET;
    cout << GREEN << "  |" << DIM << "  " << buf << "                           " << RESET << GREEN << "|\n" << RESET;
    cout << GREEN << "  +================================================+\n" << RESET;
}

void tungguEnter() {
    cout << DIM << YELLOW << "\n  Tekan Enter untuk kembali ke menu..." << RESET;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

// ============================================================
// MENU FUNCTIONS
// ============================================================
void menuDaftarNasabah() {
    header("DAFTARKAN NASABAH BARU");
    if (jumlahNasabah >= MAX) {
        cout << RED << "  [!] Database penuh.\n" << RESET;
        return;
    }

    string nama    = inputTeks(CYAN "  Nama Nasabah       : " RESET);
    string spesies = inputTeks(CYAN "  Jenis Spesies Laut : " RESET);
    double saldo   = inputAngka(string(CYAN) + "  Saldo Awal (Koin)  : " + RESET);

    string pin;
    while (true) {
        cout << CYAN << "  Buat PIN (4 digit)  : " << RESET;
        getline(cin, pin);
        if (pin.size() == 4 && pin.find_first_not_of("0123456789") == string::npos) break;
        cout << RED << "  [!] PIN harus 4 digit angka.\n" << RESET;
    }

    char vip;
    cout << CYAN << "  Daftar sebagai VIP? (y/n): " << RESET;
    cin >> vip;
    cin.ignore(numeric_limits<streamsize>::max(), '\n');

    string noRek = generateNoRek();
    while (cariRekening(noRek)) noRek = generateNoRek();

    if (vip == 'y' || vip == 'Y')
        daftar[jumlahNasabah++] = new NasabahVIP(nama, spesies, noRek, saldo, 1000.0, pin);
    else
        daftar[jumlahNasabah++] = new Nasabah(nama, spesies, noRek, saldo, pin);

    simpanFile();
    cout << GREEN << "\n  [OK] Nasabah berhasil didaftarkan!\n" << RESET;
    cout << YELLOW << "  Nomor Rekening : " << BOLD << noRek << RESET << "\n";
    cout << CYAN   << "  Simpan nomor rekening dan PIN Anda baik-baik!\n" << RESET;
}

void menuCekSaldo() {
    header("CEK SALDO");
    string nr = inputTeks(CYAN "  No. Rekening: " RESET);
    Nasabah* n = cariRekening(nr);
    if (!n) { cout << RED << "\n  [!] Rekening tidak ditemukan.\n" << RESET; return; }

    cout << "\n";
    cout << CYAN << "  Nama   : " << YELLOW << n->getNama() << RESET << "\n";
    cout << CYAN << "  Tipe   : " << BLUE   << n->getTipe() << RESET << "\n";
    cout << CYAN << "  Saldo  : " << BOLD << GREEN << fixed << setprecision(2) << n->getSaldo() << " Koin\n" << RESET;
}

void menuSetor() {
    header("SETOR SALDO");
    string nr = inputTeks(CYAN "  No. Rekening : " RESET);
    Nasabah* n = cariRekening(nr);
    if (!n) { cout << RED << "\n  [!] Rekening tidak ditemukan.\n" << RESET; return; }

    cout << GREEN << "  Saldo Saat Ini : " << BOLD << fixed << setprecision(2) << n->getSaldo() << " Koin\n" << RESET;
    double jml = inputAngka(string(CYAN) + "  Jumlah Setor   : " + RESET);

    if (n->setor(jml)) {
        catatRiwayat(nr, "SETOR", jml, n->getSaldo());
        simpanFile();
        struk("SETOR SALDO", n, jml);
    } else {
        cout << RED << "\n  [!] Jumlah harus lebih dari 0.\n" << RESET;
    }
}

void menuTarik() {
    header("TARIK SALDO");
    string nr = inputTeks(CYAN "  No. Rekening : " RESET);
    Nasabah* n = cariRekening(nr);
    if (!n) { cout << RED << "\n  [!] Rekening tidak ditemukan.\n" << RESET; return; }

    cout << CYAN << "  Verifikasi PIN: " << RESET;
    string pin; getline(cin, pin);
    if (!n->cekPin(pin)) { cout << RED << "\n  [!] PIN salah.\n" << RESET; return; }

    cout << GREEN << "  Saldo Saat Ini : " << BOLD << fixed << setprecision(2) << n->getSaldo() << " Koin\n" << RESET;
    double jml = inputAngka(string(CYAN) + "  Jumlah Tarik   : " + RESET);

    if (n->tarik(jml)) {
        catatRiwayat(nr, "TARIK", jml, n->getSaldo());
        simpanFile();
        struk("TARIK SALDO", n, jml);
    } else {
        cout << RED << "\n  [!] Saldo tidak mencukupi.\n" << RESET;
    }
}

void menuTransfer() {
    header("TRANSFER ANTAR REKENING");
    string nrAsal = inputTeks(CYAN "  Rekening Asal   : " RESET);
    Nasabah* asal = cariRekening(nrAsal);
    if (!asal) { cout << RED << "\n  [!] Rekening asal tidak ditemukan.\n" << RESET; return; }

    cout << CYAN << "  Verifikasi PIN  : " << RESET;
    string pin; getline(cin, pin);
    if (!asal->cekPin(pin)) { cout << RED << "\n  [!] PIN salah.\n" << RESET; return; }

    cout << GREEN << "  Saldo Saat Ini  : " << BOLD << fixed << setprecision(2) << asal->getSaldo() << " Koin\n" << RESET;

    string nrTujuan = inputTeks(CYAN "  Rekening Tujuan : " RESET);
    if (nrTujuan == nrAsal) { cout << RED << "\n  [!] Tidak bisa transfer ke rekening sendiri.\n" << RESET; return; }

    Nasabah* tujuan = cariRekening(nrTujuan);
    if (!tujuan) { cout << RED << "\n  [!] Rekening tujuan tidak ditemukan.\n" << RESET; return; }

    cout << CYAN << "  Tujuan          : " << YELLOW << tujuan->getNama() << RESET << "\n";

    double jml = inputAngka(string(CYAN) + "  Jumlah Transfer : " + RESET);

    if (asal->tarik(jml)) {
        tujuan->setor(jml);
        catatRiwayat(nrAsal,   "TRANSFER", jml, asal->getSaldo());
        catatRiwayat(nrTujuan, "TERIMA",   jml, tujuan->getSaldo());
        simpanFile();
        struk("TRANSFER", asal, jml, "-> " + tujuan->getNama());
        cout << GREEN << "  [OK] " << tujuan->getNama() << " menerima " << fixed << setprecision(2) << jml << " Koin.\n" << RESET;
    } else {
        cout << RED << "\n  [!] Saldo tidak mencukupi untuk transfer.\n" << RESET;
    }
}

void menuRiwayat() {
    header("RIWAYAT TRANSAKSI");
    string nr = inputTeks(CYAN "  No. Rekening: " RESET);
    Nasabah* n = cariRekening(nr);
    if (!n) { cout << RED << "\n  [!] Rekening tidak ditemukan.\n" << RESET; return; }
    tampilRiwayat(nr);
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
    cout << "\n";
    cout << CYAN << "  Total Nasabah   : " << BOLD << YELLOW << jumlahNasabah << " nasabah\n" << RESET;
    cout << CYAN << "  Nasabah Reguler : " << WHITE << reguler << "\n" << RESET;
    cout << CYAN << "  Nasabah VIP     : " << MAGENTA << vip << "\n" << RESET;
    cout << CYAN << "  Total Saldo     : " << BOLD << GREEN << fixed << setprecision(2) << total << " Koin\n" << RESET;
}

void menuInfoLengkap() {
    header("CEK DATA NASABAH LENGKAP");
    string nr = inputTeks(CYAN "  No. Rekening: " RESET);
    Nasabah* n = cariRekening(nr);
    if (n)
        n->tampilkanInfo();
    else
        cout << RED << "\n  [!] Rekening tidak ditemukan.\n" << RESET;
}

void menuCariNama() {
    header("CARI NASABAH BERDASARKAN NAMA");
    string nm = inputTeks(CYAN "  Nama (sebagian): " RESET);
    bool ketemu = false;
    cout << "\n";
    for (int i = 0; i < jumlahNasabah; i++) {
        string nama = daftar[i]->getNama();
        string nameLow = nama, queryLow = nm;
        for (char& c : nameLow)  c = tolower(c);
        for (char& c : queryLow) c = tolower(c);

        if (nameLow.find(queryLow) != string::npos) {
            string tipeWarna = (daftar[i]->getTipe() == "VIP") ? MAGENTA : BLUE;
            cout << CYAN << "  [" << i+1 << "] " << YELLOW << daftar[i]->getNama()
                 << WHITE << " | " << CYAN << daftar[i]->getNoRek()
                 << DIM << " (" << tipeWarna << daftar[i]->getTipe() << DIM << ")"
                 << RESET << "\n";
            ketemu = true;
        }
    }
    if (!ketemu) cout << RED << "  [!] Tidak ada nasabah dengan nama tersebut.\n" << RESET;
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
        cout << "\n";
        cout << BOLD << CYAN << "      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << RESET;
        cout << BOLD << CYAN << "      ~                                                ~\n" << RESET;
        cout << BOLD << CYAN << "      ~" << YELLOW << "           ><(((°>  OCEAN BANK  <°)))><         " << CYAN << "~\n" << RESET;
        cout << BOLD << CYAN << "      ~" << DIM << "          Sistem Perbankan Dunia Bawah Laut      " << BOLD << CYAN << "~\n" << RESET;
        cout << BOLD << CYAN << "      ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n" << RESET;

        header("MENU UTAMA");

        cout << BLUE    << "  [1] " << WHITE << " Daftarkan Nasabah Baru\n"          << RESET;
        cout << CYAN    << "  [2] " << WHITE << " Cek Saldo\n"                       << RESET;
        cout << GREEN   << "  [3] " << WHITE << " Setor Saldo\n"                     << RESET;
        cout << YELLOW  << "  [4] " << WHITE << " Tarik Saldo\n"                     << RESET;
        cout << MAGENTA << "  [5] " << WHITE << " Transfer Antar Rekening\n"         << RESET;
        cout << CYAN    << "  [6] " << WHITE << " Riwayat Transaksi\n"               << RESET;
        cout << BLUE    << "  [7] " << WHITE << " Laporan Keuangan\n"                << RESET;
        cout << CYAN    << "  [8] " << WHITE << " Cek Data Nasabah Lengkap\n"        << RESET;
        cout << WHITE   << "  [9] " << WHITE << " Cari Nasabah Berdasarkan Nama\n"   << RESET;
        cout << RED     << "  [0] " << WHITE << " Keluar & Simpan Data\n\n"          << RESET;

        pilihan = inputMenu(string(YELLOW) + "  Pilihan (0-9): " + RESET, 0, 9);
        clearScreen();

        switch (pilihan) {
            case 1: menuDaftarNasabah(); break;
            case 2: menuCekSaldo();      break;
            case 3: menuSetor();         break;
            case 4: menuTarik();         break;
            case 5: menuTransfer();      break;
            case 6: menuRiwayat();       break;
            case 7: menuLaporan();       break;
            case 8: menuInfoLengkap();   break;
            case 9: menuCariNama();      break;
            case 0: break;
        }

        if (pilihan != 0) tungguEnter();

    } while (pilihan != 0);

    simpanFile();
    cout << "\n" << GREEN << "  [OK] Data tersimpan. Terima kasih sudah menggunakan Ocean Bank!\n";
    cout << CYAN << "  ><(((°>  sampai jumpa lagi!\n\n" << RESET;

    for (int i = 0; i < jumlahNasabah; i++) delete daftar[i];
    return 0;
}

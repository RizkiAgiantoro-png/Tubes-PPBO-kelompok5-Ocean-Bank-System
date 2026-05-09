#include <iostream>
#include <fstream>
#include <ctime>
using namespace std;

const int MAX = 100;

// ============================================================
// ABSTRACTION & POLYMORPHISM
// ============================================================
class Rekening {
public:
    virtual void tampilkanInfo() const = 0;
    virtual string getTipe() const = 0;
    virtual bool tarik(double jumlah) = 0;
    virtual ~Rekening() {}
};

// ============================================================
// CLASS & OBJECT + ENCAPSULATION + INHERITANCE
// ============================================================
class Nasabah : public Rekening {
protected:
    string nama, spesies, noRek;
    double saldo;

public:
    // CONSTRUCTOR
    Nasabah(string nama, string spesies, string noRek, double saldo)
        : nama(nama), spesies(spesies), noRek(noRek), saldo(saldo) {}

    // DESTRUCTOR
    ~Nasabah() {}

    // ENCAPSULATION (Getters)
    string getNama()    const { return nama; }
    string getNoRek()   const { return noRek; }
    double getSaldo()   const { return saldo; }
    string getSpesies() const { return spesies; }

    // SETOR SALDO
    bool setor(double jumlah) {
        if (jumlah <= 0) return false;
        saldo += jumlah;
        return true;
    }

    // TARIK SALDO
    virtual bool tarik(double jumlah) override {
        if (jumlah <= 0 || jumlah > saldo) return false;
        saldo -= jumlah;
        return true;
    }

    // CEK DATA NASABAH
    void tampilkanInfo() const override {
        cout << "  -------------------------------------------\n";
        cout << "  Nama Nasabah : " << nama    << "\n"
            << "  Spesies Laut : " << spesies << "\n"
            << "  No. Rekening : " << noRek   << "\n"
            << "  Tipe Akun    : " << getTipe() << "\n"
            << "  Total Saldo  : " << saldo   << " Koin\n";
        cout << "  -------------------------------------------\n";
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
// INHERITANCE - NasabahVIP
// ============================================================
class NasabahVIP : public Nasabah {
    double limitKredit;
public:
    NasabahVIP(string nama, string spesies, string noRek, double saldo, double limit = 1000)
        : Nasabah(nama, spesies, noRek, saldo), limitKredit(limit) {}

    // POLYMORPHISM - VIP bisa tarik sampai limit kredit
    bool tarik(double jumlah) override {
        if (jumlah <= 0 || jumlah > (saldo + limitKredit)) return false;
        saldo -= jumlah;
        return true;
    }

    string getTipe() const override { return "VIP"; }
    double getLimitKredit() const { return limitKredit; }
};

// ============================================================
// FILE HANDLING
// ============================================================
Nasabah* daftar[MAX];
int jumlahNasabah = 0;

// Split string per kolom tanpa sstream
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
    for (int i = 0; i < jumlahNasabah; i++) {
        Nasabah* n = daftar[i];
        f << n->getTipe() << "|" << n->getNama() << "|"
        << n->getSpesies() << "|" << n->getNoRek() << "|"
        << n->getSaldo();
        if (n->getTipe() == "VIP")
            f << "|" << dynamic_cast<NasabahVIP*>(n)->getLimitKredit();
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
        int kol       = hitungKolom(baris, '|');
        string tipe   = ambilKolom(baris, '|', 0);
        string nama   = ambilKolom(baris, '|', 1);
        string spes   = ambilKolom(baris, '|', 2);
        string noRek  = ambilKolom(baris, '|', 3);
        double saldo  = stod(ambilKolom(baris, '|', 4));

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

void header(const string& s) {
    cout << "\n  =======================================================\n";
    int sp = (55 - (int)s.size()) / 2;
    cout << "  " << string(sp, ' ') << s << "\n";
    cout << "  =======================================================\n";
}

// ============================================================
// MAIN
// ============================================================
int main() {
    srand(time(0));
    muatFile();

    int pilihan;
    do {
        system("clear || cls");
        cout << "\n           ~~~~~ WELCOME TO OCEAN BANK ~~~~~\n";
        header("MENU UTAMA");
        cout << "  1. Input Data Nasabah Baru\n"
            << "  2. Cek Saldo\n"
            << "  3. Setor Saldo\n"
            << "  4. Tarik Saldo\n"
            << "  5. Laporan Keuangan Keseluruhan\n"
            << "  6. Cek Data Nasabah Lengkap\n"
            << "  7. Cari Nomor Rekening\n"
            << "  0. Keluar & Simpan Data\n\n";
        cout << "  Pilihan: ";
        if (!(cin >> pilihan)) { cin.clear(); cin.ignore(1000, '\n'); pilihan = -1; continue; }

        system("clear || cls");

        if (pilihan == 1) {
            header("INPUT DATA NASABAH BARU");
            cin.ignore();
            string nama, spesies; double saldo; char vip;
            cout << "  Nama Nasabah       : "; getline(cin, nama);
            cout << "  Jenis Spesies Laut : "; getline(cin, spesies);
            cout << "  Saldo Awal (Koin)  : "; cin >> saldo;
            cout << "  Daftar Akun VIP? (y/n): "; cin >> vip;

            if (jumlahNasabah >= MAX) {
                cout << "\n  [!] Bank sudah penuh!\n";
            } else {
                string noRek = generateNoRek();
                while (cariRekening(noRek)) noRek = generateNoRek();

                if (vip == 'y' || vip == 'Y')
                    daftar[jumlahNasabah++] = new NasabahVIP(nama, spesies, noRek, saldo);
                else
                    daftar[jumlahNasabah++] = new Nasabah(nama, spesies, noRek, saldo);

                simpanFile();
                cout << "\n  [SUKSES] Data berhasil disimpan.\n";
                cout << "  NOMOR REKENING ANDA: " << noRek << "\n";
            }

        } else if (pilihan == 2) {
            header("CEK SALDO NASABAH");
            cin.ignore(); string nr;
            cout << "  Masukkan No. Rekening: "; getline(cin, nr);
            Nasabah* n = cariRekening(nr);
            if (n) cout << "\n  Saldo " << n->getNama() << ": " << n->getSaldo() << " Koin\n";
            else   cout << "\n  [!] Rekening tidak ditemukan.\n";

        } else if (pilihan == 3 || pilihan == 4) {
            string aksi = (pilihan == 3) ? "SETOR" : "TARIK";
            header(aksi + " SALDO");
            cin.ignore(); string nr; double jml;
            cout << "  No. Rekening  : "; getline(cin, nr);
            cout << "  Jumlah " << aksi << "  : "; cin >> jml;
            Nasabah* n = cariRekening(nr);
            if (n) {
                bool ok = (pilihan == 3) ? n->setor(jml) : n->tarik(jml);
                if (ok) {
                    simpanFile();
                    cout << "\n  [BERHASIL] Saldo diperbarui.\n";
                    cout << "  Saldo Saat Ini: " << n->getSaldo() << " Koin\n";
                } else cout << "\n  [GAGAL] Periksa saldo atau jumlah input.\n";
            } else cout << "\n  [!] Rekening tidak ditemukan.\n";

        } else if (pilihan == 5) {
            header("LAPORAN KEUANGAN OCEAN BANK");
            double totalSaldo = 0;
            for (int i = 0; i < jumlahNasabah; i++) totalSaldo += daftar[i]->getSaldo();
            cout << "  Jumlah Total Nasabah : " << jumlahNasabah << " Nasabah\n";
            cout << "  Total Seluruh Saldo  : " << totalSaldo << " Koin\n";

        } else if (pilihan == 6) {
            header("INFORMASI LENGKAP NASABAH");
            cin.ignore(); string nr;
            cout << "  Masukkan No. Rekening: "; getline(cin, nr);
            Nasabah* n = cariRekening(nr);
            if (n) n->tampilkanInfo();  // POLYMORPHISM
            else   cout << "\n  [!] Rekening tidak ditemukan.\n";

        } else if (pilihan == 7) {
            header("CARI NOMOR REKENING");
            cin.ignore(); string nm;
            cout << "  Masukkan Nama Nasabah: "; getline(cin, nm);
            bool ketemu = false;
            for (int i = 0; i < jumlahNasabah; i++) {
                if (daftar[i]->getNama().find(nm) != string::npos) {
                    cout << "  > " << daftar[i]->getNama()
                        << " | No Rek: " << daftar[i]->getNoRek() << "\n";
                    ketemu = true;
                }
            }
            if (!ketemu) cout << "  [!] Nama tidak ditemukan.\n";
        }

        if (pilihan != 0) {
            cout << "\n  Tekan Enter untuk kembali...";
            cin.ignore(1000, '\n'); cin.get();
        }

    } while (pilihan != 0);

    // DESTRUCTOR - bebaskan memori
    for (int i = 0; i < jumlahNasabah; i++) delete daftar[i];
    return 0;
}
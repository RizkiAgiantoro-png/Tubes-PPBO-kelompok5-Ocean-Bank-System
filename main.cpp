#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <iomanip>
#include <sstream>
#include <ctime>

using namespace std;

// ============================================================
// [POINT 4 & 6] ABSTRACTION & POLYMORPHISM
// ============================================================
class Rekening {
public:
    virtual void tampilkanInfo() const = 0;
    virtual string getTipe() const = 0;
    virtual bool tarik(double jumlah) = 0;
    virtual ~Rekening() {} // [POINT 2] DESTRUCTOR VIRTUAL
};

// ============================================================
// [POINT 1 & 4] CLASS & OBJECT + ENCAPSULATION + INHERITANCE
// ============================================================
class Nasabah : public Rekening {
protected:
    string nama, spesies, noRek;
    double saldo;

public:
    // [POINT 2] CONSTRUCTOR
    Nasabah(string nama, string spesies, string noRek, double saldo)
        : nama(nama), spesies(spesies), noRek(noRek), saldo(saldo) {}

    // [POINT 4] ENCAPSULATION (Getters)
    string getNama()    const { return nama; }
    string getNoRek()   const { return noRek; }
    double getSaldo()   const { return saldo; }
    string getSpesies() const { return spesies; }

    // [FITUR 3] SETOR SALDO
    bool setor(double jumlah) {
        if (jumlah <= 0) return false;
        saldo += jumlah;
        return true;
    }

    // [FITUR 4] TARIK SALDO (Validasi saldo mencukupi)
    virtual bool tarik(double jumlah) override {
        if (jumlah <= 0 || jumlah > saldo) return false;
        saldo -= jumlah;
        return true;
    }

    // [FITUR 6] CEK DATA NASABAH (Informasi Lengkap)
    void tampilkanInfo() const override {
        cout << "  -------------------------------------------\n";
        cout << "  Nama Nasabah : " << nama << "\n"
             << "  Spesies Laut : " << spesies << "\n"
             << "  No. Rekening : " << noRek << "\n"
             << "  Tipe Akun    : " << getTipe() << "\n"
             << "  Total Saldo  : " << fixed << setprecision(2) << saldo << " Koin\n";
        cout << "  -------------------------------------------\n";
    }

    string getTipe() const override { return "Reguler"; }

    // [POINT 5] OVERLOADING OPERATOR (Membandingkan Saldo & Output Stream)
    bool operator>(const Nasabah& o) const { return saldo > o.saldo; }
    friend ostream& operator<<(ostream& os, const Nasabah& n) {
        os << "[" << setw(7) << left << n.getTipe() << "] " 
           << setw(15) << left << n.nama << " | " << n.noRek;
        return os;
    }
};

// [POINT 4] INHERITANCE (Nasabah VIP sebagai turunan Nasabah)
class NasabahVIP : public Nasabah {
    double limitKredit;
public:
    NasabahVIP(string nama, string spesies, string noRek, double saldo, double limit = 1000)
        : Nasabah(nama, spesies, noRek, saldo), limitKredit(limit) {}

    // [POINT 6] POLYMORPHISM (Overriding fungsi tarik untuk VIP)
    bool tarik(double jumlah) override {
        if (jumlah <= 0 || jumlah > (saldo + limitKredit)) return false;
        saldo -= jumlah;
        return true;
    }

    string getTipe() const override { return "VIP"; }
    double getLimitKredit() const { return limitKredit; }
};

// ============================================================
// [POINT 3] FILE HANDLING & DATA MANAGEMENT
// ============================================================
vector<Nasabah*> daftar;

void simpanFile() {
    ofstream f("nasabah.txt");
    for (Nasabah* n : daftar) {
        f << n->getTipe() << "|" << n->getNama() << "|"
          << n->getSpesies() << "|" << n->getNoRek() << "|"
          << fixed << setprecision(2) << n->getSaldo();
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
    while (getline(f, baris)) {
        if (baris.empty()) continue;
        vector<string> k; string tok; stringstream ss(baris);
        while (getline(ss, tok, '|')) k.push_back(tok);
        try {
            if (k[0] == "VIP" && k.size() >= 6)
                daftar.push_back(new NasabahVIP(k[1], k[2], k[3], stod(k[4]), stod(k[5])));
            else if (k.size() >= 5)
                daftar.push_back(new Nasabah(k[1], k[2], k[3], stod(k[4])));
        } catch (...) { continue; }
    }
    f.close();
}

// ============================================================
// UTILITY FUNCTIONS
// ============================================================
Nasabah* cariRekening(const string& noRek) {
    for (Nasabah* n : daftar) if (n->getNoRek() == noRek) return n;
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
// MAIN PROGRAM
// ============================================================
int main() {
    srand(time(0));
    muatFile(); // Memuat data dari file saat program dibuka

    int pilihan;
    do {
        system("clear || cls");
        cout << "\n           ~~~~~ WELCOME TO OCEAN BANK ~~~~~\n";
        header("MENU UTAMA");
        cout << "  1.Input Data Nasabah Baru\n"
             << "  2.Cek Saldo (Cepat)\n"
             << "  3.Setor Saldo\n"
             << "  4.Tarik Saldo\n"
             << "  5.Laporan Keuangan Keseluruhan\n"
             << "  6.Cek Data Nasabah Lengkap\n"
             << "  7. Cari Nomor Rekening (Jika Lupa)\n"
             << "  0. Keluar & Simpan Data\n\n";
        cout << "  Pilihan: "; 
        if (!(cin >> pilihan)) { cin.clear(); cin.ignore(1000, '\n'); pilihan = -1; continue; }

        system("clear || cls");

        if (pilihan == 1) {
            // [FITUR 1] INPUT DATA NASABAH
            header("INPUT DATA NASABAH BARU");
            cin.ignore();
            string nama, spesies; double saldo; char vip;
            cout << "  Nama Nasabah     : "; getline(cin, nama);
            cout << "  Jenis Spesies Laut: "; getline(cin, spesies);
            cout << "  Saldo Awal (Koin) : "; cin >> saldo;
            cout << "  Daftar Akun VIP? (y/n): "; cin >> vip;

            string noRek = generateNoRek();
            while (cariRekening(noRek)) noRek = generateNoRek();

            if (vip == 'y' || vip == 'Y')
                daftar.push_back(new NasabahVIP(nama, spesies, noRek, saldo));
            else
                daftar.push_back(new Nasabah(nama, spesies, noRek, saldo));

            simpanFile(); // Menyimpan ke file
            cout << "\n  [SUKSES] Data berhasil disimpan ke Ocean Bank.\n";
            cout << "  NOMOR REKENING ANDA: " << noRek << "\n";

        } else if (pilihan == 2) {
            // [FITUR 2] CEK SALDO
            header("CEK SALDO NASABAH");
            cin.ignore(); string nr;
            cout << "  Masukkan No. Rekening: "; getline(cin, nr);
            Nasabah* n = cariRekening(nr);
            if (n) cout << "\n  Total harta di Ocean Bank: " << n->getSaldo() << " Koin\n";
            else   cout << "\n  [!] Rekening tidak ditemukan.\n";

        } else if (pilihan == 3 || pilihan == 4) {
            // [FITUR 3 & 4] SETOR & TARIK
            string aksi = (pilihan == 3) ? "SETOR" : "TARIK";
            header(aksi + " SALDO");
            cin.ignore(); string nr; double jml;
            cout << "  No. Rekening : "; getline(cin, nr);
            cout << "  Jumlah " << aksi << " : "; cin >> jml;
            Nasabah* n = cariRekening(nr);
            if (n) {
                bool ok = (pilihan == 3) ? n->setor(jml) : n->tarik(jml);
                if (ok) {
                    simpanFile();
                    cout << "\n  [BERHASIL] Saldo diperbarui secara otomatis.\n";
                    cout << "  Saldo Saat Ini: " << n->getSaldo() << " Koin\n";
                } else cout << "\n  [GAGAL] Periksa saldo Anda atau jumlah input.\n";
            } else cout << "\n  [!] Rekening tidak ditemukan.\n";

        } else if (pilihan == 5) {
            // [FITUR 5] LAPORAN KEUANGAN
            header("LAPORAN KEUANGAN OCEAN BANK");
            double totalSaldo = 0;
            for (Nasabah* n : daftar) totalSaldo += n->getSaldo();
            cout << "  Jumlah Total Nasabah : " << daftar.size() << " Nasabah\n";
            cout << "  Total Seluruh Saldo  : " << fixed << setprecision(2) << totalSaldo << " Koin\n";

        } else if (pilihan == 6) {
            // [FITUR 6] CEK DATA NASABAH LENGKAP
            header("INFORMASI LENGKAP NASABAH");
            cin.ignore(); string nr;
            cout << "  Masukkan No. Rekening: "; getline(cin, nr);
            Nasabah* n = cariRekening(nr);
            if (n) n->tampilkanInfo();
            else   cout << "\n  [!] Rekening tidak ditemukan.\n";

        } else if (pilihan == 7) {
            header("CARI NOMOR REKENING");
            cin.ignore(); string nm;
            cout << "  Masukkan Nama Nasabah: "; getline(cin, nm);
            bool ketemu = false;
            for (Nasabah* n : daftar) {
                if (n->getNama().find(nm) != string::npos) {
                    cout << "  > " << n->getNama() << " | No Rek: " << n->getNoRek() << "\n";
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

    // [POINT 2] DESTRUCTOR - Membersihkan memori sebelum keluar
    for (Nasabah* n : daftar) delete n;
    return 0;
}
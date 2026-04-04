#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>
#include <vector>
#include "sqlite3.h"
using namespace std;



// Đại diện một món ăn
class Item{
private:
    int id;
    string ten;
    long gia; 
    int soluong;
    string note;

public:
    // Hàm khởi tạo 
    Item(int id = 0, string ten = "", long gia = 0, int soluong = 0, string note = ""){
        this->id = id;
        this->ten = ten;
        this->gia = gia > 0 ? gia : 0;
        this->soluong = soluong > 0? soluong : 1;
        this->note = note;
    }
    //Hàm lấy và sửa thông tin ghi chú
    string GetNote(){return note;}
    void SetNote(string text) { this->note = text;} 

    // Hàm giá lấy các giá trị
    int GetID(){return id;}
    string GetTen(){return ten;}
    long GetGia(){return gia;}
    int GetSoluong(){return soluong;}

    // Hàm tăng/ giảm giá số lượng
    void IncreaseFood(){soluong++;}
    void DecreaseFood(){if (soluong >= 1) soluong--;}

    // Hàm tính tiền
    long Value(){
        return gia*soluong;
    }
    // Hàm hiển thị món ăn tren bill
    void ShowFood(){
        cout << left << setw(5) << id 
             << setw(20) << ten 
             << setw(10) << soluong 
             << setw(15) << gia 
             << setw(15) << Value() << endl;
        if (!note.empty()){
            cout << "      [Ghi chu: " << note << "]" << endl;
        }
    }
    //Ham hien thi mon an tren menu
    void ShowFoodinMenu(){
        cout << left << setw(5) << id 
             << setw(20) << ten 
             << setw(15) << gia  << endl;

    }
};

// Danh sách gọi món của khách hàng
class Order {
private:
    vector<Item> order;
public:
    Order() {};
    // append a food to order
    void AddFood(Item Food){
        // check this food existed yet
        for (Item& temp: order){
            if (temp.GetID() == Food.GetID()){
                temp.IncreaseFood();
                return;
            }
        }
        order.push_back(Food);
    }
    // giam/ xoa mon an trong bill
    void RemoveFood(int Id){
        if (order.empty()) return;
        // Truong hop mon an nam ngay o dau
        for (auto temp = order.begin(); temp != order.end(); temp++){
            if (temp->GetID() == Id){
                if (temp->GetSoluong() > 1){
                    temp->DecreaseFood();
                } else order.erase(temp);
                return;
            }
        }
        cout << "Khong tim thay mon an co ID: " << Id << " de xoa!" << endl; 
    }
    // Calculate the bill
    long TotalBill(){
        long total = 0;
        for (Item& temp: order) total += temp.Value();
        return total;
    }
    // In hoa don
    void PrintBill(){
        if (order.empty()){
            cout << "Order dang trong!!" << endl;
            return;
        }

        cout << "\n===========================================================" << endl;
        cout << "                     HOA DON THANH TOAN                    " << endl;
        cout << "===========================================================" << endl;
        cout << left << setw(5) << "ID" 
             << setw(20) << "Ten Mon" 
             << setw(10) << "SL" 
             << setw(15) << "Don Gia" 
             << setw(15) << "Thanh Tien" << endl;
        cout << "-----------------------------------------------------------" << endl;
        
        
        for(Item& temp: order) temp.ShowFood();
        
        cout << "-----------------------------------------------------------" << endl;
        cout << right << setw(45) << "TONG CONG: " << TotalBill() << " VND" << endl;
        cout << "===========================================================" << endl;
        cout << "            Cam on quy khach - Hen gap lai!                " << endl;
    }
    void ExportBillToFile(){
        string FileName = "Bill_Current.txt";
        ofstream file(FileName, ios::app);
        if (!file.is_open()) return;
        
        file << "================ HOA DON KHACH HANG ================\n";
        file << left << setw(5) << "ID" << setw(20) << "Ten" << setw(8) << "SL" << setw(12) << "ThanhTien"<<endl;

        for (Item& temp: order){
            file << left << setw(5) << temp.GetID() 
                << setw(20) << temp.GetTen() 
                << setw(8) << temp.GetSoluong()
                << setw(12) << temp.Value() << endl;
        }
        file << "-----------------------------------------------------------" << endl;
        file << right << setw(45) << "TONG CONG: " << TotalBill() << " VND" << endl;
        file << "===========================================================" << endl;
        file << "            Cam on quy khach - Hen gap lai!                " << endl;
        file.close();
        cout << "Da xuat hoa don ra file: " << FileName << endl;
    }

    // Delete the bill
    void ResetOrder(){
        order.clear();
    }
};
int GetData(void* data, int col, char** dulieu, char** nameCol){
        vector<Item>* menuNhahang = (vector<Item>*) data;
        int id = stoi(dulieu[0]);
        string ten = dulieu[1];
        long gia = stol(dulieu[2]);

        Item newFood(id, ten, gia);
        menuNhahang->push_back(newFood);

        return 0;
    }
// Menu cua nha hang
class Menu{
private:
    vector<Item> menu;
    sqlite3* db;
public:
    Menu(){
        sqlite3_open("nhahang.db", &db);
        
        const char* sql = "CREATE TABLE IF NOT EXIST ThucDon (ID INT PRIMARY KEY, TenMon TEXT, Gia INT);";

        sqlite3_exec(db, sql, 0, 0, 0);
    };
    // Them mon an moi vao menu
    void AddtoMenu(Item newItem){
        for (Item &temp: menu) 
            if (temp.GetID() == newItem.GetID()) return;
        menu.push_back(newItem);
        return;
    }
    
    
    //Load du lieu menu tu file.
    void LoadMenu(){\
        menu.clear();
        const char* sql = "SELECT * FROM  ThucDon;";
        sqlite3_exec(db, sql, GetData, &menu, 0);
    }
    // Hien thi menu 
    void PrintMenu(){
        cout << "======================== Menu Nha Hang ========================" << endl;
        cout << left << setw(5) << "ID" 
            << setw(20) << "Ten Mon" 
            << setw(15) << "Don Gia" << endl;
        for (Item &temp: menu) temp.ShowFoodinMenu();
    }
    // Admin them mon moi vao data
    void Admin_themon(){
        int id;
        string name;
        long gia;
        cout << "\nThem mon moi vao thuc don:" << endl;
        cout << "Vui long nhap ID: "; cin >> id;
        cin.ignore();
        cout << "Nhap ten nhom (vui long thay dau cach bang dau \"_\"): "; getline(cin, name);
        cout << "Nhap gia: "; cin >> gia;
        // them vao menu tren man hinh
        Item newItem(id, name, gia);
        AddtoMenu(newItem);
        string sql = "INSERT INTO ThucDon (ID, TenMon, Gia) VALUES("
                    + to_string(id) + ", '" + name + "', " + to_string(gia) + ";";

        // them vao menu trong data
        sqlite3_exec(db, sql.c_str(), 0, 0, 0);
        cout << " Them mon thanh cong! " << endl;
    }
    // luu du lieu vao file menu.txt
    void SaveMenutoFile(string filename){
        ofstream file(filename);
        if (!file.is_open()) return;

        for (Item &temp: menu){
            // ghi file theo dinh dang
            file << temp.GetID() << " "
            << temp.GetTen() << " "
            << temp.GetGia() << endl;
        }
        file.close();
    }
    
    // Tim mon an theo ID
    Item* GetItembyId(int id){
        for (Item &temp: menu) 
            if (temp.GetID() == id) return &temp;
        return nullptr;
    }
    // dong database khi huy class
    ~Menu(){
        sqlite3_close(db);
    }
};


// Main 
int main(){
    Order Bill;
    Menu nhahang;
    nhahang.LoadMenu();
    int option;
    do{
        cout << "\n1. Xem Menu nha hang."
        << "\n2. Chon mon an."
        <<"\n3. Xoa mon / giam so luong."
        <<"\n4. In hoa don. "
        <<"\n0. Thoat bang chon."
        <<"\nChon: ";

        cin >> option;
        if (cin.fail()){
            cout << " Nhap sai, vui long nhap lai: ";
            cin.clear();
            cin.ignore(1000, '\n');
            continue;
        }
        switch (option){
            case 1:{
                nhahang.PrintMenu();// In menu nhà hàng
                break;
            }
            case 2:{
                int opt;
                cout << "Nhap ID mon an: ";
                cin >> opt;
                Item*  found = nhahang.GetItembyId(opt); // Tìm xem món ăn có trong menu không
                if (found != nullptr){
                    Bill.AddFood(*found); // Hàm thêm mon ăn vào bill
                } else {
                    cout << " Khong tim thay mon an.";
                }
                break;
            }
            case 3:{
                int opt;

                cout << "Vui long nhap mon an:";
                cin >> opt;
                Bill.RemoveFood(opt); // Hàm để giảm số lượng món trong menu
                break;
            }
            case 4:{
                Bill.PrintBill(); // Hàm in Bill ra màn hình
                char confirm; 
                cout << "Ban co muon thanh toan va in hoa don khong? (y/n):";
                cin >> confirm;
                if (confirm == 'y' || confirm == 'Y'){
                    Bill.ExportBillToFile(); // Hàm để in hóa đơn ra file
                    Bill.ResetOrder(); // Hàm để xóa bill vừa in ra file
                }
                break;
            }
            case 101:{
                string pass;

                cout << "Vui long nhap mat khau: ";
                cin >> pass;

                if (pass == "oklpassmenu"){
                    nhahang.Admin_themon(); // Admin thêm món mới vào trong Menu
                } else {
                    cout << "Mat khau sai!" << endl;
                }
                break;
            }
            case 0:
                cout << "Cam on quy khach. Dang thoat ..." << endl;
                break;
            default:
                cout << "Lua chon khong phu hop!!!" << endl;
        }
    }while (option != 0);


    return 0;
}
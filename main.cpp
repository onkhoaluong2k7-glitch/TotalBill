#include <iostream>
#include <string>
#include <iomanip>
#include <fstream>
#include <ctime>
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

// Một node của linklist
struct Node{
    Item data;
    Node* next;
    Node(Item item) : data(item), next(nullptr){}
};

// Danh sách gọi món của khách hàng
class Order {
private:
    Node* head;
public:
    Order(){head = nullptr;}
    // append a food to order
    void AddFood(Item Food){
        Node* temp = head;
        // check this food existed yet
        while (temp != nullptr){
            if (temp->data.GetID() == Food.GetID()){
                temp->data.IncreaseFood();
                return;
            }
            temp = temp->next;
        }
        //In case the new food
        Node* NewFood = new Node(Food);
        NewFood->next = head;
        head = NewFood;
    }
    // giam/ xoa mon an trong bill
    void RemoveFood(int Id){
        if (head == nullptr) return;
        // Truong hop mon an nam ngay o dau
        if (head->data.GetID() == Id){
            if (head->data.GetSoluong() > 1){
                head->data.DecreaseFood();
            }else{
                Node* temp = head;
                head = head->next;
                delete temp;
            }
        }
        // Truong hop mon an nam o giua
        Node* temp = head;
        Node* prev = nullptr;
        while (temp != nullptr && temp->data.GetID() != Id){
            prev = temp;
            temp = temp->next;
        }

        if (temp != nullptr){
            if (temp->data.GetSoluong() > 1){
                temp->data.DecreaseFood();
            } else {
                prev->next = temp->next;
                delete temp;
            }
        }else{
            cout << "Khong tim thay mon an co ID: " << Id << " de xoa!" << endl; 
        }

        return;
    }
    // Calculate the bill
    long TotalBill(){
        long total = 0;
        Node* temp = head;

        while( temp != nullptr ){
            total += temp->data.Value();
            temp = temp->next;
        }
        
        return total;
    }
    // In hoa don
    void PrintBill(){
        if (head == nullptr){
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
        
        Node* temp = head;
        while (temp != nullptr){
            temp->data.ShowFood();
            temp = temp->next;
        }
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

        Node* temp = head;
        while(temp != nullptr){
            file << left << setw(5) << temp->data.GetID() 
                << setw(20) << temp->data.GetTen() 
                << setw(8) << temp->data.GetSoluong()
                << setw(12) << temp->data.Value() << endl;
            temp = temp->next;
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
        while (head != nullptr){
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
    // destroy class
    ~Order(){
        while (head != nullptr){
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};
// Menu cua nha hang
class Menu{
private:
    Node* head;
public:
    Menu(){head = nullptr;}
    
    // Them mon an moi vao menu
    void AddtoMenu(Item newItem){
        Node* temp = head;
        while(temp != nullptr ){
            if (temp->data.GetID() == newItem.GetID()) return;
            temp = temp->next;
        }
        
        Node* newNode = new Node(newItem);
        newNode->next = head;
        head = newNode;

        return;
    }
    //Load du lieu menu tu file.
    void LoadMenufromFile(string filename){
        ifstream file(filename);
        if (!file.is_open()){
            cout << "Khong tim thay file: "<< filename << endl;
            return;
        }
        int id;
        string name;
        long gia;
        int count = 0;
        while(file >> id >> name >> gia){
            Item newItem(id, name, gia);
            AddtoMenu(newItem);
            count++;
        }
        cout << "Da nhap " << count << " mon."<< endl;
        file.close();                                                                                                   
    }
    // Hien thi menu 
    void PrintMenu(){
        cout << "======================== Menu Nha Hang ========================" << endl;
        cout << left << setw(5) << "ID" 
            << setw(20) << "Ten Mon" 
            << setw(15) << "Don Gia" << endl;
        Node* temp = head;
        while( temp != nullptr){
            temp->data.ShowFoodinMenu();
            temp = temp->next;
        }
        return;
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
        // them vao menu trong data
        SaveMenutoFile("Menu.txt");
        cout << " Them mon thanh cong! " << endl;
    }
    // luu du lieu vao file menu.txt
    void SaveMenutoFile(string filename){
        ofstream file(filename);
        if (!file.is_open()) return;

        Node* temp = head;
        while( temp != nullptr){
            // ghi file theo dinh dang
            file << temp->data.GetID() << " "
            << temp->data.GetTen() << " "
            << temp->data.GetGia() << endl;
            temp = temp->next;
        }
        file.close();
    }
    
    // Tim mon an theo ID
    Item* GetItembyId(int id){
        Node* temp = head;

        while (temp != nullptr && temp->data.GetID() != id){
            temp = temp->next;
        }

        if (temp == nullptr) return nullptr;
        return &(temp->data);
    }
};


// Main 
int main(){
    Order Bill;
    Menu nhahang;
    nhahang.LoadMenufromFile("L:\\Luong\\TotalBill\\Menu.txt");
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
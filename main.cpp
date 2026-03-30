#include <iostream>
#include <string>
#include <iomanip>
using namespace std;



// Đại diện một món ăn
class Item{
private:
    int id;
    string ten;
    long gia; 
    int soluong;

public:
    // Hàm khởi tạo 
    Item(int id = 0, string ten = "", long gia = 0, int soluong = 0){
        this->id = id;
        this->ten = ten;
        this->gia = gia > 0 ? gia : 0;
        this->soluong = soluong > 0? soluong : 1;
    }
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
        while (temp != nullptr && temp->data.GetID() == Id){
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
    void PrintBill(){
        if (head == nullptr){
            cout << "Order dang trong!!" << endl;
            return;
        }

        cout << "========================HOA DON TINH TAM========================" << endl;
        cout << left << setw(5) << "ID" 
             << setw(20) << "Ten Mon" 
             << setw(10) << "SL" 
             << setw(15) << "Don Gia" 
             << setw(15) << "Thanh Tien" << endl;
        cout << "----------------------------------------------------------------" << endl;
        Node* temp = head;
        while (temp != nullptr){
            temp->data.ShowFood();
            temp = temp->next;
        }
        cout << "-----------------------------------------------------------------" << endl;
        cout << "TONG CONG :" << TotalBill() <<" VND " << endl;
        cout << "-----------------------------------------------------------------" << endl;
    }
    void HienThiMenu(){
        cout << "======================== Menu ========================" << endl;
        cout << left << setw(5) << "ID" 
             << setw(20) << "Ten Mon" 
             << setw(15) << "Don Gia" << endl;
        Node* temp = head;
        while (temp != nullptr){
            temp->data.ShowFoodinMenu();
            temp = temp->next;
        }
        
    }
    // Delete the bill
    ~Order(){
        while (head != nullptr){
            Node* temp = head;
            head = head->next;
            delete temp;
        }
    }
};

// Main 
int main(){
    Order Bill;

    // Giả sử Menu có những món này
    Item trasua(1, "Tra Sua Chan Chau", 35000);
    Item cafe(2, "Cafe Sua Da", 25000);
    Item BanhMi(3, "Banh Mi Sai Gon", 30000);

    Bill.AddFood(trasua);
    Bill.AddFood(trasua);
    Bill.AddFood(cafe);
    Bill.AddFood(BanhMi);
    Bill.HienThiMenu();
    // In hoa don
    Bill.PrintBill();
    return 0;
}
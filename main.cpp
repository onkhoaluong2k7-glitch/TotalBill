#include <iostream>
#include <string>
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
    int getID(){return id;}
    string getTen(){return ten;}
    long getGia(){return gia;}
    int getSoluong(){return soluong;}
    // Hàm tăng/ giảm giá số lượng
    void TangSoluong(){soluong++;}
    void GiamSoluong(){if (soluong >= 1) soluong--;}
    // Hàm tính tiền
    long TingGiatien(){
        return gia*soluong;
    }
};

// Một node của linklist
struct Node{
    Item data;
    Node* next;
    Node(Item item) : data(item), next(nullptr){}
}

// Danh sách gọi món của khách hàng
class Order {
private:
    Node* head;


int main(){

    return 0;
}
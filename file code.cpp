#include <iostream>
#include <fstream> // nhập xuất mở đóng file
#include <iomanip> // căn chỉnh lề
using namespace std;
class Boat
{
protected:
	string boatID;
	string boatName;
	char Type;
	float Weight;
	bool isAvailable;
	int pricePerHour;
	float Seat;
public:
	static int totalBoat;
	static int total_Boatfree;
	Boat()
	{
		totalBoat++;
	}
	~Boat() 
	{
		totalBoat--;
	}
	void create_boat();
	void show_boat() const;
	void setAvailability(bool available);
	bool getAvailability() const;
};
void Boat::create_boat()
{
	system("CLS");
	cout << "\n\t\t\tEnter the Boat No. : ";
	cin.ignore();
	cin >>boatID ;
	cout << "\n\n\t\t\tEnter the Boat Name : ";
	cin.ignore();
	cin >> boatName;
	cout << "\n\t\t\tEnter Type of Boat : ";
	cin >> Type;
	Type = toupper(Type);
	cout << "\n\t\t\tEnter The Weight of Boat : ";
	cin >> Weight;
	cout << "\n\t\t\tEnter The Seat of Boat : ";
	cin >> Seat;
	cout << "\n\n\t\t\t New Boat information has been initialized...";
}
void Boat::show_boat() const
{
	cout << "\n\t\t\tBoat No. : ";
	cout<< boatID;
	cout << "\n\t\t\tBoat Name : ";
	cout << boatName;
	cout << "\n\t\t\tType of Boat : ";
	cout<< Type;
	cout << "\n\t\t\tEnter The Weight of Boat : ";
	cout<< Weight;
	cout << "\n\t\t\tEnter The Seat of Boat : ";
	cout<< Seat;
}
void Boat::setAvailability(bool available)
	{
	isAvailable = available;
}
bool Boat::getAvailability() const
	{
	return isAvailable;
}
int Boat::totalBoat = 0;
int Boat::total_Boatfree = 0;
class RentBoat : public Boat 
	{
	// giờ trà tính viết một cái class cho thuê Boat. giờ mình lấy lại giá trị Seat đã nhập ở trên rồi gán nó cho 1 cái biến mới gọi là
	// FreeSeat. Rồi mình viết chương trình để tạo ra câu lệnh đi thuê thuyền.
	// ví dụ như này:
	// 1. Nhập thông tin thuyền	
	// 2. thuê thuyền
	// 3. hủy thuê thuyền
	// 4. tra cứu thông tin thuyền còn dư
	// trà đang nói tới tính năng số 2 với số 3 á, chắc dùng class RentBoat để xử lý nó, mỗi lần thuê thì FreeRent mình -1, còn hủy thuê thì +1.

};

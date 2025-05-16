#include <iostream>
#include <fstream> // nhập xuất mở đóng file
#include <iomanip> // căn chỉnh lề
#include <cstdlib>  // rand() dùng để random
#include <ctime>    // time() dùng để đặt time từ 1/1/1970, dùng để random
#include <sstream>  // stringstream Chuyển đổi giữa số và chuỗi, xử lý chuỗi
using namespace std;
struct RentalRecord {
	string boatID;
	string ticketCode;
	float seatsRented;
	string action;
};
struct BoatRecord {
	string boatID;
	string boatName;
	char Type;
	float Weight;
	bool isAvailable;
	int pricePerHour;
	float Seat;
};
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
		isAvailable = true;
		totalBoat++;
		pricePerHour = 0;
	}
	~Boat() 
	{
		totalBoat--;
	}
	void create_boat();		// hàm tạo thuyền
	void show_boat() const;	// hàm show thông tin
	void setAvailability(bool available); // hàm đặt trạng thái của thuyền
	bool getAvailability() const;	// hàm get trạng thái của thuyền
	float getSeat() const;	// hàm get số lượng seat
};
// tổng hợp các hàm phục vụ
void displayHeader();		// hàm đặt tiêu đề, dùng để trang trí
bool isValidBoatType(char Type);		// hàm kiểm tra tính hợp lệ của nhập chữ
bool isValidNumber(const string& str);	// hàm kiểm tra tính hợp lệ của nhập số
void pressEnterToContinue();            
string getCurrentTimestamp();           

void displayHeader() {		
	cout << "\n\t\t\t======================================";
	cout << "\n\t\t\t      Boat Rental Management System    ";
	cout << "\n\t\t\t======================================\n";
}
bool isValidBoatType(char type) {
	return (type == 'S' || type == 'M' || type == 'L');
}
bool isValidNumber(const string& str) { 
	try {
		size_t pos;
		stof(str, &pos);	// stof chuyển kiểu dữ liệu str thành float, sau đó gán giá trị cuối cùng vào địa chỉ của pos
		return pos == str.length();		// trả pos về độ dài của length
	}
	catch (...) {
		return false;
	}
}
// các hàm của Boat
void Boat::create_boat()		// hàm khởi tạo cho con thuyền
{
	system("CLS");			// xóa màn hình
	displayHeader();
	// nhập ID thuyền
	do {
		cout << "\t\t\tEnter Boat No. (e.g., B001): ";
		cin.ignore();
		getline(cin, boatID);
		if (boatID.empty() || boatID.length() > 10) {		// .empty() kiểm tra có trống k, .length() ktr độ dài
			cout << "\t\t\tError: Boat ID must be non-empty and up to 10 characters.\n";
		}
	} while (boatID.empty() || boatID.length() > 10);

	// nhập tên của thuyền
	do {
		cout << "\t\t\tEnter Boat Name: ";
		getline(cin, boatName);
		if (boatName.empty()) {
			cout << "\t\t\tError: Boat name cannot be empty.\n";
		}
	} while (boatName.empty());			// làm lại cho đến khi boatName ko empty

	cout << "\n\n\t\t\tEnter the Boat Name : ";
	cin >> boatName;
	// lệnh nhập loại thuyền gồm S, M và L
	do {
		cout << "\t\t\tEnter Type of Boat (S/M/L): ";
		cin >> Type;
		Type = toupper(Type);
		if (!isValidBoatType(Type)) {
			cout << "\t\t\tError: Type must be S, M, or L.\n";
		}
	} while (!isValidBoatType(Type));
	// lệnh nhập khối lượng
	do {
		cout << "\t\t\tEnter Weight of Boat (tons): ";
		string input;
		cin >> input;
		if (!isValidNumber(input) || stof(input) <= 0) {
			cout << "\t\t\tError: Weight must be a positive number.\n";
		}
		else {
			Weight = stof(input);
			break;
		}
	} while (true);
	// lệnh nhập số lượng ghế
	do {
		cout << "\n\t\t\tEnter The Seat of Boat : ";
		string input;
		cin >> input;
		if (!isValidNumber(input) || stof(input) <= 0) {
			cout << "\t\t\tError: Seat must be a positive number. \n";
		}
		else {
			Seat = stof(input);
		}
	} while (true);
	do {
		cout << "\n\t\t\tEnter price per hour (VND) : ";
		string input;
		cin >> input;
		if (!isValidNumber(input) || stof(input) <= 0) {
			cout << "\t\t\tError: Price must be a positive number. \n";
		}
		else {
			pricePerHour = stof(input);
			break;
		}
	} while (true);
	cout << "\n\n\t\t\t New Boat information has been initialized...";
}
// hàm hiển thị thông tin của con thuyền
void Boat::show_boat() const {
	cout << "\n\t\t\t---------------- Boat Details ----------------";
	cout << "\n\t\t\tBoat No.      : " << boatID;
	cout << "\n\t\t\tBoat Name     : " << boatName;
	cout << "\n\t\t\tType          : " << Type;
	cout << "\n\t\t\tWeight        : " << fixed << setprecision(2) << Weight << " tons";
	cout << "\n\t\t\tSeats         : " << Seat;
	cout << "\n\t\t\tPrice/Hour    : $" << pricePerHour;
	cout << "\n\t\t\tAvailable     : " << (isAvailable ? "Yes" : "No");
	cout << "\n\t\t\t----------------------------------------------";
}
void Boat::setAvailability(bool available)		// hàm set giá trị để con thuyền hoạt động hay không, đồng thời cộng giá trị thuyền available
	{
	if (isAvailable != available) {
		isAvailable = available;
		total_Boatfree += available ? 1 : -1;
	}
}
bool Boat::getAvailability() const				// hàm get giá trị của con thuyền, xem nó có hoạt động hay không
	{
	return isAvailable;
}
float Boat::getSeat() const {					// hàm get giá trị của số ghế mặc định của thuyền
	return Seat;
}
int Boat::totalBoat = 0;						// khởi tạo giá trị của 2 biến static
int Boat::total_Boatfree = 0;

// class mới, phục vụ cho thuê
class RentBoat : public Boat					
{ 
protected:	
	float FreeSeat;								// số ghế trống
	string ticketCode;							// mã vé
public:
		 RentBoat() : FreeSeat(0), ticketCode("") {}		//hàm khởi tạo
	void initialize_free_seat();				// hàm đặt giá trị cho free seat
	void rentSeat(float seats,float hours);						// hàm thuê n seats, nếu isAvailable = true và FreeSeat lớn hơn số ghế còn trống thì cho thuê
	void show_remaining_seat() const;				// xuất ra số ghế còn trống
	string generateTicketCode();					// hàm random ticket code, dùng để đặt 1 mã bất kì cho vé
	void get_FreeSeat() const;
	void get_TicketCode() const;
	void cancel_rent_boat(float seats);						// hàm hủy thuê vé
};
// các hàm của class RentBoat

// hàm đặt giá trị freeseat= số seat ban đầu
void RentBoat::initialize_free_seat() {	
	FreeSeat = Seat;
}
// hàm cho thuê, chứa 2 giá trị đầu vào là seats và giờ cần thuê  
void RentBoat::rentSeat(float seats,float hours) {	
	if (seats <= 0) {
		cout << "\n\t\t\tError: Number of seats must be positive.\n";
		return;
	}
	if (isAvailable && FreeSeat >= seats) {
		FreeSeat -= seats;
		ticketCode = generateTicketCode();
		cout << "\n\t\t\tSuccessfully rented " << seats << " seats.";
		cout << "\n\t\t\tYour ticket code is: " << ticketCode;
		cout << "\n\t\t\tTotal cost : VND " << seats * pricePerHour* hours << endl;
		if (FreeSeat == 0) {
			setAvailability(false);
		}
		RentalRecord record = { boatID, ticketCode, seats , "Rent" };
		ofstream out("HISTORY_FILE.txt", ios::app);
		if (out) {
			out << record.boatID << endl;
			out << record.ticketCode << endl;
			out << record.seatsRented << endl;
			out << record.action << endl;
			out.close();
		}
	}
	else {
		cout << "\n\t\t\tError: Not enough seats available or boat is not available.\n";
	}
}
// hàm in thông tin số ghế dư
void RentBoat:: show_remaining_seat() const		
{
	cout << "\n\t\t\tRemaining Free Seats: " << FreeSeat << endl;
}
// hàm tạo mã vé
string RentBoat::generateTicketCode() {
	int code = rand() % 100;	// rand() random số bất kì, nếu k có srand(time(0)) thì các ticketcode sẽ như nhau
	stringstream ss;
	ss <<setw(3)<<setfill('0') << code;
	ticketCode = ss.str();	// trả về kiểu dữ liệu string
	return ticketCode;
}
void RentBoat::cancel_rent_boat(float seats) {
	if (seats <= 0) {
		cout << "\n\t\t\tNumber of seats to cancel must be positive. \n";
	}
	if (seats <= (Seat - FreeSeat)) {
		FreeSeat += seats;
		setAvailability(true);
		string oldTicket = ticketCode;
		ticketCode = "";
		cout << "\n\t\t\tSuccessfully cancelled " << seats << " seats.\n";
		RentalRecord record = { boatID, oldTicket, seats,  "Cancel" };
		ofstream out("HISTORY_FILE.txt", ios::app);
		if (out) {
			out << record.boatID << endl;
			out << record.ticketCode << endl;
			out << record.seatsRented << endl;
			out << record.action << endl;
			out.close();
		}
	}
	else {
		cout << "\n\t\t\tError: Cannot cancel more seats than rented.\n";
	}
}


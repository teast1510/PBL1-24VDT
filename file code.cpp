#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <limits>
#include <algorithm>
#include <map>
#define MAX_BOATS 100
using namespace std;

// Cấu trúc RentalRecord
struct RentalRecord {
    string boatID;
    string ticketCode;
    float seatsRented;
    int shiftIndex; // Chỉ số ca thuê
    string action; // "RENT" hoặc "CANCEL"
    string timestamp; // Thời gian giả định
};

// Cấu trúc Shift
struct Shift {
    int startHour;
    int endHour;
    float pricePerHour;
    float seatsBooked; // Số ghế đã được thuê trong ca
    Shift() : startHour(0), endHour(0), pricePerHour(0), seatsBooked(0) {}
};

// Lớp Boat
class Boat {
protected:
    string boatID;
    string boatName;
    char Type;
    float Weight;
    bool isAvailable;
    float Seat;
    int numberOfTrip;
    vector<Shift> timeTrips;
    int rentalCount; // Đếm số lần thuê
public:
    static int totalBoat;
    static int total_Boatfree;
    Boat() {
        isAvailable = true;
        rentalCount = 0;
        totalBoat++;
        total_Boatfree++;
    }
    ~Boat() {
        totalBoat--;
        if (isAvailable) total_Boatfree--;
    }
    void create_boat();
    void show_boat() const;
    void setAvailability(bool available) {
        if (isAvailable != available) {
            isAvailable = available;
            total_Boatfree += available ? 1 : -1;
        }
    }
    bool getAvailability() const { return isAvailable; }
    float getSeat() const { return Seat; }
    string getBoatID() const { return boatID; }
    int getRentalCount() const { return rentalCount; }
    void incrementRentalCount() { rentalCount++; }
    const vector<Shift>& getTimeTrips() const { return timeTrips; }
    int getNumberOfTrips() const { return numberOfTrip; }
    void bookSeats(int shiftIndex, float seats);
    void cancelSeats(int shiftIndex, float seats);
    void saveToFile(ofstream& outFile) const;
    void loadFromFile(ifstream& inFile);
};

// Khởi tạo static members
int Boat::totalBoat = 0;
int Boat::total_Boatfree = 0;

// Hàm hỗ trợ
void displayHeader() {
    cout << "\n\t\t\t======================================";
    cout << "\n\t\t\t      HỆ THỐNG QUẢN LÝ THUÊ THUYỀN    ";
    cout << "\n\t\t\t======================================\n";
}

bool isValidBoatType(char type) {
    return (type == 'S' || type == 'M' || type == 'L');
}

bool isValidNumber(const string& str) {
    try {
        size_t pos;
        stof(str, &pos);
        return pos == str.length();
    }
    catch (...) {
        return false;
    }
}

void pressEnterToContinue() {
    cout << "\n\t\t\tNhấn Enter để tiếp tục...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void displayMenu() {
    system("CLS");
    displayHeader();
    cout << "\n\t\t\t1. Thêm thuyền mới";
    cout << "\n\t\t\t2. Hiển thị tất cả thuyền";
    cout << "\n\t\t\t3. Thuê ghế";
    cout << "\n\t\t\t4. Hủy thuê";
    cout << "\n\t\t\t5. Hiển thị thuyền sẵn sàng";
    cout << "\n\t\t\t6. Hiển thị lịch sử thuê";
    cout << "\n\t\t\t7. Hiển thị thuyền được thuê nhiều nhất";
    cout << "\n\t\t\t8. Tổng hợp số ghế thuê của một thuyền";
    cout << "\n\t\t\t9. Thoát";
    cout << "\n\t\t\tNhập lựa chọn (1-9): ";
}

string generateTicketCode() {
    static int counter = 0;
    stringstream ss;
    ss << "TICKET" << setw(4) << setfill('0') << counter++;
    return ss.str();
}

string getTimestamp() {
    static int counter = 0;
    stringstream ss;
    ss << "2025-05-17 " << setw(2) << setfill('0') << counter++ << ":00:00";
    return ss.str();
}

// Hàm tạo thuyền
void Boat::create_boat() {
    system("CLS");
    displayHeader();

    // Nhập ID thuyền
    do {
        cout << "\t\t\tNhập mã thuyền (VD: B001): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, boatID);
        if (boatID.empty() || boatID.length() > 10) {
            cout << "\t\t\tLỗi: Mã thuyền không được trống và tối đa 10 ký tự.\n";
        }
    } while (boatID.empty() || boatID.length() > 10);

    // Nhập tên thuyền
    do {
        cout << "\t\t\tNhập tên thuyền: ";
        getline(cin, boatName);
        if (boatName.empty()) {
            cout << "\t\t\tLỗi: Tên thuyền không được trống.\n";
        }
    } while (boatName.empty());

    // Nhập loại thuyền
    do {
        cout << "\t\t\tNhập loại thuyền (S/M/L): ";
        cin >> Type;
        Type = toupper(Type);
        if (!isValidBoatType(Type)) {
            cout << "\t\t\tLỗi: Loại thuyền phải là S, M hoặc L.\n";
        }
    } while (!isValidBoatType(Type));

    // Nhập khối lượng
    do {
        cout << "\t\t\tNhập khối lượng thuyền (tấn): ";
        string input;
        cin >> input;
        if (!isValidNumber(input) || stof(input) <= 0) {
            cout << "\t\t\tLỗi: Khối lượng phải là số dương.\n";
        }
        else {
            Weight = stof(input);
            break;
        }
    } while (true);

    // Nhập số ghế
    do {
        cout << "\t\t\tNhập số ghế của thuyền: ";
        string input;
        cin >> input;
        if (!isValidNumber(input) || stof(input) <= 0) {
            cout << "\t\t\tLỗi: Số ghế phải là số dương.\n";
        }
        else {
            Seat = stof(input);
            break;
        }
    } while (true);

    // Nhập số lượng ca
    do {
        cout << "\t\t\tNhập số lượng ca: ";
        string input;
        cin >> input;
        if (!isValidNumber(input) || stoi(input) <= 0) {
            cout << "\t\t\tLỗi: Số ca phải là số dương.\n";
        }
        else {
            numberOfTrip = stoi(input);
            break;
        }
    } while (true);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    timeTrips.resize(numberOfTrip);

    // Nhập chi tiết từng ca
    for (int i = 0; i < numberOfTrip; i++) {
        bool validShift = false;
        while (!validShift) {
            cout << "\t\t\tNhập chi tiết ca " << i + 1 << ":\n";

            // Nhập giờ bắt đầu
            do {
                cout << "\t\t\tNhập giờ bắt đầu (0-23): ";
                string input;
                cin >> input;
                if (!isValidNumber(input) || stoi(input) < 0 || stoi(input) > 23) {
                    cout << "\t\t\tLỗi: Giờ bắt đầu phải từ 0 đến 23.\n";
                }
                else {
                    timeTrips[i].startHour = stoi(input);
                    break;
                }
            } while (true);

            // Nhập giờ kết thúc
            do {
                cout << "\t\t\tNhập giờ kết thúc (0-23): ";
                string input;
                cin >> input;
                if (!isValidNumber(input) || stoi(input) <= timeTrips[i].startHour || stoi(input) > 23) {
                    cout << "\t\t\tLỗi: Giờ kết thúc phải sau giờ bắt đầu và từ 0 đến 23.\n";
                }
                else {
                    timeTrips[i].endHour = stoi(input);
                    break;
                }
            } while (true);

            // Kiểm tra trùng ca
            validShift = true;
            for (int j = 0; j < i; j++) {
                if (timeTrips[i].startHour < timeTrips[j].endHour && timeTrips[i].endHour > timeTrips[j].startHour) {
                    cout << "\t\t\tLỗi: Ca này trùng với ca " << j + 1 << ". Vui lòng nhập lại.\n";
                    validShift = false;
                    break;
                }
            }

            if (validShift) {
                // Nhập giá mỗi giờ
                do {
                    cout << "\t\t\tNhập giá mỗi giờ: ";
                    string input;
                    cin >> input;
                    if (!isValidNumber(input) || stof(input) <= 0) {
                        cout << "\t\t\tLỗi: Giá phải là số dương.\n";
                    }
                    else {
                        timeTrips[i].pricePerHour = stof(input);
                        break;
                    }
                } while (true);
            }
        }
    }

    isAvailable = true;
    cout << "\n\t\t\tThông tin thuyền mới đã được khởi tạo...";
    pressEnterToContinue();
}

// Hàm hiển thị thông tin thuyền
void Boat::show_boat() const {
    cout << "\t\t\tMã thuyền: " << boatID << endl;
    cout << "\t\t\tTên thuyền: " << boatName << endl;
    cout << "\t\t\tLoại thuyền: " << Type << endl;
    cout << "\t\t\tKhối lượng: " << Weight << " tấn" << endl;
    cout << "\t\t\tSố ghế: " << Seat << endl;
    cout << "\t\t\tTrạng thái: " << (isAvailable ? "Sẵn sàng" : "Không sẵn sàng") << endl;
    cout << "\t\t\tSố ca: " << numberOfTrip << endl;
    for (int i = 0; i < numberOfTrip; i++) {
        cout << "\t\t\tCa " << i + 1 << ": " << timeTrips[i].startHour << "h-"
            << timeTrips[i].endHour << "h, Giá: " << timeTrips[i].pricePerHour
            << "/giờ, Ghế đã thuê: " << timeTrips[i].seatsBooked << endl;
    }
    cout << "\t\t\tSố lần thuê: " << rentalCount << endl;
}

// Hàm lưu thuyền vào file
void Boat::saveToFile(ofstream& outFile) const {
    outFile << boatID << endl;
    outFile << boatName << endl;
    outFile << Type << endl;
    outFile << Weight << endl;
    outFile << isAvailable << endl;
    outFile << Seat << endl;
    outFile << numberOfTrip << endl;
    for (const auto& shift : timeTrips) {
        outFile << shift.startHour << " " << shift.endHour << " "
            << shift.pricePerHour << " " << shift.seatsBooked << endl;
    }
    outFile << rentalCount << endl;
}

// Hàm đọc thuyền từ file
void Boat::loadFromFile(ifstream& inFile) {
    getline(inFile, boatID);
    getline(inFile, boatName);
    inFile >> Type >> Weight >> isAvailable >> Seat >> numberOfTrip;
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    timeTrips.resize(numberOfTrip);
    for (int i = 0; i < numberOfTrip; i++) {
        inFile >> timeTrips[i].startHour >> timeTrips[i].endHour
            >> timeTrips[i].pricePerHour >> timeTrips[i].seatsBooked;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    inFile >> rentalCount;
    inFile.ignore(numeric_limits<streamsize>::max(), '\n');
}

// Hàm thuê ghế
void Boat::bookSeats(int shiftIndex, float seats) {
    if (shiftIndex >= 0 && shiftIndex < numberOfTrip) {
        timeTrips[shiftIndex].seatsBooked += seats;
        if (timeTrips[shiftIndex].seatsBooked >= Seat) {
            setAvailability(false);
        }
        incrementRentalCount();
    }
}

// Hàm hủy ghế
void Boat::cancelSeats(int shiftIndex, float seats) {
    if (shiftIndex >= 0 && shiftIndex < numberOfTrip) {
        timeTrips[shiftIndex].seatsBooked -= seats;
        if (timeTrips[shiftIndex].seatsBooked < 0) timeTrips[shiftIndex].seatsBooked = 0;
        if (timeTrips[shiftIndex].seatsBooked < Seat) {
            setAvailability(true);
        }
    }
}

// Quản lý danh sách thuyền và lịch sử thuê
class BoatManager {
private:
    vector<Boat> boats;
    vector<RentalRecord> rentalHistory;
public:
    void addBoat() {
        Boat boat;
        boat.create_boat();
        boats.push_back(boat);
        saveBoatsToFile();
    }

    void displayAllBoats() const {
        system("CLS");
        displayHeader();
        if (boats.empty()) {
            cout << "\t\t\tKhông có thuyền nào.\n";
        }
        else {
            for (const auto& boat : boats) {
                boat.show_boat();
                cout << "\t\t\t-------------------\n";
            }
        }
        pressEnterToContinue();
    }

    void rentSeats() {
        system("CLS");
        displayHeader();
        string boatID;
        cout << "\t\t\tNhập mã thuyền (VD: B001): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, boatID);

        auto it = find_if(boats.begin(), boats.end(),
            [&boatID](const Boat& b) { return b.getBoatID() == boatID; });

        if (it == boats.end()) {
            cout << "\t\t\tLỗi: Không tìm thấy thuyền.\n";
            pressEnterToContinue();
            return;
        }

        if (!it->getAvailability()) {
            cout << "\t\t\tLỗi: Thuyền không sẵn sàng.\n";
            pressEnterToContinue();
            return;
        }

        cout << "\t\t\tCác ca có sẵn:\n";
        const auto& shifts = it->getTimeTrips();
        for (int i = 0; i < it->getNumberOfTrips(); i++) {
            cout << "\t\t\tCa " << i + 1 << ": " << shifts[i].startHour << "h-"
                << shifts[i].endHour << "h, Ghế còn: "
                << it->getSeat() - shifts[i].seatsBooked << endl;
        }

        int shiftIndex;
        do {
            cout << "\t\t\tNhập số thứ tự ca (1-" << it->getNumberOfTrips() << "): ";
            string input;
            cin >> input;
            if (!isValidNumber(input) || stoi(input) <= 0 || stoi(input) > it->getNumberOfTrips()) {
                cout << "\t\t\tLỗi: Ca không hợp lệ.\n";
            }
            else {
                shiftIndex = stoi(input) - 1;
                break;
            }
        } while (true);

        float seats;
        do {
            cout << "\t\t\tNhập số ghế muốn thuê: ";
            string input;
            cin >> input;
            if (!isValidNumber(input) || stof(input) <= 0 ||
                stof(input) > it->getSeat() - shifts[shiftIndex].seatsBooked) {
                cout << "\t\t\tLỗi: Số ghế không hợp lệ hoặc vượt quá số ghế còn lại.\n";
            }
            else {
                seats = stof(input);
                break;
            }
        } while (true);

        RentalRecord record;
        record.boatID = boatID;
        record.ticketCode = generateTicketCode();
        record.seatsRented = seats;
        record.shiftIndex = shiftIndex;
        record.action = "RENT";
        record.timestamp = getTimestamp();
        rentalHistory.push_back(record);

        it->bookSeats(shiftIndex, seats);
        saveRentalHistoryToFile();
        saveBoatsToFile();

        cout << "\t\t\tThuê thành công! Mã vé: " << record.ticketCode << endl;
        pressEnterToContinue();
    }

    void cancelRent() {
        system("CLS");
        displayHeader();
        string ticketCode;
        cout << "\t\t\tNhập mã vé: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, ticketCode);

        auto it = find_if(rentalHistory.begin(), rentalHistory.end(),
            [&ticketCode](const RentalRecord& r) { return r.ticketCode == ticketCode && r.action == "RENT"; });

        if (it == rentalHistory.end()) {
            cout << "\t\t\tLỗi: Không tìm thấy vé hợp lệ.\n";
            pressEnterToContinue();
            return;
        }

        auto boatIt = find_if(boats.begin(), boats.end(),
            [&it](const Boat& b) { return b.getBoatID() == it->boatID; });

        if (boatIt == boats.end()) {
            cout << "\t\t\tLỗi: Không tìm thấy thuyền liên quan.\n";
            pressEnterToContinue();
            return;
        }

        RentalRecord record = *it;
        record.action = "CANCEL";
        record.timestamp = getTimestamp();
        rentalHistory.push_back(record);

        boatIt->cancelSeats(it->shiftIndex, it->seatsRented);
        saveRentalHistoryToFile();
        saveBoatsToFile();

        cout << "\t\t\tHủy thuê thành công!\n";
        pressEnterToContinue();
    }

    void showAvailableBoats() const {
        system("CLS");
        displayHeader();
        bool found = false;
        for (const auto& boat : boats) {
            if (boat.getAvailability()) {
                boat.show_boat();
                cout << "\t\t\t-------------------\n";
                found = true;
            }
        }
        if (!found) {
            cout << "\t\t\tKhông có thuyền sẵn sàng.\n";
        }
        pressEnterToContinue();
    }

    void displayRentalHistory() const {
        system("CLS");
        displayHeader();
        if (rentalHistory.empty()) {
            cout << "\t\t\tKhông có lịch sử thuê.\n";
        }
        else {
            for (const auto& record : rentalHistory) {
                cout << "\t\t\tMã thuyền: " << record.boatID << endl;
                cout << "\t\t\tMã vé: " << record.ticketCode << endl;
                cout << "\t\t\tSố ghế: " << record.seatsRented << endl;
                cout << "\t\t\tCa: " << record.shiftIndex + 1 << endl;
                cout << "\t\t\tHành động: " << record.action << endl;
                cout << "\t\t\tThời gian: " << record.timestamp << endl;
                cout << "\t\t\t-------------------\n";
            }
        }
        pressEnterToContinue();
    }

    void showBestChoiceBoat() const {
        system("CLS");
        displayHeader();
        if (boats.empty()) {
            cout << "\t\t\tKhông có thuyền nào.\n";
        }
        else {
            auto maxBoat = max_element(boats.begin(), boats.end(),
                [](const Boat& a, const Boat& b) { return a.getRentalCount() < b.getRentalCount(); });
            cout << "\t\t\tThuyền được thuê nhiều nhất:\n";
            maxBoat->show_boat();
        }
        pressEnterToContinue();
    }

    void showSeatsPerShift() const {
        system("CLS");
        displayHeader();
        string boatID;
        cout << "\t\t\tNhập mã thuyền (VD: B001): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, boatID);

        auto it = find_if(boats.begin(), boats.end(),
            [&boatID](const Boat& b) { return b.getBoatID() == boatID; });

        if (it == boats.end()) {
            cout << "\t\t\tLỗi: Không tìm thấy thuyền.\n";
            pressEnterToContinue();
            return;
        }

        cout << "\t\t\tTổng hợp số ghế thuê của thuyền " << boatID << ":\n";
        const auto& shifts = it->getTimeTrips();
        for (int i = 0; i < it->getNumberOfTrips(); i++) {
            cout << "\t\t\tCa " << i + 1 << " (" << shifts[i].startHour << "h-"
                << shifts[i].endHour << "h): " << shifts[i].seatsBooked << " ghế\n";
        }
        pressEnterToContinue();
    }

    void saveBoatsToFile() const {
        ofstream outFile("boats.txt");
        outFile << boats.size() << endl;
        for (const auto& boat : boats) {
            boat.saveToFile(outFile);
        }
        outFile.close();
    }

    void loadBoatsFromFile() {
        ifstream inFile("boats.txt");
        if (!inFile.is_open()) return;
        int numBoats;
        inFile >> numBoats;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n');
        boats.clear();
        for (int i = 0; i < numBoats; i++) {
            Boat boat;
            boat.loadFromFile(inFile);
            boats.push_back(boat);
        }
        inFile.close();
    }

    void saveRentalHistoryToFile() const {
        ofstream outFile("rental_history.txt");
        outFile << rentalHistory.size() << endl;
        for (const auto& record : rentalHistory) {
            outFile << record.boatID << endl;
            outFile << record.ticketCode << endl;
            outFile << record.seatsRented << endl;
            outFile << record.shiftIndex << endl;
            outFile << record.action << endl;
            outFile << record.timestamp << endl;
        }
        outFile.close();
    }

    void loadRentalHistoryFromFile() {
        ifstream inFile("rental_history.txt");
        if (!inFile.is_open()) return;
        int numRecords;
        inFile >> numRecords;
        inFile.ignore(numeric_limits<streamsize>::max(), '\n');
        rentalHistory.clear();
        for (int i = 0; i < numRecords; i++) {
            RentalRecord record;
            getline(inFile, record.boatID);
            getline(inFile, record.ticketCode);
            inFile >> record.seatsRented >> record.shiftIndex;
            inFile.ignore(numeric_limits<streamsize>::max(), '\n');
            getline(inFile, record.action);
            getline(inFile, record.timestamp);
            rentalHistory.push_back(record);
        }
        inFile.close();
    }
};

// Hàm chính
int main() {
    BoatManager manager;
    manager.loadBoatsFromFile();
    manager.loadRentalHistoryFromFile();
    int choice;

    do {
        displayMenu();
        string input;
        cin >> input;
        if (!isValidNumber(input)) {
            cout << "\t\t\tLỗi: Lựa chọn phải là số.\n";
            pressEnterToContinue();
            continue;
        }
        choice = stoi(input);

        switch (choice) {
        case 1:
            manager.addBoat();
            break;
        case 2:
            manager.displayAllBoats();
            break;
        case 3:
            manager.rentSeats();
            break;
        case 4:
            manager.cancelRent();
            break;
        case 5:
            manager.showAvailableBoats();
            break;
        case 6:
            manager.displayRentalHistory();
            break;
        case 7:
            manager.showBestChoiceBoat();
            break;
        case 8:
            manager.showSeatsPerShift();
            break;
        case 9:
            cout << "\t\t\tThoát chương trình.\n";
            pressEnterToContinue();
            break;
        default:
            cout << "\t\t\tLựa chọn không hợp lệ.\n";
            pressEnterToContinue();
        }
    } while (choice != 9);

    return 0;
}

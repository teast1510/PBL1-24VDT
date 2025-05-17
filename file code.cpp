#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstdlib>
#include <sstream>
#include <vector>
#include <limits>
#include <algorithm>
#include <map>
#include <ctime>
#include <string>
#define MAX_BOATS 100
using namespace std;

// Cấu trúc RentalRecord
struct RentalRecord {
    string boatID;
    string ticketCode;
    float seatsRented;
    int shiftIndex;
    string action; // "RENT" or "CANCEL"
};

// Cấu trúc Shift
struct Shift {
    int startHour;
    int endHour;
    float pricePerHour;
    float seatsBooked;
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
    int rentalCount;
public:
    static int totalBoat;
    static int total_Boatfree;
    Boat() : isAvailable(true), rentalCount(0), Seat(0), numberOfTrip(0) {
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
    cout << "\n\t\t\t      BOAT RENTAL MANAGEMENT SYSTEM     ";
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
    cout << "\n\t\t\tPress Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}

void displayMenu() {
    system("CLS");
    displayHeader();
    cout << "\n\t\t\t1. Add new boat";
    cout << "\n\t\t\t2. Display all boats";
    cout << "\n\t\t\t3. Rent Seat";
    cout << "\n\t\t\t4. Cancel Rented Seat";
    cout << "\n\t\t\t5. Display Available Boats";
    cout << "\n\t\t\t6. Display Rental History";
    cout << "\n\t\t\t7. Display The Most Rented Boats";
    cout << "\n\t\t\t8. Total number of seats rented on a boat";
    cout << "\n\t\t\t9. End system";
    cout << "\n\t\t\tEnter selection (1-9): ";
}

// Hàm tạo mã vé ngẫu nhiên với kiểm tra trùng lặp
string generateTicketCode(const vector<RentalRecord>& rentalHistory) {
    static bool initialized = false;
    if (!initialized) {
        srand(static_cast<unsigned int>(time(0)));
        initialized = true;
    }
    const string chars = "ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    string ticket;
    bool unique;
    do {
        ticket = "TICKET";
        for (int i = 0; i < 6; i++) {
            ticket += chars[rand() % chars.length()];
        }
        unique = true;
        for (const auto& record : rentalHistory) {
            if (record.ticketCode == ticket) {
                unique = false;
                break;
            }
        }
    } while (!unique);
    return ticket;
}

// Hàm tạo thuyền
void Boat::create_boat() {
    system("CLS");
    displayHeader();

    do {
        cout << "\t\t\tEnter Boat ID (VD: B001): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, boatID);
        if (boatID.empty() || boatID.length() > 10) {
            cout << "\t\t\tError: Boat code cannot be empty and maximum 10 characters.\n";
        }
    } while (boatID.empty() || boatID.length() > 10);

    do {
        cout << "\t\t\tEnter Boat name: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, boatName);
        if (boatName.empty() || boatName.length() > 10) {
        cout << "\t\t\tError: Boat name cannot be empty.\n";
    }
    } while (boatName.empty());

    do {
        cout << "\t\t\tEnter Boat Type (S/M/L): ";
        cin >> Type;
        Type = toupper(Type);
        if (!isValidBoatType(Type)) {
            cout << "\t\t\tError: Boat type must be S, M or L.\n";
        }
    } while (!isValidBoatType(Type));

    do {
        cout << "\t\t\tEnter boat weight (tons): ";
        string input;
        cin >> input;
        if (!isValidNumber(input) || stof(input) <= 0) {
            cout << "\t\t\tError: Boat Weight must be positive number.\n";
        }
        else {
            Weight = stof(input);
            break;
        }
    } while (true);

    do {
        cout << "\t\t\tEnter the number of seats on the boat: ";
        string input;
        cin >> input;
        if (!isValidNumber(input) || stof(input) <= 0) {
            cout << "\t\t\tError: Seat number must be positive.\n";
        }
        else {
            Seat = stof(input);
            break;
        }
    } while (true);

    do {
        cout << "\t\t\tEnter the Shift Number: ";
        string input;
        cin >> input;
        if (!isValidNumber(input) || stoi(input) <= 0) {
            cout << "\t\t\tError: The Shift Number must be positive.\n";
        }
        else {
            numberOfTrip = stoi(input);
            break;
        }
    } while (true);

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    timeTrips.resize(numberOfTrip);

    for (int i = 0; i < numberOfTrip; i++) {
        bool validShift = false;
        while (!validShift) {
            cout << "\t\t\tEnter shift details: " << i + 1 << " \n";

            do {
                cout << "\t\t\tEnter start time (0-23): ";
                string input;
                cin >> input;
                if (!isValidNumber(input) || stoi(input) < 0 || stoi(input) > 23) {
                    cout << "\t\t\tError: Start time must be between 0 and 23.\n";
                }
                else {
                    timeTrips[i].startHour = stoi(input);
                    break;
                }
            } while (true);

            do {
                cout << "\t\t\tEnter end time (0-23): ";
                string input;
                cin >> input;
                if (!isValidNumber(input) || stoi(input) <= timeTrips[i].startHour || stoi(input) > 23) {
                    cout << "\t\t\tError: End time must be after start time and between 0 and 23.\n";
                }
                else {
                    timeTrips[i].endHour = stoi(input);
                    break;
                }
            } while (true);

            validShift = true;
            for (int j = 0; j < i; j++) {
                if (timeTrips[i].startHour < timeTrips[j].endHour && timeTrips[i].endHour > timeTrips[j].startHour) {
                    cout << "\t\t\tError: This shift overlaps with shift " << j + 1 << ". Please Re-Enter.\n";
                    validShift = false;
                    break;
                }
            }

            if (validShift) {
                do {
                    cout << "\t\t\tEnter Price per Hour: ";
                    string input;
                    cin >> input;
                    if (!isValidNumber(input) || stof(input) <= 0) {
                        cout << "\t\t\tError: Price must be positive\n";
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
    cout << "\n\t\t\tNew boat information has been initialized...";
    pressEnterToContinue();
}

// Hàm hiển thị thông tin thuyền
void Boat::show_boat() const {
    cout << "\t\t\tBoat ID: " << boatID << endl;
    cout << "\t\t\tBoat Name: " << boatName << endl;
    cout << "\t\t\tBoat Type: " << Type << endl;
    cout << "\t\t\tWeight: " << Weight << " ton(s)" << endl;
    cout << "\t\t\tNumber of Seats: " << Seat << endl;
    cout << "\t\t\tStatus: " << (isAvailable ? "Active" : "InActive") << endl;
    cout << "\t\t\tNumber of Shifts: " << numberOfTrip << endl;
    for (int i = 0; i < numberOfTrip; i++) {
        cout << "\t\t\tShift " << i + 1 << ": " << timeTrips[i].startHour << "h-"
            << timeTrips[i].endHour << "h, Price per Hour: " << timeTrips[i].pricePerHour
            << "/Hour, Number of Seats Rented: " << timeTrips[i].seatsBooked << endl;
    }
    cout << "\t\t\tNumber of rentals: " << rentalCount << endl;
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
        // Check if any shift still has full seats
        bool fullyBooked = false;
        for (const auto& shift : timeTrips) {
            if (shift.seatsBooked >= Seat) {
                fullyBooked = true;
                break;
            }
        }
        if (!fullyBooked) {
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
            cout << "\t\t\tThere is no boat.\n";
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
        cout << "\t\t\tEnter boat code (eg: B001): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, boatID);

        auto it = find_if(boats.begin(), boats.end(),
            [&boatID](const Boat& b) { return b.getBoatID() == boatID; });

        if (it == boats.end()) {
            cout << "\t\t\tError: Boat not found.\n";
            pressEnterToContinue();
            return;
        }

        Boat& boat = *it; // Use reference to avoid iterator invalidation

        if (!boat.getAvailability()) {
            cout << "\t\t\tError: Boat not ready.\n";
            pressEnterToContinue();
            return;
        }

        cout << "\t\t\tAvailable shifts:\n";
        const auto& shifts = boat.getTimeTrips();
        for (int i = 0; i < boat.getNumberOfTrips(); i++) {
            cout << "\t\t\tShift " << i + 1 << ": " << shifts[i].startHour << "h-"
                << shifts[i].endHour << "h, Seats Remaining: "
                << boat.getSeat() - shifts[i].seatsBooked << endl;
        }

        int shiftIndex;
        do {
            cout << "\t\t\tEnter shift number (1-" << boat.getNumberOfTrips() << "): ";
            string input;
            cin >> input;
            if (!isValidNumber(input) || stoi(input) <= 0 || stoi(input) > boat.getNumberOfTrips()) {
                cout << "\t\t\tError: Invalid shift.\n";
            }
            else {
                shiftIndex = stoi(input) - 1;
                break;
            }
        } while (true);

        float seats;
        do {
            cout << "\t\t\tEnter the number of seats you want to rent: ";
            string input;
            cin >> input;
            if (!isValidNumber(input) || stof(input) <= 0 ||
                stof(input) > boat.getSeat() - shifts[shiftIndex].seatsBooked) {
                cout << "\t\t\tError: Invalid seat number or exceeds remaining seats.\n";
            }
            else {
                seats = stof(input);
                break;
            }
        } while (true);

        RentalRecord record;
        record.boatID = boatID;
        record.ticketCode = generateTicketCode(rentalHistory);
        record.seatsRented = seats;
        record.shiftIndex = shiftIndex;
        record.action = "RENT";
        rentalHistory.push_back(record);

        boat.bookSeats(shiftIndex, seats);
        saveRentalHistoryToFile();
        saveBoatsToFile();

        cout << "\t\t\tRental successful! Ticket code: " << record.ticketCode << endl;
        pressEnterToContinue();
    }

    void cancelRent() {
        system("CLS");
        displayHeader();
        string ticketCode;
        cout << "\t\t\tEnter ticket code: ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, ticketCode);

        auto it = find_if(rentalHistory.begin(), rentalHistory.end(),
            [&ticketCode](const RentalRecord& r) {
                return r.ticketCode == ticketCode && r.action == "RENT";
            });

        if (it == rentalHistory.end()) {
            cout << "\t\t\tError: No valid ticket found or ticket already cancelled.\n";
            pressEnterToContinue();
            return;
        }

        RentalRecord& record = *it; // Use reference

        auto boatIt = find_if(boats.begin(), boats.end(),
            [&record](const Boat& b) { return b.getBoatID() == record.boatID; });

        if (boatIt == boats.end()) {
            cout << "\t\t\tError: No related boat found.\n";
            pressEnterToContinue();
            return;
        }

        Boat& boat = *boatIt; // Use reference

        // Create cancellation record
        RentalRecord cancelRecord = record;
        cancelRecord.action = "CANCEL";
        rentalHistory.push_back(cancelRecord);

        // Update boat seats
        boat.cancelSeats(record.shiftIndex, record.seatsRented);

        // Update files
        saveRentalHistoryToFile();
        saveBoatsToFile();

        cout << "\t\t\tCancellation successful!\n";
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
            cout << "\t\t\tNo boat available.\n";
        }
        pressEnterToContinue();
    }

    void displayRentalHistory() const {
        system("CLS");
        displayHeader();
        if (rentalHistory.empty()) {
            cout << "\t\t\tNo rental history.\n";
        }
        else {
            for (const auto& record : rentalHistory) {
                cout << "\t\t\tBoat ID: " << record.boatID << endl;
                cout << "\t\t\tTicket Code: " << record.ticketCode << endl;
                cout << "\t\t\tNumber of Seats: " << record.seatsRented << endl;
                cout << "\t\t\tShift: " << record.shiftIndex + 1 << endl;
                cout << "\t\t\tAction: " << record.action << endl;
                cout << "\t\t\t-------------------\n";
            }
        }
        pressEnterToContinue();
    }

    void showBestChoiceBoat() const {
        system("CLS");
        displayHeader();
        if (boats.empty()) {
            cout << "\t\t\tNo Boat Suitable.\n";
        }
        else {
            auto maxBoat = max_element(boats.begin(), boats.end(),
                [](const Boat& a, const Boat& b) { return a.getRentalCount() < b.getRentalCount(); });
            cout << "\t\t\tMost rented boat:\n";
            maxBoat->show_boat();
        }
        pressEnterToContinue();
    }

    void showSeatsPerShift() const {
        system("CLS");
        displayHeader();
        string boatID;
        cout << "\t\t\tEnter Boat ID (VD: B001): ";
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        getline(cin, boatID);

        auto it = find_if(boats.begin(), boats.end(),
            [&boatID](const Boat& b) { return b.getBoatID() == boatID; });

        if (it == boats.end()) {
            cout << "\t\t\tError: Can't Find a Boat.\n";
            pressEnterToContinue();
            return;
        }

        const Boat& boat = *it;
        cout << "\t\t\tTotal number of rental seats on the boat " << boatID << ":\n";
        const auto& shifts = boat.getTimeTrips();
        for (int i = 0; i < boat.getNumberOfTrips(); i++) {
            cout << "\t\t\tShift " << i + 1 << " (" << shifts[i].startHour << "h-"
                << shifts[i].endHour << "h): " << shifts[i].seatsBooked << " Seats\n";
        }
        pressEnterToContinue();
    }

    void saveBoatsToFile() const {
        ofstream outFile("boats.txt");
        if (!outFile.is_open()) {
            cout << "\t\t\tError: Could not open boats.txt for writing.\n";
            return;
        }
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
        if (!outFile.is_open()) {
            cout << "\t\t\tError: Could not open rental_history.txt for writing.\n";
            return;
        }
        outFile << rentalHistory.size() << endl;
        for (const auto& record : rentalHistory) {
            outFile << record.boatID << endl;
            outFile << record.ticketCode << endl;
            outFile << record.seatsRented << endl;
            outFile << record.shiftIndex << endl;
            outFile << record.action << endl;
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
            cout << "\t\t\tError: You must choose a number.\n";
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
            cout << "\t\t\tEnd system.\n";
            pressEnterToContinue();
            break;
        default:
            cout << "\t\t\tYour choice is not available.\n";
            pressEnterToContinue();
        }
    } while (choice != 9);

    return 0;
}


#include <iostream>
#include <string>
#include <queue>
#include <vector>
#include <unordered_map>
#include <fstream>
#include <sstream>
#include <cstdlib>
#include <windows.h>

using namespace std;

// ANSI escape codes for colors
#define RESET   "\033[0m"
#define BOLD    "\033[1m"
#define RED     "\033[31m"
#define GREEN   "\033[32m"
#define YELLOW  "\033[33m"
#define BLUE    "\033[34m"
#define MAGENTA "\033[35m"
#define CYAN    "\033[36m"

void clearConsole() {
#ifdef _WIN32
    system("CLS"); // Clear console for Windows
#else
    system("clear"); // Clear console for Unix/Linux
#endif
}

void executeCommand() {
    clearConsole(); // Clear screen at the start of the command
    std::cout << "Executing new command..." << std::endl;
    // Perform the command's operations here
}

void setConsoleColors(int textColor, int bgColor)
{
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTextAttribute(hConsole, (bgColor << 4) | textColor);
}

void resetConsoleColors()
{
#ifdef _WIN32
    setConsoleColors(7, 0); // Reset to default colors for Windows (light grey text, black bg)
#else
    cout << "\033[0m"; // Reset to default for Unix/Linux
#endif
}

// Function to center-align text in the console window
void printCentered(const string& text)
{
    const int width = 80; // Set the width for centering
    int padding = (width - text.length()) / 2;
    cout << string(padding, ' ') << text << endl;
}

// Structure for Train Schedule (Node in Linked List)
struct Train
{
    int trainID;
    string trainName;
    string departureTime;
    string arrivalTime;
    int maxCapacity;
    int bookedSeats;
    Train* next;

    Train(int id, string name, string dep, string arr, int capacity)
        : trainID(id), trainName(name), departureTime(dep), arrivalTime(arr),
          maxCapacity(capacity), bookedSeats(0), next(nullptr) {}
};


// Train Linked List
class TrainList
{
    Train* head;

public:
    TrainList() : head(nullptr) {}

    void addTrain(int id, string name, string dep, string arr, int capacity)
    {
        Train* newTrain = new Train(id, name, dep, arr, capacity);
        if (!head)
        {
            head = newTrain;
        }
        else
        {
            Train* temp = head;
            while (temp->next) temp = temp->next;
            temp->next = newTrain;
        }
    }

    void displayTrains()
    {
        Train* temp = head;
        printCentered(std::string(BOLD) + CYAN + "Available Trains:\n" + RESET);  // Fixed here
        cout << string(70, '=') << endl;
        while (temp)
        {
            cout << GREEN << "Train ID: " << RESET << temp->trainID << "\n"
                 << BLUE << "Name: " << RESET << temp->trainName << "\n"
                 << MAGENTA << "Departure: " << RESET << temp->departureTime << "\n"
                 << YELLOW << "Arrival: " << RESET << temp->arrivalTime << "\n"
                 << CYAN << "Seats Booked: " << RESET << temp->bookedSeats << "/" << temp->maxCapacity << "\n";
            cout << string(45, '-') << endl;
            temp = temp->next;
        }
    }

    Train* findTrain(int id)
    {
        Train* temp = head;
        while (temp)
        {
            if (temp->trainID == id) return temp;
            temp = temp->next;
        }
        return nullptr;
    }
};

// Binary Search Tree for Passenger Data
struct Passenger
{
    int passengerID;
    string name;
    Passenger* left;
    Passenger* right;

    Passenger(int id, string name) : passengerID(id), name(name), left(nullptr), right(nullptr) {}
};

class PassengerTree
{
    Passenger* root;

    void inOrder(Passenger* node)
    {
        if (node)
        {
            inOrder(node->left);
            cout << GREEN << "Passenger ID: " << RESET << node->passengerID << "\n"
                 << CYAN << "Name: " << RESET << node->name << "\n";
            cout << string(40, '-') << endl;
            inOrder(node->right);
        }
    }

public:
    PassengerTree() : root(nullptr) {}

    void addPassenger(int id, string name)
    {
        Passenger* newPassenger = new Passenger(id, name);
        if (!root)
        {
            root = newPassenger;
        }
        else
        {
            Passenger* current = root;
            Passenger* parent = nullptr;
            while (current)
            {
                parent = current;
                if (id < current->passengerID)
                {
                    current = current->left;
                }
                else
                {
                    current = current->right;
                }
            }
            if (id < parent->passengerID) parent->left = newPassenger;
            else parent->right = newPassenger;
        }
    }

    void displayPassengers()
    {
        printCentered(std::string(BOLD) + CYAN + "Passenger List:" + RESET);  // Fixed here
        cout << string(40, '=') << endl;
        inOrder(root);
    }
};

// Graph for Train Routes (BFS)
class TrainRoutes
{
    vector<vector<int>> adjList;

public:
    TrainRoutes(int n) : adjList(n) {}

    void addRoute(int src, int dest, bool silent = false)
    {
        if (src >= adjList.size() || dest >= adjList.size())
        {
            cout << RED << "Invalid station ID. Try again." << RESET << "\n";
            return;
        }
        adjList[src].push_back(dest);
        adjList[dest].push_back(src);
        if (!silent)
        {
            cout << GREEN << "Route added successfully." << RESET << "\n";
        }
    }

    void bfs(int start)
    {
        if (start >= adjList.size())
        {
            cout << RED << "Invalid starting station ID. Try again." << RESET << "\n";
            return;
        }

        vector<bool> visited(adjList.size(), false);
        queue<int> q;
        q.push(start);
        visited[start] = true;

        printCentered(std::string(BOLD) + CYAN + "BFS Traversal starting from station " + to_string(start) + ":" + RESET);  // Fixed here
        while (!q.empty())
        {
            int current = q.front();
            q.pop();
            cout << GREEN << "Visited station: " << RESET << current << "\n";

            for (int neighbor : adjList[current])
            {
                if (!visited[neighbor])
                {
                    visited[neighbor] = true;
                    q.push(neighbor);
                }
            }
        } cout<<endl;
    }
};

struct User
{
    int userID;
    string userName;
    string password;
    User* next;

    User(int id, string name, string pass)
        : userID(id), userName(name), password(pass), next(nullptr) {}
};

class UserList
{
    User* head;

public:
    UserList() : head(nullptr) {
     loadFromFile();
    }
     ~UserList() {
        // Free memory for all users in the list
        User* temp;
        while (head) {
            temp = head;
            head = head->next;
            delete temp;
        }
    }

    // Register a new user
    bool registerUser(int id, string name, string password)
    {
        // Check if user already exists
        User* temp = head;
        while (temp)
        {
            if (temp->userID == id)
            {
                cout << RED << "User ID already exists. Try a different ID." << RESET << endl;
                return false;
            }
            temp = temp->next;
        }

        // Create and add the new user to the list
        User* newUser = new User(id, name, password);
        newUser->next = head;
        head = newUser;
        // Save the new user to the file
        saveToFile();

        cout << GREEN << "Registration successful!" << RESET << endl;
        return true;
    }

    // Login an existing user
    User* loginUser(int id, string password)
    {
        User* temp = head;
         while (temp) {
            if (temp->userID == id && temp->password == password) {
               return temp;  // User found and authenticated
            }
            temp = temp->next;
        }
        cout << RED << "Invalid credentials. Please try again." << RESET << endl;
        return nullptr;
    }
    // Load users from the file into the list
    void loadFromFile()
    {
       ifstream inputFile("users.txt");
        if (!inputFile.is_open()) {
            cout << RED << "Error: Could not open users.txt." << RESET << endl;
            return;
        }
         string line;
        while (getline(inputFile, line))
        {
            stringstream ss(line);
            int id;
            string name, password;
            if (ss >> id) {
                getline(ss, name, ',');  // Read name until the comma
                getline(ss, password);  // Read the rest as password
                addUserToList(id, name, password);
            }
        }
        inputFile.close();
    }

    // Save all users to the file
    void saveToFile()
    {
          ofstream outputFile("users.txt", ios::trunc);  // Overwrite file
        if (!outputFile.is_open()) {
            cout << RED << "Error: Could not open users.txt for writing." << RESET << endl;
            return;
        }
        User* temp = head;
        while (temp)
        {
            outputFile << temp->userID << " " << temp->userName << "," << temp->password << endl;
            temp = temp->next;
        }
        outputFile.close();
    }

    // Add a user to the linked list
    void addUserToList(int id, string name, string password)
    {
        User* newUser = new User(id, name, password);
        newUser->next = head;
        head = newUser;
    }
};


class TrainSystem
{
    TrainList trains;
    PassengerTree passengers;
    TrainRoutes routes;
    unordered_map<int, unordered_map<int, int>> bookingDetails; // {trainID: {passengerID: seatCount}}
    UserList users;

    bool adminLogin()
    {  setConsoleColors(0, 6);

        const string ADMIN_EMAIL = "admin@gmail.com";
        const string ADMIN_PASSWORD = "admin123";

        string email, password;
        cout << BOLD << "\nEnter Admin Email: " << RESET;
        cin >> email;
        cout << BOLD << "Enter Admin Password: " << RESET;
        cin >> password;

        if (email == ADMIN_EMAIL && password == ADMIN_PASSWORD)
        {
            cout << GREEN << "\nLogin Successful!" << RESET << "\n\n";
            return true;
        }
        else
        {
            cout << RED << "\nInvalid Credentials. Access Denied.\n" << RESET << "\n";
             Sleep(2000);
            return false;
        }
    }

    void displayBookingDetails()
    {
        printCentered(std::string(BOLD) + CYAN + "Booking Details:" + RESET);
        cout << string(40, '=') << endl;
        for (const auto& trainEntry : bookingDetails)
        {
            int trainID = trainEntry.first;
            Train* train = trains.findTrain(trainID);
            if (train)
            {
                cout << BLUE << "Train ID: " << RESET << trainID << " - " << train->trainName << "\n";
                cout << string(40, '-') << endl;
                for (const auto& passengerEntry : trainEntry.second)
                {
                    cout << GREEN << "Passenger ID: " << RESET << passengerEntry.first << "\n"
                         << CYAN << "Seats Booked: " << RESET << passengerEntry.second << "\n";
                }
                cout << string(40, '=') << endl;
            }
        }
    }

public:
    TrainSystem(int stations) : routes(stations)
    {
        printCentered(std::string(BOLD) + CYAN + "Welcome to RailLink: Your Trusted Train Ticket Booking System!" + RESET);  // Fixed here
        cout << "Bringing convenience and connectivity to your travel plans.\n";
        trains.addTrain(1, "Express 101", "10:00 AM", "02:00 PM", 100);
        trains.addTrain(2, "Superfast 202", "11:00 AM", "03:00 PM", 150);
        trains.addTrain(3, "Local 303", "12:00 PM", "04:00 PM", 80);

        routes.addRoute(0, 1, true);
        routes.addRoute(1, 2, true);
        routes.addRoute(2, 3, true);
    }

    void adminMenu()
    {
         setConsoleColors(0, 6);
         clearConsole();
        if (!adminLogin()) return;

        int choice;
        do
        {
            printCentered(std::string(BOLD) + "Admin Menu" + RESET);  // Fixed here
            cout << string(70, '-') << endl;
            cout << "1. Add Train\n2. Display Trains\n3. Add Route\n4. View Booking Details\n5. Exit\n";
            cout << BOLD << "Enter choice: " << RESET;
            cin >> choice;
            switch (choice)
            {
            case 1:
                 setConsoleColors(0, 6);
            {    clearConsole();
                int id, capacity;
                string name, dep, arr;
                cout << BOLD << "\nEnter Train ID: " << RESET;
                cin >> id;
                cin.ignore();
                cout << BOLD << "Enter Train Name: " << RESET;
                getline(cin, name);
                cout << BOLD << "Enter Departure Time: " << RESET;
                getline(cin, dep);
                cout << BOLD << "Enter Arrival Time: " << RESET;
                getline(cin, arr);
                cout << BOLD << "Enter Maximum Capacity: " << RESET;
                cin >> capacity;
                trains.addTrain(id, name, dep, arr, capacity);
                cout << GREEN << "\nTrain added successfully." << RESET << "\n";
                break;
            }
            case 2:
                 setConsoleColors(0, 8);
                 clearConsole();
                trains.displayTrains();
                break;
            case 3:
            {
                 clearConsole();
                int src, dest;
                cout << BOLD << "\nEnter Source Station ID: " << RESET;
                cin >> src;
                cout << BOLD << "Enter Destination Station ID: " << RESET;
                cin >> dest;
                routes.addRoute(src, dest);
                break;
            }
            case 4:
                 clearConsole();
                displayBookingDetails();
                break;
            case 5:
                cout << GREEN << "\nExiting Admin Menu." << RESET << "\n";
                break;
            default:
                cout << RED << "\nInvalid choice. Try again." << RESET << "\n";
            }
        }
        while (choice != 5);
    }
// New function to handle User Registration
    void userRegistration()
    {
        int userID;
        string userName, password;

        cout << BOLD << "Enter User ID: " << RESET;
        cin >> userID;
        cin.ignore(); // To clear buffer before reading user name
        cout << BOLD << "Enter User Name: " << RESET;
        getline(cin, userName);
        cout << BOLD << "Enter Password: " << RESET;
        cin >> password;

        users.registerUser(userID, userName, password);
    }

    // New function to handle User Login
    User* userLogin()
    {
        int userID;
        string password;

        cout << BOLD << "Enter User ID: " << RESET;
        cin >> userID;
        cout << BOLD << "Enter Password: " << RESET;
        cin >> password;

        return users.loginUser(userID, password);
    }
    void userMenu()
    {
        clearConsole();
        int choice;
        User* currentUser = nullptr;
        do
        {
            printCentered(std::string(BOLD) + CYAN + "User Menu" + RESET);
            cout << string(70, '-') << endl;
            cout << "1. Register\n2. Login\n3. View Train Schedule\n4. Book Ticket\n5. View Routes\n6. Exit\n";
            cout << BOLD << "Enter choice: " << RESET;
            cin >> choice;

            switch (choice)
            {
            case 1:
                userRegistration();
                break;

            case 2:

                currentUser = userLogin();
                if (currentUser)
                {
                    cout << GREEN << "Login successful! Welcome, " << currentUser->userName << RESET << endl;
                }
                break;


            case 3:
                clearConsole();
                if (!currentUser)
                {
                    cout << RED << "You must log in first." << RESET << endl;
                    break;
                }
                trains.displayTrains();
                break;

            case 4:
                clearConsole();
                if (!currentUser)
                {
                    cout << RED << "You must log in first." << RESET << endl;
                    break;
                }
                bookTicket(currentUser);  // Add the ticket booking function here
                break;

            case 5:

                 if (!currentUser) {
                    cout << RED << "You must log in first." << RESET << endl;
                    break;
                }

                viewTrainRoute();  // Call the new function to view the train route
                break;
            case 6:
                cout << GREEN << "\nExiting User Menu." << RESET << "\n";
                break;

            default:
                cout << RED << "\nInvalid choice. Try again." << RESET << "\n";
            }
        }
        while (choice != 6);
    }
    void viewTrainRoute() {
    int trainID;
    cout << "Enter Train ID to view the route (BFS): ";
    cin >> trainID;

    // Ensure the train ID is valid
    Train* train = trains.findTrain(trainID);
    if (train) {
        routes.bfs(trainID);  // Call BFS starting from the train ID entered by the user
    } else {
        cout << RED << "Train not found." << RESET << endl;
    }
}


    // Ticket booking function
    void bookTicket(User* user)
    {
        int trainID, seatCount;
        cout << BOLD << "Enter Train ID: " << RESET;
        cin >> trainID;
        Train* train = trains.findTrain(trainID);

        if (train)
        {
            cout << BOLD << "Enter Number of Seats: " << RESET;
            cin >> seatCount;

            if (seatCount <= (train->maxCapacity - train->bookedSeats))
            {
                train->bookedSeats += seatCount;
                bookingDetails[trainID][user->userID] += seatCount;
                cout << GREEN << "Booking successful!" << RESET << endl;
            }
            else
            {
                cout << RED << "Not enough available seats." << RESET << endl;
            }
        }
        else
        {
            cout << RED << "Train not found." << RESET << endl;
        }
    }
};

int main()
{

    TrainSystem system(10);

    int role;
    do
    {
          // White text (15), Blue background (1)

        clearConsole();

        printCentered(std::string(BOLD) + CYAN + "Welcome to RailLink: Your Trusted Train Ticket Booking System!" + RESET);  // Fixed here
        cout << "   Bringing convenience and connectivity to your travel plans.\n\n";
       cout << string(70, '*') <<"\n"<<endl;
        cout << "1. Admin\n2. User\n3. Exit\n";
        cout << BOLD << "Enter role: " << RESET;
        cin >> role;
        switch (role)
        {
        case 1:
            setConsoleColors(0, 6);  // White text (15), Blue background (1)

            system.adminMenu();
            break;
        case 2:
            setConsoleColors(0, 60);
            system.userMenu();
            break;
        case 3:
            cout << GREEN << "\nExiting system." << RESET << "\n";
            break;
        default:
            cout << RED << "\nInvalid choice. Try again." << RESET << "\n";
        }
    }
    while (role != 3);
resetConsoleColors();
    return 0;
}

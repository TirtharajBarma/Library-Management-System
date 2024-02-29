#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <iomanip>

using namespace std;

// Define structures for Book and User
struct Book {
    int id;
    string title;
    string author;
    bool available;
};

struct User {
    string username;
    string password;
};

// Function prototypes
void displayMenu();
void addBook(vector<Book> &books);
void removeBook(vector<Book> &books);
void displayBooks(const vector<Book> &books);
void registerUser();
bool authenticateUser(const string &username, const string &password);

// File paths
const string booksFilePath = "books.txt";
const string usersFilePath = "users.txt";

int main() {
    vector<Book> books;
    // Initialize other necessary variables and data structures

    bool exitProgram = false;
    while (!exitProgram) {
        displayMenu();
        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        switch (choice) {
            case 1:
                registerUser();
                break;
            case 2: {
                string username, password;
                cout << "Enter username: ";
                cin >> username;
                cout << "Enter password: ";
                cin >> password;
                if (authenticateUser(username, password)) {
                    addBook(books);
                } else {
                    cout << "Authentication failed. Access denied.\n";
                }
                break;
            }
            case 3:
                removeBook(books);
                break;
            case 4:
                displayBooks(books);
                break;
            case 0:
                exitProgram = true;
                cout << "Exiting program...\n";
                break;
            default:
                cout << "Invalid choice. Please try again.\n";
        }
    }

    return 0;
}

// Function to display menu options
void displayMenu() {
    cout << "\nLibrary Management System\n";
    cout << "1. Register User\n";
    cout << "2. Add Book\n";
    cout << "3. Remove Book\n";
    cout << "4. Display Books\n";
    cout << "0. Exit\n";
}

// Function to add a book to the library
void addBook(vector<Book> &books) {
    Book newBook;
    cout << "Enter title of the book: ";
    cin.ignore();
    getline(cin, newBook.title);
    cout << "Enter author of the book: ";
    getline(cin, newBook.author);
    newBook.available = true;
    // Generate unique ID for the book
    if (!books.empty()) {
        newBook.id = books.back().id + 1;
    } else {
        newBook.id = 1;
    }
    // Add the book to the vector
    books.push_back(newBook);

    // Update books data file
    ofstream outfile(booksFilePath, ios::app);
    if (!outfile.is_open()) {
        cerr << "Error: Unable to open file for writing.\n";
        return;
    }
    outfile << left << setw(5) << newBook.id << "|"
            << setw(25) << newBook.title << "|"
            << setw(25) << newBook.author << "|"
            << setw(10) << (newBook.available ? "Yes" : "No") << endl;
    outfile.close();

    cout << "Book added successfully!\n";
}

// Function to remove a book from the library
void removeBook(vector<Book> &books) {
    if (books.empty()) {
        cout << "No books available to remove.\n";
        return;
    }
    cout << "Select the book to remove:\n";
    for (const auto &book : books) {
        cout << left << setw(5) << book.id << "|"
                << setw(25) << book.title << "|"
                << setw(25) << book.author << "|"
                << setw(10) << (book.available ? "Yes" : "No") << endl;
    }
    int id;
    cout << "Enter the ID of the book to remove: ";
    cin >> id;
    for (auto it = books.begin(); it != books.end(); ++it) {
        if (it->id == id) {
            books.erase(it);
            break;
        }
    }
    // Update books data file
    ofstream outfile(booksFilePath);
    if (!outfile.is_open()) {
        cerr << "Error: Unable to open file for writing.\n";
        return;
    }
    for (const auto &book : books) {
        outfile << left << setw(5) << book.id << "|"
                << setw(25) << book.title << "|"
                << setw(25) << book.author << "|"
                << setw(10) << (book.available ? "Yes" : "No") << endl;
    }
    outfile.close();
    cout << "Book removed successfully!\n";
}

// Function to display all books in the library
void displayBooks(const vector<Book> &books) {
    if (books.empty()) {
        cout << "No books available.\n";
        return;
    }
    cout << "-------------------------------------------------------------------------------\n";
    cout << "| ID |        Title         |         Author         | Available |\n";
    cout << "-------------------------------------------------------------------------------\n";
    for (const auto &book : books) {
        cout << left << "| " << setw(2) << book.id << " | "
                << setw(20) << book.title << " | "
                << setw(20) << book.author << " | "
                << setw(9) << (book.available ? "Yes" : "No") << " |\n";
    }
    cout << "-------------------------------------------------------------------------------\n";
}

// Function to register a new user
void registerUser() {
    string username, password;
    cout << "Enter new username: ";
    cin >> username;
    cout << "Enter password: ";
    cin >> password;

    // Update users data file
    ofstream outfile(usersFilePath, ios::app);
    if (!outfile.is_open()) {
        cerr << "Error: Unable to open file for writing.\n";
        return;
    }
    outfile << username << "|" << password << endl;
    outfile.close();

    cout << "User registered successfully!\n";
}

// Function to authenticate user
bool authenticateUser(const string &username, const string &password) {
    ifstream infile(usersFilePath);
    if (!infile.is_open()) {
        cerr << "Error: Unable to open file for reading.\n";
        return false;
    }
    string line;
    while (getline(infile, line)) {
        size_t pos = line.find("|");
        string storedUsername = line.substr(0, pos);
        string storedPassword = line.substr(pos + 1);
        if (storedUsername == username && storedPassword == password) {
            infile.close();
            return true;
        }
    }
    infile.close();
    return false;
}

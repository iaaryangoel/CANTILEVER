#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <set>
#include <string>
#include <algorithm>

using namespace std;

class User {
public:
    string username;
    string name;
    vector<string> friends;
    vector<string> messages;

    User() {}

    User(string uname, string n) {
        username = uname;
        name = n;
    }

    void addFriend(string friendUsername) {
        friends.push_back(friendUsername);
    }

    void postMessage(string msg) {
        messages.push_back(msg);
    }
};

// In-memory user map (username → User)
map<string, User> userDB;

// File paths
const string USERS_FILE = "users.txt";
const string FRIENDS_FILE = "friends.txt";
const string POSTS_FILE = "posts.txt";

// Load users from file
void loadUsers() {
    ifstream fin(USERS_FILE);
    if (!fin) return;

    string uname, name;
    while (fin >> uname) {
        getline(fin >> ws, name);
        userDB[uname] = User(uname, name);
    }
    fin.close();
}

// Save users to file
void saveUsers() {
    ofstream fout(USERS_FILE);
    for (auto& pair : userDB) {
        fout << pair.first << "\n" << pair.second.name << "\n";
    }
    fout.close();
}

// Add new user
void createProfile() {
    string uname, name;
    cout << "Enter username: ";
    cin >> uname;

    if (userDB.find(uname) != userDB.end()) {
        cout << "Username already exists!\n";
        return;
    }

    cout << "Enter your full name: ";
    cin.ignore();
    getline(cin, name);

    userDB[uname] = User(uname, name);
    saveUsers();
    cout << "Profile created successfully!\n";
}

// Add friend
void addFriend() {
    string u1, u2;
    cout << "Enter your username: ";
    cin >> u1;
    cout << "Enter friend's username: ";
    cin >> u2;

    if (u1 == u2) {
        cout << "❌ You cannot add yourself as a friend.\n";
        return;
    }

    if (userDB.find(u1) == userDB.end() || userDB.find(u2) == userDB.end()) {
        cout << "❌ One or both usernames not found.\n";
        return;
    }

    // Check if users are already friends
    vector<string>& friendsList = userDB[u1].friends;
    if (find(friendsList.begin(), friendsList.end(), u2) != friendsList.end()) {
        cout << "👬 You are already friends with " << u2 << ".\n";
        return;
    }

    // Add friends mutually
    userDB[u1].addFriend(u2);
    userDB[u2].addFriend(u1);

    ofstream fout(FRIENDS_FILE, ios::app);
    fout << u1 << " " << u2 << "\n";
    fout.close();

    cout << "✅ Friend added successfully!\n";
}

// Post a message
void postMessage() {
    string sender, receiver, msg;

    cout << "Enter your username (sender): ";
    cin >> sender;

    cout << "Enter receiver's username: ";
    cin >> receiver;
    cin.ignore();

    if (userDB.find(sender) == userDB.end() || userDB.find(receiver) == userDB.end()) {
        cout << "❌ One or both usernames not found.\n";
        return;
    }

    cout << "Enter your message: ";
    getline(cin, msg);

    string formattedMsg = "To " + receiver + ": " + msg;
    userDB[sender].postMessage(formattedMsg);

    formattedMsg = "From " + sender + ": " + msg;
    userDB[receiver].postMessage(formattedMsg);

    ofstream fout(POSTS_FILE, ios::app);
    fout << sender << " -> " << receiver << ": " << msg << "\n";
    fout.close();

    cout << "📨 Message sent successfully from " << sender << " to " << receiver << "!\n";
}

// View profile
void viewProfile() {
    string uname;
    cout << "Enter username to view: ";
    cin >> uname;

    if (userDB.find(uname) == userDB.end()) {
        cout << "User not found.\n";
        return;
    }

    User& u = userDB[uname];
    cout << "\n--- Profile ---\n";
    cout << "Name: " << u.name << "\n";
    cout << "Username: " << u.username << "\n";
    cout << "Friends: ";
    for (auto& f : u.friends) cout << f << " ";
    cout << "\n";
    cout << "Messages:\n";
    for (auto& m : u.messages) cout << "- " << m << "\n";
    cout << "---------------\n";
}

// Menu
void showMenu() {
    cout << "\n===== Simple Social Network =====\n";
    cout << "1. Create Profile\n";
    cout << "2. Add Friend\n";
    cout << "3. Post Message\n";
    cout << "4. View Profile\n";
    cout << "5. Exit\n";
    cout << "Enter your choice: ";
}

int main() {
    loadUsers();

    int choice;
    while (true) {
        showMenu();
        cin >> choice;

        switch (choice) {
            case 1: createProfile(); break;
            case 2: addFriend(); break;
            case 3: postMessage(); break;
            case 4: viewProfile(); break;
            case 5: cout << "Goodbye!\n"; return 0;
            default: cout << "Invalid choice.\n";
        }
    }
}

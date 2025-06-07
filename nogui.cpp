//#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <chrono>
#include <thread>
#include <memory>      
#include <algorithm>   
#include <stdexcept>    
#include <list>        
#include <limits>       

using namespace std;

// Forward declarations
class Player;

// =================================================================================
// === 1. OOP: ABSTRACT BASE CLASS & POLYMORPHISM ==================================
// =================================================================================
// *** ADDED: A pure abstract base class for any character in the game.
class Character {
protected:
    string name;
    int health;

public:
    Character(string n, int h) : name(n), health(h) {}
    virtual ~Character() = default; // Virtual destructor for base class

    // *** ADDED: Pure virtual function makes Character an abstract class
    virtual void displayStatus() const = 0;

    string getName() const { return name; }
    int getHealth() const { return health; }
    void takeDamage(int damage) {
        health -= damage;
        if (health < 0) health = 0;
    }
};
// =================================================================================

// Class for Player (now inherits from Character)
class Player : public Character {
private:
    list<string> inventory; // *** CHANGED: Using std::list for inventory (Linked List)
    int moves;
    int coins;
    int enemiesDefeated;

public:
    Player(string n);
    void heal(int amount);
    void addToInventory(string item);
    void addCoins(int amount);
    void useMove();
    void incrementEnemiesDefeated();

    int getMoves() const;
    int getCoins() const;
    int getEnemiesDefeated() const;
    list<string> getInventory() const;
    void sortInventory(); // *** ADDED: Method to demonstrate sorting algorithm

    // *** ADDED: Overridden virtual function for Polymorphism
    void displayStatus() const override;
};

// =================================================================================
// === 1. OOP: OPERATOR OVERLOADING ================================================
// =================================================================================
// *** ADDED: Overloading the << operator to easily print Player's stats.
ostream& operator<<(ostream& os, const Player& player);
// =================================================================================

// Class for Enemy (now inherits from Character)
class Enemy : public Character {
private:
    string description;

public:
    Enemy(string n, string desc, int hr);
    string getDescription() const;

    // *** ADDED: Overridden virtual function for Polymorphism
    void displayStatus() const override;
};

// Class for Treasure
class Treasure {
private:
    string item1;
    string item2;
    string key;

public:
    Treasure(string i1, string i2, string k);
    string getItem1() const;
    string getItem2() const;
    string getKey() const;
};

// Class for Room
class Room {
private: // *** CHANGED: Encapsulation - Members are now private
    string name;
    Enemy enemy;
    Treasure treasure;
    string challenge;

public:
    Room(string n, Enemy e, Treasure t, string c);

    // *** ADDED: Getters for private members
    string getName() const;
    const Enemy& getEnemy() const;
    const Treasure& getTreasure() const;
    string getChallenge() const;
};

// Class for Dungeon
class Dungeon {
private:
    // *** CHANGED: Using smart pointers for automatic memory management (Advanced C++ Feature)
    vector<unique_ptr<Room>> rooms; // A vector of unique pointers to Rooms
    queue<Enemy> enemyQueue;
    stack<const Room*> roomStack;  // Stack holds non-owning (raw) pointers
    int currentRoomIndex;          // *** ADDED: To track the current room

public:
    Dungeon();
    // *** CHANGED: Destructor ~Dungeon() is removed. unique_ptr handles memory automatically (Rule of Zero).

    void displayRules() const;
    const Room* getCurrentRoom() const;    // *** CHANGED: To get current room
    const Room* advanceToNextRoom();     // *** CHANGED: To move to the next room
    const Room* backtrack();             // *** CHANGED: Backtracking logic updated
    void displayRanking(const Player& player) const;
};

// =================================================================================
// === 3. ADVANCED C++: TEMPLATES ==================================================
// =================================================================================
// A template function to print items from any standard container.
template<typename T>
void printContainer(const T& container) {
    for (const auto& item : container) {
        cout << item << " ";
    }
    cout << endl;
}
// =================================================================================

// =================================================================================
// === Player Class Implementation =================================================
// =================================================================================

Player::Player(string n) : Character(n, 100), moves(10), coins(0), enemiesDefeated(0) {}

void Player::heal(int amount) {
    health += amount;
    if (health > 100) health = 100;
}

void Player::addToInventory(string item) {
    inventory.push_back(item);
}

void Player::addCoins(int amount) {
    coins += amount;
}

void Player::useMove() {
    moves--;
}

void Player::incrementEnemiesDefeated() {
    enemiesDefeated++;
}

int Player::getMoves() const { return moves; }
int Player::getCoins() const { return coins; }
int Player::getEnemiesDefeated() const { return enemiesDefeated; }
list<string> Player::getInventory() const { return inventory; }

// Implementation for sorting the player's inventory (Sorting Algorithm)
void Player::sortInventory() {
    inventory.sort(); // Uses the list's built-in sort
}

//  Implementation of the overridden virtual function from Character
void Player::displayStatus() const {
    cout << "Player: " << name << " | Health: " << health << endl;
}

// Implementation of the overloaded << operator
ostream& operator<<(ostream& os, const Player& player) {
    os << "\n--- Player Stats ---" << endl;
    os << "Name: " << player.getName() << endl;
    os << "Health: " << player.getHealth() << endl;
    os << "Moves Left: " << player.getMoves() << endl;
    os << "Coins Collected: " << player.getCoins() << endl;
    os << "Enemies Defeated: " << player.getEnemiesDefeated() << endl;
    os << "Inventory (Sorted): ";
    // *** ADDED: Using the template function to print the inventory
    printContainer(player.getInventory());
    os << "--------------------" << endl;
    return os;
}
// =================================================================================

// =================================================================================
// === Enemy Class Implementation ==================================================
// =================================================================================
// 
Enemy::Enemy(string n, string desc, int hr) : Character(n, hr), description(desc) {}

string Enemy::getDescription() const { return description; }

// *** ADDED: Implementation of the overridden virtual function from Character
void Enemy::displayStatus() const {
    cout << "Enemy: " << name << " | Health Required to Win: " << health << endl;
}
// =================================================================================

// Treasure Class Implementation
Treasure::Treasure(string i1, string i2, string k) : item1(i1), item2(i2), key(k) {}
string Treasure::getItem1() const { return item1; }
string Treasure::getItem2() const { return item2; }
string Treasure::getKey() const { return key; }

// =================================================================================
// === Room Class Implementation ===================================================
// =================================================================================
Room::Room(string n, Enemy e, Treasure t, string c) : name(n), enemy(e), treasure(t), challenge(c) {}

// getters for encapsulated members
string Room::getName() const { return name; }
const Enemy& Room::getEnemy() const { return enemy; }
const Treasure& Room::getTreasure() const { return treasure; }
string Room::getChallenge() const { return challenge; }
// =================================================================================

// =================================================================================
// === Dungeon Class Implementation ================================================
// =================================================================================
Dungeon::Dungeon() : currentRoomIndex(-1) { // Start before the first room
    // Using std::make_unique for smart pointers (Advanced C++ Feature)
    rooms.push_back(make_unique<Room>("Base", Enemy("Shadow Stalker", "A stealthy, dark creature.", 15), Treasure("5 Coins", "Armour", "Key1"), "Collect 5 coins"));
    rooms.push_back(make_unique<Room>("Bronze", Enemy("Viper", "A venomous menace.", 25), Treasure("5 Coins", "Health Booster Potion", "Key2"), "Exit the room within 5 seconds"));
    rooms.push_back(make_unique<Room>("Platinum", Enemy("Crawler", "A fast, wall-climbing creature.", 35), Treasure("Health Booster Potion", "Armour", "Key3"), "Defeat the enemy without armour"));
    rooms.push_back(make_unique<Room>("Silver", Enemy("Hunter", "A swift and deadly assassin.", 50), Treasure("5 Coins", "Armour", "Key4"), "Riddle: I have no voice, but I can teach you all I know. What am I? (Answer: book)"));
    rooms.push_back(make_unique<Room>("Gold", Enemy("Boss", "The ultimate challenge.", 70), Treasure("5 Coins", "Health Booster Potion", "Key5"), "Defeat the boss"));

    for (const auto& room : rooms) {
        enemyQueue.push(room->getEnemy());
    }
}

void Dungeon::displayRules() const {
    cout << "Welcome to Dungeon Escape!\n";
    cout << "Rules:\n";
    cout << "1. You have 10 moves to escape the dungeon.\n";
    cout << "2. Each room has an enemy, a treasure, and a challenge.\n";
    cout << "3. Defeating enemies gets you treasure.\n";
    cout << "4. If your health drops below 20, you lose.\n";
    cout << "5. Clear the final room to win.\n";
    cout << "Good luck!\n";
}

// Function to get the current room using the index
const Room* Dungeon::getCurrentRoom() const {
    if (currentRoomIndex >= 0 && currentRoomIndex < rooms.size()) {
        return rooms[currentRoomIndex].get();
    }
    return nullptr;
}

const Room* Dungeon::advanceToNextRoom() {
    if (currentRoomIndex < (int)rooms.size() - 1) {
        currentRoomIndex++;
        const Room* nextRoom = rooms[currentRoomIndex].get();
        roomStack.push(nextRoom);
        return nextRoom;
    }
    return nullptr; // No more rooms
}

const Room* Dungeon::backtrack() {
    if (roomStack.size() > 1) {
        roomStack.pop(); // Pop current room
        const Room* previousRoom = roomStack.top(); // See what the new top is
        // Find the index of the previous room
        for (size_t i = 0; i < rooms.size(); ++i) {
            if (rooms[i].get() == previousRoom) {
                currentRoomIndex = i;
                break;
            }
        }
        return previousRoom;
    }
    return nullptr; // Can't backtrack
}

// displayRanking uses the overloaded << operator for cleaner code.
void Dungeon::displayRanking(const Player& player) const {
    cout << "\n======== GAME OVER ========" << endl;
    cout << player; // Use the overloaded operator
}

// =================================================================================
// === 2. ALGORITHMS: RECURSION ====================================================
// =================================================================================
// The main game logic is now a recursive function.
void gameLoop(Player& player, Dungeon& dungeon) {
    // Base Cases for recursion: conditions that end the game.
    if (player.getHealth() < 20) {
        cout << "\nGame Over! Your health dropped below 20.\n";
        dungeon.displayRanking(player);
        return;
    }
    if (player.getMoves() <= 0) {
        cout << "\nGame Over! You ran out of moves.\n";
        dungeon.displayRanking(player);
        return;
    }

    const Room* currentRoom = dungeon.getCurrentRoom();
    if (!currentRoom) { // If the game has just started
        currentRoom = dungeon.advanceToNextRoom();
    }

    // Display room and player info
    cout << "\n----------------------------------------" << endl;
    cout << "You are in Room: " << currentRoom->getName() << endl;
    player.displayStatus(); // Using the polymorphic function
    cout << "Moves Remaining: " << player.getMoves() << endl;
    cout << "Enemy: " << currentRoom->getEnemy().getName() << " - " << currentRoom->getEnemy().getDescription() << endl;
    cout << "----------------------------------------" << endl;
    cout << "Choose your action:\n";
    cout << "1. Fight enemy\n";
    cout << "2. Attempt to bypass\n";
    cout << "3. Backtrack to previous room\n";
    cout << "4. Quit game\n";
    cout << "Enter choice: ";

    int choice;
    cin >> choice;

    // =================================================================================
    // === 3. ADVANCED C++: EXCEPTION HANDLING =========================================
    // =================================================================================
    // simple input validation to handle non-numeric input.
    if (cin.fail()) {
        cin.clear(); // Clear error flags
        cin.ignore(numeric_limits<streamsize>::max(), '\n'); // Discard bad input
        cout << "\nInvalid input. Please enter a number." << endl;
        gameLoop(player, dungeon); // Recursive call to try the turn again
        return;
    }
    // =================================================================================

    player.useMove(); // An action costs one move

    switch (choice) {
        case 1: { // Fight
            const Enemy& enemy = currentRoom->getEnemy();
            if (player.getHealth() >= enemy.getHealth()) {
                cout << "\nVictory! You defeated the " << enemy.getName() << ".\n";
                player.takeDamage(enemy.getHealth());
                cout << "You collected the treasure!\n";
                player.addToInventory(currentRoom->getTreasure().getItem1());
                player.addToInventory(currentRoom->getTreasure().getItem2());
                player.addCoins(10);
                player.incrementEnemiesDefeated();

                const Room* nextRoom = dungeon.advanceToNextRoom();
                if (!nextRoom) {
                    cout << "\nCongratulations! You cleared the final room and escaped the dungeon!\n";
                    player.sortInventory(); // Sort inventory before final display
                    dungeon.displayRanking(player);
                    return; // End game
                }
            } else {
                cout << "\nYou were too weak! You flee, taking damage.\n";
                player.takeDamage(10);
            }
            break;
        }
        case 2: { // Bypass
            cout << "\nYou sneak past, avoiding the fight but finding no treasure.\n";
            player.takeDamage(5); // Minor penalty for bypassing
            const Room* nextRoom = dungeon.advanceToNextRoom();
            if (!nextRoom) {
                cout << "\nCongratulations! You snuck out of the final room and escaped!\n";
                player.sortInventory();
                dungeon.displayRanking(player);
                return; // End game
            }
            break;
        }
        case 3: { // Backtrack
            if (dungeon.backtrack()) {
                cout << "\nYou backtrack to the previous room.\n";
            } else {
                cout << "\nThere is no room to backtrack to!\n";
            }
            break;
        }
        case 4: { // Quit
            cout << "\nYou have quit the dungeon.\n";
            dungeon.displayRanking(player);
            return; // End game
        }
        default:
            cout << "\nInvalid choice. You hesitate and lose a turn.\n";
            break;
    }

    // Recursive call to continue to the next turn
    gameLoop(player, dungeon);
}

int main() {
    char playAgainChoice = 'y';

    // *** CHANGED: Replaced recursive main() call with a proper do-while loop
    do {
        string playerName;
        cout << "Enter your name: ";
        cin >> playerName;

        Player player(playerName);
        Dungeon dungeon;

        dungeon.displayRules();

        // *** CHANGED: Start the game using the new recursive game loop
        gameLoop(player, dungeon);

        cout << "\nPlay again? (y/n): ";
        cin >> playAgainChoice;

    } while (playAgainChoice == 'y' || playAgainChoice == 'Y');

    cout << "Thanks for playing!" << endl;
    return 0;
}

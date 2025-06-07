#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <stack>
#include <memory>
#include <list>
#include <algorithm>
#include <limits>
#include <sstream>   // For building inventory string
#include <stdexcept> // For exception handling

using namespace std;

// --- All your classes (Character, Player, Enemy, Treasure, Room, etc.) go here ---
// --- No changes are needed in the classes from Character to Dungeon ---

/**
 * @brief Base abstract class for all characters in the game.
 * Provides common attributes like name and health, and a pure virtual function for displaying status.
 */
class Character
{
protected:
    string name;
    int health;

public:
    /**
     * @brief Constructor for the Character class.
     * @param n The name of the character.
     * @param h The initial health of the character.
     */
    Character(string n, int h) : name(n), health(h) {}

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     */
    virtual ~Character() = default;

    /**
     * @brief Pure virtual function to display the status of the character.
     * Must be implemented by derived classes.
     */
    virtual void displayStatus() const = 0;

    /**
     * @brief Gets the name of the character.
     * @return The name of the character.
     */
    string getName() const { return name; }

    /**
     * @brief Gets the current health of the character.
     * @return The current health of the character.
     */
    int getHealth() const { return health; }

    /**
     * @brief Reduces the character's health by a specified amount.
     * Health cannot drop below 0.
     * @param damage The amount of damage to take.
     */
    void takeDamage(int damage)
    {
        health -= damage;
        if (health < 0)
            health = 0;
    }
};

/**
 * @brief Represents the player character in the game.
 * Inherits from Character and includes player-specific attributes like inventory, moves, coins, and enemies defeated.
 */
class Player : public Character
{
private:
    list<string> inventory;
    int moves;
    int coins;
    int enemiesDefeated;

public:
    /**
     * @brief Constructor for the Player class.
     * Initializes player with a name, default health (100), moves (10), coins (0), and enemies defeated (0).
     * @param n The name of the player.
     */
    Player(string n) : Character(n, 100), moves(10), coins(0), enemiesDefeated(0) {}

    /**
     * @brief Heals the player by a specified amount, up to a maximum of 100 health.
     * @param amount The amount of health to restore.
     */
    void heal(int amount)
    {
        health += amount;
        if (health > 100)
            health = 100;
    }

    /**
     * @brief Adds an item to the player's inventory.
     * This is a templated method, allowing it to accept various types that can be streamed to a string.
     * @tparam T The type of the item to add (e.g., string, int, etc.).
     * @param item The item to add to the inventory.
     */
    template <typename T>
    void addToInventory(const T &item)
    {
        stringstream ss;
        ss << item;
        inventory.push_back(ss.str());
    }

    /**
     * @brief Adds coins to the player's coin count.
     * @param amount The number of coins to add.
     */
    void addCoins(int amount) { coins += amount; }

    /**
     * @brief Decrements the player's available moves by one.
     * Moves cannot drop below 0.
     */
    void useMove()
    {
        if (moves > 0)
            moves--;
    }

    /**
     * @brief Increments the count of enemies defeated by the player.
     */
    void incrementEnemiesDefeated() { enemiesDefeated++; }

    /**
     * @brief Gets the number of moves remaining for the player.
     * @return The number of moves left.
     */
    int getMoves() const { return moves; }

    /**
     * @brief Gets the total number of coins collected by the player.
     * @return The total coins.
     */
    int getCoins() const { return coins; }

    /**
     * @brief Gets the number of enemies defeated by the player.
     * @return The count of defeated enemies.
     */
    int getEnemiesDefeated() const { return enemiesDefeated; }

    /**
     * @brief Gets the player's inventory.
     * @return A list of strings representing the items in the inventory.
     */
    list<string> getInventory() const { return inventory; }

    /**
     * @brief Sorts the player's inventory alphabetically (case-insensitive).
     * Uses a lambda expression as a custom comparison predicate for std::list::sort.
     */
    void sortInventory()
    {
        inventory.sort([](const string &a, const string &b)
                       {
                           string lowerA, lowerB;
                           transform(a.begin(), a.end(), back_inserter(lowerA), ::tolower); // Convert to lowercase
                           transform(b.begin(), b.end(), back_inserter(lowerB), ::tolower); // Convert to lowercase
                           return lowerA < lowerB;                                          // Case-insensitive comparison
                       });
    }

    /**
     * @brief Displays the player's basic status (name and health) to the console.
     * Overrides the pure virtual function from Character.
     */
    void displayStatus() const override
    {
        cout << "Player: " << name << " | Health: " << health << endl;
    }
};

/**
 * @brief Overloads the stream insertion operator for the Player class.
 * Provides a formatted output of all player statistics.
 * @param os The output stream.
 * @param player The Player object to output.
 * @return The output stream.
 */
ostream &operator<<(ostream &os, const Player &player)
{
    os << "\n--- Player Stats ---\n";
    os << "Name: " << player.getName() << "\n";
    os << "Health: " << player.getHealth() << "\n";
    os << "Moves Left: " << player.getMoves() << "\n";
    os << "Coins Collected: " << player.getCoins() << "\n";
    os << "Enemies Defeated: " << player.getEnemiesDefeated() << "\n";
    os << "Inventory (Sorted): ";
    player.getInventory();
    for (const auto &item : player.getInventory())
        os << item << " ";
    os << "\n--------------------\n";
    return os;
}

/**
 * @brief Represents an enemy character in the game.
 * Inherits from Character and includes a description specific to the enemy.
 */
class Enemy : public Character
{
private:
    string description;

public:
    /**
     * @brief Constructor for the Enemy class.
     * @param n The name of the enemy.
     * @param desc A description of the enemy.
     * @param hp The health points of the enemy.
     */
    Enemy(string n, string desc, int hp) : Character(n, hp), description(desc) {}

    /**
     * @brief Gets the description of the enemy.
     * @return The enemy's description.
     */
    string getDescription() const { return description; }

    /**
     * @brief Displays the enemy's basic status (name and health required to win) to the console.
     * Overrides the pure virtual function from Character.
     */
    void displayStatus() const override
    {
        cout << "Enemy: " << name << " | Health Required to Win: " << health << endl;
    }
};

/**
 * @brief Represents a treasure found in a room.
 * Contains two items and a key.
 */
class Treasure
{
private:
    string item1, item2, key;

public:
    /**
     * @brief Constructor for the Treasure class.
     * @param i1 The first item in the treasure.
     * @param i2 The second item in the treasure.
     * @param k The key associated with the treasure.
     */
    Treasure(string i1, string i2, string k) : item1(i1), item2(i2), key(k) {}

    /**
     * @brief Gets the first item from the treasure.
     * @return The first item string.
     */
    string getItem1() const { return item1; }

    /**
     * @brief Gets the second item from the treasure.
     * @return The second item string.
     */
    string getItem2() const { return item2; }

    /**
     * @brief Gets the key from the treasure.
     * @return The key string.
     */
    string getKey() const { return key; }
};

/**
 * @brief Represents a single room within the dungeon.
 * Each room has a name, an enemy, a treasure, and a challenge.
 */
class Room
{
private:
    string name;
    Enemy enemy;
    Treasure treasure;
    string challenge;

public:
    /**
     * @brief Constructor for the Room class.
     * @param n The name of the room.
     * @param e The Enemy present in the room.
     * @param t The Treasure found in the room.
     * @param c The challenge associated with the room.
     */
    Room(string n, Enemy e, Treasure t, string c) : name(n), enemy(e), treasure(t), challenge(c) {}

    /**
     * @brief Gets the name of the room.
     * @return The room's name.
     */
    string getName() const { return name; }

    /**
     * @brief Gets the enemy present in the room.
     * @return A constant reference to the Enemy object.
     */
    const Enemy &getEnemy() const { return enemy; }

    /**
     * @brief Gets the treasure found in the room.
     * @return A constant reference to the Treasure object.
     */
    const Treasure &getTreasure() const { return treasure; }

    /**
     * @brief Gets the challenge associated with the room.
     * @return The challenge string.
     */
    string getChallenge() const { return challenge; }
};

/**
 * @brief A templated manager class for storing and retrieving game assets.
 * Uses unique_ptr to manage memory for stored assets, ensuring proper deallocation.
 * @tparam T The type of asset to manage (e.g., Room, Enemy, etc.).
 */
template <typename T>
class GameAssetManager
{
private:
    vector<unique_ptr<T>> assets; // Stores assets using smart pointers

public:
    /**
     * @brief Adds a new asset to the manager.
     * Takes ownership of the unique_ptr.
     * @param asset A unique_ptr to the asset to add.
     */
    void addAsset(unique_ptr<T> asset)
    {
        assets.push_back(move(asset)); // Move semantics to transfer ownership
    }

    /**
     * @brief Retrieves a constant pointer to an asset at a specific index.
     * Throws an out_of_range exception if the index is invalid.
     * @param index The index of the asset to retrieve.
     * @return A constant pointer to the asset.
     * @throws out_of_range If the index is outside the bounds of the assets vector.
     */
    const T *getAsset(size_t index) const
    {
        if (index >= 0 && index < assets.size())
        {
            return assets[index].get(); // Get raw pointer from unique_ptr
        }
        throw out_of_range("Asset index out of bounds."); // Exception for invalid access
    }

    /**
     * @brief Gets the total number of assets currently managed.
     * @return The count of assets.
     */
    size_t getAssetCount() const
    {
        return assets.size();
    }
};

/**
 * @brief Represents the dungeon structure, containing multiple rooms, an enemy queue, and a room stack for navigation.
 */
class Dungeon
{
private:
    GameAssetManager<Room> roomManager; // Manages rooms using the templated asset manager
    queue<Enemy> enemyQueue;            // Queue of enemies (not directly used for gameplay logic, but for demonstration)
    stack<const Room *> roomStack;      // Stack to keep track of visited rooms for backtracking
    int currentRoomIndex;               // Index of the current room in the roomManager

public:
    /**
     * @brief Constructor for the Dungeon class.
     * Initializes the rooms and populates the enemy queue.
     */
    Dungeon() : currentRoomIndex(0)
    { // Initialize currentRoomIndex to 0 for the first room
        // Add rooms to the room manager
        roomManager.addAsset(make_unique<Room>("Base",
                                               Enemy("Shadow Stalker", "A stealthy, dark creature.", 15),
                                               Treasure("5 Coins", "Armour", "Key1"),
                                               "Collect 5 coins"));
        roomManager.addAsset(make_unique<Room>("Bronze",
                                               Enemy("Viper", "A venomous menace.", 25),
                                               Treasure("5 Coins", "Health Booster Potion", "Key2"),
                                               "Exit the room within 5 seconds"));
        roomManager.addAsset(make_unique<Room>("Platinum",
                                               Enemy("Crawler", "A fast, wall-climbing creature.", 35),
                                               Treasure("Health Booster Potion", "Armour", "Key3"),
                                               "Defeat the enemy without armour"));
        roomManager.addAsset(make_unique<Room>("Silver",
                                               Enemy("Hunter", "A swift and deadly assassin.", 50),
                                               Treasure("5 Coins", "Armour", "Key4"),
                                               "Riddle: I have no voice, but I can teach you all I know. What am I? (Answer: book)"));
        roomManager.addAsset(make_unique<Room>("Gold",
                                               Enemy("Boss", "The ultimate challenge.", 70),
                                               Treasure("5 Coins", "Health Booster Potion", "Key5"),
                                               "Defeat the boss"));

        // Populate enemy queue using a lambda to process rooms
        // This demonstrates iterating through assets and handling potential exceptions.
        for (size_t i = 0; i < roomManager.getAssetCount(); ++i)
        {
            try
            {
                const Room *room = roomManager.getAsset(i);
                enemyQueue.push(room->getEnemy());
            }
            catch (const out_of_range &e)
            {
                // Catching exception if getAsset fails, though unlikely with a valid loop
                cerr << "Error loading enemy for room: " << e.what() << endl;
            }
        }
    }

    /**
     * @brief Returns the game rules as a string.
     * @return A string containing the game rules.
     */
    string getRules() const
    {
        return "\nWelcome to Dungeon Escape!\n\n"
               "1. You have 10 moves to escape the dungeon.\n"
               "2. Each room has an enemy, a treasure, and a challenge.\n"
               "3. Defeating enemies gets you treasure.\n"
               "4. If your health drops below 20, you lose.\n"
               "5. Clear the final room to win.\n\n"
               "Good luck!\n";
    }

    /**
     * @brief Gets the current room the player is in.
     * Uses exception handling for safe access to roomManager.
     * @return A constant pointer to the current Room object, or nullptr if no room is set or an error occurs.
     */
    const Room *getCurrentRoom() const
    {
        try
        {
            return roomManager.getAsset(currentRoomIndex);
        }
        catch (const out_of_range &e)
        {
            // Log the error but return nullptr to indicate no current room
            cerr << "Error getting current room: " << e.what() << endl;
            return nullptr;
        }
    }

    /**
     * @brief Advances the player to the next room in the dungeon.
     * Pushes the new room onto the room stack.
     * @return A constant pointer to the next Room object, or nullptr if there are no more rooms.
     */
    const Room *advanceToNextRoom()
    {
        if (currentRoomIndex < static_cast<int>(roomManager.getAssetCount()))
        { // Allow advancing to currentRoomIndex if it matches size, then increment
            // If currentRoomIndex is valid, push current room before advancing
            if (currentRoomIndex >= 0 && currentRoomIndex < static_cast<int>(roomManager.getAssetCount()))
            {
                try
                {
                    const Room *current = roomManager.getAsset(currentRoomIndex);
                    roomStack.push(current);
                }
                catch (const out_of_range &e)
                {
                    cerr << "Error pushing current room to stack (unexpected): " << e.what() << endl;
                    return nullptr;
                }
            }

            // Increment currentRoomIndex to point to the next room
            currentRoomIndex++;

            if (currentRoomIndex < static_cast<int>(roomManager.getAssetCount()))
            { // Check bounds after increment
                try
                {
                    const Room *nextRoom = roomManager.getAsset(currentRoomIndex);
                    return nextRoom;
                }
                catch (const out_of_range &e)
                {
                    // Should not happen if currentRoomIndex is valid relative to getAssetCount
                    cerr << "Error advancing to next room (unexpected): " << e.what() << endl;
                    return nullptr;
                }
            }
        }
        return nullptr; // No more rooms
    }

    /**
     * @brief Allows the player to backtrack to the previously visited room.
     * Pops the current room from the stack and sets the previous room as current.
     * @return A constant pointer to the previous Room object, or nullptr if no previous room exists.
     */
    const Room *backtrack()
    {
        if (roomStack.size() > 1)
        {                    // Need at least two rooms to backtrack
            roomStack.pop(); // Remove current room
            if (!roomStack.empty())
            {                                           // Check if stack is not empty after popping
                const Room *prevRoom = roomStack.top(); // Get the previous room
                // Find the index of the previous room
                // This loop is necessary to update currentRoomIndex to match the room on stack
                for (size_t i = 0; i < roomManager.getAssetCount(); ++i)
                {
                    try
                    {
                        if (roomManager.getAsset(i) == prevRoom)
                        {
                            currentRoomIndex = static_cast<int>(i);
                            break;
                        }
                    }
                    catch (const out_of_range &e)
                    {
                        // Log error if getAsset fails during backtrack search
                        cerr << "Error during backtrack room search: " << e.what() << endl;
                    }
                }
                return prevRoom;
            }
        }
        return nullptr; // Cannot backtrack further
    }

    /**
     * @brief Displays the final ranking and player stats to the console after the game ends.
     * @param player The Player object whose stats are to be displayed.
     */
    void displayRanking(const Player &player) const
    {
        cout << "\n======== GAME OVER ========\n"
             << player;
    }
};

// Enum to manage different game states for the GUI
enum class GameState
{
    NAME_INPUT,
    INSTRUCTIONS,
    PLAYING,
    GAME_OVER
};

/**
 * @brief Manages the Graphical User Interface (GUI) for the Dungeon Escape game.
 */
class GUI
{
private:
    sf::RenderWindow window;
    sf::Font font;
    bool fontLoaded;

    // UI Elements
    sf::Text titleText, instructionsText, rulesTitleText, rulesBodyText, startButtonLabel;
    sf::Text statusText[7];
    sf::RectangleShape buttons[4];
    sf::Text buttonLabels[4];
    sf::RectangleShape startButton;
    sf::RectangleShape nameInputField, statusPanel;
    sf::Text nameInputText, namePromptText;

    // Colors
    sf::Color bgColor = sf::Color(30, 30, 40);
    sf::Color panelColor = sf::Color(45, 45, 55);
    sf::Color buttonColor = sf::Color(60, 60, 75);
    sf::Color buttonHoverColor = sf::Color(80, 80, 100);
    sf::Color textColor = sf::Color(200, 200, 220);
    sf::Color titleColor = sf::Color(255, 215, 0);
    sf::Color healthGoodColor = sf::Color(100, 255, 100);
    sf::Color healthWarningColor = sf::Color(255, 255, 100);
    sf::Color healthCriticalColor = sf::Color(255, 100, 100);
    sf::Color messageColor = sf::Color(240, 240, 240);

    string enteredName;
    string statusMessage;

public:
    GUI() : window(sf::VideoMode(800, 600), "Dungeon Escape", sf::Style::Close | sf::Style::Titlebar), fontLoaded(false), enteredName("")
    {
        try
        {
            if (!font.loadFromFile("C:/Windows/Fonts/segoeui.ttf"))
            {
                throw runtime_error("Could not load system font 'segoeui.ttf'.");
            }
            fontLoaded = true;
        }
        catch (const runtime_error &e)
        {
            cerr << "Error: " << e.what() << " Text will not display.\n";
        }
        setupUI();
    }

    bool isOpen() const { return window.isOpen(); }
    void close() { window.close(); }
    bool pollEvent(sf::Event &event) { return window.pollEvent(event); }
    string getPlayerName() const { return enteredName; }

    /**
     * @brief Handles a single SFML event and updates game state accordingly.
     * @param event The SFML event to process.
     * @param gameState The current game state (will be modified).
     * @param choice The player's action choice (will be set).
     */
    void handleEvent(const sf::Event &event, GameState &gameState, int &choice)
    {
        if (event.type == sf::Event::Closed)
            close();

        switch (gameState)
        {
        case GameState::NAME_INPUT:
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128)
                {
                    if (event.text.unicode == 8 && !enteredName.empty()) // Backspace
                        enteredName.pop_back();
                    else if (event.text.unicode == 13) // Enter
                        gameState = GameState::INSTRUCTIONS;
                    else if (event.text.unicode != 8 && event.text.unicode != 13)
                        enteredName += static_cast<char>(event.text.unicode);
                    nameInputText.setString(enteredName);
                }
            }
            break;

        case GameState::INSTRUCTIONS:
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                if (startButton.getGlobalBounds().contains(mousePos))
                    gameState = GameState::PLAYING;
            }
            break;

        case GameState::PLAYING:
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                for (int i = 0; i < 4; ++i)
                {
                    if (buttons[i].getGlobalBounds().contains(mousePos))
                    {
                        choice = i + 1;
                        break;
                    }
                }
            }
            break;

        case GameState::GAME_OVER:
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)
                close();
            break;
        }
    }

    /**
     * @brief Updates all visual elements (status, hover effects, etc.).
     */
    void update(GameState gameState, const Player &player, const Room *room, const string &message)
    {
        if (!fontLoaded)
            return;

        // Update hover effects based on current mouse position
        sf::Vector2f mousePos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        if (gameState == GameState::INSTRUCTIONS)
        {
            startButton.setFillColor(startButton.getGlobalBounds().contains(mousePos) ? buttonHoverColor : buttonColor);
        }
        if (gameState == GameState::PLAYING)
        {
            for (int i = 0; i < 4; ++i)
            {
                buttons[i].setFillColor(buttons[i].getGlobalBounds().contains(mousePos) ? buttonHoverColor : buttonColor);
            }
        }

        // Update status text
        updateStatus(player, room, message);
    }

    /**
     * @brief Draws all GUI elements based on the current game state.
     * @param gameState The current state of the game.
     * @param rules The game rules string for the instructions screen.
     * @param gameOverMessage The message to display on the game over screen.
     * @param player The player object to display stats from on the game over screen.
     */
    void draw(GameState gameState, const string &rules, const string &gameOverMessage, const Player &player) // Modified
    {
        window.clear(bgColor);
        if (!fontLoaded)
        {
            // Fallback if font fails
            sf::Text errorText("Font not loaded!", font, 24);
            errorText.setFillColor(sf::Color::Red);
            window.draw(errorText);
            window.display();
            return;
        }

        switch (gameState)
        {
        case GameState::NAME_INPUT:
            window.draw(namePromptText);
            window.draw(nameInputField);
            window.draw(nameInputText);
            break;
        case GameState::INSTRUCTIONS:
            rulesBodyText.setString(rules);
            window.draw(rulesTitleText);
            window.draw(rulesBodyText);
            window.draw(startButton);
            window.draw(startButtonLabel);
            break;
        case GameState::PLAYING:
            window.draw(titleText);
            window.draw(instructionsText);
            window.draw(statusPanel);
            for (int i = 0; i < 7; ++i)
                window.draw(statusText[i]);
            for (int i = 0; i < 4; ++i)
            {
                window.draw(buttons[i]);
                window.draw(buttonLabels[i]);
            }
            if (!statusMessage.empty())
            {
                sf::Text messageText(statusMessage, font, 20);
                messageText.setFillColor(messageColor);
                messageText.setPosition(20.f, 520.f);
                window.draw(messageText);
            }
            break;
        case GameState::GAME_OVER:
            drawGameOver(gameOverMessage, player); // Modified
            break;
        }
        window.display();
    }

private:
    void setupUI();
    void updateStatus(const Player &player, const Room *room, const string &message);
    void drawGameOver(const string &message, const Player &player); // Modified

    // Helper to center text origin
    void centerOrigin(sf::Text &text)
    {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    }
};

/**
 * @brief Sets up all the UI elements, texts, buttons, etc.
 */
void GUI::setupUI()
{
    if (!fontLoaded)
        return;

    // Main Game Title & Instructions
    titleText.setFont(font);
    titleText.setString("Dungeon Escape");
    titleText.setCharacterSize(60);
    titleText.setFillColor(titleColor);
    titleText.setStyle(sf::Text::Bold);
    centerOrigin(titleText);
    titleText.setPosition(window.getSize().x / 2.f, 60.f);
    instructionsText.setFont(font);
    instructionsText.setString("Choose an action:");
    instructionsText.setCharacterSize(28);
    instructionsText.setFillColor(textColor);
    instructionsText.setPosition(20.f, 120.f);

    // Status Panel & Text
    statusPanel.setSize({window.getSize().x - 40.f, 200.f});
    statusPanel.setFillColor(panelColor);
    statusPanel.setPosition(20.f, 180.f);
    statusPanel.setOutlineThickness(1.f);
    statusPanel.setOutlineColor({80, 80, 95});
    for (int i = 0; i < 7; ++i)
    {
        statusText[i].setFont(font);
        statusText[i].setCharacterSize(20);
        statusText[i].setFillColor(textColor);
        statusText[i].setPosition(40.f, 200.f + i * 25);
    }

    // Main Action Buttons
    const sf::Vector2f buttonSize(180.f, 55.f);
    const string labels[] = {"Fight", "Bypass", "Backtrack", "Quit"};
    for (int i = 0; i < 4; ++i)
    {
        buttons[i].setSize(buttonSize);
        buttons[i].setPosition(20.f + i * (buttonSize.x + 20.f), 440.f);
        buttons[i].setOutlineThickness(2.f);
        buttons[i].setOutlineColor({100, 100, 120});
        buttonLabels[i].setFont(font);
        buttonLabels[i].setString(labels[i]);
        buttonLabels[i].setCharacterSize(22);
        buttonLabels[i].setFillColor(textColor);
        centerOrigin(buttonLabels[i]);
        buttonLabels[i].setPosition(buttons[i].getPosition().x + buttonSize.x / 2.f, buttons[i].getPosition().y + buttonSize.y / 2.f);
    }

    // Name Input Screen
    namePromptText.setFont(font);
    namePromptText.setString("Enter your name and press Enter:");
    namePromptText.setCharacterSize(30);
    namePromptText.setFillColor(textColor);
    centerOrigin(namePromptText);
    namePromptText.setPosition(window.getSize().x / 2.f, 220.f);
    nameInputField.setSize({400.f, 50.f});
    nameInputField.setFillColor(sf::Color::White);
    nameInputField.setOutlineColor({100, 100, 100});
    nameInputField.setOutlineThickness(2.f);
    nameInputField.setOrigin(200.f, 25.f);
    nameInputField.setPosition(window.getSize().x / 2.f, 280.f);
    nameInputText.setFont(font);
    nameInputText.setCharacterSize(28);
    nameInputText.setFillColor(sf::Color::Black);
    nameInputText.setPosition(nameInputField.getPosition().x - 190, nameInputField.getPosition().y - 15);

    // Instructions Screen
    rulesTitleText.setFont(font);
    rulesTitleText.setString("Game Instructions");
    rulesTitleText.setCharacterSize(50);
    rulesTitleText.setFillColor(titleColor);
    rulesTitleText.setStyle(sf::Text::Bold);
    centerOrigin(rulesTitleText);
    rulesTitleText.setPosition(window.getSize().x / 2.f, 80.f);
    rulesBodyText.setFont(font);
    rulesBodyText.setCharacterSize(24);
    rulesBodyText.setFillColor(textColor);
    rulesBodyText.setPosition(100.f, 150.f);
   // CORRECTED LINES
startButton.setSize({200.f, 60.f});
startButton.setOutlineThickness(2.f);
startButton.setOutlineColor({100, 100, 120});
// Manually set the origin for the RectangleShape
startButton.setOrigin(startButton.getSize().x / 2.f, startButton.getSize().y / 2.f);
startButton.setPosition(window.getSize().x / 2.f, 480.f);
    startButtonLabel.setFont(font);
    startButtonLabel.setString("Start Game");
    startButtonLabel.setCharacterSize(28);
    startButtonLabel.setFillColor(textColor);
    centerOrigin(startButtonLabel);
    startButtonLabel.setPosition(startButton.getPosition());
}

/**
 * @brief Updates the status text elements.
 */
void GUI::updateStatus(const Player &player, const Room *room, const string &message)
{
    statusText[0].setString("Room: " + (room ? room->getName() : "N/A"));
    statusText[1].setString("Health: " + to_string(player.getHealth()));
    statusText[2].setString("Moves Remaining: " + to_string(player.getMoves()));
    statusText[3].setString("Enemy: " + (room ? room->getEnemy().getName() : "N/A"));
    statusText[4].setString("Enemy Desc: " + (room ? room->getEnemy().getDescription() : "N/A"));
    statusText[5].setString("Coins: " + to_string(player.getCoins()));

    stringstream ss;
    ss << "Inventory: ";
    if (player.getInventory().empty())
        ss << "Empty";
    else
    {
        string invStr;
        for (const auto &item : player.getInventory())
            invStr += item + ", ";
        ss << invStr.substr(0, invStr.length() - 2);
    }
    statusText[6].setString(ss.str());

    if (player.getHealth() > 50)
        statusText[1].setFillColor(healthGoodColor);
    else if (player.getHealth() > 20)
        statusText[1].setFillColor(healthWarningColor);
    else
        statusText[1].setFillColor(healthCriticalColor);

    statusMessage = message;
}

/**
 * @brief Draws the specific Game Over screen, including player stats.
 * @param message The game over message.
 * @param player The Player object whose stats are to be displayed.
 */
void GUI::drawGameOver(const string &message, const Player &player) // Modified
{
    sf::Text gameOverText(message, font, 40);
    gameOverText.setFillColor(titleColor);
    gameOverText.setStyle(sf::Text::Bold);
    centerOrigin(gameOverText);
    gameOverText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 150); // Adjusted Y to make space

    window.draw(gameOverText);

    // Display Player Stats
    float currentY = window.getSize().y / 2.0f - 80;
    float lineHeight = 25.0f;
    float startX = window.getSize().x / 2.0f - 150; // Aligns stats to the left

    sf::Text playerNameText("Name: " + player.getName(), font, 20);
    playerNameText.setFillColor(textColor);
    playerNameText.setPosition(startX, currentY);
    window.draw(playerNameText);
    currentY += lineHeight;

    sf::Text playerHealthText("Health: " + to_string(player.getHealth()), font, 20);
    playerHealthText.setFillColor(textColor);
    playerHealthText.setPosition(startX, currentY);
    window.draw(playerHealthText);
    currentY += lineHeight;

    sf::Text playerMovesText("Moves Left: " + to_string(player.getMoves()), font, 20);
    playerMovesText.setFillColor(textColor);
    playerMovesText.setPosition(startX, currentY);
    window.draw(playerMovesText);
    currentY += lineHeight;

    sf::Text playerCoinsText("Coins Collected: " + to_string(player.getCoins()), font, 20);
    playerCoinsText.setFillColor(textColor);
    playerCoinsText.setPosition(startX, currentY);
    window.draw(playerCoinsText);
    currentY += lineHeight;

    sf::Text playerEnemiesText("Enemies Defeated: " + to_string(player.getEnemiesDefeated()), font, 20);
    playerEnemiesText.setFillColor(textColor);
    playerEnemiesText.setPosition(startX, currentY);
    window.draw(playerEnemiesText);
    currentY += lineHeight;

    stringstream inventorySs;
    inventorySs << "Inventory (Sorted): ";
    if (player.getInventory().empty())
        inventorySs << "Empty";
    else
    {
        string invStr;
        for (const auto &item : player.getInventory())
            invStr += item + ", ";
        // Remove trailing ", " if exists
        if (invStr.length() > 2) {
            inventorySs << invStr.substr(0, invStr.length() - 2);
        } else {
             inventorySs << invStr; // Handle case where it might be " " or empty
        }
    }
    sf::Text playerInventoryText(inventorySs.str(), font, 20);
    playerInventoryText.setFillColor(textColor);
    playerInventoryText.setPosition(startX, currentY);
    window.draw(playerInventoryText);
    currentY += lineHeight;


    // Adjust prompt text position
    sf::Text promptText("Click or press any key to exit.", font, 20);
    promptText.setFillColor(textColor);
    centerOrigin(promptText);
    promptText.setPosition(window.getSize().x / 2.0f, currentY + 50); // Move prompt further down

    window.draw(promptText);
}

/**
 * @brief The main game loop that integrates game logic with the SFML GUI.
 */
void gameLoopWithGUI(Player &player, Dungeon &dungeon, GUI &gui)
{
    GameState gameState = GameState::NAME_INPUT;
    const Room *currentRoom = nullptr;
    string message = "";
    string gameOverMessage = "";

    while (gui.isOpen())
    {
        // 1. EVENT HANDLING
        int choice = -1;
        sf::Event event;
        while (gui.pollEvent(event))
        {
            gui.handleEvent(event, gameState, choice);
        }

        // 2. GAME LOGIC UPDATES
        if (gameState == GameState::PLAYING)
        {
            // First time entering PLAYING state, initialize the first room
            if (currentRoom == nullptr)
            {
                currentRoom = dungeon.advanceToNextRoom();
                if (!currentRoom)
                {
                    gameOverMessage = "Error: No rooms available.";
                    gameState = GameState::GAME_OVER;
                }
                else
                {
                    message = "You have entered the " + currentRoom->getName() + " room.";
                }
            }

            // Check for a player action from the event handler
            if (choice > 0)
            {
                player.useMove();
                switch (choice)
                {
                case 1: // Fight
                {
                    const Enemy &enemy = currentRoom->getEnemy();
                    if (player.getHealth() >= enemy.getHealth())
                    {
                        player.takeDamage(enemy.getHealth());
                        player.addToInventory(currentRoom->getTreasure().getItem1());
                        player.addToInventory(currentRoom->getTreasure().getItem2());
                        player.addCoins(10);
                        player.incrementEnemiesDefeated();
                        message = "Victory! You defeated the " + enemy.getName() + ".";
                        currentRoom = dungeon.advanceToNextRoom();
                    }
                    else
                    {
                        player.takeDamage(10);
                        message = "Too weak! You fled and took damage.";
                    }
                }
                break;
                case 2: // Bypass
                    player.takeDamage(5);
                    message = "You bypassed the enemy, taking minor damage.";
                    currentRoom = dungeon.advanceToNextRoom();
                    break;
                case 3: // Backtrack
                {
                    const Room *previousRoom = dungeon.backtrack();
                    if (previousRoom)
                    {
                        currentRoom = previousRoom;
                        message = "You backtracked to the " + currentRoom->getName() + " room.";
                    }
                    else
                    {
                        message = "No room to backtrack to!";
                    }
                }
                break;
                case 4: // Quit
                    gameOverMessage = "You have quit the dungeon.";
                    gameState = GameState::GAME_OVER;
                    break;
                }
                // Check for win/lose conditions after an action
                if (gameState == GameState::PLAYING)
                {
                    if (!currentRoom)
                    {
                        gameOverMessage = "Congratulations! You escaped!";
                        gameState = GameState::GAME_OVER;
                    }
                    else if (player.getHealth() < 20)
                    {
                        gameOverMessage = "Game Over! Your health is critical.";
                        gameState = GameState::GAME_OVER;
                    }
                    else if (player.getMoves() <= 0)
                    {
                        gameOverMessage = "Game Over! You ran out of moves.";
                        gameState = GameState::GAME_OVER;
                    }
                }
            }
        }

        if (gameState == GameState::GAME_OVER)
        {
            player.sortInventory();
        }

        // 3. UPDATE & DRAW
        gui.update(gameState, player, currentRoom, message);
        gui.draw(gameState, dungeon.getRules(), gameOverMessage, player); // Modified
    }
}

/**
 * @brief Main function of the Dungeon Escape game.
 */
int main()
{
    cout << "Welcome to Dungeon Escape (GUI Mode)!\n";

    GUI gui;
    if (!gui.isOpen())
    {
        cerr << "Failed to initialize GUI. Exiting.\n";
        return 1;
    }

    // Wait for name input to finish before creating player
    while (gui.isOpen())
    {
        GameState tempState = GameState::NAME_INPUT;
        int dummyChoice;
        sf::Event event;
        while (gui.pollEvent(event))
        {
            gui.handleEvent(event, tempState, dummyChoice);
            if (event.type == sf::Event::Closed)
                gui.close();
        }

        if (tempState != GameState::NAME_INPUT)
            break; // Exit loop once name is entered

        // Pass a temporary player object for initial draw as actual player isn't created yet
        gui.update(GameState::NAME_INPUT, Player(""), nullptr, "");
        gui.draw(GameState::NAME_INPUT, "", "", Player("")); // Pass empty player for name input screen
    }

    string playerName = gui.getPlayerName();
    if (playerName.empty())
    {
        playerName = "Adventurer";
    }

    Player player(playerName);
    Dungeon dungeon;

    // Start the main game loop
    gameLoopWithGUI(player, dungeon, gui);

    // After GUI closes, display final stats to console (optional, as GUI now shows them)
    dungeon.displayRanking(player);

    cout << "Thanks for playing Dungeon Escape!" << endl;
    return 0;
}
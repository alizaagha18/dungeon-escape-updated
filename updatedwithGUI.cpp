#include <SFML/Graphics.hpp> // Required for SFML graphics functionalities
#include <iostream>          // Required for input/output operations (cout, cin, cerr)
#include <string>            // Required for string manipulation
#include <vector>            // Required for std::vector container
#include <queue>             // Required for std::queue container
#include <stack>             // Required for std::stack container
#include <memory>            // Required for smart pointers (std::unique_ptr)
#include <list>              // Required for std::list container
#include <algorithm>         // Required for std::transform and std::sort (for sorting)
#include <limits>            // Required for numeric_limits (though not explicitly used for limits in the final code)
#include <sstream>           // Required for std::stringstream for string building
#include <stdexcept>         // Required for standard exception types (e.g., out_of_range, runtime_error)

using namespace std; // Using the standard namespace to avoid prefixing std::

/**
 * @brief Base abstract class for all characters in the game.
 * Provides common attributes like name and health, and a pure virtual function for displaying status.
 */
class Character
{
protected: // Protected members are accessible within the class and by derived classes.
    string name;
    int health;

public: // Public members are accessible from outside the class.
    /**
     * @brief Constructor for the Character class.
     * @param n The name of the character.
     * @param h The initial health of the character.
     */
    Character(string n, int h) : name(n), health(h) {}

    /**
     * @brief Virtual destructor to ensure proper cleanup of derived classes.
     * It's crucial for polymorphic classes to have a virtual destructor.
     */
    virtual ~Character() = default; // Using default destructor as no custom cleanup is needed.

    /**
     * @brief Pure virtual function to display the status of the character.
     * Must be implemented by derived classes, making Character an abstract class.
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
            health = 0; // Ensure health does not go negative.
    }
};

/**
 * @brief Represents the player character in the game.
 * Inherits from Character and includes player-specific attributes like inventory, moves, coins, and enemies defeated.
 */
class Player : public Character
{
private:
    list<string> inventory; // Player's inventory, stored as a list of strings.
    int moves;              // Number of moves remaining for the player.
    int coins;              // Total coins collected by the player.
    int enemiesDefeated;    // Count of enemies the player has defeated.

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
            health = 100; // Ensure health does not exceed maximum.
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
        stringstream ss; // Use stringstream to convert any type T to a string.
        ss << item;
        inventory.push_back(ss.str()); // Add the string representation of the item to the list.
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
            moves--; // Only decrement if moves are available.
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
        // Sorts the list using a lambda for case-insensitive comparison.
        inventory.sort([](const string &a, const string &b)
                       {
                           string lowerA, lowerB;
                           // Convert strings to lowercase for comparison.
                           transform(a.begin(), a.end(), back_inserter(lowerA), ::tolower);
                           transform(b.begin(), b.end(), back_inserter(lowerB), ::tolower);
                           return lowerA < lowerB; // Lexicographical comparison of lowercase strings.
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
    player.getInventory(); // This call doesn't modify the internal list, it returns a copy.
                           // The inventory is assumed to be sorted by player.sortInventory() before this is called for display.
    for (const auto &item : player.getInventory())
        os << item << " "; // Iterate and print each item.
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
    string description; // Unique description for the enemy.

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
    string item1, item2, key; // Two items and a key composing the treasure.

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
    string name;       // Name of the room.
    Enemy enemy;       // The enemy residing in this room.
    Treasure treasure; // The treasure found in this room.
    string challenge;  // A specific challenge for this room.

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
    vector<unique_ptr<T>> assets; // Stores assets using smart pointers (unique ownership).

public:
    /**
     * @brief Adds a new asset to the manager.
     * Takes ownership of the unique_ptr.
     * @param asset A unique_ptr to the asset to add.
     */
    void addAsset(unique_ptr<T> asset)
    {
        assets.push_back(move(asset)); // Use std::move to transfer ownership of the unique_ptr.
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
        if (index < assets.size()) // Check if the index is within valid bounds.
        {
            return assets[index].get(); // Return the raw pointer managed by unique_ptr.
        }
        // Throw an exception for invalid access.
        throw out_of_range("Asset index out of bounds.");
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
    GameAssetManager<Room> roomManager; // Manages rooms using the templated asset manager.
    queue<Enemy> enemyQueue;            // A queue to store enemies (demonstrates queue usage).
    stack<const Room *> roomStack;      // A stack to keep track of visited rooms for backtracking.
    int currentRoomIndex;               // The index of the current room within the roomManager.

public:
    /**
     * @brief Constructor for the Dungeon class.
     * Initializes the rooms and populates the enemy queue.
     */
    Dungeon() : currentRoomIndex(0) // Initialize currentRoomIndex to 0 for the first room.
    {
        // Add predefined rooms to the room manager.
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

        // Populate enemy queue by iterating through managed rooms.
        for (size_t i = 0; i < roomManager.getAssetCount(); ++i)
        {
            try
            {
                const Room *room = roomManager.getAsset(i); // Get room using the manager.
                enemyQueue.push(room->getEnemy());          // Add the enemy to the queue.
            }
            catch (const out_of_range &e)
            {
                // Catching exception if getAsset fails, though unlikely with a valid loop.
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
            return roomManager.getAsset(currentRoomIndex); // Attempt to get the current room.
        }
        catch (const out_of_range &e)
        {
            // Log the error but return nullptr to indicate no current room.
            cerr << "Error getting current room: " << e.what() << endl;
            return nullptr;
        }
    }

    /**
     * @brief Advances the player to the next room in the dungeon.
     * Pushes the current room onto the room stack before advancing.
     * @return A constant pointer to the next Room object, or nullptr if there are no more rooms.
     */
    const Room *advanceToNextRoom()
    {
        if (currentRoomIndex < static_cast<int>(roomManager.getAssetCount()))
        {
            // If currentRoomIndex is valid, push current room before advancing.
            if (currentRoomIndex >= 0 && currentRoomIndex < static_cast<int>(roomManager.getAssetCount()))
            {
                try
                {
                    const Room *current = roomManager.getAsset(currentRoomIndex);
                    roomStack.push(current); // Push current room onto stack for backtracking.
                }
                catch (const out_of_range &e)
                {
                    cerr << "Error pushing current room to stack (unexpected): " << e.what() << endl;
                    return nullptr;
                }
            }

            // Increment currentRoomIndex to point to the next room.
            currentRoomIndex++;

            // Check bounds again after incrementing to ensure the next room exists.
            if (currentRoomIndex < static_cast<int>(roomManager.getAssetCount()))
            {
                try
                {
                    const Room *nextRoom = roomManager.getAsset(currentRoomIndex);
                    return nextRoom; // Return the next room.
                }
                catch (const out_of_range &e)
                {
                    // Should not happen if currentRoomIndex is valid relative to getAssetCount.
                    cerr << "Error advancing to next room (unexpected): " << e.what() << endl;
                    return nullptr;
                }
            }
        }
        return nullptr; // No more rooms to advance to.
    }

    /**
     * @brief Allows the player to backtrack to the previously visited room.
     * Pops the current room from the stack and sets the previous room as current.
     * @return A constant pointer to the previous Room object, or nullptr if no previous room exists.
     */
    const Room *backtrack()
    {
        if (roomStack.size() > 1)
        {                               // Need at least two rooms in stack to backtrack (current + previous).
            roomStack.pop();            // Remove current room from the stack.
            if (!roomStack.empty())     // Check if stack is not empty after popping.
            {
                const Room *prevRoom = roomStack.top(); // Get the previous room from the top of the stack.
                // Find the index of the previous room to update currentRoomIndex.
                for (size_t i = 0; i < roomManager.getAssetCount(); ++i)
                {
                    try
                    {
                        if (roomManager.getAsset(i) == prevRoom)
                        {
                            currentRoomIndex = static_cast<int>(i); // Update the current room index.
                            break;                                   // Found the room, exit loop.
                        }
                    }
                    catch (const out_of_range &e)
                    {
                        // Log error if getAsset fails during backtrack search.
                        cerr << "Error during backtrack room search: " << e.what() << endl;
                    }
                }
                return prevRoom;
            }
        }
        return nullptr; // Cannot backtrack further (stack is empty or only has one room).
    }

    /**
     * @brief Displays the final ranking and player stats to the console after the game ends.
     * @param player The Player object whose stats are to be displayed.
     */
    void displayRanking(const Player &player) const
    {
        cout << "\n======== GAME OVER ========\n"
             << player; // Uses the overloaded operator<< for Player.
    }
};

// Enum to manage different game states for the GUI.
enum class GameState
{
    NAME_INPUT,   // State for player name entry.
    INSTRUCTIONS, // State for displaying game instructions.
    PLAYING,      // Main game loop state.
    GAME_OVER     // State for displaying game over screen.
};

/**
 * @brief Manages the Graphical User Interface (GUI) for the Dungeon Escape game.
 * Uses SFML for rendering and event handling.
 */
class GUI
{
private:
    sf::RenderWindow window; // The SFML window where everything is drawn.
    sf::Font font;           // The font used for all text in the GUI.
    bool fontLoaded;         // Flag to indicate if the font was loaded successfully.

    // UI Elements (SFML Text, Shapes)
    sf::Text titleText, instructionsText, rulesTitleText, rulesBodyText, startButtonLabel;
    sf::Text statusText[7]; // Array of sf::Text for displaying player and room status.
    sf::RectangleShape buttons[4];
    sf::Text buttonLabels[4];
    sf::RectangleShape startButton;
    sf::RectangleShape nameInputField, statusPanel;
    sf::Text nameInputText, namePromptText;

    // Custom Colors for UI.
    sf::Color bgColor = sf::Color(30, 30, 40);           // Background color.
    sf::Color panelColor = sf::Color(45, 45, 55);        // Color for UI panels.
    sf::Color buttonColor = sf::Color(60, 60, 75);       // Default button color.
    sf::Color buttonHoverColor = sf::Color(80, 80, 100); // Button color on hover.
    sf::Color textColor = sf::Color(200, 200, 220);      // Default text color.
    sf::Color titleColor = sf::Color(255, 215, 0);       // Gold-like color for titles.
    sf::Color healthGoodColor = sf::Color(100, 255, 100);    // Green for good health.
    sf::Color healthWarningColor = sf::Color(255, 255, 100); // Yellow for warning health.
    sf::Color healthCriticalColor = sf::Color(255, 100, 100); // Red for critical health.
    sf::Color messageColor = sf::Color(240, 240, 240);       // Color for status messages.

    string enteredName;  // Stores the player's name entered via GUI.
    string statusMessage; // Stores the current message displayed in game (e.g., action results).

public:
    /**
     * @brief Constructor for the GUI class.
     * Initializes the SFML window and attempts to load the font.
     */
    GUI() : window(sf::VideoMode(800, 600), "Dungeon Escape", sf::Style::Close | sf::Style::Titlebar), fontLoaded(false), enteredName("")
    {
        try
        {
            // Attempt to load a system font. Path might need adjustment based on OS.
            if (!font.loadFromFile("C:/Windows/Fonts/segoeui.ttf"))
            {
                throw runtime_error("Could not load system font 'segoeui.ttf'.");
            }
            fontLoaded = true; // Set flag if font loaded successfully.
        }
        catch (const runtime_error &e)
        {
            cerr << "Error: " << e.what() << " Text will not display.\n"; // Log error if font fails.
        }
        setupUI(); // Call helper to set up all UI elements' initial properties.
    }

    /**
     * @brief Checks if the SFML window is currently open.
     * @return True if the window is open, false otherwise.
     */
    bool isOpen() const { return window.isOpen(); }

    /**
     * @brief Closes the SFML window.
     */
    void close() { window.close(); }

    /**
     * @brief Polls for an SFML event.
     * @param event A reference to an sf::Event object to store the polled event.
     * @return True if an event was available, false otherwise.
     */
    bool pollEvent(sf::Event &event) { return window.pollEvent(event); }

    /**
     * @brief Gets the player name entered through the GUI.
     * @return The string containing the player's entered name.
     */
    string getPlayerName() const { return enteredName; }

    /**
     * @brief Handles a single SFML event and updates game state accordingly.
     * @param event The SFML event to process.
     * @param gameState The current game state (will be modified based on input).
     * @param choice The player's action choice (will be set if an action button is clicked).
     */
    void handleEvent(const sf::Event &event, GameState &gameState, int &choice)
    {
        if (event.type == sf::Event::Closed)
            close(); // Close window if the close button is clicked.

        switch (gameState)
        {
        case GameState::NAME_INPUT:
            if (event.type == sf::Event::TextEntered)
            {
                if (event.text.unicode < 128) // Process only ASCII characters.
                {
                    if (event.text.unicode == 8 && !enteredName.empty()) // Backspace key (ASCII 8).
                        enteredName.pop_back();
                    else if (event.text.unicode == 13) // Enter key (ASCII 13).
                        gameState = GameState::INSTRUCTIONS; // Move to instructions screen.
                    else if (event.text.unicode != 8 && event.text.unicode != 13)
                        enteredName += static_cast<char>(event.text.unicode); // Append character to name.
                    nameInputText.setString(enteredName); // Update the SFML text object for display.
                }
            }
            break;

        case GameState::INSTRUCTIONS:
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                if (startButton.getGlobalBounds().contains(mousePos)) // Check if "Start Game" button was clicked.
                    gameState = GameState::PLAYING; // Move to playing state.
            }
            break;

        case GameState::PLAYING:
            if (event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left)
            {
                sf::Vector2f mousePos = window.mapPixelToCoords({event.mouseButton.x, event.mouseButton.y});
                for (int i = 0; i < 4; ++i)
                {
                    if (buttons[i].getGlobalBounds().contains(mousePos)) // Check which action button was clicked.
                    {
                        choice = i + 1; // Set choice (1 for Fight, 2 for Bypass, etc.).
                        break;
                    }
                }
            }
            break;

        case GameState::GAME_OVER:
            if (event.type == sf::Event::KeyPressed || event.type == sf::Event::MouseButtonPressed)
                close(); // Any key press or mouse click closes the window on game over.
            break;
        }
    }

    /**
     * @brief Updates all visual elements (status, hover effects, etc.).
     * @param gameState The current game state.
     * @param player The player object whose stats need to be updated.
     * @param room A pointer to the current room for displaying room info.
     * @param message The current status message to display.
     */
    void update(GameState gameState, const Player &player, const Room *room, const string &message)
    {
        if (!fontLoaded)
            return; // Don't update if font failed to load.

        // Update hover effects for buttons based on current mouse position.
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

        // Update status text based on current player and room data.
        updateStatus(player, room, message);
    }

    /**
     * @brief Draws all GUI elements based on the current game state.
     * @param gameState The current state of the game.
     * @param rules The game rules string for the instructions screen.
     * @param gameOverMessage The message to display on the game over screen.
     * @param player The player object to display stats from on the game over screen.
     */
    void draw(GameState gameState, const string &rules, const string &gameOverMessage, const Player &player)
    {
        window.clear(bgColor); // Clear the window with the background color.
        if (!fontLoaded)
        {
            // Fallback: display an error if font failed to load.
            sf::Text errorText("Font not loaded!", font, 24);
            errorText.setFillColor(sf::Color::Red);
            window.draw(errorText);
            window.display();
            return;
        }

        // Draw elements based on the current game state.
        switch (gameState)
        {
        case GameState::NAME_INPUT:
            window.draw(namePromptText);
            window.draw(nameInputField);
            window.draw(nameInputText);
            break;
        case GameState::INSTRUCTIONS:
            rulesBodyText.setString(rules); // Set rules text.
            window.draw(rulesTitleText);
            window.draw(rulesBodyText);
            window.draw(startButton);
            window.draw(startButtonLabel);
            break;
        case GameState::PLAYING:
            window.draw(titleText);
            window.draw(instructionsText);
            window.draw(statusPanel);
            for (int i = 0; i < 7; ++i) // Draw all status lines.
                window.draw(statusText[i]);
            for (int i = 0; i < 4; ++i) // Draw all action buttons and their labels.
            {
                window.draw(buttons[i]);
                window.draw(buttonLabels[i]);
            }
            if (!statusMessage.empty()) // Draw current status message if not empty.
            {
                sf::Text messageText(statusMessage, font, 20);
                messageText.setFillColor(messageColor);
                messageText.setPosition(20.f, 520.f);
                window.draw(messageText);
            }
            break;
        case GameState::GAME_OVER:
            drawGameOver(gameOverMessage, player); // Call helper to draw game over screen with player stats.
            break;
        }
        window.display(); // Display everything drawn to the window.
    }

private: // Private helper methods for GUI.
    /**
     * @brief Sets up all the UI elements, texts, buttons, etc., with their initial properties and positions.
     */
    void setupUI();

    /**
     * @brief Updates the content of the status text elements based on current game data.
     * @param player The current player object.
     * @param room A pointer to the current room.
     * @param message The message to display.
     */
    void updateStatus(const Player &player, const Room *room, const string &message);

    /**
     * @brief Draws the specific Game Over screen, including the game over message and detailed player stats.
     * @param message The main game over message (e.g., "Game Over! You ran out of moves.").
     * @param player The Player object whose stats are to be displayed.
     */
    void drawGameOver(const string &message, const Player &player);

    /**
     * @brief Helper function to center the origin of an sf::Text object.
     * This simplifies positioning text by its center point.
     * @param text The sf::Text object to center.
     */
    void centerOrigin(sf::Text &text)
    {
        sf::FloatRect bounds = text.getLocalBounds();
        text.setOrigin(bounds.left + bounds.width / 2.f, bounds.top + bounds.height / 2.f);
    }
};

/**
 * @brief Implementation of setupUI for the GUI class.
 * Initializes all UI elements' fonts, sizes, colors, and positions.
 */
void GUI::setupUI()
{
    if (!fontLoaded)
        return; // Do nothing if font wasn't loaded.

    // Main Game Title & Instructions text setup.
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

    // Status Panel background setup.
    statusPanel.setSize({window.getSize().x - 40.f, 200.f});
    statusPanel.setFillColor(panelColor);
    statusPanel.setPosition(20.f, 180.f);
    statusPanel.setOutlineThickness(1.f);
    statusPanel.setOutlineColor({80, 80, 95});

    // Setup for individual status text lines.
    for (int i = 0; i < 7; ++i)
    {
        statusText[i].setFont(font);
        statusText[i].setCharacterSize(20);
        statusText[i].setFillColor(textColor);
        statusText[i].setPosition(40.f, 200.f + i * 25); // Position each line vertically.
    }

    // Setup for main action buttons (Fight, Bypass, Backtrack, Quit).
    const sf::Vector2f buttonSize(180.f, 55.f);
    const string labels[] = {"Fight", "Bypass", "Backtrack", "Quit"};
    for (int i = 0; i < 4; ++i)
    {
        buttons[i].setSize(buttonSize);
        buttons[i].setPosition(20.f + i * (buttonSize.x + 20.f), 440.f); // Position buttons horizontally.
        buttons[i].setOutlineThickness(2.f);
        buttons[i].setOutlineColor({100, 100, 120});

        buttonLabels[i].setFont(font);
        buttonLabels[i].setString(labels[i]);
        buttonLabels[i].setCharacterSize(22);
        buttonLabels[i].setFillColor(textColor);
        centerOrigin(buttonLabels[i]);
        // Position label in the center of its corresponding button.
        buttonLabels[i].setPosition(buttons[i].getPosition().x + buttonSize.x / 2.f, buttons[i].getPosition().y + buttonSize.y / 2.f);
    }

    // Name Input Screen elements setup.
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
    nameInputField.setOrigin(200.f, 25.f); // Set origin to center for easier positioning.
    nameInputField.setPosition(window.getSize().x / 2.f, 280.f);

    nameInputText.setFont(font);
    nameInputText.setCharacterSize(28);
    nameInputText.setFillColor(sf::Color::Black);
    // Position text relative to the input field, slightly offset.
    nameInputText.setPosition(nameInputField.getPosition().x - 190, nameInputField.getPosition().y - 15);

    // Instructions Screen elements setup.
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
    rulesBodyText.setPosition(100.f, 150.f); // Rules text position.

    // Start Button for instructions screen.
    startButton.setSize({200.f, 60.f});
    startButton.setOutlineThickness(2.f);
    startButton.setOutlineColor({100, 100, 120});
    startButton.setOrigin(startButton.getSize().x / 2.f, startButton.getSize().y / 2.f); // Center origin.
    startButton.setPosition(window.getSize().x / 2.f, 480.f); // Position the button.

    startButtonLabel.setFont(font);
    startButtonLabel.setString("Start Game");
    startButtonLabel.setCharacterSize(28);
    startButtonLabel.setFillColor(textColor);
    centerOrigin(startButtonLabel);
    startButtonLabel.setPosition(startButton.getPosition()); // Position label in the center of the button.
}

/**
 * @brief Updates the status text elements displayed in the playing state.
 * @param player The player object to get stats from.
 * @param room A pointer to the current room to get room and enemy info.
 * @param message The message string to display (e.g., action results).
 */
void GUI::updateStatus(const Player &player, const Room *room, const string &message)
{
    // Set text for each status line using player and room data.
    statusText[0].setString("Room: " + (room ? room->getName() : "N/A")); // Display room name, or N/A if no room.
    statusText[1].setString("Health: " + to_string(player.getHealth()));
    statusText[2].setString("Moves Remaining: " + to_string(player.getMoves()));
    statusText[3].setString("Enemy: " + (room ? room->getEnemy().getName() : "N/A"));
    statusText[4].setString("Enemy Desc: " + (room ? room->getEnemy().getDescription() : "N/A"));
    statusText[5].setString("Coins: " + to_string(player.getCoins()));

    // Build inventory string.
    stringstream ss;
    ss << "Inventory: ";
    if (player.getInventory().empty())
        ss << "Empty";
    else
    {
        string invStr;
        for (const auto &item : player.getInventory())
            invStr += item + ", "; // Append each item with a comma and space.
        // Remove trailing ", " if it exists to avoid an extra comma.
        if (invStr.length() > 2)
            ss << invStr.substr(0, invStr.length() - 2);
        else
            ss << invStr; // Handles cases where invStr might be empty or just " ".
    }
    statusText[6].setString(ss.str()); // Set the formatted inventory string.

    // Change health text color based on player's health level.
    if (player.getHealth() > 50)
        statusText[1].setFillColor(healthGoodColor);
    else if (player.getHealth() > 20)
        statusText[1].setFillColor(healthWarningColor);
    else
        statusText[1].setFillColor(healthCriticalColor);

    statusMessage = message; // Store the current message to be drawn.
}

/**
 * @brief Draws the specific Game Over screen, including the game over message and detailed player stats.
 * @param message The main game over message.
 * @param player The Player object whose stats are to be displayed.
 */
void GUI::drawGameOver(const string &message, const Player &player)
{
    // Setup and draw the main "Game Over" message.
    sf::Text gameOverText(message, font, 40);
    gameOverText.setFillColor(titleColor);
    gameOverText.setStyle(sf::Text::Bold);
    centerOrigin(gameOverText);
    // Position adjusted to make space for player stats below.
    gameOverText.setPosition(window.getSize().x / 2.0f, window.getSize().y / 2.0f - 150);

    window.draw(gameOverText);

    // Display Player Stats individually.
    float currentY = window.getSize().y / 2.0f - 80; // Starting Y position for stats.
    float lineHeight = 25.0f;                       // Vertical spacing between stat lines.
    float startX = window.getSize().x / 2.0f - 150; // X position for stats (left-aligned).

    // Player Name.
    sf::Text playerNameText("Name: " + player.getName(), font, 20);
    playerNameText.setFillColor(textColor);
    playerNameText.setPosition(startX, currentY);
    window.draw(playerNameText);
    currentY += lineHeight;

    // Player Health.
    sf::Text playerHealthText("Health: " + to_string(player.getHealth()), font, 20);
    playerHealthText.setFillColor(textColor);
    playerHealthText.setPosition(startX, currentY);
    window.draw(playerHealthText);
    currentY += lineHeight;

    // Player Moves Left.
    sf::Text playerMovesText("Moves Left: " + to_string(player.getMoves()), font, 20);
    playerMovesText.setFillColor(textColor);
    playerMovesText.setPosition(startX, currentY);
    window.draw(playerMovesText);
    currentY += lineHeight;

    // Player Coins Collected.
    sf::Text playerCoinsText("Coins Collected: " + to_string(player.getCoins()), font, 20);
    playerCoinsText.setFillColor(textColor);
    playerCoinsText.setPosition(startX, currentY);
    window.draw(playerCoinsText);
    currentY += lineHeight;

    // Player Enemies Defeated.
    sf::Text playerEnemiesText("Enemies Defeated: " + to_string(player.getEnemiesDefeated()), font, 20);
    playerEnemiesText.setFillColor(textColor);
    playerEnemiesText.setPosition(startX, currentY);
    window.draw(playerEnemiesText);
    currentY += lineHeight;

    // Player Inventory (Sorted).
    stringstream inventorySs;
    inventorySs << "Inventory (Sorted): ";
    if (player.getInventory().empty())
        inventorySs << "Empty";
    else
    {
        string invStr;
        for (const auto &item : player.getInventory()) // Iterates through (assumed sorted) inventory.
            invStr += item + ", ";
        // Remove trailing ", " if it exists.
        if (invStr.length() > 2)
        {
            inventorySs << invStr.substr(0, invStr.length() - 2);
        }
        else
        {
            inventorySs << invStr; // Handles cases where invStr might be empty or just " ".
        }
    }
    sf::Text playerInventoryText(inventorySs.str(), font, 20);
    playerInventoryText.setFillColor(textColor);
    playerInventoryText.setPosition(startX, currentY);
    window.draw(playerInventoryText);
    currentY += lineHeight; // Update Y for the next element.


    // Adjust and draw the "Click or press any key to exit" prompt.
    sf::Text promptText("Click or press any key to exit.", font, 20);
    promptText.setFillColor(textColor);
    centerOrigin(promptText);
    promptText.setPosition(window.getSize().x / 2.0f, currentY + 50); // Position below stats.

    window.draw(promptText);
}

/**
 * @brief The main game loop that integrates game logic with the SFML GUI.
 * Manages game states, updates game elements, and orchestrates drawing.
 * @param player The Player object for the current game session.
 * @param dungeon The Dungeon object managing rooms and game rules.
 * @param gui The GUI object responsible for rendering and input.
 */
void gameLoopWithGUI(Player &player, Dungeon &dungeon, GUI &gui)
{
    GameState gameState = GameState::NAME_INPUT; // Start in the name input state.
    const Room *currentRoom = nullptr;           // Pointer to the current room.
    string message = "";                         // Message displayed in the game.
    string gameOverMessage = "";                 // Message displayed on game over screen.

    while (gui.isOpen()) // Loop as long as the GUI window is open.
    {
        // 1. EVENT HANDLING
        int choice = -1; // Reset choice for each loop iteration.
        sf::Event event;
        while (gui.pollEvent(event)) // Poll for all pending SFML events.
        {
            gui.handleEvent(event, gameState, choice); // Process the event.
        }

        // 2. GAME LOGIC UPDATES
        if (gameState == GameState::PLAYING)
        {
            // First time entering PLAYING state, initialize the first room.
            if (currentRoom == nullptr)
            {
                currentRoom = dungeon.advanceToNextRoom(); // Move to the first room.
                if (!currentRoom)
                {
                    gameOverMessage = "Error: No rooms available."; // Error if no rooms.
                    gameState = GameState::GAME_OVER;
                }
                else
                {
                    message = "You have entered the " + currentRoom->getName() + " room."; // Initial room message.
                }
            }

            // Check for a player action triggered by the event handler (choice > 0).
            if (choice > 0)
            {
                player.useMove(); // Decrement a move for any action.
                switch (choice)
                {
                case 1: // Fight action.
                {
                    const Enemy &enemy = currentRoom->getEnemy();
                    if (player.getHealth() >= enemy.getHealth()) // Player wins if health is higher.
                    {
                        player.takeDamage(enemy.getHealth()); // Player takes damage equal to enemy's health (cost of fighting).
                        player.addToInventory(currentRoom->getTreasure().getItem1());
                        player.addToInventory(currentRoom->getTreasure().getItem2());
                        player.addCoins(10);
                        player.incrementEnemiesDefeated();
                        message = "Victory! You defeated the " + enemy.getName() + ".";
                        currentRoom = dungeon.advanceToNextRoom(); // Move to next room.
                    }
                    else
                    {
                        player.takeDamage(10);           // Player takes damage for fleeing.
                        message = "Too weak! You fled and took damage.";
                    }
                }
                break;
                case 2: // Bypass action.
                    player.takeDamage(5); // Minor damage for bypassing.
                    message = "You bypassed the enemy, taking minor damage.";
                    currentRoom = dungeon.advanceToNextRoom(); // Move to next room.
                    break;
                case 3: // Backtrack action.
                {
                    const Room *previousRoom = dungeon.backtrack(); // Attempt to backtrack.
                    if (previousRoom)
                    {
                        currentRoom = previousRoom; // Update current room to previous.
                        message = "You backtracked to the " + currentRoom->getName() + " room.";
                    }
                    else
                    {
                        message = "No room to backtrack to!"; // Cannot backtrack message.
                    }
                }
                break;
                case 4: // Quit action.
                    gameOverMessage = "You have quit the dungeon.";
                    gameState = GameState::GAME_OVER; // Change to game over state.
                    break;
                }
                // Check for win/lose conditions after an action, if still in PLAYING state.
                if (gameState == GameState::PLAYING)
                {
                    if (!currentRoom) // No more rooms means player escaped.
                    {
                        gameOverMessage = "Congratulations! You escaped!";
                        gameState = GameState::GAME_OVER;
                    }
                    else if (player.getHealth() < 20) // Health too low.
                    {
                        gameOverMessage = "Game Over! Your health is critical.";
                        gameState = GameState::GAME_OVER;
                    }
                    else if (player.getMoves() <= 0) // No more moves.
                    {
                        gameOverMessage = "Game Over! You ran out of moves.";
                        gameState = GameState::GAME_OVER;
                    }
                }
            }
        }

        // Sort player inventory when game ends for consistent display.
        if (gameState == GameState::GAME_OVER)
        {
            player.sortInventory();
        }

        // 3. UPDATE & DRAW (GUI rendering phase)
        gui.update(gameState, player, currentRoom, message);             // Update GUI elements based on game state.
        gui.draw(gameState, dungeon.getRules(), gameOverMessage, player); // Draw everything to the window.
    }
}

/**
 * @brief Main function of the Dungeon Escape game.
 * Sets up the game and runs the main GUI game loop.
 */
int main()
{
    cout << "Welcome to Dungeon Escape (GUI Mode)!\n"; // Initial console message.

    GUI gui; // Create GUI object.
    if (!gui.isOpen())
    {
        cerr << "Failed to initialize GUI. Exiting.\n"; // Error if GUI window cannot be created.
        return 1;
    }

    // Loop to handle name input screen before starting the main game.
    while (gui.isOpen())
    {
        GameState tempState = GameState::NAME_INPUT; // Temporary state for event handling.
        int dummyChoice;                              // Dummy variable for choice, not used here.
        sf::Event event;
        while (gui.pollEvent(event))
        {
            gui.handleEvent(event, tempState, dummyChoice); // Handle events for name input.
            if (event.type == sf::Event::Closed)
                gui.close(); // Allow closing the window during name input.
        }

        if (tempState != GameState::NAME_INPUT)
            break; // Exit loop once name input is complete (state changes).

        // Pass a temporary player object for initial draw as actual player isn't created yet.
        // This is safe because GUI::update/draw for NAME_INPUT state doesn't use player data.
        gui.update(GameState::NAME_INPUT, Player(""), nullptr, "");
        gui.draw(GameState::NAME_INPUT, "", "", Player(""));
    }

    string playerName = gui.getPlayerName(); // Get the name entered by the player.
    if (playerName.empty())
    {
        playerName = "Adventurer"; // Default name if no name is entered.
    }

    Player player(playerName); // Create the Player object with the determined name.
    Dungeon dungeon;           // Create the Dungeon object.

    // Start the main game loop, passing the player, dungeon, and gui objects.
    gameLoopWithGUI(player, dungeon, gui);

    // After GUI closes, display final stats to console (optional, as GUI now shows them).
    dungeon.displayRanking(player);

    cout << "Thanks for playing Dungeon Escape!" << endl; // Final console message.
    return 0;
}

#include "Stronghold.h"
#include <limits>

// Global variables
Kingdom* kingdoms[MAX_KINGDOMS];
int kingdomCount = 0;
Map* gameMap;
Market* market;
DiplomacyManager* diplomacy;
CommunicationSystem* comms;

// Function prototypes
void initializeGame();
void gameLoop();
void saveGameState();
void loadGameState();
void displayKingdomMenu(Kingdom* kingdom);
void handleKingdomAction(Kingdom* kingdom);
void handleDiplomacyAction(Kingdom* kingdom);
void handleTradeAction(Kingdom* kingdom);
void handleWarAction(Kingdom* kingdom);
void handleMapAction(Kingdom* kingdom);
void simulateOtherKingdoms();
Kingdom* selectTargetKingdom(Kingdom* currentKingdom);
void clearScreen();
void waitForEnter();

int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    cout << "===============================" << endl;
    cout << "      STRONGHOLD GAME          " << endl;
    cout << "===============================" << endl;

    bool newGame = true;
    char choice;
    cout << "Do you want to load a saved game? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        loadGameState();
        newGame = false;
    }

    if (newGame) {
        initializeGame();
    }

    gameLoop();

    // Clean up
    for (int i = 0; i < kingdomCount; i++) {
        delete kingdoms[i];
    }
    delete gameMap;
    delete market;
    delete diplomacy;
    delete comms;

    return 0;
}

void initializeGame() {
    gameMap = new Map(MAP_SIZE, MAP_SIZE);
    market = new Market();
    diplomacy = new DiplomacyManager();
    comms = new CommunicationSystem();

    cout << "Enter a name for your kingdom: ";
    char kingdomName[MAX_NAME_LENGTH];
    cin.ignore();
    cin.getline(kingdomName, MAX_NAME_LENGTH);

    kingdoms[0] = new Kingdom(kingdomName);
    kingdomCount++;

    int x = rand() % MAP_SIZE;
    int y = rand() % MAP_SIZE;
    gameMap->placeKingdom(kingdoms[0], x, y);

    const char* aiNames[] = { "Northland", "Westeros", "Eastfall", "Southreach" };
    for (int i = 0; i < 4; i++) {
        kingdoms[kingdomCount] = new Kingdom(aiNames[i]);
        kingdoms[kingdomCount]->addGold(500 + rand() % 500);
        kingdoms[kingdomCount]->addFood(300 + rand() % 300);
        kingdoms[kingdomCount]->addWood(400 + rand() % 200);
        kingdoms[kingdomCount]->addStone(200 + rand() % 200);
        kingdoms[kingdomCount]->recruitSoldiers(50 + rand() % 50);

        bool validPosition = false;
        int kx, ky;
        while (!validPosition) {
            kx = rand() % MAP_SIZE;
            ky = rand() % MAP_SIZE;
            if (!gameMap->isOccupied(kx, ky)) {
                validPosition = true;
            }
        }
        gameMap->placeKingdom(kingdoms[kingdomCount], kx, ky);
        kingdomCount++;
    }

    cout << "Game initialized with " << kingdomCount << " kingdoms!" << endl;
    waitForEnter();
}

void gameLoop() {
    bool gameRunning = true;
    int turn = 1;

    while (gameRunning) {
        clearScreen();
        cout << "======= TURN " << turn << " =======" << endl;

        Kingdom* playerKingdom = kingdoms[0];
        displayKingdomMenu(playerKingdom);

        simulateOtherKingdoms();

        for (int i = 0; i < kingdomCount; i++) {
            kingdoms[i]->processTurn();
        }

        if (playerKingdom->getPopulation() <= 0) {
            cout << "Your kingdom has fallen! Game over!" << endl;
            gameRunning = false;
        }

        turn++;
        saveGameState();
    }
}

void displayKingdomMenu(Kingdom* kingdom) {
    bool backToMain = false;
    while (!backToMain) {
        clearScreen();
        cout << "====== " << kingdom->getName() << " ======" << endl;
        kingdom->displayStatus();

        cout << endl << "Options:" << endl;
        cout << "1. Internal Kingdom Management" << endl;
        cout << "2. Diplomacy and Treaties" << endl;
        cout << "3. Trade and Market" << endl;
        cout << "4. Military and Warfare" << endl;
        cout << "5. View Map" << endl;
        cout << "6. Messages and Communication" << endl;
        cout << "7. End Turn" << endl;
        cout << "8. Save and Exit" << endl;

        int choice;
        cout << "Enter your choice: ";
        cin >> choice;
        if (cin.fail()) {
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input." << endl;
            waitForEnter();
            continue;
        }

        switch (choice) {
        case 1: handleKingdomAction(kingdom); break;
        case 2: handleDiplomacyAction(kingdom); break;
        case 3: handleTradeAction(kingdom); break;
        case 4: handleWarAction(kingdom); break;
        case 5: handleMapAction(kingdom); break;
        case 6:
            comms->showMessages(kingdom->getName());
            comms->sendNewMessage(kingdom);
            waitForEnter();
            break;
        case 7: backToMain = true; break;
        case 8: saveGameState(); exit(0);
        default: cout << "Invalid option." << endl; waitForEnter();
        }
    }
}

void handleKingdomAction(Kingdom* kingdom) {
    clearScreen();
    cout << "===== Kingdom Management =====" << endl;
    cout << "1. Build Structure" << endl;
    cout << "2. Recruit Units" << endl;
    cout << "3. Collect Taxes" << endl;
    cout << "4. Research Technology" << endl;
    cout << "5. Manage Population" << endl;
    cout << "6. Back" << endl;

    int subchoice;
    cout << "Enter your choice: ";
    cin >> subchoice;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input." << endl;
        waitForEnter();
        return;
    }

    switch (subchoice) {
    case 1: kingdom->buildStructure(); break;
    case 2: kingdom->recruitUnits(); break;
    case 3: kingdom->collectTaxes(); break;
    case 4: kingdom->researchTechnology(); break;
    case 5: kingdom->managePopulation(); break;
    case 6: return;
    default: cout << "Invalid option." << endl;
    }
    waitForEnter();
}

void handleDiplomacyAction(Kingdom* kingdom) {
    clearScreen();
    cout << "===== Diplomacy and Treaties =====" << endl;
    cout << "1. View Current Treaties" << endl;
    cout << "2. Propose New Treaty" << endl;
    cout << "3. Break Existing Treaty" << endl;
    cout << "4. Check Relations" << endl;
    cout << "5. Back" << endl;

    int subchoice;
    cout << "Enter your choice: ";
    cin >> subchoice;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input." << endl;
        waitForEnter();
        return;
    }

    switch (subchoice) {
    case 1: diplomacy->viewTreaties(kingdom); break;
    case 2: {
        Kingdom* target = selectTargetKingdom(kingdom);
        if (target) diplomacy->proposeTreaty(kingdom, target);
        break;
    }
    case 3: diplomacy->breakTreaty(kingdom); break;
    case 4: diplomacy->checkRelations(kingdom); break;
    case 5: return;
    default: cout << "Invalid option." << endl;
    }
    waitForEnter();
}

void handleTradeAction(Kingdom* kingdom) {
    clearScreen();
    cout << "===== Trade and Market =====" << endl;
    cout << "1. View Market Prices" << endl;
    cout << "2. Buy Resources" << endl;
    cout << "3. Sell Resources" << endl;
    cout << "4. Propose Trade Deal" << endl;
    cout << "5. View Trade Offers" << endl;
    cout << "6. Smuggling Operations" << endl;
    cout << "7. Back" << endl;

    int subchoice;
    cout << "Enter your choice: ";
    cin >> subchoice;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input." << endl;
        waitForEnter();
        return;
    }

    switch (subchoice) {
    case 1: market->displayPrices(); break;
    case 2: market->buyResources(kingdom); break;
    case 3: market->sellResources(kingdom); break;
    case 4: {
        Kingdom* target = selectTargetKingdom(kingdom);
        if (target) market->proposeTrade(kingdom, target);
        break;
    }
    case 5: market->viewTradeOffers(kingdom); break;
    case 6: market->initiateSmuggling(kingdom); break;
    case 7: return;
    default: cout << "Invalid option." << endl;
    }
    waitForEnter();
}

void handleWarAction(Kingdom* kingdom) {
    clearScreen();
    cout << "===== Military and Warfare =====" << endl;
    cout << "1. View Military Status" << endl;
    cout << "2. Train Troops" << endl;
    cout << "3. Declare War" << endl;
    cout << "4. Launch Attack" << endl;
    cout << "5. Fortify Position" << endl;
    cout << "6. Spy on Kingdom" << endl;
    cout << "7. Back" << endl;

    int subchoice;
    cout << "Enter your choice: ";
    cin >> subchoice;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input." << endl;
        waitForEnter();
        return;
    }

    switch (subchoice) {
    case 1: kingdom->displayMilitary(); break;
    case 2: kingdom->trainTroops(); break;
    case 3: {
        Kingdom* target = selectTargetKingdom(kingdom);
        if (target) diplomacy->declareWar(kingdom, target);
        break;
    }
    case 4: {
        Kingdom* target = selectTargetKingdom(kingdom);
        if (target) gameMap->launchAttack(kingdom, target);
        break;
    }
    case 5: kingdom->fortify(); break;
    case 6: {
        Kingdom* target = selectTargetKingdom(kingdom);
        if (target) kingdom->spyOn(target);
        break;
    }
    case 7: return;
    default: cout << "Invalid option." << endl;
    }
    waitForEnter();
}

void handleMapAction(Kingdom* kingdom) {
    clearScreen();
    cout << "===== Map View =====" << endl;
    cout << "1. View World Map" << endl;
    cout << "2. View Territory Details" << endl;
    cout << "3. Move Kingdom" << endl;
    cout << "4. Expand Territory" << endl;
    cout << "5. Back" << endl;

    int subchoice;
    cout << "Enter your choice: ";
    cin >> subchoice;
    if (cin.fail()) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Invalid input." << endl;
        waitForEnter();
        return;
    }

    switch (subchoice) {
    case 1: gameMap->map_display(); break;
    case 2: gameMap->displayTerritory(kingdom); break;
    case 3: gameMap->moveKingdom(kingdom); break;
    case 4: gameMap->expandTerritory(kingdom); break;
    case 5: return;
    default: cout << "Invalid option." << endl;
    }
    waitForEnter();
}

void simulateOtherKingdoms() {
    for (int i = 1; i < kingdomCount; i++) {
        Kingdom* aiKingdom = kingdoms[i];
        int action = rand() % 5;
        switch (action) {
        case 0: aiKingdom->collectTaxes(); break;
        case 1: aiKingdom->buildStructure(); break;
        case 2: aiKingdom->recruitUnits(); break;
        case 3: aiKingdom->trainTroops(); break;
        case 4: aiKingdom->managePopulation(); break;
        }
        if (rand() % 10 < 3) {
            int targetIndex = rand() % kingdomCount;
            if (targetIndex != i) {
                diplomacy->proposeTreaty(aiKingdom, kingdoms[targetIndex]);
            }
        }
    }
    cout << endl << "AI kingdoms have taken their turns." << endl;
}

Kingdom* selectTargetKingdom(Kingdom* currentKingdom) {
    clearScreen();
    cout << "Select target kingdom:" << endl;
    int validCount = 0;
    for (int i = 0; i < kingdomCount; i++) {
        if (kingdoms[i] != currentKingdom) {
            cout << validCount + 1 << ". " << kingdoms[i]->getName() << endl;
            validCount++;
        }
    }
    cout << validCount + 1 << ". Cancel" << endl;
    int choice;
    cout << "Enter your choice: ";
    cin >> choice;
    if (cin.fail() || choice <= 0 || choice > validCount + 1) {
        cin.clear();
        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        return nullptr;
    }
    if (choice == validCount + 1) return nullptr;
    int index = 0, counter = 0;
    for (int i = 0; i < kingdomCount; i++) {
        if (kingdoms[i] != currentKingdom) {
            counter++;
            if (counter == choice) {
                index = i;
                break;
            }
        }
    }
    return kingdoms[index];
}

void saveGameState() {
    ofstream outFile("savegame.dat", ios::binary);
    if (!outFile) {
        cout << "Error saving game." << endl;
        return;
    }
    outFile.write((char*)&kingdomCount, sizeof(kingdomCount));
    for (int i = 0; i < kingdomCount; i++) {
        kingdoms[i]->saveToFile(outFile);
    }
    gameMap->saveToFile(outFile);
    diplomacy->saveToFile(outFile);
    market->saveToFile(outFile);
    comms->saveToFile(outFile);
    outFile.close();
    cout << "Game saved successfully!" << endl;
}

void loadGameState() {
    ifstream inFile("savegame.dat", ios::binary);
    if (!inFile) {
        cout << "No saved game found." << endl;
        initializeGame();
        return;
    }
    for (int i = 0; i < kingdomCount; i++) {
        delete kingdoms[i];
        kingdoms[i] = nullptr;
    }
    delete gameMap;
    delete market;
    delete diplomacy;
    delete comms;

    inFile.read((char*)&kingdomCount, sizeof(kingdomCount));
    for (int i = 0; i < kingdomCount; i++) {
        kingdoms[i] = new Kingdom();
        kingdoms[i]->loadFromFile(inFile);
    }
    gameMap = new Map();
    gameMap->loadFromFile(inFile);
    diplomacy = new DiplomacyManager();
    diplomacy->loadFromFile(inFile);
    market = new Market();
    market->loadFromFile(inFile);
    comms = new CommunicationSystem();
    comms->loadFromFile(inFile);
    inFile.close();
    cout << "Game loaded successfully!" << endl;
}

void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
}

void waitForEnter() {
    cout << endl << "Press Enter to continue...";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cin.get();
}
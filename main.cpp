#include "Stronghold.h"
#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
using namespace std;
void displayMenu() {
    cout << "\n STRONGHOLD KINGDOM SIMULATOR \n";
    cout << "1. Next Turn\n";
    cout << "2. Show Kingdom Status\n";
    cout << "3. Manage Resources\n";
    cout << "4. Manage Population\n";
    cout << "5. Manage Military\n";
    cout << "6. Manage Economy\n";
    cout << "7. Manage Government\n";
    cout << "8. Manage Bank\n";
    cout << "9. Save Game\n";
    cout << "10. Load Game\n";
    cout << "0. Quit\n";
    cout << "Enter your choice: ";
}

void manageResources(Kingdom& kingdom) {
    while (true) {
        cout << "\n=== RESOURCE MANAGEMENT ===\n";
        cout << "Resources:\n";
        for (int i = 0; i < kingdom.getResourceCount(); i++) {
            cout << i + 1 << ". " << kingdom.getResources()[i].getName()
                << ": " << kingdom.getResources()[i].getAmount()
                << " (+" << kingdom.getResources()[i].getProductionRate()
                << ", -" << kingdom.getResources()[i].getConsumptionRate() << ")\n";
        }

        cout << "\n1. Adjust Production Rate\n";
        cout << "2. Adjust Consumption Rate\n";
        cout << "3. Return to Main Menu\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 3) break;

        if (choice == 1 || choice == 2) {
            cout << "Select resource (1-" << kingdom.getResourceCount() << "): ";
            int resourceIndex;
            cin >> resourceIndex;

            if (resourceIndex < 1 || resourceIndex > kingdom.getResourceCount()) {
                cout << "Invalid resource selection.\n";
                continue;
            }

            resourceIndex--;

            if (choice == 1) {
                cout << "Enter new production rate: ";
                int newRate;
                cin >> newRate;

                try {
                    kingdom.getResources()[resourceIndex].setProductionRate(newRate);
                    cout << "Production rate updated.\n";
                }
                catch (StrongholdException& e) {
                    cout << "Error: " << e.getMessage() << endl;
                }
            }
            else {
                cout << "Enter new consumption rate: ";
                int newRate;
                cin >> newRate;

                try {
                    kingdom.getResources()[resourceIndex].setConsumptionRate(newRate);
                    cout << "Consumption rate updated.\n";
                }
                catch (StrongholdException& e) {
                    cout << "Error: " << e.getMessage() << endl;
                }
            }
        }
    }
}

void managePopulation(Kingdom& kingdom) {
    while (true) {
        cout << "\n POPULATION MANAGEMENT \n";
        cout << "Total Population: " << kingdom.getPopulation().getTotal() << "\n";
        cout << "Peasants: " << kingdom.getPopulation().getCountByClass(PEASANT) << "\n";
        cout << "Merchants: " << kingdom.getPopulation().getCountByClass(MERCHANT) << "\n";
        cout << "Nobles: " << kingdom.getPopulation().getCountByClass(NOBLE) << "\n";
        cout << "Clergy: " << kingdom.getPopulation().getCountByClass(CLERGY) << "\n";
        cout << "Military: " << kingdom.getPopulation().getCountByClass(MILITARY) << "\n";
        cout << "Happiness: " << kingdom.getPopulation().getHappinessLevel() << "%\n";
        cout << "Health: " << kingdom.getPopulation().getHealthLevel() << "%\n";

        cout << "\n1. Add Person\n";
        cout << "2. Return to Main Menu\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 2) break;

        if (choice == 1) {
            cout << "Select social class:\n";
            cout << "1. Peasant\n";
            cout << "2. Merchant\n";
            cout << "3. Noble\n";
            cout << "4. Clergy\n";
            cout << "Enter choice: ";

            int classChoice;
            cin >> classChoice;

            SocialClass socialClass;
            switch (classChoice) {
            case 1: socialClass = PEASANT; break;
            case 2: socialClass = MERCHANT; break;
            case 3: socialClass = NOBLE; break;
            case 4: socialClass = CLERGY; break;
            default:
                cout << "Invalid choice. Defaulting to Peasant.\n";
                socialClass = PEASANT;
            }

            try {
                kingdom.getPopulation().addPerson(socialClass);
                cout << "Person added.\n";
            }
            catch (StrongholdException& e) {
                cout << "Error: " << e.getMessage() << endl;
            }
        }
    }
}

void manageMilitary(Kingdom& kingdom) {
    while (true) {
        cout << "\n=== MILITARY MANAGEMENT ===\n";
        cout << "Soldiers: " << kingdom.getMilitary().getSoldiers() << "\n";
        cout << "Training: " << kingdom.getMilitary().getTraining() << "%\n";
        cout << "Morale: " << kingdom.getMilitary().getMorale() << "%\n";
        cout << "Paid Status: " << (kingdom.getMilitary().getIsPaid() ? "Paid" : "Unpaid") << "\n";
        cout << "Military Strength: " << kingdom.getMilitary().getStrength() << "\n";

        cout << "\n1. Recruit Soldiers\n";
        cout << "2. Pay Military\n";
        cout << "3. Feed Military\n";
        cout << "4. Battle Simulation\n";
        cout << "5. Return to Main Menu\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 5) break;

        switch (choice) {
        case 1: {
            cout << "How many soldiers to recruit? ";
            int count;
            cin >> count;

            try {
                kingdom.getMilitary().recruit(kingdom.getPopulation(), count);
                cout << "Recruitment complete.\n";
            }
            catch (StrongholdException& e) {
                cout << "Error: " << e.getMessage() << endl;
            }
            break;
        }
        case 2: {
            kingdom.getMilitary().pay(kingdom.getEconomy());
            cout << "Military payment processed.\n";
            break;
        }
        case 3: {
            Resource* food = kingdom.findResource("Food");
            if (food) {
                kingdom.getMilitary().feed(*food);
                cout << "Military fed.\n";
            }
            else {
                cout << "Error: Food resource not found.\n";
            }
            break;
        }
        case 4: {
            cout << "Enter enemy strength: ";
            int enemyStrength;
            cin >> enemyStrength;

            bool victory = kingdom.getMilitary().battle(enemyStrength);
            if (victory) {
                cout << "Victory! Your military prevailed.\n";
            }
            else {
                cout << "Defeat! Your military was defeated.\n";
            }
            break;
        }
        }
    }
}

void manageEconomy(Kingdom& kingdom) {
    while (true) {
        cout << "\n=== ECONOMY MANAGEMENT ===\n";
        cout << "Treasury: " << kingdom.getEconomy().getTreasury() << " gold\n";
        cout << "Tax Rate: " << (kingdom.getEconomy().getTaxRate() * 100) << "%\n";
        cout << "Inflation Rate: " << (kingdom.getEconomy().getInflationRate() * 100) << "%\n";

        cout << "\n1. Set Tax Rate\n";
        cout << "2. Collect Taxes\n";
        cout << "3. Make Expense\n";
        cout << "4. Return to Main Menu\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 4) break;

        switch (choice) {
        case 1: {
            cout << "Enter new tax rate (0-100%): ";
            float rate;
            cin >> rate;

            try {
                kingdom.getEconomy().setTaxRate(rate / 100.0f);
                cout << "Tax rate updated.\n";
            }
            catch (StrongholdException& e) {
                cout << "Error: " << e.getMessage() << endl;
            }
            break;
        }
        case 2: {
            kingdom.getEconomy().collectTaxes(kingdom.getPopulation());
            cout << "Taxes collected.\n";
            break;
        }
        case 3: {
            cout << "Enter expense amount: ";
            float amount;
            cin >> amount;

            try {
                bool success = kingdom.getEconomy().payExpense(amount);
                if (success) {
                    cout << "Expense paid.\n";
                }
                else {
                    cout << "Insufficient funds.\n";
                }
            }
            catch (StrongholdException& e) {
                cout << "Error: " << e.getMessage() << endl;
            }
            break;
        }
        }
    }
}

void manageGovernment(Kingdom& kingdom) {
    while (true) {
        cout << "\n GOVERNMENT MANAGEMENT \n";
        cout << "Leadership Type: ";
        switch (kingdom.getGovernment().getLeadershipType()) {
        case MONARCHY:  cout << "Monarchy"; break;
        case DEMOCRACY:  cout << "Democracy"; break;
        case DICTATORSHIP:  cout << "Dictatorship"; break;
        case OLIGARCHY:  cout << "Oligarchy"; break;
        }
        cout << endl;
        cout << "Leader: " << kingdom.getGovernment().getLeaderName() << "\n";
        cout << "Competence: " << kingdom.getGovernment().getLeaderCompetence() << "%\n";
        cout << "Corruption: " << kingdom.getGovernment().getLeaderCorruption() << "%\n";
        cout << "Term Remaining: " << kingdom.getGovernment().getTermYears() << " years\n";

        cout << "\n1. Change Leadership Type\n";
        cout << "2. Set Leader Name\n";
        cout << "3. Hold Election (Democracy Only)\n";
        cout << "4. Return to Main Menu\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 4) break;

        switch (choice) {
        case 1: {
            cout << "Select leadership type:\n";
            cout << "1. Monarchy\n";
            cout << "2. Democracy\n";
            cout << "3. Dictatorship\n";
            cout << "4. Oligarchy\n";
            cout << "Enter choice: ";

            int typeChoice;
            cin >> typeChoice;

            LeadershipType type;
            switch (typeChoice) {
            case 1: type = MONARCHY; break;
            case 2: type = DEMOCRACY; break;
            case 3: type = DICTATORSHIP; break;
            case 4: type = OLIGARCHY; break;
            default:
                cout << "Invalid choice. No changes made.\n";
                continue;
            }

            kingdom.getGovernment().setLeadershipType(type);
            cout << "Leadership type changed.\n";
            break;
        }
        case 2: {
            cout << "Enter leader name: ";
            char name[50];
            cin.ignore();
            cin.getline(name, 50);

            kingdom.getGovernment().setLeaderName(name);
            cout << "Leader name updated.\n";
            break;
        }
        case 3: {
            if (kingdom.getGovernment().getLeadershipType() == DEMOCRACY) {
                kingdom.getGovernment().holdElection(kingdom.getPopulation());
                cout << "Election held. New leader elected.\n";
            }
            else {
                cout << "Elections can only be held in a Democracy.\n";
            }
            break;
        }
        }
    }
}

void manageBank(Kingdom& kingdom) {
    while (true) {
        cout << "\n=== BANK MANAGEMENT ===\n";
        cout << "Reserves: " << kingdom.getBank().getReserves() << " gold\n";
        cout << "Interest Rate: " << (kingdom.getBank().getInterestRate() * 100) << "%\n";
        cout << "Corruption Status: " << (kingdom.getBank().getIsCorrupt() ? "Corrupt" : "Clean") << "\n";

        cout << "\n1. Take Loan\n";
        cout << "2. Audit Bank\n";
        cout << "3. Return to Main Menu\n";
        cout << "Enter your choice: ";

        int choice;
        cin >> choice;

        if (choice == 3) break;

        switch (choice) {
        case 1: {
            cout << "Enter loan amount: ";
            float amount;
            cin >> amount;

            try {
                bool success = kingdom.getBank().takeLoan(kingdom.getEconomy(), amount);
                if (success) {
                    cout << "Loan approved and deposited in treasury.\n";
                }
                else {
                    cout << "Loan denied. Insufficient bank reserves.\n";
                }
            }
            catch (StrongholdException& e) {
                cout << "Error: " << e.getMessage() << endl;
            }
            break;
        }
        case 2: {
            bool corruption = kingdom.getBank().audit(kingdom.getGovernment());
            if (corruption) {
                cout << "Audit complete. Corruption detected in government!\n";
            }
            else {
                cout << "Audit complete. No corruption detected.\n";
            }
            break;
        }
        }
    }
}

int main() {

    srand(time(NULL));

    cout << "STRONGHOLD KINGDOM SIMULATOR \n";
    cout << "Enter kingdom name: ";
    char kingdomName[50];
    cin.getline(kingdomName, 50);

    Kingdom kingdom(kingdomName);
    cout << "Kingdom of " << kingdomName << " established!\n";

    bool running = true;
    while (running) {

        if (kingdom.checkGameOver()) {
            cout << "\n** GAME OVER **\n";
            cout << "Your kingdom has fallen into ruin.\n";
            break;
        }

        Event* currentEvent = kingdom.getCurrentEvent();
        if (currentEvent->getIsActive()) {
            cout << "\n!!! " << currentEvent->getEventName() << " is affecting your kingdom !!!\n";
        }

        displayMenu();

        int choice;
        cin >> choice;

        switch (choice) {
        case 0:
            running = false;
            break;
        case 1:
            kingdom.nextTurn();
            cout << "Turn " << kingdom.getTurn() << " completed.\n";
            break;
        case 2:
            kingdom.displayStatus();
            break;
        case 3:
            manageResources(kingdom);
            break;
        case 4:
            managePopulation(kingdom);
            break;
        case 5:
            manageMilitary(kingdom);
            break;
        case 6:
            manageEconomy(kingdom);
            break;
        case 7:
            manageGovernment(kingdom);
            break;
        case 8:
            manageBank(kingdom);
            break;
        case 9: {
            cout << "Enter save filename: ";
            char filename[100];
            cin.ignore();
            cin.getline(filename, 100);

            try {
                kingdom.saveGame(filename);
                cout << "Game saved successfully.\n";
            }
            catch (StrongholdException& e) {
                cout << "Error saving game: " << e.getMessage() << endl;
            }
            break;
        }
        case 10: {
            cout << "Enter load filename: ";
            char filename[100];
            cin.ignore();
            cin.getline(filename, 100);

            bool success = kingdom.loadGame(filename);
            if (success) {
                cout << "Game loaded successfully.\n";
            }
            else {
                cout << "Error loading game.\n";
            }
            break;
        }
        default:
            cout << "Invalid choice. Please try again.\n";
        }
    }

    cout << "Thank you for playing Stronghold Kingdom Simulator!\n";
    return 0;
}
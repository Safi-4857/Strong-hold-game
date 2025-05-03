#define _CRT_SECURE_NO_WARNINGS
#include "Stronghold.h"
#include <iostream>
#include <cstring>
#include<string>
#include <cstdlib>
#include <ctime>

using namespace std;
StrongholdException::StrongholdException(const char* msg) {
    message = new char[strlen(msg) + 1];
    strcpy(message, msg);
}
Person* Population::getPopulation() {

    return nullptr;
}
StrongholdException::~StrongholdException() {
    delete[] message;
}

const char* StrongholdException::getMessage() const {
    return message;
}

Resource::Resource() {
    name[0] = '\0';
    amount = 0;
    productionRate = 0;
    consumptionRate = 0;
    price = 0.0f;
}

Resource::Resource(const char* name, int amount, int productionRate, int consumptionRate, float price) {
    strcpy(this->name, name);
    this->amount = amount;
    this->productionRate = productionRate;
    this->consumptionRate = consumptionRate;
    this->price = price;
}

Resource::~Resource() {

}

void Resource::setName(const char* name) {
    strcpy(this->name, name);
}

void Resource::setAmount(int amount) {
    if (amount < 0) {
        throw StrongholdException("Resource amount cannot be negative");
    }
    this->amount = amount;
}

void Resource::setProductionRate(int rate) {
    this->productionRate = rate;
}

void Resource::setConsumptionRate(int rate) {
    this->consumptionRate = rate;
}

void Resource::setPrice(float price) {
    if (price < 0) {
        throw StrongholdException("Resource price cannot be negative");
    }
    this->price = price;
}

const char* Resource::getName() const {
    return name;
}

int Resource::getAmount() const {
    return amount;
}

int Resource::getProductionRate() const {
    return productionRate;
}

int Resource::getConsumptionRate() const {
    return consumptionRate;
}

float Resource::getPrice() const {
    return price;
}

void Resource::produce() {
    amount += productionRate;
}

void Resource::consume() {
    if (amount >= consumptionRate) {
        amount -= consumptionRate;
    }
    else {
        amount = 0;
    }
}

bool Resource::isDepletingSoon() const {
    return amount <= consumptionRate * 3;
}

Person::Person() {
    socialClass = PEASANT;
    isAlive = true;
    happiness = 50;
    health = 100;
    age = 20;
}

Person::Person(SocialClass socialClass) {
    this->socialClass = socialClass;
    isAlive = true;
    happiness = 50;
    health = 100;
    age = 20;
}

Person::~Person() {

}

void Person::setSocialClass(SocialClass socialClass) {
    this->socialClass = socialClass;
}

void Person::setAlive(bool alive) {
    this->isAlive = alive;
}

void Person::setHappiness(int happiness) {
    if (happiness < 0) happiness = 0;
    if (happiness > 100) happiness = 100;
    this->happiness = happiness;
}

void Person::setHealth(int health) {
    if (health < 0) health = 0;
    if (health > 100) health = 100;
    this->health = health;
}

void Person::setAge(int age) {
    if (age < 0) {
        throw StrongholdException("Age cannot be negative");
    }
    this->age = age;
}

SocialClass Person::getSocialClass() const {
    return socialClass;
}

bool Person::getAlive() const {
    return isAlive;
}

int Person::getHappiness() const {
    return happiness;
}

int Person::getHealth() const {
    return health;
}

int Person::getAge() const {
    return age;
}

void Person::update(bool hasFood, bool hasShelter) {

    age++;


    if (!hasFood) {
        health -= 10;
        happiness -= 15;
    }

    if (!hasShelter) {
        health -= 5;
        happiness -= 10;
    }


    if (age > 60) {
        health -= 2;
    }

    if (health <= 0) {
        isAlive = false;
    }


    if (happiness < 0) happiness = 0;
    if (happiness > 100) happiness = 100;
    if (health < 0) health = 0;
    if (health > 100) health = 100;
}

const char* Person::getSocialClassName() const {
    switch (socialClass) {
    case PEASANT:
        return "Peasant";
    case MERCHANT:
        return "Merchant";
    case NOBLE:
        return "Noble";
    case CLERGY:
        return "Clergy";
    case MILITARY:
        return "Military";
    default:
        return "Unknown";
    }
}


Population::Population(int initialCapacity) {
    capacity = initialCapacity;
    count = 0;
    people = new Person[capacity];


    socialClassCounts = new int[5];
    for (int i = 0; i < 5; i++) {
        socialClassCounts[i] = 0;
    }
}

Population::~Population() {
    delete[] people;
    delete[] socialClassCounts;
}

void Population::addPerson(SocialClass socialClass) {
    if (count >= capacity) {

        Person* newPeople = new Person[capacity * 2];
        for (int i = 0; i < count; i++) {
            newPeople[i] = people[i];
        }
        delete[] people;
        people = newPeople;
        capacity *= 2;
    }

    people[count] = Person(socialClass);
    count++;
    socialClassCounts[socialClass]++;
}

void Population::removePerson(int index) {
    if (index < 0 || index >= count) {
        throw StrongholdException("Invalid person index");
    }

    socialClassCounts[people[index].getSocialClass()]--;

    for (int i = index; i < count - 1; i++) {
        people[i] = people[i + 1];
    }

    count--;
}

int Population::getTotal() const {
    return count;
}

int Population::getCountByClass(SocialClass socialClass) const {
    return socialClassCounts[socialClass];
}

float Population::getHappinessLevel() const {
    if (count == 0) return 0;

    float totalHappiness = 0;
    for (int i = 0; i < count; i++) {
        totalHappiness += people[i].getHappiness();
    }

    return totalHappiness / count;
}

float Population::getHealthLevel() const {
    if (count == 0) return 0;

    float totalHealth = 0;
    for (int i = 0; i < count; i++) {
        totalHealth += people[i].getHealth();
    }

    return totalHealth / count;
}

bool Population::isRevoltLikely() const {
    return getHappinessLevel() < 30;
}

void Population::update(bool hasFood, bool hasShelter) {
    for (int i = 0; i < count; i++) {
        people[i].update(hasFood, hasShelter);

        if (!people[i].getAlive()) {
            removePerson(i);
            i--;
        }
    }
}

void Population::naturalGrowth(float growthRate) {
    int newPeople = (int)(count * growthRate);

    for (int i = 0; i < newPeople; i++) {

        int random = rand() % 100;
        SocialClass newClass;

        if (random < 70) {
            newClass = PEASANT;
        }
        else if (random < 85) {
            newClass = MERCHANT;
        }
        else if (random < 95) {
            newClass = CLERGY;
        }
        else {
            newClass = NOBLE;
        }

        addPerson(newClass);
    }
}

void Population::handleFamine(float severityRate) {
    int deaths = (int)(count * severityRate);

    for (int i = 0; i < deaths; i++) {
        if (count > 0) {

            int index = rand() % count;
            if (people[index].getSocialClass() != PEASANT && rand() % 3 != 0) {

                continue;
            }
            removePerson(index);
        }
    }
}

void Population::handleDisease(float severityRate) {
    int deaths = (int)(count * severityRate);

    for (int i = 0; i < deaths; i++) {
        if (count > 0) {

            int index = rand() % count;
            removePerson(index);
        }
    }
}


Military::Military() {
    soldiers = 0;
    training = 50;
    morale = 50;
    isPaid = true;
    weapons = NULL;
    food = NULL;
}

Military::~Military() {

}

void Military::setSoldiers(int count) {
    if (count < 0) {
        throw StrongholdException("Soldier count cannot be negative");
    }
    soldiers = count;
}

void Military::setTraining(int level) {
    if (level < 0 || level > 100) {
        throw StrongholdException("Training level must be between 0 and 100");
    }
    training = level;
}

void Military::setMorale(int level) {
    if (level < 0 || level > 100) {
        throw StrongholdException("Morale level must be between 0 and 100");
    }
    morale = level;
}

void Military::setPaid(bool paid) {
    isPaid = paid;
}

int Military::getSoldiers() const {
    return soldiers;
}

int Military::getTraining() const {
    return training;
}

int Military::getMorale() const {
    return morale;
}

bool Military::getIsPaid() const {
    return isPaid;
}

void Military::recruit(Population& population, int count) {
    if (count <= 0) {
        throw StrongholdException("Recruitment count must be positive");
    }


    int peasants = population.getCountByClass(PEASANT);
    if (peasants < count) {
        count = peasants;
    }

    for (int i = 0; i < count; i++) {

        for (int j = 0; j < population.getTotal(); j++) {

            if (population.getPopulation()[j].getSocialClass() == PEASANT) {
                population.removePerson(j);
                break;
            }
        }
    }

    soldiers += count;
}

void Military::feed(Resource& food) {
    int needed = soldiers * 2;

    if (food.getAmount() >= needed) {
        food.setAmount(food.getAmount() - needed);

        morale += 5;
        if (morale > 100) morale = 100;
    }
    else {

        morale -= 10;
        if (morale < 0) morale = 0;
    }
}

void Military::pay(Economy& economy) {
    float cost = soldiers * 5.0f;

    if (economy.getTreasury() >= cost) {
        economy.setTreasury(economy.getTreasury() - cost);
        isPaid = true;
        morale += 10;
        if (morale > 100) morale = 100;
    }
    else {
        isPaid = false;
        morale -= 20;
        if (morale < 0) morale = 0;
    }
}

bool Military::battle(int enemyStrength) {
    int ourStrength = getStrength();


    morale -= 10;
    if (morale < 0) morale = 0;


    float casualtyRate = 0.1f;
    if (ourStrength < enemyStrength) {
        casualtyRate = 0.2f;
    }

    int casualties = (int)(soldiers * casualtyRate);
    soldiers -= casualties;
    if (soldiers < 0) soldiers = 0;

    return ourStrength > enemyStrength;
}

int Military::getStrength() const {
    return soldiers * (training + morale) / 100;
}

void Military::update() {

    training += 1;
    if (training > 100) training = 100;


    if (!isPaid) {
        morale -= 5;
        if (morale < 0) morale = 0;
    }
}


Economy::Economy() {
    treasury = 1000.0f;
    taxRate = 0.1f;
    inflationRate = 0.02f;
}

Economy::~Economy() {

}

void Economy::setTreasury(float amount) {
    if (amount < 0) {
        throw StrongholdException("Treasury cannot be negative");
    }
    treasury = amount;
}

void Economy::setTaxRate(float rate) {
    if (rate < 0 || rate > 1) {
        throw StrongholdException("Tax rate must be between 0 and 1");
    }
    taxRate = rate;
}

void Economy::setInflationRate(float rate) {
    if (rate < 0) {
        throw StrongholdException("Inflation rate cannot be negative");
    }
    inflationRate = rate;
}

float Economy::getTreasury() const {
    return treasury;
}

float Economy::getTaxRate() const {
    return taxRate;
}

float Economy::getInflationRate() const {
    return inflationRate;
}

void Economy::collectTaxes(Population& population) {
    float income = 0;

    income += population.getCountByClass(PEASANT) * 1.0f * taxRate;
    income += population.getCountByClass(MERCHANT) * 5.0f * taxRate;
    income += population.getCountByClass(NOBLE) * 20.0f * taxRate;
    income += population.getCountByClass(CLERGY) * 3.0f * taxRate;

    treasury += income;
}

bool Economy::payExpense(float amount) {
    if (amount <= 0) {
        throw StrongholdException("Expense amount must be positive");
    }

    if (treasury >= amount) {
        treasury -= amount;
        return true;
    }

    return false;
}

void Economy::adjustInflation(float marketstable) {

    if (marketstable < 0.5f) {
        inflationRate += 0.01f;
    }
    else {
        inflationRate -= 0.005f;
    }

    if (inflationRate < 0.01f) inflationRate = 0.01f;
    if (inflationRate > 0.2f) inflationRate = 0.2f;
}

void Economy::update() {

    treasury = treasury * (1.0f - inflationRate);
}

Government::Government() {
    leadershipType = MONARCHY;
    strcpy(leaderName, "Shiekh Safi");
    leaderCompetence = 50;
    leaderCorruption = 10;
    termYears = 10;
}

Government::~Government() {

}

void Government::setLeadershipType(LeadershipType type) {
    leadershipType = type;
}

void Government::setLeaderName(const char* name) {
    strcpy(leaderName, name);
}

void Government::setLeaderCompetence(int level) {
    if (level < 0 || level > 100) {
        throw StrongholdException("Leader competence must be between 0 and 100");
    }
    leaderCompetence = level;
}

void Government::setLeaderCorruption(int level) {
    if (level < 0 || level > 100) {
        throw StrongholdException("Leader corruption must be between 0 and 100");
    }
    leaderCorruption = level;
}

void Government::setTermYears(int years) {
    if (years < 0) {
        throw StrongholdException("Term years cannot be negative");
    }
    termYears = years;
}

LeadershipType Government::getLeadershipType() const {
    return leadershipType;
}

const char* Government::getLeaderName() const {
    return leaderName;
}

int Government::getLeaderCompetence() const {
    return leaderCompetence;
}

int Government::getLeaderCorruption() const {
    return leaderCorruption;
}

int Government::getTermYears() const {
    return termYears;
}

void Government::holdElection(Population& population) {

    if (leadershipType != DEMOCRACY) {
        return;
    }

    leaderCompetence = rand() % 101;
    leaderCorruption = rand() % 101;

    char newName[50];
    sprintf(newName, "Leader %d", rand() % 1000);
    strcpy(leaderName, newName);

    termYears = 4;
}

bool Government::attemptCoup(Military& military, Population& population) {

    float coupChance = 0.0f;

    if (military.getMorale() < 30) {
        coupChance += 0.3f;
    }

    if (population.getHappinessLevel() < 30) {
        coupChance += 0.3f;
    }

    float roll = (float)rand() / RAND_MAX;

    if (roll < coupChance) {

        leadershipType = DICTATORSHIP;
        leaderCompetence = rand() % 101;
        leaderCorruption = 30 + rand() % 71;

        char newName[50];
        sprintf(newName, "Dictator %d", rand() % 1000);
        strcpy(leaderName, newName);

        termYears = 10 + rand() % 10;

        return true;
    }

    return false;
}

float Government::getDecisionQuality() const {

    return (float)leaderCompetence / 100.0f * (1.0f - (float)leaderCorruption / 100.0f);
}

void Government::update() {

    if (termYears > 0) {
        termYears--;
    }

    leaderCorruption += 1;
    if (leaderCorruption > 100) leaderCorruption = 100;
}

Bank::Bank() {
    reserves = 5000.0f;
    interestRate = 0.05f;
    loanCapacity = 10;
    loanCount = 0;
    loans = new float[loanCapacity];
    isCorrupt = false;
}

Bank::~Bank() {
    delete[] loans;
}

void Bank::setReserves(float amount) {
    if (amount < 0) {
        throw StrongholdException("Bank reserves cannot be negative");
    }
    reserves = amount;
}

void Bank::setInterestRate(float rate) {
    if (rate < 0 || rate > 1) {
        throw StrongholdException("Interest rate must be between 0 and 1");
    }
    interestRate = rate;
}

void Bank::setCorrupt(bool corrupt) {
    isCorrupt = corrupt;
}

float Bank::getReserves() const {
    return reserves;
}

float Bank::getInterestRate() const {
    return interestRate;
}

bool Bank::getIsCorrupt() const {
    return isCorrupt;
}

bool Bank::takeLoan(Economy& economy, float amount) {
    if (amount <= 0) {
        throw StrongholdException("Loan amount must be positive");
    }

    if (reserves < amount) {
        return false;
    }

    if (loanCount >= loanCapacity) {

        float* newLoans = new float[loanCapacity * 2];
        for (int i = 0; i < loanCount; i++) {
            newLoans[i] = loans[i];
        }
        delete[] loans;
        loans = newLoans;
        loanCapacity *= 2;
    }

    loans[loanCount++] = amount * (1.0f + interestRate);

    reserves -= amount;
    economy.setTreasury(economy.getTreasury() + amount);

    return true;
}

void Bank::repayLoan(Economy& economy, int loanIndex) {
    if (loanIndex < 0 || loanIndex >= loanCount) {
        throw StrongholdException("Invalid loan index");
    }

    float amount = loans[loanIndex];

    if (economy.getTreasury() >= amount) {
        economy.setTreasury(economy.getTreasury() - amount);
        reserves += amount;

        for (int i = loanIndex; i < loanCount - 1; i++) {
            loans[i] = loans[i + 1];
        }
        loanCount--;
    }
}

bool Bank::audit(Government& government) {

    float corruptionChance = (float)government.getLeaderCorruption() / 100.0f;

    if (isCorrupt) {
        corruptionChance *= 0.5f;
    }

    float roll = (float)rand() / RAND_MAX;

    return roll < corruptionChance;
}

void Bank::update() {

    reserves *= 1.01f;

    if (reserves < 1000.0f) {
        interestRate += 0.01f;
    }
    else {
        interestRate -= 0.005f;
    }

    if (interestRate < 0.02f) interestRate = 0.02f;
    if (interestRate > 0.2f) interestRate = 0.2f;
}

Event::Event() {
    type = FAMINE;
    severity = 50;
    duration = 3;
    currentDuration = 0;
    isActive = false;
}

Event::~Event() {
}

void Event::setType(EventType type) {
    this->type = type;
}

void Event::setSeverity(int level) {
    if (level < 0 || level > 100) {
        throw StrongholdException("Event severity must be between 0 and 100");
    }
    severity = level;
}

void Event::setDuration(int turns) {
    if (turns < 0) {
        throw StrongholdException("Event duration cannot be negative");
    }
    duration = turns;
}

void Event::setActive(bool active) {
    isActive = active;
}

EventType Event::getType() const {
    return type;
}

int Event::getSeverity() const {
    return severity;
}

int Event::getDuration() const {
    return duration;
}

int Event::getCurrentDuration() const {
    return currentDuration;
}

bool Event::getIsActive() const {
    return isActive;
}

void Event::trigger() {
    isActive = true;
    currentDuration = 0;
}

void Event::resolve() {
    isActive = false;
    currentDuration = 0;
}

void Event::update() {
    if (isActive) {
        currentDuration++;

        if (currentDuration >= duration) {
            resolve();
        }
    }
}

void Event::applyEffects(Kingdom& kingdom) {
    if (!isActive) return;

    float severityFactor = (float)severity / 100.0f;

    switch (type) {
    case FAMINE: {

        kingdom.getPopulation().handleFamine(severityFactor * 0.05f);

        Resource* food = kingdom.findResource("Food");
        if (food != NULL) {
            food->setProductionRate(food->getProductionRate() / 2);
        }
        break;
    }
    case DISEASE:

        kingdom.getPopulation().handleDisease(severityFactor * 0.1f);
        break;

    case WAR:

        kingdom.getMilitary().battle(100 * severity);
        kingdom.getEconomy().payExpense(1000 * severityFactor);
        break;

    case NATURAL_DISASTER:
        kingdom.getPopulation().handleFamine(severityFactor * 0.03f);


        for (int i = 0; i < kingdom.getResourceCount(); i++) {
            Resource& res = kingdom.getResources()[i];
            res.setProductionRate((int)(res.getProductionRate() * 0.7f));
        }
        break;
    }
}

const char* Event::getEventName() const {
    switch (type) {
    case FAMINE: return "Famine";
    case DISEASE: return "Plague";
    case WAR: return "War";
    case NATURAL_DISASTER: return "Natural Disaster";
    default: return "Unknown Event";
    }
}

Kingdom::Kingdom(const char* name) {
    strcpy(this->name, name);

    population = new Population(100);
    military = new Military();
    economy = new Economy();
    government = new Government();
    bank = new Bank();

    resourceCount = 0;
    resources = new Resource[10];

    addResource("Food", 1000, 200, 100, 1.0f);
    addResource("Wood", 500, 100, 50, 2.0f);
    addResource("Stone", 300, 50, 20, 5.0f);
    addResource("Iron", 100, 20, 10, 10.0f);
    addResource("Gold", 50, 10, 5, 50.0f);

    for (int i = 0; i < 50; i++) {
        population->addPerson(PEASANT);
    }
    for (int i = 0; i < 10; i++) {
        population->addPerson(MERCHANT);
    }
    for (int i = 0; i < 5; i++) {
        population->addPerson(NOBLE);
    }
    for (int i = 0; i < 5; i++) {
        population->addPerson(CLERGY);
    }

    military->setSoldiers(10);
    military->setTraining(50);
    military->setMorale(80);

    currentEvent = new Event();

    turn = 1;

    treasuryLogger = new Logger<float>("treasury_log.txt");
}

Kingdom::~Kingdom() {
    delete population;
    delete military;
    delete economy;
    delete government;
    delete bank;
    delete[] resources;
    delete currentEvent;
    delete treasuryLogger;
}

Population& Kingdom::getPopulation() {
    return *population;
}

Military& Kingdom::getMilitary() {
    return *military;
}

Economy& Kingdom::getEconomy() {
    return *economy;
}

Government& Kingdom::getGovernment() {
    return *government;
}

Bank& Kingdom::getBank() {
    return *bank;
}

Resource* Kingdom::getResources() {
    return resources;
}

int Kingdom::getResourceCount() const {
    return resourceCount;
}

Event* Kingdom::getCurrentEvent() {
    return currentEvent;
}

int Kingdom::getTurn() const {
    return turn;
}

void Kingdom::addResource(const char* name, int amount, int productionRate, int consumptionRate, float price) {
    if (resourceCount >= 10) {
        throw StrongholdException("Maximum resource limit reached");
    }

    resources[resourceCount++] = Resource(name, amount, productionRate, consumptionRate, price);
}

Resource* Kingdom::findResource(const char* name) {
    for (int i = 0; i < resourceCount; i++) {
        if (strcmp(resources[i].getName(), name) == 0) {
            return &resources[i];
        }
    }

    return NULL;
}

void Kingdom::nextTurn() {
    // Log current treasury state
    treasuryLogger->log(economy->getTreasury());

    // Update resources
    for (int i = 0; i < resourceCount; i++) {
        resources[i].produce();
        resources[i].consume();
    }

    // Update population
    Resource* food = findResource("Food");
    bool hasFood = food != NULL && food->getAmount() > population->getTotal() * 2;

    Resource* wood = findResource("Wood");
    bool hasShelter = wood != NULL && wood->getAmount() > population->getTotal();

    population->update(hasFood, hasShelter);
    population->naturalGrowth(0.05f);

    // Update military
    military->update();

    // Update economy
    economy->update();
    economy->collectTaxes(*population);

    // Update government
    government->update();

    // Check for leadership changes
    if (government->getLeadershipType() == DEMOCRACY && government->getTermYears() <= 0) {
        government->holdElection(*population);
    }

    if (military->getMorale() < 30 && population->getHappinessLevel() < 30) {
        government->attemptCoup(*military, *population);
    }

    bank->update();

    if (currentEvent->getIsActive()) {
        currentEvent->applyEffects(*this);
        currentEvent->update();
    }
    else if (rand() % 10 == 0) {
        generateRandomEvent();
    }

    turn++;
}

void Kingdom::generateRandomEvent() {

    EventType type = static_cast<EventType>(rand() % 4);
    int severity = 30 + rand() % 71;
    int duration = 1 + rand() % 5;

    currentEvent->setType(type);
    currentEvent->setSeverity(severity);
    currentEvent->setDuration(duration);
    currentEvent->trigger();
}

bool Kingdom::checkGameOver() const {

    if (population->getTotal() < 10) {
        return true;
    }

    if (economy->getTreasury() <= 0 && military->getSoldiers() > 0) {
        return true;
    }

    return false;
}

void Kingdom::saveGame(const char* filename) {
    ofstream file(filename);

    if (!file.is_open()) {
        throw StrongholdException("Could not open file for saving");
    }

    file << name << endl;
    file << turn << endl;


    file << population->getTotal() << endl;
    file << population->getCountByClass(PEASANT) << " ";
    file << population->getCountByClass(MERCHANT) << " ";
    file << population->getCountByClass(NOBLE) << " ";
    file << population->getCountByClass(CLERGY) << " ";
    file << population->getCountByClass(MILITARY) << endl;

    file << military->getSoldiers() << " ";
    file << military->getTraining() << " ";
    file << military->getMorale() << " ";
    file << military->getIsPaid() << endl;

    file << economy->getTreasury() << " ";
    file << economy->getTaxRate() << " ";
    file << economy->getInflationRate() << endl;


    file << static_cast<int>(government->getLeadershipType()) << " ";
    file << government->getLeaderName() << " ";
    file << government->getLeaderCompetence() << " ";
    file << government->getLeaderCorruption() << " ";
    file << government->getTermYears() << endl;

    file << bank->getReserves() << " ";
    file << bank->getInterestRate() << " ";
    file << bank->getIsCorrupt() << endl;

    file << resourceCount << endl;
    for (int i = 0; i < resourceCount; i++) {
        file << resources[i].getName() << " ";
        file << resources[i].getAmount() << " ";
        file << resources[i].getProductionRate() << " ";
        file << resources[i].getConsumptionRate() << " ";
        file << resources[i].getPrice() << endl;
    }

    file << currentEvent->getIsActive() << " ";
    file << static_cast<int>(currentEvent->getType()) << " ";
    file << currentEvent->getSeverity() << " ";
    file << currentEvent->getDuration() << " ";
    file << currentEvent->getCurrentDuration() << endl;

    file.close();
}

bool Kingdom::loadGame(const char* filename) {
    ifstream file(filename);

    if (!file.is_open()) {
        return false;
    }

    try {

        file >> name;
        file >> turn;

        int total, peasants, merchants, nobles, clergy, militaryCount;
        file >> total;
        file >> peasants >> merchants >> nobles >> clergy >> militaryCount;

        delete population;
        population = new Population(total);

        for (int i = 0; i < peasants; i++) {
            population->addPerson(PEASANT);
        }
        for (int i = 0; i < merchants; i++) {
            population->addPerson(MERCHANT);
        }
        for (int i = 0; i < nobles; i++) {
            population->addPerson(NOBLE);
        }
        for (int i = 0; i < clergy; i++) {
            population->addPerson(CLERGY);
        }
        for (int i = 0; i < militaryCount; i++) {
            population->addPerson(MILITARY);
        }

        int soldiers, training, morale;
        bool isPaid;
        file >> soldiers >> training >> morale >> isPaid;
        military->setSoldiers(soldiers);
        military->setTraining(training);
        military->setMorale(morale);
        military->setPaid(isPaid);

        float treasury, taxRate, inflationRate;
        file >> treasury >> taxRate >> inflationRate;
        economy->setTreasury(treasury);
        economy->setTaxRate(taxRate);
        economy->setInflationRate(inflationRate);

        int leadershipTypeInt;
        char leaderName[50];
        int competence, corruption, termYears;
        file >> leadershipTypeInt >> leaderName >> competence >> corruption >> termYears;
        government->setLeadershipType(static_cast<LeadershipType>(leadershipTypeInt));
        government->setLeaderName(leaderName);
        government->setLeaderCompetence(competence);
        government->setLeaderCorruption(corruption);
        government->setTermYears(termYears);

        float reserves, interestRate;
        bool isCorrupt;
        file >> reserves >> interestRate >> isCorrupt;
        bank->setReserves(reserves);
        bank->setInterestRate(interestRate);
        bank->setCorrupt(isCorrupt);

        int count;
        file >> count;

        delete[] resources;
        resources = new Resource[10];
        resourceCount = 0;

        for (int i = 0; i < count; i++) {
            char name[20];
            int amount, prodRate, consRate;
            float price;

            file >> name >> amount >> prodRate >> consRate >> price;
            addResource(name, amount, prodRate, consRate, price);
        }

        bool isActive;
        int eventType, severity, duration, currentDuration;
        file >> isActive >> eventType >> severity >> duration >> currentDuration;

        currentEvent->setActive(isActive);
        currentEvent->setType(static_cast<EventType>(eventType));
        currentEvent->setSeverity(severity);
        currentEvent->setDuration(duration);

        file.close();
        return true;
    }
    catch (...) {
        file.close();
        return false;
    }
}

void Kingdom::displayStatus() const {
    cout << "=== Kingdom of " << name << " (Turn " << turn << ") ===" << endl;
    cout << endl;

    cout << "Population: " << population->getTotal() << " people" << endl;
    cout << "  Peasants: " << population->getCountByClass(PEASANT) << endl;
    cout << "  Merchants: " << population->getCountByClass(MERCHANT) << endl;
    cout << "  Nobles: " << population->getCountByClass(NOBLE) << endl;
    cout << "  Clergy: " << population->getCountByClass(CLERGY) << endl;
    cout << "Happiness: " << population->getHappinessLevel() << "%" << endl;
    cout << "Health: " << population->getHealthLevel() << "%" << endl;
    if (population->isRevoltLikely()) {
        cout << "WARNING: Revolt is likely!" << endl;
    }
    cout << endl;

    cout << "Military: " << military->getSoldiers() << " soldiers" << endl;
    cout << "  Training: " << military->getTraining() << "%" << endl;
    cout << "  Morale: " << military->getMorale() << "%" << endl;
    cout << "  Status: " << (military->getIsPaid() ? "Paid" : "Unpaid") << endl;
    cout << "  Strength: " << military->getStrength() << endl;
    cout << endl;

    cout << "Economy:" << endl;
    cout << "  Treasury: " << economy->getTreasury() << " gold" << endl;
    cout << "  Tax Rate: " << (economy->getTaxRate() * 100) << "%" << endl;
    cout << "  Inflation: " << (economy->getInflationRate() * 100) << "%" << endl;
    cout << endl;

    cout << "Government:" << endl;
    cout << "  Type: ";
    switch (government->getLeadershipType()) {
    case MONARCHY:  cout << "Monarchy"; break;
    case DEMOCRACY:  cout << "Democracy"; break;
    case DICTATORSHIP:  cout << "Dictatorship"; break;
    case OLIGARCHY:  cout << "Oligarchy"; break;
    }
    cout << endl;
    cout << "  Leader: " << government->getLeaderName() << endl;
    cout << "  Competence: " << government->getLeaderCompetence() << "%" << endl;
    cout << "  Corruption: " << government->getLeaderCorruption() << "%" << endl;
    cout << "  Term Remaining: " << government->getTermYears() << " years" << endl;
    cout << endl;

    cout << "Bank:" << endl;
    cout << "  Reserves: " << bank->getReserves() << " gold" << endl;
    cout << "  Interest Rate: " << (bank->getInterestRate() * 100) << "%" << endl;
    cout << endl;


    cout << "Resources:" << endl;
    for (int i = 0; i < resourceCount; i++) {
        cout << "  " << resources[i].getName() << ": " << resources[i].getAmount();
        cout << " (+" << resources[i].getProductionRate() << ", -" << resources[i].getConsumptionRate() << ")";
        if (resources[i].isDepletingSoon()) {
            cout << " LOW";
        }
        cout << endl;
    }
    cout << endl;

    if (currentEvent->getIsActive()) {
        cout << "Active Event = " << currentEvent->getEventName() << endl;
        cout << "  Severity = " << currentEvent->getSeverity() << "%" << endl;
        cout << "  Duration = " << currentEvent->getCurrentDuration() << "/" << currentEvent->getDuration() << " turns" << endl;
        cout << endl;
    }
}
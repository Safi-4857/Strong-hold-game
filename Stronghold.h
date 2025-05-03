#ifndef STRONGHOLD_H
#define STRONGHOLD_H
#define _CRT_SECURE_NO_WARNINGS

#include <fstream>
#include <ctime>
#include <cstdlib>
#include<string>

class Resource;
class Population;
class Economy;
class Military;
class Government;
class Bank;
class Kingdom;
class Event;

class StrongholdException {
private:
    char* message;
public:
    StrongholdException(const char* msg);
    ~StrongholdException();
    const char* getMessage() const;
};

class Resource {
private:
    char name[20];
    int amount;
    int productionRate;
    int consumptionRate;
    float price;
public:
    Resource();
    Resource(const char* name, int amount, int productionRate, int consumptionRate, float price);
    ~Resource();

    void setName(const char* name);
    void setAmount(int amount);
    void setProductionRate(int rate);
    void setConsumptionRate(int rate);
    void setPrice(float price);

    const char* getName() const;
    int getAmount() const;
    int getProductionRate() const;
    int getConsumptionRate() const;
    float getPrice() const;

    void produce();
    void consume();
    bool isDepletingSoon() const;
};

enum SocialClass { PEASANT, MERCHANT, NOBLE, CLERGY, MILITARY };

class Person {
private:
    SocialClass socialClass;
    bool isAlive;
    int happiness;
    int health;
    int age;
public:
    Person();
    Person(SocialClass socialClass);
    ~Person();

    void setSocialClass(SocialClass socialClass);
    void setAlive(bool alive);
    void setHappiness(int happiness);
    void setHealth(int health);
    void setAge(int age);

    SocialClass getSocialClass() const;
    bool getAlive() const;
    int getHappiness() const;
    int getHealth() const;
    int getAge() const;

    void update(bool hasFood, bool hasShelter);
    const char* getSocialClassName() const;
};

class Population {
private:
    Person* people;
    int capacity;
    int count;
    int* socialClassCounts;
public:
    Population(int initialCapacity = 100);
    ~Population();

    void addPerson(SocialClass socialClass);
    void removePerson(int index);
    int getTotal() const;
    int getCountByClass(SocialClass socialClass) const;
    float getHappinessLevel() const;
    float getHealthLevel() const;
    bool isRevoltLikely() const;
    void update(bool hasFood, bool hasShelter);

    void naturalGrowth(float growthRate);
    void handleFamine(float severityRate);
    void handleDisease(float severityRate);
    Person* getPopulation();
};

class Military {
private:
    int soldiers;
    int training;
    int morale;
    bool isPaid;
    Resource* weapons;
    Resource* food;
public:
    Military();
    ~Military();

    void setSoldiers(int count);
    void setTraining(int level);
    void setMorale(int level);
    void setPaid(bool paid);

    int getSoldiers() const;
    int getTraining() const;
    int getMorale() const;
    bool getIsPaid() const;

    void recruit(Population& population, int count);
    void feed(Resource& food);
    void pay(Economy& economy);
    bool battle(int enemyStrength);
    int getStrength() const;
    void update();
};
class Economy {
private:
    float treasury;
    float taxRate;
    float inflationRate;
public:
    Economy();
    ~Economy();

    void setTreasury(float amount);
    void setTaxRate(float rate);
    void setInflationRate(float rate);

    float getTreasury() const;
    float getTaxRate() const;
    float getInflationRate() const;

    void collectTaxes(Population& population);
    bool payExpense(float amount);
    void adjustInflation(float marketStability);
    void update();
};

enum LeadershipType { MONARCHY, DEMOCRACY, DICTATORSHIP, OLIGARCHY };

class Government {
private:
    LeadershipType leadershipType;
    char leaderName[50];
    int leaderCompetence;
    int leaderCorruption;
    int termYears;
public:
    Government();
    ~Government();

    void setLeadershipType(LeadershipType type);
    void setLeaderName(const char* name);
    void setLeaderCompetence(int level);
    void setLeaderCorruption(int level);
    void setTermYears(int years);

    LeadershipType getLeadershipType() const;
    const char* getLeaderName() const;
    int getLeaderCompetence() const;
    int getLeaderCorruption() const;
    int getTermYears() const;

    void holdElection(Population& population);
    bool attemptCoup(Military& military, Population& population);
    float getDecisionQuality() const;
    void update();
};

class Bank {
private:
    float reserves;
    float interestRate;
    float* loans;
    int loanCount;
    int loanCapacity;
    bool isCorrupt;
public:
    Bank();
    ~Bank();

    void setReserves(float amount);
    void setInterestRate(float rate);
    void setCorrupt(bool corrupt);

    float getReserves() const;
    float getInterestRate() const;
    bool getIsCorrupt() const;

    bool takeLoan(Economy& economy, float amount);
    void repayLoan(Economy& economy, int loanIndex);
    bool audit(Government& government);
    void update();
};


enum EventType { FAMINE, DISEASE, WAR, NATURAL_DISASTER };

class Event {
private:
    EventType type;
    int severity;
    int duration;
    int currentDuration;
    bool isActive;
public:
    Event();
    ~Event();

    void setType(EventType type);
    void setSeverity(int level);
    void setDuration(int turns);
    void setActive(bool active);

    EventType getType() const;
    int getSeverity() const;
    int getDuration() const;
    int getCurrentDuration() const;
    bool getIsActive() const;

    void trigger();
    void resolve();
    void update();
    void applyEffects(Kingdom& kingdom);
    const char* getEventName() const;
};

template <typename T>
class Logger {
private:
    char filename[50];
    T* data;
    int capacity;
    int count;
public:
    Logger(const char* filename, int capacity = 100) {
        strcpy(this->filename, filename);
        this->capacity = capacity;
        this->data = new T[capacity];
        this->count = 0;
    }

    ~Logger() {
        delete[] data;
    }

    void log(T value) {
        if (count < capacity) {
            data[count++] = value;
        }
    }

    void saveToFile() {
        std::ofstream file(filename);
        if (file.is_open()) {
            for (int i = 0; i < count; i++) {
                file << data[i] << std::endl;
            }
            file.close();
        }
        else {
            throw StrongholdException("Could not open file for logging");
        }
    }

    T* getData() const {
        return data;
    }

    int getCount() const {
        return count;
    }
};

class Kingdom {
private:
    char name[50];
    Population* population;
    Military* military;
    Economy* economy;
    Government* government;
    Bank* bank;
    Resource* resources;
    int resourceCount;
    Event* currentEvent;
    int turn;
    Logger<float>* treasuryLogger;

public:
    Kingdom(const char* name);
    ~Kingdom();

    Population& getPopulation();
    Military& getMilitary();
    Economy& getEconomy();
    Government& getGovernment();
    Bank& getBank();
    Resource* getResources();
    int getResourceCount() const;
    Event* getCurrentEvent();
    int getTurn() const;

    void addResource(const char* name, int amount, int productionRate, int consumptionRate, float price);
    Resource* findResource(const char* name);
    void nextTurn();
    void generateRandomEvent();
    bool checkGameOver() const;
    void saveGame(const char* filename);
    bool loadGame(const char* filename);
    void displayStatus() const;
};

#endif

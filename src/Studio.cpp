#include "../include/Studio.h"
#include <fstream>
#include <sstream>
//empty constructor
Studio::Studio() : open(false) {}
//constructor
Studio::Studio(const std::string &configFilePath):open(false),trainers(),workout_options(),actionsLog() {
    std::ifstream file(configFilePath);
    if (file.is_open()) {
        int numOfTrainers;
        std::string toRead;
        int wrkID = 0;
        std::getline(file, toRead);
        while (toRead.at(0) == '#' || (toRead.compare("") == 0)) {
            std::getline(file, toRead);
        }
        std::istringstream read(toRead);
        read >> numOfTrainers;
        std::getline(file, toRead);
        while ((toRead[0] == '#') || (toRead.compare("") == 0)) {
            std::getline(file, toRead);
        }
        int comma = 0;
        int trainerCapacity;
        for (int j = 0; j < (int) toRead.size(); j++) {
            if (toRead.at(j) == ',') {
                read = std::istringstream(toRead.substr(comma, j - comma));
                read >> trainerCapacity;
                Trainer *trainer = new Trainer(trainerCapacity);
                trainers.push_back(trainer);
                comma = j + 1;
            }
        }
        read = std::istringstream(toRead.substr(comma, (int) (toRead.rfind('\n') - comma)));
        read >> trainerCapacity;
        Trainer *trainer = new Trainer(trainerCapacity);
        trainers.push_back(trainer);
        std::getline(file, toRead);
        while ((toRead[0] == '#') || (toRead.compare("") == 0)) {
            std::getline(file, toRead);
        }
        bool finish = false;
        while (!finish) {
            std::string wrkName;
            WorkoutType wrkType;
            std::string wrkPrice;
            int comma = 0;
            int i = 0;
            while (i < (int) toRead.size()) {
                if (toRead.at(i) == ',') {
                    if (comma == 0) {
                        wrkName = toRead.substr(0, i);
                        comma = comma + 1;
                    } else if (comma == 1) {
                        if (toRead.at(i - 1) == 'c') {
                            wrkType = ANAEROBIC;
                        } else if (toRead.at(i - 1) == 'o') {
                            wrkType = CARDIO;
                        } else {
                            wrkType = MIXED;
                        }
                        comma = comma + 1;
                    }
                }
                if (comma == 2) {
                    std::istringstream read(toRead.substr(i + 1, toRead.size() - i - 1));
                    read >> wrkPrice;
                    comma = comma + 1;
                }
                i = i + 1;
            }
            workout_options.push_back(Workout(wrkID, wrkName, std::stoi((std::string) wrkPrice), wrkType));
            wrkID = wrkID + 1;
            std::getline(file, toRead);
            if (toRead.compare("") == 0) {
                finish = true;
            }
        }
    }
}
//copy constructor
Studio::Studio(const Studio &other) {
    this->clear();
    this->open=other.open;
    for (int i = 0;i<(int)other.trainers.size();i++) {
        Trainer* t = new Trainer(*other.trainers[i]);
        trainers.push_back(t);
    }
    for (auto it = other.workout_options.begin(); it < other.workout_options.end(); it++) {
        workout_options.push_back(*it);
    }
    for (int i = 0;i<(int)other.actionsLog.size();i++) {
        actionsLog.push_back(other.actionsLog[i]->clone());
    }
}
//Destructor
Studio::~Studio() {
    clear();
}
void Studio::clear() {
    for (std::size_t i = 0; i < trainers.size(); i++) {
        delete trainers[i];
    }
    for (std::size_t i = 0; i < actionsLog.size(); i++) {
        delete actionsLog[i];
    }
    trainers.clear();
    actionsLog.clear();
    workout_options.clear();
}
//move constructor
Studio::Studio(Studio&& other):open(other.open),trainers(other.trainers),workout_options(other.workout_options),actionsLog(other.actionsLog){
    other.open = false;
    other.trainers.clear();
    other.workout_options.clear();
    other.actionsLog.clear();
}
//copy assignment
Studio& Studio::operator=(const Studio &other) {
    if (this != &other) {
        this->clear();
        open = other.open;
        for (int i = 0; i < (int)other.trainers.size(); i++) {
            Trainer *t = new Trainer(*other.trainers[i]);
            trainers.push_back(t);
        }
        for (auto it = other.workout_options.begin(); it < other.workout_options.end(); it++) {
            workout_options.push_back(*it);
        }
        for (int i = 0; i < (int)other.actionsLog.size(); i++) {
            actionsLog.push_back(other.actionsLog[i]->clone());
        }
    }
    return *this;
}
//move assignment
Studio& Studio::operator=(Studio &&other) {
    if(this != &other) {
        this->clear();
        open = other.open;
        for (int i = 0; i < (int)other.trainers.size(); i++) {
            trainers.push_back(other.trainers[i]);
        }
        for(Workout toPush:other.workout_options){
            workout_options.push_back(toPush);
        }
        for (int i = 0; i < (int)other.actionsLog.size(); i++) {
            actionsLog.push_back(other.actionsLog[i]);
        }
        other.clear();
    }
    return *this;
}
void Studio:: start() {
    open = true;
    std::cout << "Studio is now open!" << std::endl;
    int custID=0;
    while (open) {
        std::string line;
        std::getline(std::cin, line);
        std::vector<std::string> words;
        std::istringstream reader(line);
        BaseAction* action = nullptr;
        std::string word;
        while(reader >> word) {
            words.push_back(word);
        }
        if (words[0].compare("open") == 0) {
            std::vector < Customer * > customerList;
            std::string tID = words[1];
            for(std::size_t i = 2; i < words.size(); i++){
                Customer* customer = nullptr;
                std::string custName = words[i].substr(0,words[i].find(','));
                std::string type = words[i].substr(words[i].find(',') + 1);
                if (type.compare("swt") == 0) {
                    customer = new SweatyCustomer(custName, custID);
                    custID++;
                } else if (type.compare("chp") == 0) {
                    customer = new CheapCustomer(custName, custID);
                    custID++;
                } else if (type.compare("mcl") == 0) {
                    customer = new HeavyMuscleCustomer(custName, custID);
                    custID++;
                } else if (type.compare("fbd") == 0) {
                    customer = new FullBodyCustomer(custName, custID);
                    custID++;
                }
                if (customer != nullptr){
                    customerList.push_back(customer);
                }
            }
            action = new OpenTrainer(std::stoi(tID),customerList);

        }

        if (words[0].compare("order") == 0){
            std::string stringTrainerID = words[1];
            action = new Order(std::stoi(stringTrainerID));
        }

        else if (words[0].compare("move") == 0) {
            std::string src=words[1];
            std::string dst=words[2];
            std::string cID=words[3];
            action = new MoveCustomer(std::stoi(src),std::stoi(dst), std::stoi(cID));
        }

        else if (words[0].compare("close") == 0) {
            std::string tID = words[1];
            action = new Close(std::stoi(tID));
        }

        else if (words[0].compare("closeall") == 0) {
            action = new CloseAll();
            action->act(*this);
            return;
        }

        else if (words[0].compare("workout_options") == 0) {
            action = new PrintWorkoutOptions();
        }

        else if (words[0].compare("status") == 0) {
            std::string tID = words[1];
            action = new PrintTrainerStatus(std::stoi(tID));
        }
        else if (words[0].compare("log") == 0) {
            action = new PrintActionsLog();
        }
        else if (words[0].compare("backup") == 0) {
            action = new BackupStudio();
        }

        else if (words[0].compare("restore") == 0) {
            action = new RestoreStudio();
        }

    action->act(*this);
    this->actionsLog.push_back(action);

    }
}

int Studio::getNumOfTrainers() const {
    return trainers.size();
}
Trainer* Studio::getTrainer(int tid) {
        return trainers.at(tid);
}
const std::vector<BaseAction*>& Studio::getActionsLog() const{
    return actionsLog;
}
std::vector<Workout>& Studio::getWorkoutOptions() {
    return workout_options;
}
void Studio::closeStudio() {
    open = false;
}

//int main(){

//}


#ifndef STUDIO_H_
#define STUDIO_H_

#include <vector>
#include <string>
#include "..//include/Workout.h"
#include "..//include/Trainer.h"
#include "..//include/Action.h"


class Studio{		
public:
    //empty constructor
	Studio();
    //constructor
    Studio(const std::string &configFilePath);
    //copy constructor
    Studio(const Studio &other);
    //Destructor
    virtual ~Studio();
    //move constructor
    Studio(Studio &&other);
    //copy assignment
    Studio& operator=(const Studio &other);
    //move assignment
    Studio& operator=(Studio &&other);
    //functions
    void start();
    int getNumOfTrainers() const;
    Trainer* getTrainer(int tid);
	const std::vector<BaseAction*>& getActionsLog() const; // Return a reference to the history of actions
    std::vector<Workout>& getWorkoutOptions();
    void closeStudio();

private:
    bool open;
    std::vector<Trainer*> trainers;
    std::vector<Workout> workout_options;
    std::vector<BaseAction*> actionsLog;
    void clear();
};

#endif
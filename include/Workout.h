#ifndef WORKOUT_H_
#define WORKOUT_H_

#include <string>

enum WorkoutType{
    ANAEROBIC, MIXED, CARDIO
};

class Workout{
public:
    //constructor
    Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type);
    ~Workout();
    //functions
    int getId() const;
    std::string getName() const;
    int getPrice() const;
    WorkoutType getType() const;
    bool comparator(const Workout& wrk2);
private:
	const int id;
    const std::string name;
    const int price;
    const WorkoutType type;
};


#endif
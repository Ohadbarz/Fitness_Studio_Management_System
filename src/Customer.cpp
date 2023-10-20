#include "../include/Customer.h"
#include <algorithm>
#include <iostream>

Customer::Customer(std::string c_name, int c_id):name(c_name),id(c_id){}

Customer::~Customer() = default;

int Customer::getId() const {
    return id;
}
std::string Customer::getName() const {
    return name;
}
//sweaty customer constructor
SweatyCustomer::SweatyCustomer(std::string name, int id): Customer(name,id) {}
SweatyCustomer:: ~SweatyCustomer() = default;
//sweaty customer function
std::vector<int> SweatyCustomer::order(const std::vector <Workout> &workout_options) {
    std::vector<int> swtVector;
    for (auto it = workout_options.begin();it!=workout_options.end();it++) {
        if(it->getType()==CARDIO){
            swtVector.push_back(it->getId());
        }
    }
    return swtVector;
}
std::string SweatyCustomer::toString() const {
    std::string s = this->getName() + ",swt";
    return s;
}
Customer* SweatyCustomer::clone() {
    return new SweatyCustomer(this->getName(), this->getId());
}
//cheap customer constructor
CheapCustomer::CheapCustomer(std::string name, int id): Customer(name,id){}
CheapCustomer:: ~CheapCustomer() = default;

//cheap customer functions
std::vector<int> CheapCustomer::order(const std::vector <Workout> &workout_options) {
    std::vector<int> chpVector;
    int cheapestId = 0;
    for (std::size_t i = 0 ; i<workout_options.size();i++) {
        if(workout_options.at(i).getPrice()<workout_options.at(cheapestId).getPrice()){
            cheapestId = i;
        }
        else if(workout_options.at(i).getPrice()==workout_options.at(cheapestId).getPrice()){
            if(workout_options.at(i).getId()<workout_options.at(cheapestId).getId()){
                cheapestId = i;
            }
        }
    }
    chpVector.push_back(workout_options.at(cheapestId).getId());
    return chpVector;
}
std::string CheapCustomer::toString() const {
   std::string s = this->getName() + ",chp";
   return s;
}
Customer* CheapCustomer::clone() {
    return new CheapCustomer(this->getName(), this->getId());
}
//comparator
bool comparator(std::pair<int,int> &p1,std::pair<int,int> &p2){
    if(p1.second!=p2.second){
    return p1.second > p2.second;
    }
    else{
        return p1.first < p2.first;
    }
}
//heavy muscle customer constructor
HeavyMuscleCustomer::HeavyMuscleCustomer(std::string name, int id): Customer(name,id) {}
HeavyMuscleCustomer:: ~HeavyMuscleCustomer() = default;
//heavy muscle customer constructor
std::vector<int> HeavyMuscleCustomer::order(const std::vector <Workout> &workout_options) {
    std::vector <int> mclVector;
    std::vector <std::pair<int,int>> anaerobicWorkouts;
    for (auto it =workout_options.begin();it!=workout_options.end();it++) {
        if (it->getType() == ANAEROBIC) {
            std::pair <int,int> p (it->getId(),it->getPrice());
            anaerobicWorkouts.push_back(p);
        }
        std::sort(anaerobicWorkouts.begin(),anaerobicWorkouts.end(), comparator);
    }


    for(auto it = anaerobicWorkouts.begin(); it != anaerobicWorkouts.end(); it++){
        mclVector.push_back(it->first);
    }


    return mclVector;
}

std::string HeavyMuscleCustomer::toString() const {
    std::string s = this->getName() + ",mcl";
    return s;
}
Customer* HeavyMuscleCustomer::clone() {
    return new HeavyMuscleCustomer(this->getName(), this->getId());
}
//full body customer
FullBodyCustomer::FullBodyCustomer(std::string name, int id): Customer(name,id) {};
FullBodyCustomer:: ~FullBodyCustomer() = default;
//full body functions
std::vector<int> FullBodyCustomer::order(const std::vector <Workout> &workout_options) {
    std::vector<int> fbdVector;
    bool cardioFlag = false;
    bool mixedFlag = false;
    bool anaerobicFlag = false;
    int cheapestCardioId = -1;
    int expensiveMixId = -1;
    int cheapestAnaerobicId = -1;
    for(std:: size_t i = 0;i<workout_options.size();i++) {
        if (workout_options.at(i).getType() == CARDIO) {
            cardioFlag = true;
            if(cheapestCardioId<0){
                cheapestCardioId = i;
            }
            else if (workout_options.at(i).getPrice() < workout_options.at(cheapestCardioId).getPrice()) {
                cheapestCardioId = i;
            } else if (workout_options.at(i).getPrice() == workout_options.at(cheapestCardioId).getPrice()) {
                if (workout_options.at(i).getId() < workout_options.at(cheapestCardioId).getId()) {
                    cheapestCardioId = i;
                }
            }
        }
        else if (workout_options.at(i).getType() == MIXED) {
                    mixedFlag = true;
                    if(expensiveMixId<0){
                        expensiveMixId = i;
                    }
                    else if (workout_options.at(i).getPrice() > workout_options.at(expensiveMixId).getPrice()) {
                        expensiveMixId = i;
                    } else if (workout_options.at(i).getPrice() == workout_options.at(expensiveMixId).getPrice()) {
                        if (workout_options.at(i).getId() < workout_options.at(expensiveMixId).getId()) {
                            expensiveMixId = i;
                        }
                    }
                }
        else if(workout_options.at(i).getType() == ANAEROBIC){
                anaerobicFlag = true;
                if(cheapestAnaerobicId<0){
                    cheapestAnaerobicId = i;
                }
                else if (workout_options.at(i).getPrice() < workout_options.at(cheapestAnaerobicId).getPrice()) {
                    cheapestAnaerobicId = i;
                } else if (workout_options.at(i).getPrice() ==
                           workout_options.at(cheapestAnaerobicId).getPrice()) {
                    if (workout_options.at(i).getId() < workout_options.at(cheapestAnaerobicId).getId()) {
                        cheapestAnaerobicId = i;
                    }
                }
            }
        }
    if (cardioFlag) {
        fbdVector.push_back(workout_options.at(cheapestCardioId).getId());
    }
    if (mixedFlag) {
        fbdVector.push_back(workout_options.at(expensiveMixId).getId());
    }
    if (anaerobicFlag) {
        fbdVector.push_back(workout_options.at(cheapestAnaerobicId).getId());
    }
    return fbdVector;
}
std::string FullBodyCustomer::toString() const {
    std::string s = this->getName() + ",fbd";
    return s;
}
Customer* FullBodyCustomer::clone() {
    return new FullBodyCustomer(this->getName(), this->getId());
}
#include <iostream>
//int main() {
//    HeavyMuscleCustomer *c1 = new HeavyMuscleCustomer("alon", 0);
//    FullBodyCustomer *c2 = new FullBodyCustomer("rotem", 1);
//    SweatyCustomer *c3= new SweatyCustomer("shcena", 2);
//    CheapCustomer *c4= new CheapCustomer("shcena", 3);
//    std::cout << c2->toString()<< std::endl;
//    std::cout << c3->toString()<< std::endl;
//    Workout wrk1(6, "zumba", 20, CARDIO);
//    Workout wrk2(8, "pilates", 80, ANAEROBIC);
//    Workout wrk3(7, "run", 60, MIXED);
//    Workout wrk4(4, "cycling", 60, ANAEROBIC);
//    Workout wrk5(5, "swim", 20, CARDIO);
//    Workout wrk6(2, "walking", 60, MIXED);
//    std::vector<Workout> workoutOptions;
//    workoutOptions.push_back(wrk1);
//    workoutOptions.push_back(wrk2);
//    workoutOptions.push_back(wrk3);
//    workoutOptions.push_back(wrk4);
//    workoutOptions.push_back(wrk5);
//    workoutOptions.push_back(wrk6);
//    std::vector<int> order = c2->order(workoutOptions);
//    for(int i = 0; i < (int)order.size();i++){
//        std::cout<<order[i]<<std::endl;
//    }
//}

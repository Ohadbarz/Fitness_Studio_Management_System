#include "../include/Workout.h"
#include <iostream>
//constructor
Workout::Workout(int w_id, std::string w_name, int w_price, WorkoutType w_type):id(w_id),name(w_name),price(w_price),type(w_type) {}

Workout::~Workout() = default;

int Workout::getId() const {
    return id;
}
std::string Workout::getName() const {
    return name;
}
int Workout::getPrice() const {
    return price;
}
WorkoutType Workout::getType() const {
    return type;
}
bool Workout::comparator(const Workout& wrk2){
    return (this->getPrice()<wrk2.getPrice());
}
#include <iostream>
//int main(){
// Workout wrk(5, "zumba", 70, CARDIO);
// std::cout<<wrk.getPrice()<< std::endl;
// std::cout<<wrk.getId()<< std::endl;
// std::cout<<wrk.getName()<< std::endl;
// std::cout<<wrk.getType()<< std::endl;
// Workout wrk2(6,"zumba",80, ANAEROBIC);
// std::cout<<true<<std::endl;
// std::cout<<wrk.comparator(wrk2)<< std::endl;
//}
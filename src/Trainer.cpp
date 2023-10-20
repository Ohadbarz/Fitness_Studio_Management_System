
#include <iostream>
#include "../include/Trainer.h"
#include <vector>
#include <algorithm>
//constructor
Trainer::Trainer(int t_capacity) : capacity(t_capacity),open(false),customersList(),orderList(),salary(0){}
//copy constructor
Trainer::Trainer(const Trainer &other){
    this->clear();
    capacity = other.capacity;
    open = other.open;
    salary = other.salary;
    for(auto it = other.orderList.begin();it<other.orderList.end();it++){
        orderList.push_back(*it);
    }
    for(auto it = other.customersList.begin();it<other.customersList.end();it++){
        customersList.push_back((*it)->clone());
    }

}
//Destructor
Trainer::~Trainer() {clear();}
//clear function
void Trainer::clear() {
    if(!customersList.empty()){
        capacity = 0;
        open = false;
        salary = 0;
        for (std::size_t i = 0; i < customersList.size(); i++) {
            delete customersList[i];
        }
        customersList.clear();
        orderList.clear();
    }
}
//move constructor
Trainer::Trainer(Trainer&& other):capacity(other.capacity),open(other.open), salary(other.salary){
    this->customersList = other.customersList;
    other.customersList.clear();
    orderList = std::move(other.orderList);
    other.customersList.clear();
    other.capacity=0;
    other.salary = 0;
    other.open = false;

}
//copy assignment
Trainer& Trainer::operator=(const Trainer &other) {
    if (this != &other) {
        this->clear();
        capacity = other.capacity;
        open = other.open;
        salary = other.salary;
        for (auto it = other.orderList.begin(); it < other.orderList.end(); it++) {
            orderList.push_back(*it);
        }
        for (auto it = other.customersList.begin(); it < other.customersList.end(); it++) {
            customersList.push_back(*it);
        }
    }
    return *this;

}
//move assignment
Trainer& Trainer::operator=(Trainer &&other) {
    if(this!=&other){
        this->clear();
        capacity = other.capacity;
        open = other.open;
        salary = other.salary;
        for (auto it = other.orderList.begin(); it < other.orderList.end(); it++) {
            orderList.push_back(*it);
        }
        for (auto it = other.customersList.begin(); it < other.customersList.end(); it++) {
            customersList.push_back(*it);
        }
        other.clear();
    }
    return *this;
}
//functions
int Trainer::getCapacity() const {
    return capacity;
}
void Trainer::addCustomer(Customer *customer) {
    int numOfCustomers = customersList.size();
    if(numOfCustomers < capacity&& this->isOpen()){
        customersList.push_back(customer);
    }
}
int Trainer::getSalary() {
    return salary;
}
bool Trainer::isOpen() {
    return open;
}
Customer* Trainer::getCustomer(int id) {
    Customer* customer = nullptr;
    for(std::size_t i =0; i<customersList.size(); i++){
        if (customersList.at(i)->getId() == id){
            customer = customersList.at(i);
        }
    }
    return customer;
}
std::vector<Customer*>& Trainer::getCustomers() {
    return customersList;
}
std::vector<OrderPair>& Trainer::getOrders() {
    return orderList;
}
void Trainer::removeCustomer(int id) {
    if (isOpen()) {
        std::vector <OrderPair> newVec;
        for (std::size_t i = 0; i < customersList.size(); i++) {
            if (customersList.at(i)->getId() == id) {
                customersList.erase(customersList.begin() + i);
            }
        }
        if (!orderList.empty()) {
            for(std::size_t i = 0;i<orderList.size();i++) {
                if(orderList.at(i).first!=id) {
                    newVec.push_back(orderList.at(i));
                }
                else if(orderList.at(i).first==id){
                    salary = salary - orderList.at(i).second.getPrice();
                }
            }
            orderList = std::vector<OrderPair>(newVec);
        }
    }
}
void Trainer::order(const int customer_id, const std::vector<int> workout_ids,
                    const std::vector <Workout> &workout_options) {
    if (!isOpen()) {
        std::cout << "Trainer does not exist or is not open" << std::endl;
    } else {
        for (std::size_t i = 0; i < workout_ids.size(); i++) {
            for (std::size_t j = 0; j < workout_options.size(); j++) {
                if (workout_options[j].getId() == workout_ids[i]) {
                    std::pair<int, Workout> p1(customer_id, workout_options[j]);
                    orderList.push_back(p1);
                    salary = salary + p1.second.getPrice();
                }
            }
        }
    }
}
void Trainer::closeTrainer() {
    open = false;
    for (std::size_t i = 0; i < customersList.size(); i++) {
        delete customersList[i];
    }
    customersList.clear();
    orderList.clear();
}
void Trainer::openTrainer() {
        this->open = true;
}
//
//int main() {
//    Trainer *t1 = new Trainer(100);
//////    t1->openTrainer();
//////    Trainer *t2 = new Trainer(3);
//////    t2->openTrainer();
//////    std:: cout<< t2->isOpen()<<std::endl;
//////  std:: cout<< t2->getSalary()<<std::endl;
//////    std:: cout<< t2->getCapacity()<<std::endl;
//    HeavyMuscleCustomer *c1 = new HeavyMuscleCustomer("alon", 13);
//////    FullBodyCustomer *c2 = new FullBodyCustomer("rotem", 1);
//////    SweatyCustomer *c3 = new SweatyCustomer("shcena", 2);
//////    CheapCustomer *c4 = new CheapCustomer("shcena", 3);
//////    std::cout << c2->toString()<< std::endl;
//////    std::cout << c3->toString()<< std::endl;
//////    Workout wrk1(6, "zumba", 20, CARDIO);
//////    Workout wrk2(8, "pilates", 80, ANAEROBIC);
//////    Workout wrk3(7, "run", 60, MIXED);
//    Workout wrk4(4, "cycling", 60, ANAEROBIC);
//////    Workout wrk5(5, "swim", 20, CARDIO);
//////    Workout wrk6(2, "walking", 60, MIXED);
//////    std::vector <Workout> workoutOptions;
//////    workoutOptions.push_back(wrk1);
//////    workoutOptions.push_back(wrk2);
//////    workoutOptions.push_back(wrk3);
//////    workoutOptions.push_back(wrk4);
//////    workoutOptions.push_back(wrk5);
//////    workoutOptions.push_back(wrk6);
//////    std::vector<int> order1 = c1->order(workoutOptions);
//////    std::vector<int> order2 = c2->order(workoutOptions);
//////    std::vector<int> order3 = c3->order(workoutOptions);
//    t1->openTrainer();
//    t1->addCustomer(c1);
////
////
//    Customer* str = t1->getCustomer(c1->getId());
//    std::cout << str->getId() << std::endl;
//}
///    t2->addCustomer(c2);
////    t2->addCustomer(c3);
////    t2->order(c1->getId(), order1, workoutOptions);
////    t2->order(c2->getId(), order2, workoutOptions);
////    t2->order(c3->getId(), order3, workoutOptions);
//////    for (int i = 0; i < t2->getCustomers().size(); i++) {
//////        std::cout << t2->getCustomers().at(i)->getName() << std::endl;
//////        std::cout << t2->getCustomers().at(i)->getId() << std::endl;
//////    }
//////    for (OrderPair o: t2->getOrders()) {
//////        std::cout << o.first << std::endl;
//////        std::cout << o.second.getName() << std::endl;
//////    }
//////    t2->removeCustomer(c2->getId());
//////    for (int i = 0; i < t2->getCustomers().size(); i++) {
//////        std::cout << t2->getCustomers().at(i)->getName() << std::endl;
//////        std::cout << t2->getCustomers().at(i)->getId() << std::endl;
//////    }
//////    for (OrderPair o: t2->getOrders()) {
//////        std::cout << o.first << std::endl;
//////        std::cout << o.second.getName() << std::endl;
//////    }
//////    t2->closeTrainer();
//////    std::cout << t2->getSalary() << std::endl;
//////}

#ifndef TRAINER_H_
#define TRAINER_H_

#include <vector>
#include "../include/Customer.h"
#include "../include/Workout.h"
#include "iostream"
#include <string>

typedef std::pair<int, Workout> OrderPair;

class Trainer{
public:
    //constructor
    Trainer(int t_capacity);
    //copy constructor
    Trainer(const Trainer &other);
    //Destructor
    virtual ~Trainer();
    //move constructor
    Trainer(Trainer &&other);
    //copy assignment
    Trainer& operator=(const Trainer &other);
    //move assignment
    Trainer& operator=(Trainer &&other);
    //functions
    int getCapacity() const;
    void addCustomer(Customer* customer);
    void removeCustomer(int id);
    Customer* getCustomer(int id);
    std::vector<Customer*>& getCustomers();
    std::vector<OrderPair>& getOrders();
    void order(const int customer_id, const std::vector<int> workout_ids, const std::vector<Workout>& workout_options);
    void openTrainer();
    void closeTrainer();
    int getSalary();
    bool isOpen();
private:
    int capacity;
    bool open;
    std::vector<Customer*> customersList;
    std::vector<OrderPair> orderList; //A list of pairs for each order for the trainer - (customer_id, Workout)
    int salary;
    void clear();
};


#endif
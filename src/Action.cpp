#include "../include/Action.h"
#include "../include/Studio.h"
#include <algorithm>




extern Studio* backup;

BaseAction::BaseAction():errorMsg(""),status() {};
BaseAction::~BaseAction() = default;
void BaseAction:: complete(){
  status = COMPLETED;
}
ActionStatus BaseAction::getStatus() const {
 return status;
}
void BaseAction:: error(std::string errorMsg){
    status = ERROR;
    this->errorMsg = errorMsg;
}
std::string BaseAction:: getErrorMsg() const{
    return errorMsg;
}

OpenTrainer::OpenTrainer(int id, std::vector<Customer *> &customersList):BaseAction(), trainerId(id),customers(customersList) {
    str = "";
}
OpenTrainer::~OpenTrainer() = default;
void OpenTrainer::act(Studio &studio) {

    str = "open " + std::to_string(trainerId);
    for(auto it = customers.begin();it<customers.end();it++) {
        str = str +" " +(*it)->toString();
    }

    if(trainerId < studio.getNumOfTrainers()) {
        Trainer *t = studio.getTrainer(trainerId);
        if (t != nullptr && !t->isOpen()) {
            t->openTrainer();

            for (std::size_t i = 0; i < customers.size(); i++) {
                if ((int)i < t->getCapacity()) {
                    t->addCustomer(customers.at(i));
                } else {
                    delete customers.at(i);
                }
            }
            BaseAction::complete();
        }
        else{
            std::string err = "Trainer does not exist or is already open";
            BaseAction::error(err);
            std::cout<< err <<std::endl;
        }
    }
    else{
        std::string err = "Trainer does not exist or is already open";
        BaseAction::error(err);
        std::cout<< err <<std::endl;
    }
    this->customers.clear();
}
std::string OpenTrainer::toString() const {
        std:: string toAdd = "";
    if(BaseAction::getStatus()==COMPLETED){
        toAdd = " Completed";
    }
    else{
        toAdd = " Error: "+ this->getErrorMsg();
    }
    return str + toAdd;
}

BaseAction* OpenTrainer::clone() {
    OpenTrainer* copy = new OpenTrainer(trainerId,customers);
    copy->str = str;
    return copy;
}
Order::Order(int id):BaseAction(),trainerId(id) {};
Order::~Order() = default;

void Order::act(Studio &studio){
    std::string name = "";
    if(trainerId < studio.getNumOfTrainers()) {
        Trainer *t = studio.getTrainer(trainerId);
        if (t != nullptr && t->isOpen()){
            for(std::size_t i = 0; i < t->getCustomers().size();i++ ){
                std::vector<int> vectorID = t->getCustomers().at(i)->order(studio.getWorkoutOptions());

                t->order(t->getCustomers().at(i)->getId(),vectorID,studio.getWorkoutOptions());
            }
            for(auto it = t->getOrders().begin();it < t->getOrders().end();it++){
                std::string name;
                int id = it->first;
                for(std::size_t i = 0;i<t->getCustomers().size();i++){
                    int toCompare = t->getCustomers()[(int)i]->getId();
                    if (toCompare == id){
                        name = t->getCustomers()[(int)i]->getName();
                        std::cout<< name + " is doing " + it->second.getName()<< std::endl;
                    }

                }
            }
            BaseAction::complete();
            }
        else{
            std::string err = "Trainer does not exist or is already open";
            BaseAction::error(err);
            std::cout<< err <<std::endl;
        }
    }
    else{
        std::string err = "Trainer does not exist or is already open";
        BaseAction::error(err);
        std::cout<< err <<std::endl;
    }
}


std::string Order::toString() const{
    std::string ret = "order " + std::to_string(trainerId);
    if(BaseAction::getStatus() == COMPLETED){
        ret = ret + " Completed";
    }
    else{
        ret = ret + " Error: "+ this->getErrorMsg();
    }
    return ret;
}
BaseAction* Order::clone() {
    Order* copy = new Order(trainerId);
    return copy;
}

MoveCustomer::MoveCustomer(int src, int dst, int customerId):BaseAction(),srcTrainer(src),dstTrainer(dst),id(customerId) {};
MoveCustomer::~MoveCustomer() = default;
void MoveCustomer::act(Studio &studio) {
    if (srcTrainer < studio.getNumOfTrainers() && dstTrainer < studio.getNumOfTrainers()) {
        Trainer *dst = studio.getTrainer(dstTrainer);
        Trainer *src = studio.getTrainer(srcTrainer);
        if (src->isOpen() && dst->isOpen() && src != nullptr && dst != nullptr &&
            dst->getCapacity() > (int) dst->getCustomers().size() && src->getCustomer(id)!= nullptr) {
            dst->addCustomer(src->getCustomer(id));
            dst->order(id, dst->getCustomer(id)->order(studio.getWorkoutOptions()), studio.getWorkoutOptions());
            src->removeCustomer(id);
            if (src->getCustomers().empty()) {
                src->closeTrainer();
            }
            complete();
        } else {
            std::string err = "cannot move customer";
            BaseAction::error(err);
            std::cout<< err <<std::endl;
        }
    } else {
        std::string err = "cannot move customer";
        BaseAction::error(err);
        std::cout<< err <<std::endl;
    }
}
std::string MoveCustomer::toString() const{
    std::string ret = "move " + std::to_string(srcTrainer)+" "+ std::to_string(dstTrainer)+" "+ std::to_string(id) + " ";
    if(BaseAction::getStatus() == COMPLETED){
        ret = ret + "Completed";
    }
    else{
        ret = ret + "Error: "+ this->getErrorMsg();
    }
    return ret;
}
BaseAction* MoveCustomer::clone() {
    MoveCustomer* copy = new MoveCustomer(srcTrainer,dstTrainer,id);
    return copy;
}

Close::Close(int id):BaseAction(),trainerId(id){};
Close::~Close() = default;
void Close::act(Studio &studio){
    if(trainerId<studio.getNumOfTrainers()) {
        Trainer *t = studio.getTrainer(trainerId);
        if (t->isOpen() && t != nullptr) {
            t->closeTrainer();
            std::cout << "Trainer " + std::to_string(trainerId) + " closed. Salary " + std::to_string(t->getSalary()) +
                         "NIS"<<std::endl;
            complete();
        }
        else{
            std::string err = "Trainer does not exist or is already open";
            BaseAction::error(err);
            std::cout<< err <<std::endl;
        }
    }
    else{
        std::string err = "Trainer does not exist or is already open";
        BaseAction::error(err);
        std::cout<< err <<std::endl;
    }
}
BaseAction* Close::clone() {
    Close* copy = new Close(trainerId);
    return copy;
}
std::string Close::toString() const{
    std::string ret = "close " + std::to_string(trainerId);
    if(BaseAction::getStatus() == COMPLETED){
        ret = ret + " Completed";
    }
    else{
        ret = ret + " Error: "+ this->getErrorMsg();
    }
    return ret;
}
CloseAll::CloseAll():BaseAction() {};
CloseAll::~CloseAll() = default;
void CloseAll::act(Studio &studio) {
    for(int i = 0;i<studio.getNumOfTrainers();i++) {
        Trainer *t = studio.getTrainer(i);
        if (t->isOpen()) {
            Close closeT = Close(i);
            closeT.act(studio);
        }
    }
    studio.closeStudio();
    complete();
    delete this;
}
std::string CloseAll::toString() const {
    std::string ret = "closeall";
    return ret;
}
BaseAction* CloseAll::clone() {
    CloseAll* copy = new CloseAll();
    return copy;
}
PrintWorkoutOptions::PrintWorkoutOptions():BaseAction() {};
PrintWorkoutOptions::~PrintWorkoutOptions() = default;
void PrintWorkoutOptions::act(Studio &studio) {
    for(auto it = studio.getWorkoutOptions().begin();it<studio.getWorkoutOptions().end();it++){
        std::cout<<(*it).getName() + ", " + std::to_string((*it).getType()) + ", " + std::to_string((*it).getPrice())<<std::endl;
    }
    complete();
}
std::string PrintWorkoutOptions::toString() const {
    std::string s = "workout_options Completed";
    return s;
}
BaseAction* PrintWorkoutOptions::clone() {
    PrintWorkoutOptions* copy = new PrintWorkoutOptions();
    return copy;
}

PrintTrainerStatus::PrintTrainerStatus(int id):BaseAction(), trainerId(id){};
PrintTrainerStatus::~PrintTrainerStatus() = default;
void PrintTrainerStatus::act(Studio &studio){
    Trainer* t = studio.getTrainer(trainerId);
    if (!t->isOpen()){
        std::cout<<"Trainer " + std::to_string(trainerId) + " status: closed"<<std::endl;
    }
    else{
        std::cout<<"Trainer " + std::to_string(trainerId) + " status: open"<<std::endl;
        std::cout<<"Customers:"<<std::endl;
        for(auto it = t->getCustomers().begin(); it < t->getCustomers().end();it++){
            std::cout<<std::to_string((*it)->getId()) + " " + ((*it)->getName())<<std::endl;
        }
        std::cout<<"orders:"<<std::endl;
        for(auto it = t->getOrders().begin(); it < t->getOrders().end();it++){
            std::cout<<(*it).second.getName() + " " + std::to_string((*it).second.getPrice()) + "NIS "+ std::to_string((*it).first)<<std::endl;
        }
        std::cout<<"Current Trainer's Salary: " + std::to_string(t->getSalary()) + "NIS" <<std::endl;

    }
    complete();
}
std::string PrintTrainerStatus::toString() const {
    return "status " + std::to_string(trainerId) + " Completed";
}
BaseAction* PrintTrainerStatus::clone() {
    PrintTrainerStatus* copy = new PrintTrainerStatus(trainerId);
    return copy;
}

PrintActionsLog::PrintActionsLog(): BaseAction() {};
PrintActionsLog::~PrintActionsLog() = default;
void PrintActionsLog::act(Studio &studio) {
    for(std::size_t i = 0;i<studio.getActionsLog().size(); i++){
        std::cout<<studio.getActionsLog().at(i)->toString()<<std::endl;
    }
    complete();
}

std::string PrintActionsLog::toString() const {
    return "log Completed";
}
BaseAction* PrintActionsLog::clone() {
    PrintActionsLog* copy = new PrintActionsLog();
    return copy;
}

BackupStudio::BackupStudio():BaseAction() {};
BackupStudio::~BackupStudio() = default;
void BackupStudio::act(Studio &studio) {
    Studio *temp = backup;
    backup = new Studio(studio);
    if (temp != nullptr) {
        delete temp;
    }
    complete();
}

std::string BackupStudio::toString() const{
    return "backup Completed";
}
BaseAction* BackupStudio::clone() {
    BackupStudio* copy = new BackupStudio();
    return copy;
}

RestoreStudio::RestoreStudio():BaseAction() {};
RestoreStudio::~RestoreStudio() = default;
void RestoreStudio::act(Studio &studio){
    if (backup!= nullptr){
        studio = *backup;
        complete();
    }
    else{
        std::string err = "No backup available";
        BaseAction::error(err);
        std::cout<< err <<std::endl;
    }

}
std::string RestoreStudio::toString() const{
    std::string ret = "restore";
    if(BaseAction::getStatus() == COMPLETED){
        ret = ret + " Completed";
    }
    else{
        ret = ret + " Error: "+ this->getErrorMsg();
    }
    return ret;
}
BaseAction* RestoreStudio::clone() {
    RestoreStudio* copy = new RestoreStudio();
    return copy;
}
//int main(){
//    std::cout<<"the old lady"<<std::endl;
//}








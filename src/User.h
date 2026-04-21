#include <iostream>
#include <string>


using namespace std;

class User{
    private:
        string name;
        string userID;
        string userPass;
        string userCode;
        string userRole;
        static int number;
    public:
        User(string name, string userId, string userRole): name(name), userID(userId), userCode(userCode), userRole(userRole){
            number++;
            userCode = userRole.substr(0,1) + to_string(number);
        }

        friend class Logger;
        //virtual void displayDetails() = 0; 
        void setValues(string name, string userId, string userCode, string userRole, int caseVer){
            this->name =name;
            this->userID =userId;
            this->userCode =userCode;
            this->userRole =userRole;
        }
        void saveLoggs();                     //Loggs function should be made
        void saveInforToFile();               //Saves info 
        void displayInfo();
};


void User::saveInforToFile() {

};

void User::displayInfo() {
    cout << "ID: " << userID << endl;
    cout << "Name: " << name << endl;
    cout << "Code: " << userCode << endl;
    cout << "Role: " << userRole << endl;
};
int User::number = 0;











class Admin: public User {
    static int num;
    int caseVerified;

public:
    void setValues(int num, string name, string userId, string userRole, int caseVer){

    }
};
class Analyst: public User {

};
class IntakeOfficer: public User {
    int caseOpened;
};
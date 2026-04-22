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
    
};
int User::number = 0;


class Admin: public User {
    int caseVerified;

public:
    Admin(string name, string userId, string userCode, string userRole, int caseVerified): caseVerified(caseVerified), User(name, userId, userCode,  userRole){}
    //void setValues(int num, string name, string userId, string userRole, int caseVer){}

    void displayDetails() override {
    cout << "ID: " << userID << endl;
    cout << "Name: " << name << endl;
    cout << "Code: " << userCode << endl;
    cout << "Role: " << userRole << endl;
    cout << "Case Verified : " << caseVerified << endl;
    };
};

class Analyst: public User {
    int casenvestigated;
public:
    Analyst( string name, string userId, string userCode, string userRole,int casenvestigated): casenvestigated(casenvestigated), User(name, userId, userCode, userRole){}

    void displayDetails() override {
    cout << "ID: " << userID << endl;
    cout << "Name: " << name << endl;
    cout << "Code: " << userCode << endl;
    cout << "Role: " << userRole << endl;
    cout << "Case Investegated : " << casenvestigated << endl;
    };


};
class IntakeOfficer: public User {
    int caseOpened;
public:
    IntakeOfficer(string name, string userId, string userCode, string userRole, int caseOpened): caseOpened(caseOpened), User(name, userId, userCode, userRole){}

    void displayDetails() override {
    cout << "ID: " << userID << endl;
    cout << "Name: " << name << endl;
    cout << "Code: " << userCode << endl;
    cout << "Role: " << userRole << endl;
    cout << "Case Opened : " << caseOpened << endl;
    };
};
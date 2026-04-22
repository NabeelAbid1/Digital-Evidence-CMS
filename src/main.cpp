#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <Windows.h>
#include <unordered_map>  //Provides a hash map (key-value container)
#include <functional>     //Allows storing functions
#include <thread>         //For sleep (cross platform)
#include <chrono>         //Used with thread
#include <sstream>        //For stringstream

//classes headers
#include "User.h"
#include "Case.h"
#include "Evidence.h"

using namespace std;


//Function declarations
int AdminRoles();
int AnalystRoles();
int IntakeOfficerRoles();
User* loginUser(string userName, string userPass, const string& filename);





int main(){
    int LoginChoise;

    //Map the login strategies
    unordered_map<int, function<int()>> loginStrategies = {
        {1, AdminRoles},
        {2, AnalystRoles},
        {3, IntakeOfficerRoles},

    };

    while(true){
    //Designing Menu system
        system("cls");
        cout << "**Digital Evidence and Case Management System**"<<endl;
        this_thread::sleep_for(chrono::milliseconds(2000));  //Sleep(2000);
        
        //Login Page
        start:
        cout << "**Login User**"<<endl;                 
        cout << "\t1. Login as Admin" << endl;          //Admin/Chief: Full access to re-open cases and view all logs.
        cout << "\t2. Login as Analyst" << endl;        //Analyst: Can view and add notes to assigned cases only.
        cout << "\t3. Login as Intake Officer" << endl; //Intake Officer: Can register cases but cannot modify evidence.
        cout << "Enter number to login... : "; 
        cin >> LoginChoise;

        auto it = loginStrategies.find(LoginChoise);
        if (it!=loginStrategies.end()){
            int result = it->second();   //calls the relative function of login
            if(result == 99) {
                cout<<"Login failed! Returning to Login page\n";
                this_thread::sleep_for(chrono::microseconds(2000));  //sleep(2000)
                continue; //skip this iteration (go to start)
            }
        } else {
            cout <<"Choice must be from 1-3\n";
        }


        
    }
  

}

int AdminRoles(){
    User* CurrentLoggedAdmin = nullptr;

    //Design login menu of admin
    int choice;
    cout << "Enter\n";
    cout << "\t1. Enter Credentials\n";
    cout << "\t99. Back to Login page\n";
    cin >> choice;

    if(choice == 99) return choice;
    if(choice != 99 && choice != 1) {
        cout << "Invalid Choice\n";
        return 99;
    }
    int num=0;
    while(true){
        //Taking credentials
        string loginUserInput, loginUserPass;
        cout << "Enter login id";
        cin >> loginUserInput;
        cout << "Enter password";
        cin >> loginUserPass;
        cout << "Verifying Credentials...\n";

        this_thread::sleep_for(chrono::seconds(2));

        string adminfile = "E:\Projects\Digital Evidence and Case Management System Sem02 (OOP)\Digital-Evidence-CMS\data\admin.txt";
            
        //call login function
        CurrentLoggedAdmin = loginUser(loginUserInput, loginUserPass, adminfile);
        
        //if Not verified 
            //User can enter credentials three 3 only for preventing brute force attack then send back to login page
        if(CurrentLoggedAdmin!=nullptr) break;
        if(num <=3){
            cout << "Try again\n";
            num++;
            system("cls");
            continue;
        }else if (num > 3){
            cout <<"Login limit exceeds\n";
            system("cls");
            return 99;
        }     
    }

    //Design the Main admin menu where list all the functions of admin

    


    
   
}

int AnalystRoles(){
    User* CurrentLoggedAnalyst = nullptr;

    //Design login menu of admin
    int choice;
    cout << "Enter\n";
    cout << "\t1. Enter Credentials\n";
    cout << "\t99. Back to Login page\n";
    cin >> choice;

    if(choice == 99) return choice;
    if(choice != 99 && choice != 1) {
        cout << "Invalid Choice\n";
        return 99;
    }
    int num=0;
    while(true){
        //Taking credentials
        string loginUserInput, loginUserPass;
        cout << "Enter login id";
        cin >> loginUserInput;
        cout << "Enter password";
        cin >> loginUserPass;
        cout << "Verifying Credentials...\n";

        this_thread::sleep_for(chrono::seconds(2));

        string analystfile = "E:\Projects\Digital Evidence and Case Management System Sem02 (OOP)\Digital-Evidence-CMS\data\analyst.txt";
            
        //call login function
        CurrentLoggedAnalyst = loginUser(loginUserInput, loginUserPass, analystfile);
        
        //if Not verified 
            //User can enter credentials three 3 only for preventing brute force attack then send back to login page
        if(CurrentLoggedAnalyst!=nullptr) break;
        if(num <=3){
            cout << "Try again\n";
            num++;
            system("cls");
            continue;
        }else if (num > 3){
            cout <<"Login limit exceeds\n";
            system("cls");
            return 99;
        }     
    }

    //Design the Main admin menu where list all the functions of admin

}

int IntakeOfficerRoles(){ 
    User* CurrentLoggedIntakeOfficer = nullptr;

    //Design login menu of admin
    int choice;
    cout << "Enter\n";
    cout << "\t1. Enter Credentials\n";
    cout << "\t99. Back to Login page\n";
    cin >> choice;

    if(choice == 99) return choice;
    if(choice != 99 && choice != 1) {
        cout << "Invalid Choice\n";
        return 99;
    }
    int num=0;
    while(true){
        //Taking credentials
        string loginUserInput, loginUserPass;
        cout << "Enter login id";
        cin >> loginUserInput;
        cout << "Enter password";
        cin >> loginUserPass;
        cout << "Verifying Credentials...\n";

        this_thread::sleep_for(chrono::seconds(2));

        string IntakeOfficerfile = "E:\Projects\Digital Evidence and Case Management System Sem02 (OOP)\Digital-Evidence-CMS\data\IntakeOfficer.txt";
            
        //call login function
        CurrentLoggedIntakeOfficer = loginUser(loginUserInput, loginUserPass, IntakeOfficerfile);
        
        //if Not verified 
            //User can enter credentials three 3 only for preventing brute force attack then send back to login page
        if(CurrentLoggedIntakeOfficer!=nullptr) break;
        if(num <=3){
            cout << "Try again\n";
            num++;
            system("cls");
            continue;
        }else if (num > 3){
            cout <<"Login limit exceeds\n";
            system("cls");
            return 99;
        }     
    }

    //Design the Main admin menu where list all the functions of admin

}

User* loginUser(string userName, string userPass, const string& filename){
    User* loggedInUser = nullptr;
    string line;
    ifstream file(filename);
    if(!file){
        cout << "Error!\tUnable to open file\n";
        return nullptr;
    }
    
    while(getline(file,line)){
        stringstream ss(line);
        string piecesInfo;
        vector<string> data;

        while(getline(ss, piecesInfo, '|')){
            data.push_back(piecesInfo);
        }
        
        if(data[1]==userName && data[2]==userPass){ //MatchFound
            if(data[4]=="Admin"){
                loggedInUser = new Admin(data[0], data[1], data[3], data[4], stoi(data[5]));
            }
            else if(data[4]=="Analyst"){
                loggedInUser = new Analyst(data[0], data[1], data[3], data[4], stoi(data[5]));
            }
            else if(data[4]=="IntakeOfficer"){
                loggedInUser = new IntakeOfficer(data[0], data[1], data[3], data[4], stoi(data[5]));
            }
        }
        
    }
    file.close();
    if(loggedInUser==nullptr) {
        cout << "Invalid username or password" << endl;
        this_thread::sleep_for(chrono::seconds(2));
    }
    return loggedInUser;


}

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

//classes headers
#include "User.h"
#include "Case.h"
#include "Evidence.h"

using namespace std;


//Function declarations
int AdminLogin();
int AnalystLogin();
int IntakeOfficerLogin();





int main(){
    int LoginChoise;

    //Map the login strategies
    unordered_map<int, function<int()>> loginStrategies = {
        {1, AdminLogin},
        {2, AnalystLogin},
        {3, IntakeOfficerLogin},

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

        /*
        if(LoginChoise  ==  1){
            if(AdminLogin() == 99) {
                system("cls");
                goto start;
            }
        }
        else if(LoginChoise == 2 ){

        }
        else if(LoginChoise == 3){

        }
        else{
            cout << "Choice must be from 1-3" <<endl;
            Sleep(2000);
            system("cls");
            goto start;
        }
        */
       
    }
  

}

int AdminLogin(){
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
    

    string loginUserInput, loginUserPass;
    cout << "Enter login id";
    cin >> loginUserInput;
    cout << "Enter password";
    cin >> loginUserPass;
    cout << "Verifying Credentials...\n";

    this_thread::sleep_for(chrono::seconds(2));

    ifstream adminfile("E:\Projects\Digital Evidence and Case Management System Sem02 (OOP)\Digital-Evidence-CMS\data\admin.txt", ios::app);
    if(!adminfile) return 99;
    
    string id, pass, num;
    //int num;
    Admin admin();
    while(getline(adminfile, num, '|' ) && getline(adminfile, id, '|') && getline(adminfile, pass, '|')) {
        if(!(id==loginUserInput && pass==loginUserPass)) {
            cout << "Wrong credentials\n";
            return 99;
        }

    }

    //Call verifying function with relevent user id and pass file which then return bool

    //if Not verified 
        //User can enter credentials three 3 only for preventing brute force attack then send back to login page
    /*
    if() {
        cout << "Wrong Credentials\n";
    }
    */
    //if verifieed
    /*
    if() {
        cout << "Logged In successful\n";
        //Update the log file
        //Transfer to Admin Rolls (Make Seprate functions for each roll)
    }
    */

    //Actual work such as authenticating user and pass first by reading admins info file

}

int AnalystLogin(){
    int choice;
    cout << "Enter\n";
    cout << "\t1. Enter Credentials\n";
    cout << "\t99. Back to Login page\n";
    cin >> choice;

    if(choice == 99) return choice;
    if(choice != 99 && choice != 1) {
        cout << "Invalid Choice\nBacking to Login page\n...";
        return choice;
    }
    
    string loginUserInput, loginUserPass;
    cout << "Enter login id";
    //input id
    cout << "Enter password";
    //input pass
    cout << "Verifying Credentials...\n";
    //Call verifying function with relevent user id and pass file which then return bool

    //if Not verified 
        //User can enter credentials three 3 only for preventing brute force attack then send back to login page
    /*
    if() {
        cout << "Wrong Credentials\n";
    }
    */
    //if verified
    /*
    if() {
        cout << "Logged In successful\n";
        //Update the log file
        //Transfer to Admin Rolls (Make Seprate functions for each roll)
    }
    */

    //Actual work such as authenticating user and pass first by reading admins info file

}

int IntakeOfficerLogin(){
    int choice;
    cout << "Enter\n";
    cout << "\t1. Enter Credentials\n";
    cout << "\t99. Back to Login page\n";
    cin >> choice;

    if(choice == 99) return choice;
    if(choice != 99 && choice != 1) {
        cout << "Invalid Choice\nBacking to Login page\n...";
        return choice;
    }
    
    string loginUserInput, loginUserPass;
    cout << "Enter login id";
    //input id
    cout << "Enter password";
    //input pass
    cout << "Verifying Credentials...\n";
    //Call verifying function with relevent user id and pass file which then return bool

    //if Not verified 
        //User can enter credentials three 3 only for preventing brute force attack then send back to login page
    /*
    if() {
        cout << "Wrong Credentials\n";
    }
    */
    //if verified
    /*
    if() {
        cout << "Logged In successful\n";
        //Update the log file
        //Transfer to Admin Rolls (Make Seprate functions for each roll)
    }
    */

    //Actual work such as authenticating user and pass first by reading admins info file

}
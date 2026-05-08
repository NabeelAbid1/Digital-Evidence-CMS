#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include <vector>
#include <unordered_map>  //Provides a hash map (key-value container)
#include <functional>     //Allows storing functions
#include <thread>         //For sleep (cross platform)
#include <chrono>         //Used with thread
#include <sstream>        //For stringstream

#include <Windows.h>

//classes headers
#include "User.h"
#include "Case.h"
#include "Logger.h"

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
                //this_thread::sleep_for(chrono::microseconds(2000));
                Sleep(2000);
                continue; //skip this iteration (go to start)
            }
            if(result == 10) {
                cout << "Logot successfully\n";
                //this_thread::sleep_for(chrono::microseconds(2000));  
                Sleep(2000);
                continue;
            }
        } else {
            cout <<"Choice must be from 1-3\n";
            Sleep(1500);
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
    cin.ignore();

    if(choice == 99) return choice;
    if(choice != 1) {
        cout << "Invalid Choice\n";
        return 99;
    }
    int num=0;
    Logger& log = Logger::get();//cleaner function call
    while(true){
        //Taking credentials
        string loginUserInput, loginUserPass;
        cout << "Enter login id : ";
        cin >> loginUserInput;
        cout << "Enter password : ";
        cin >> loginUserPass;
        cout << "Verifying Credentials...\n";

        this_thread::sleep_for(chrono::seconds(2));

        string adminfile = "data/admin.txt";
            
        //call login function
        CurrentLoggedAdmin = loginUser(loginUserInput, loginUserPass, adminfile);
        
        //if Not verified 
            //User can enter credentials three 3 only for preventing brute force attack then send back to login page
        if(CurrentLoggedAdmin!=nullptr) {
            break;
        }
        log.logLoginFailed(loginUserInput);
        if(num <=3){
            cout << "Try again\n";
            Sleep(1000);
            num++;
            system("cls");
            continue;
        }else if (num >= 3){
            cout <<"Login limit exceeds\n";
            Sleep(1000);
            system("cls");
            return 99;
        }  
    }
    log.logLogin(*CurrentLoggedAdmin);
    while(true){
        int CaseId;
        cout <<"Enter Case ID : ";
        cin >> CaseId;
        cout <<"Creating case...";
        Sleep(1200);

        string filename = "data/cases/CASE_" + to_string(CaseId) + ".txt";

        Case* loadedCase = nullptr;
        loadedCase = Case::loadCase(filename); 

        if(loadedCase == nullptr){
            cout << "Error! No case found\n";
            continue;
        }

        Sleep(2000);
        cout <<"Case is opened\n";
        log.logCaseOpened(*CurrentLoggedAdmin, *loadedCase);

        //Admin Jobs
        while(true){

            int choice;
            //create logg at every choice (do as you prefer)
            cout << "0. Display Case\n";
            cout<<"1. Check Evidences\n"; 
            cout<<"2. Close Case\n";
            cout<<"3. Lock Case\n";
            cout<<"4. Unlock Case\n";
            cout<<"5. Save Case\n";
            cin >> choice;
            cin.ignore();

            if(choice == 3){
                loadedCase->lockCase();
                log.logStatusChanged(*CurrentLoggedAdmin, *loadedCase, loadedCase->getStatus());
            }
            else if(choice == 4){
                loadedCase->unLockCase();
                log.logStatusChanged(*CurrentLoggedAdmin, *loadedCase, loadedCase->getStatus());
            }
            else if (choice == 2){
                if(loadedCase->getStatus()==3){
                    cout <<"Error! Case is already closed";
                    continue;
                }
                if(loadedCase->getStatus()==2){
                    int oldStatus = loadedCase->getStatus();
                    loadedCase->advanceStatus();
                    log.logStatusChanged(*CurrentLoggedAdmin, *loadedCase, oldStatus);
                }

            }else if (choice == 1){
                loadedCase->displayEvidence();
                int opt;
                cout << "Do you want to verify evidences?(1/0) : ";
                cin >> opt;
                cin.ignore();
                if(opt==1){ continue; }
                loadedCase->verifyEvidenceIntegrity();
                log.logEvidenceVerified(*CurrentLoggedAdmin, *loadedCase);
            }
            else if(choice ==0){
                loadedCase->displayCase();
            }
            else if (choice == 5) {
                loadedCase->saveCase();
                log.logCaseSaved(*CurrentLoggedAdmin, *loadedCase);
                delete loadedCase;
                cout <<"Case is saved\n";
                break;
            }
        }
    
    }
    //Design the Main admin menu where list all the functions of admin

    


    
   
}

int AnalystRoles(){
    User* CurrentLoggedAnalyst = nullptr;

    //Design login menu of admin
    int choice;
    cout << "Enter\n";
    cout << "\t1. Enter Credentials : ";
    cout << "\t99. Back to Login page : ";
    cin >> choice;

    if(choice == 99) return choice;
    if(choice != 99 && choice != 1) {
        cout << "Invalid Choice ";
        Sleep(1000);
        return 99;
    }
    int num=0;
    Logger& log = Logger::get();//cleaner function call
    while(true){
        //Taking credentials
        string loginUserInput, loginUserPass;
        cout << "Enter login id : ";
        cin >> loginUserInput;
        cout << "Enter password : ";
        cin >> loginUserPass;
        cout << "Verifying Credentials...\n";

        //this_thread::sleep_for(chrono::seconds(2));
        Sleep(2000);

        string analystfile = "data/analyst.txt";
            
        //call login function
        CurrentLoggedAnalyst = loginUser(loginUserInput, loginUserPass, analystfile);
        
        //if Not verified 
            //User can enter credentials three 3 only for preventing brute force attack then send back to login page
        if(CurrentLoggedAnalyst!=nullptr)  {
            cout <<"Login Successfully\n";
            Sleep(1500);
            break;
        }
        log.logLoginFailed(loginUserInput);
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
    log.logLogin(*CurrentLoggedAnalyst);
    //Design the Main admin menu where list all the functions of admin
    while(true){
        system("cls");
        int choice;
        cout << "**Intake Officer**"<<endl;                 
        cout << "\t1. Open Case from file" << endl;          
        cout << "\t10. Logout" << endl;       
       
        cout << "Enter choice... : "; 
        cin >> choice;
        cin.ignore();

        if(choice == 10){
            return choice;
        }
        if(choice!=1){
            cout <<"Invaild input\n";
            Sleep(1200);
            continue;
        }
        

        int CaseId;

        cout <<"Enter Case id : ";
        cin >> CaseId;
        cin.ignore();
        cout <<"Opening case...";

        string filename = "data/cases/CASE_" + to_string(CaseId) + ".txt";

        Case* loadedCase = nullptr;
        loadedCase = Case::loadCase(filename); 

        if(loadedCase == nullptr){
            cout << "Error! No case found\n";
            continue;
        }

        
        Sleep(2000);
        cout <<"Case is opened\n";
        log.logCaseOpened(*CurrentLoggedAnalyst, *loadedCase);

        //Analyst Jobs
        while(true){
            int choice;
            //create logg at every choice (do as you prefer)
            cout<<"1. Add Evidences\n"; 
            cout<<"2. Submitted Case\n";
            cout<<"3. Save Case\n";
            cin >> choice;
            cin.ignore();
            if(choice==2){
                if(loadedCase->getStatus()==2){
                    cout <<"Case is already submitted\n";
                    break;
                }
                int oldStatus = loadedCase->getStatus();
                loadedCase->advanceStatus();
                log.logStatusChanged(*CurrentLoggedAnalyst, *loadedCase, oldStatus);

            }
            else if(choice == 3){
                loadedCase->saveCase();
                log.logCaseSaved(*CurrentLoggedAnalyst, *loadedCase);
                delete loadedCase;
                cout<<"Cases is saved\n";
                break;
            }
            else if(choice == 1){
                //Adding Evidences
                
                while(true){
                    if(loadedCase->getIsLocked()){
                        cout <<"Error! Case is locked\nCannot add evidences";
                        break;
                    }
                    system("cls");
                    cout << "Enter evidences\n";

                    int opt;
                    cout <<"1. Video\n";
                    cout <<"2. Audio\n";
                    cout <<"3. Image\n";
                    cout <<"4. Nothing\n";
                    //cout <<"5. Nothing\n";

                    cin >> opt;
                    cin.ignore();

                    if(opt == 1 ){
                        cout << "Video Evidence\n...";

                        string duration, resolution, filename;
                        int id;
                        double fileSize;

                        cout <<"Enter id : ";
                        cin >> id;
                        cin.ignore();

                        cout <<"Enter filename : ";
                        getline(cin, filename);

                        cout <<"Enter size : ";
                        cin>>fileSize;
                        cin.ignore();

                        cout <<"Enter duration : ";
                        cin>> duration;
                        cin.ignore();

                        cout <<"Enter resolution : ";
                        getline(cin, resolution);

                        VideoEvidence* vidEv = new VideoEvidence(fileSize, id, filename, duration, resolution);
                        bool added = loadedCase->addEvidence(vidEv);
                        if(added) log.logEvidenceAdded(*CurrentLoggedAnalyst, *vidEv, *loadedCase);
                        else      log.logEvidenceBlocked(*CurrentLoggedAnalyst, *vidEv, *loadedCase);

                        cout <<"Video evidence added successfully\n";
                        Sleep(2000);
                        
                    }
                    else if(opt == 2 ){
                        string duration,  filename;
                        int id,sampleRateHz;
                        double fileSize;

                        cout <<"Enter id : ";
                        cin >> id;
                        cin.ignore();

                        cout <<"Enter filename : ";
                        getline(cin, filename);

                        cout <<"Enter size : ";
                        cin>>fileSize;
                        cin.ignore();

                        cout <<"Enter duration : ";
                        cin>> duration;
                        cin.ignore();

                        cout <<"Enter sampleRateHz : ";
                        cin >> sampleRateHz;

                        AudioEvidence* audEv = new AudioEvidence(fileSize, id, filename, duration, sampleRateHz);
                        bool added = loadedCase->addEvidence(audEv);
                        if(added) log.logEvidenceAdded(*CurrentLoggedAnalyst, *audEv, *loadedCase);
                        else      log.logEvidenceBlocked(*CurrentLoggedAnalyst, *audEv, *loadedCase);

                        cout <<"Audio evidence added successfully\n";
                        Sleep(2000);

                    }
                    else if(opt == 3 ){
                        cout << "Image Evidence\n...";

                        string captureDevice, resolution, filename, format;
                        int id;
                        double fileSize;

                        cout <<"Enter id ";
                        cin >> id;
                        cin.ignore();

                        cout <<"Enter filename ";
                        getline(cin, filename);

                        cout <<"Enter size ";
                        cin>>fileSize;
                        cin.ignore();

                        cout <<"Enter captureDevice ";
                        cin>> captureDevice;
                        cin.ignore();

                        cout <<"Enter resolution ";
                        getline(cin, resolution);

                        cout <<"Enter format ";
                        getline(cin, format);

                        ImageEvidence* imgEv = new ImageEvidence(fileSize, id, filename, resolution, format, captureDevice);
                        bool added = loadedCase->addEvidence(imgEv);
                        if(added) log.logEvidenceAdded(*CurrentLoggedAnalyst, *imgEv, *loadedCase);
                        else      log.logEvidenceBlocked(*CurrentLoggedAnalyst, *imgEv, *loadedCase);

                        cout <<"Image evidence added successfully\n";
                        Sleep(2000);
                        
                    }
                    
                    else if (opt == 4){
                        cout << "List of Evidences that are added\n";
                        loadedCase->displayEvidence();
                        cout <<"Case is submitted\n";
                        Sleep(1000);
                        
                        break;
                    }
                }
        
    
                

            }

        

        
    
        
        }
    }


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
    Logger& log = Logger::get(); //cleaner function call
    while(true){
        //Taking credentials
        string loginUserInput, loginUserPass;
        cout << "Enter login id : ";
        cin >> loginUserInput;
        cout << "Enter password : ";
        cin >> loginUserPass;
        cout << "Verifying Credentials...\n";

        //this_thread::sleep_for(chrono::seconds(2));
        Sleep(2000);

        string IntakeOfficerfile = "data/IntakeOfficer.txt";
            
        //call login function
        CurrentLoggedIntakeOfficer = loginUser(loginUserInput, loginUserPass, IntakeOfficerfile);
        
        //if Not verified 
            //User can enter credentials three 3 only for preventing brute force attack then send back to login page
        if(CurrentLoggedIntakeOfficer!=nullptr) {
            cout <<"Login Successfully\n";
            Sleep(1500);
            break;
        }
        log.logLoginFailed(loginUserInput);
        if(num <=3){
            cout << "Try again\n";
            num++;
            system("cls");
            continue;
        }else if (num >= 3){
            cout <<"Login limit exceeds\n";
            system("cls");
            return 99;
        }     
    }
    log.logLogin(*CurrentLoggedIntakeOfficer);

    //Design the Main admin menu where list all the functions of admin
    while(true){
        system("cls");
        int choice;
        cout << "**Intake Officer**"<<endl;                 
        cout << "\t1. Register Case" << endl;          
        cout << "\t10. Logout" << endl;       
       
        cout << "Enter choice... : "; 
        cin >> choice;
        cin.ignore(); //*

        if(choice == 10){
            return choice;
        }
        if(choice!=1){
            cout <<"Invaild input\n";
            Sleep(1200); //*
            continue;
        }
        string vicName, discription, registName, crimeType;
        int threatLevel;

        cout <<"Enter Victim name\n";
        getline(cin, vicName);

        cout <<"Enter discription\n";
        getline(cin, discription);

        cout <<"Enter crime type\n";
        getline(cin, crimeType);

        cout <<"Enter threat level (1-5)\n";
        cin >> threatLevel;
        cin.ignore();

        cout <<"Entr register by name\n";
        getline(cin, registName);

        Case c1(vicName, discription, crimeType, threatLevel, registName);

        cout <<"Creating case..";
        Sleep(3000);
        cout <<"Case is created\n";
        //log.logCaseCreated(*CurrentLoggedIntakeOfficer, c1);

        //Adding Evidences
        while(true){
            system("cls");
            cout << "Enter evidences\n";

            int opt;
            cout <<"1. Video\n";
            cout <<"2. Audio\n";
            cout <<"3. Image\n";
            cout <<"4. Nothing\n";

            cin >> opt; //*
            cin.ignore(); //*

            if(opt == 1 ){
                cout << "Video Evidence\n...";

                string duration, resolution, filename;
                int id;
                double fileSize;

                cout <<"Enter id : ";
                cin >> id;
                cin.ignore();

                cout <<"Enter filename : ";
                getline(cin, filename);

                cout <<"Enter size : ";
                cin>>fileSize;
                cin.ignore();

                cout <<"Enter duration : ";
                cin>> duration;
                cin.ignore();

                cout <<"Enter resolution : ";
                getline(cin, resolution);

                VideoEvidence* vidEv = new VideoEvidence(fileSize, id, filename, duration, resolution);
                bool added = c1.addEvidence(vidEv);
                if(added) log.logEvidenceAdded(*CurrentLoggedIntakeOfficer, *vidEv, c1);
                else      log.logEvidenceBlocked(*CurrentLoggedIntakeOfficer, *vidEv, c1);

                cout <<"Video evidence added successfully\n";
                Sleep(2000);
                
            }
            else if(opt == 2 ){
                string duration,  filename;
                int id,sampleRateHz;
                double fileSize;

                cout <<"Enter id : ";
                cin >> id;
                cin.ignore();

                cout <<"Enter filename : ";
                getline(cin, filename);

                cout <<"Enter size : ";
                cin>>fileSize;
                cin.ignore();

                cout <<"Enter duration : ";
                cin>> duration;
                cin.ignore();

                cout <<"Enter sampleRateHz : ";
                cin >> sampleRateHz;

                AudioEvidence* audEv = new AudioEvidence(fileSize, id, filename, duration, sampleRateHz);
                bool added = c1.addEvidence(audEv);
                if(added) log.logEvidenceAdded(*CurrentLoggedIntakeOfficer, *audEv, c1);
                else      log.logEvidenceBlocked(*CurrentLoggedIntakeOfficer, *audEv, c1);

                cout <<"Audio evidence added successfully\n";
                Sleep(2000);

            }
            else if(opt == 3 ){
                cout << "Image Evidence\n...";

                string captureDevice, resolution, filename, format;
                int id; 
                double size;

                cout <<"Enter id : ";
                cin >> id;
                cin.ignore();

                cout <<"Enter filename : ";
                getline(cin, filename);

                cout <<"Enter size : ";
                cin>>size;
                cin.ignore();

                cout <<"Enter captureDevice : ";
                cin>> captureDevice;
                cin.ignore();

                cout <<"Enter resolution : ";
                getline(cin, resolution);

                cout <<"Enter format : ";
                getline(cin, format);

                ImageEvidence* imgEv = new ImageEvidence(size, id, filename, resolution, format, captureDevice);
                bool added = c1.addEvidence(imgEv);
                if(added) log.logEvidenceAdded(*CurrentLoggedIntakeOfficer, *imgEv, c1);
                else      log.logEvidenceBlocked(*CurrentLoggedIntakeOfficer, *imgEv, c1);

                cout <<"Image evidence added successfully\n";
                Sleep(2000);
                
            }
            
            else if (opt == 4){
                cout << "List of Evidences that are added\n";
                c1.displayEvidence();
                Sleep(5000);
                cout <<"Case is submitted\n"; //This isnot showing stops at c1.displayEvidence()
                Sleep(2000);
                c1.saveCase();
                log.logCaseCreated(*CurrentLoggedIntakeOfficer, c1);
                log.logCaseSaved(*CurrentLoggedIntakeOfficer, c1);
                break;
            }
        }
        
        

    }
    

}

User* loginUser(string userName, string userPass, const string& filename){
    User* loggedInUser = nullptr;
    string line;
    ifstream file(filename);
    if(!file){
        cout << "Error!\tUnable to open file\n";
        Sleep(1200);
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
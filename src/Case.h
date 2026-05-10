#ifndef CASE_H
#define CASE_H

#include <iostream>
//#include <Windows.h>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "Evidence.h"
using namespace std;

class Case {
private:
    const int cId;
    string victim;
    string discription;
    string crimeType;
    int threatLevel; // 1-5
    int status; // 0=OPEN, 1=UNDER_INVESTIGATION, 2=SUBMITTED, 3=CLOSED
    bool isLocked; // true when SUBMITTED, blocks edits
    string registerBy;

    vector<Evidence*> evidenceList; //evidence storage
    static int totalCases;

public:
    Case(string v, string d, string crime, int level, string registerBy)
        : cId(++totalCases),victim(v),discription(d),crimeType(crime),threatLevel(level),status(1),isLocked(false), registerBy(registerBy) {
            //cout <<"\nCase Created\n"; //for testing -> it is creating
        }

    //After reading from file and initializing the Case object for that
    Case(int caseID, string vic, string dis, string crime, int level, string registerBy, bool status, bool isLocked): cId(caseID),victim(vic),discription(dis),crimeType(crime),threatLevel(level), registerBy(registerBy),status(status),isLocked(isLocked) {}

    ~Case() {}

    friend class Logger;

    bool addEvidence(Evidence* e) {
        if (isLocked) {
            cout << "[ERROR] Case #" << cId << " is SUBMITTED. Evidence cannot be added.\n";
            return false;
        }
        evidenceList.push_back(e);
        cout << "[LOG] Evidence " << e->getId() << " added to Case #" << cId << ".\n";
        return true;
    }

    void displayEvidence() const {
        if (evidenceList.empty()) {
            cout << "  No evidence logged for this case.\n";
            return;
        }
        cout << "\n  Evidence List for Case #" << cId << "\n";
        for (size_t i = 0; i < evidenceList.size(); i++) {
            cout << "\n  [" << i + 1 << "]";
            evidenceList[i]->display(); //resolved issues
        }
    }

    void verifyEvidenceIntegrity() const {
        cout << "\n  Integrity Check: Case #" << cId << "\n";
        for (const Evidence* e : evidenceList) {
            cout << "  Evidence " << e->getId()<< " | Hash: " << e->getHash() << " -> [VERIFIED]\n";
        }
    }

    int getEvidenceCount() const { return (int)evidenceList.size(); }

    bool setDiscription(string d) {
        if (isLocked) {
            cout << "[ERROR] Case #" << cId << " is locked. Cannot modify.\n";
            return false;
        }
        discription = d;
        return true;
    }

    void lockCase(){
        isLocked = true;
        cout << "[SYSTEM] Case #" << cId << " SUBMITTED and LOCKED. Record is now READ-ONLY.\n";
    }
    void unLockCase(){
        isLocked = false;
        cout << "[SYSTEM] Case #" << cId << " is Unlocked.\n";
    }

    bool setThreatLevel(int level) {
        if (isLocked) {
            cout << "[ERROR] Case #" << cId << " is locked. Cannot modify.\n";
            return false;
        }
        if (level < 1 || level > 5) {
            cout << "[ERROR] Threat level must be 1-5.\n";
            return false;
        }
        threatLevel = level;
        return true;
    }

    bool advanceStatus() {
        if (status == 0) {
            status = 1;
        } else if (status == 1) {
            status   = 2;
            isLocked = true;
            cout << "[SYSTEM] Case #" << cId << " SUBMITTED. Record is now READ-ONLY.\n";
        } else if (status == 2) {
            status = 3;
        } else {
            cout << "[INFO] Case #" << cId << " is already CLOSED.\n";
            return false;
        }
        return true;
    }


    //geters
    int        getId()          const { return cId; }
    string     getVictim()      const { return victim; }
    string     getDiscription() const { return discription; }
    string     getCrimeType()   const { return crimeType; }
    int        getThreatLevel() const { return threatLevel; }
    int        getStatus()      const { return status; }
    bool       getIsLocked()    const { return isLocked; }
    static int getTotalCases()        { return totalCases; }

    void displayCase() const {
        string statusStr;
        switch (status) { //to replace the num with string
            case 0: statusStr = "OPEN";                 break;
            case 1: statusStr = "UNDER INVESTIGATION";  break;
            case 2: statusStr = "SUBMITTED (Locked)";   break;
            case 3: statusStr = "CLOSED";               break;
            default: statusStr = "UNKNOWN";
        }

        cout << "\nCASE FILE #" << cId << "\n"
            << "  Victim      : " << victim      << "\n"
            << "  Crime Type  : " << crimeType   << "\n"
            << "  Description : " << discription << "\n"
            << "  Threat Level: (" << threatLevel << "/5)\n"
            << "  Status      : " << statusStr   << "\n"
            << "  Evidence    : " << evidenceList.size() << " item(s)\n";

        displayEvidence();
    }   

    string getCurrentTimestamp() const {
        time_t now = time(0);
        tm* timeinfo = localtime(&now);
        char buffer[80];
        strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", timeinfo);
        return string(buffer);
    }
    // a helper function for writing evidence in file
    static Evidence* deserializeEvidence(const string& line) {
        //write evidence records based on type of evidence
        stringstream ss(line);
        string type;
        getline(ss, type, '|');
        
        if (type == "VIDEO") {
            string eId, filename, fileSize, format, resolution, duration, hash;
            getline(ss, eId, '|');
            getline(ss, filename, '|');
            getline(ss, fileSize, '|');
            getline(ss, format, '|');
            getline(ss, resolution, '|');
            getline(ss, duration, '|');
            getline(ss, hash, '|');
            
            VideoEvidence* video = new VideoEvidence(
                eId, filename, stod(fileSize),hash, format, resolution, duration);
            //video->setHashValue(hash);
            return video;
        }
        else if (type == "AUDIO") {
            string eId, filename, fileSize, duration, hash, sampleRateHz, format;
            getline(ss, eId, '|');
            getline(ss, filename, '|');
            getline(ss, fileSize, '|');
            getline(ss, duration, '|');
            getline(ss, sampleRateHz, '|');
            getline(ss, format, '|');
            getline(ss, hash, '|');
            
            AudioEvidence* audio = new AudioEvidence(
                eId, filename, stod(fileSize), hash, duration, stoi(sampleRateHz), format);
           //disk->setHashValue(hash);
            return audio;
        }
        if (type == "IMAGE") {
            string eId, filename, fileSize, format, resolution, captureDevice, hash;
            getline(ss, eId, '|');
            getline(ss, filename, '|');
            getline(ss, fileSize, '|');
            getline(ss, format, '|');
            getline(ss, resolution, '|');
            getline(ss, captureDevice, '|');
            getline(ss, hash, '|');
            
            ImageEvidence* image = new ImageEvidence(
                eId, filename, stod(fileSize),hash, resolution, format, captureDevice);
            //video->setHashValue(hash);
            return image;
        }
    
        cerr << "ERROR: Unknown evidence type: " << type << endl;
        return nullptr;
    } 

    // void saveToFile() const {
    //     string fileName = "data/Cases/Case" + to_string(cId);
    //     ofstream file(fileName);
    //     if(!file){
    //         cout << "Error!\tUnable to save\n";
    //     }

    // }
    void saveCase() const;
    static Case* loadCase(const string& filename);
};

int Case::totalCases = 0;

void Case::saveCase() const {
    //filename will be based on caseId
    string filename = "data/cases/CASE_" + to_string(cId) + ".txt";
    cout << "Case is being save...";
    //Sleep(2000);
    
    ofstream outFile(filename, ios::trunc);
    
    if (!outFile.is_open()) {
        cerr << "ERROR: Could not open file for saving case: " << filename << endl;
        return;
    }
    
    // Write case metadata
    outFile << "CASE_ID:" << cId << endl;
    outFile << "VICTIM:" << victim << endl;
    outFile << "DESCRIPTION:" << discription << endl;
    outFile << "CRIME_TYPE:" << crimeType << endl;
    outFile << "THREAT_LEVEL:" << threatLevel << endl;
    outFile << "STATUS:" << status << endl;
    outFile << "IS_LOCKED:" << (isLocked ? "1" : "0") << endl;
    outFile << "REGISTERED_BY:" << registerBy << endl;
    outFile << "LAST_MODIFIED:" << getCurrentTimestamp() << endl;
    
    // Separator for evidence section
    outFile << "EVIDENCE:" << endl;
    
    //Write all evidence record
    for (int i = 0; i < evidenceList.size(); i++) {
        outFile << evidenceList[i]->serializeToString() << endl;
    }
    
    outFile << "END_EVIDENCE" << endl;
    
    outFile.close();
    cout << "Case " << cId << " is saved successfully.\n" << endl;
}

Case* Case::loadCase(const string& filename) {
    ifstream inFile(filename);
    
    if (!inFile.is_open()) {
        cerr << "ERROR: Could not open file for loading case: " << filename << endl;
        return nullptr;
    }
    
    //set default values
    string line;
    int caseId = -1;
    string victim = "";
    string description = "";
    string crimeType = "";
    int threatLevel = 0;
    int status = 0;
    bool isLocked = false;
    string registeredBy = "";
    vector<Evidence*> evidenceList;
    
    bool readingEvidence = false;
    
    //Read file line by line
    while (getline(inFile, line)) {
        // Skip empty lines
        if (line.empty()) continue;
        
        // Check for evidence section markers
        if (line == "EVIDENCE:") {
            readingEvidence = true;
            continue;
        }
    
        if (line == "END_EVIDENCE") {
            readingEvidence = false;
            continue;
        }
        
        //read evidence record via helper function
        if (readingEvidence) {
            Evidence* evidence = deserializeEvidence(line);
            if (evidence != nullptr) {
                evidenceList.push_back(evidence);
            }
            continue;
        }
        
        // Read case metadata using delimiter ':'
        size_t delimPos = line.find(':');
        if (delimPos == string::npos) continue;
        
        string key = line.substr(0, delimPos);
        string value = line.substr(delimPos + 1);
        
        if (key == "CASE_ID") {
            caseId = stoi(value);
        }
        else if (key == "VICTIM") {
            victim = value;
        }
        else if (key == "DESCRIPTION") {
            description = value;
        }
        else if (key == "CRIME_TYPE") {
            crimeType = value;
        }
        else if (key == "THREAT_LEVEL") {
            threatLevel = stoi(value);
        }
        else if (key == "STATUS") {
            status = stoi(value);
        }
        else if (key == "IS_LOCKED") {
            isLocked = (stoi(value) == 1);
        }
        else if (key == "REGISTERED_BY") {
            registeredBy = value;
        }
        // LAST_MODIFIED is only for reference, no need to store cz it will overwrite again in saveCase function
    }
    
    inFile.close();
    
    // Validate loaded data
    if (caseId == -1) {
        cerr << "ERROR: Invalid case file - missing CASE_ID" << endl;
        return nullptr;
    }
    
    // Create and return Case object
    Case* loadedCase = new Case(caseId, victim, description, crimeType, threatLevel, registeredBy, status,isLocked);

    loadedCase->evidenceList = evidenceList;
    
    cout << "Case " << caseId << " is loaded successfully." << endl;
    return loadedCase;
}
#endif // CASE_H

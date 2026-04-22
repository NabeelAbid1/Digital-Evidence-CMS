#ifndef CASE_H
#define CASE_H

#include <iostream>
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

    vector<Evidence*> evidenceList; //evidence storage
    static int totalCases;

public:
    Case(string v, string d, string crime, int level)
        : cId(totalCases++),victim(v),discription(d),crimeType(crime),threatLevel(level),status(1),isLocked(false) {}

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
            evidenceList[i]->display(); 
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
};

int Case::totalCases = 0;

#endif // CASE_H
#ifndef LOGGER_H
#define LOGGER_H

#include <fstream>
#include <string>
#include <ctime>
#include "User.h"
#include "Case.h"
#include "Evidence.h"
using namespace std;

class Logger {
private:
    ofstream logFile;
    static Logger* instance;

    friend class User;
    Logger() {
        logFile.open("data/system_log.txt", ios::app); // append to not overwrite on existing logs
    }

    static string timestamp() {
        time_t now = time(nullptr);
        char buf[20];
        strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&now));
        return string(buf);
    }

    void write(const string& message) {
        logFile << "[" << timestamp() << "] " << message << "\n";
        logFile.flush(); // write to disk immediately in case of crash
    }

public:
    static Logger& get() { // Sone instance whole program
        if (!instance) instance = new Logger();
        return *instance;
    }

    // Evidence log instances
    void logEvidenceAdded(const User& u, const Evidence& e, const Case& c) {
        write(u.userCode + " added " + e.eId + " to Case #" + to_string(c.cId));
    }

    void logEvidenceBlocked(const User& u, const Evidence& e, const Case& c) {
        write(u.userCode + " tried to add " + e.eId + " to Case #" + to_string(c.cId) + " [BLOCKED]");
    }

    // Case
    void logCaseCreated(const User& u, const Case& c) {
        write(u.userCode + " created Case #" + to_string(c.cId) + " (" + c.crimeType + ")");
    }

    void logCaseOpened(const User& u, const Case& c) {
        write(u.userCode + " opened Case #" + to_string(c.cId) + " (" + c.crimeType + ")");
    }

    void logCaseSaved(const User& u, const Case& c) {
        write(u.userCode + " saved Case #" + to_string(c.cId));
    }

    void logEvidenceVerified(const User& u, const Case& c) {
        write(u.userCode + " verified evidence integrity for Case #" + to_string(c.cId));
    }

    void logStatusChanged(const User& u, const Case& c, int oldStatus) {
        string statusNames[] = { "OPEN", "UNDER INVESTIGATION", "SUBMITTED", "CLOSED" };
        write(u.userCode + " changed Case #" + to_string(c.cId) + " status: "
              + statusNames[oldStatus] + " -> " + statusNames[c.status]);
    }

    void logDescriptionEdit(const User& u, const Case& c, const string& oldVal, const string& newVal) {
        write(u.userCode + " edited Case #" + to_string(c.cId) + " description: [" + oldVal + "] -> [" + newVal + "]");
    }

    void logThreatEdit(const User& u, const Case& c, int oldLevel, int newLevel) {
        write(u.userCode + " changed Case #" + to_string(c.cId) + " threat: "
              + to_string(oldLevel) + " -> " + to_string(newLevel));
    }

    // Auth
    void logLogin(const User& u) {
        write(u.userCode + " (" + u.name + ") logged in as " + u.userRole);
    }

    void logLoginFailed(const string& id) {
        write("Login failed for ID: [" + id + "]");
    }

    void logLogout(const User& u) {
        write(u.userCode + " logged out.");
    }
};

Logger* Logger::instance = nullptr;

#endif
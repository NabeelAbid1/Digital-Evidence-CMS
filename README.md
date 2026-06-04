# Digital Evidence & Case Management System (DECMS)

> **A C++ OOP-based forensic tool for securing the Chain of Custody in digital forensics investigations.**

---

## Project Overview

**DECMS** (Digital Evidence & Case Management System) is a specialized console-based application designed to simulate the workflow of a Digital Forensics Lab. It enables Law Enforcement Officers and Forensic Analysts to securely **register criminal cases**, **catalog digital evidence**, and maintain an auditable **Chain of Custody** to ensure evidence admissibility in court proceedings.

Unlike standard file managers, DECMS is purpose-built for **Data Integrity** and **Accountability**. It leverages Object-Oriented Programming (OOP) principles to ensure that evidence cannot be tampered with once submitted, and every access event is automatically logged with timestamps.

---

## Key Features

### 1. **Case Management**
* **Case Registration:** Create new criminal cases with unique Case IDs, victim information, crime descriptions, crime types, and threat level classification (1-5 scale).
* **Status Lifecycle Tracking:** Monitor case progression through multiple states:
  - `OPEN` → `UNDER_INVESTIGATION` → `SUBMITTED` → `CLOSED`
* **Secure Case Lockdown:** Once a case reaches "SUBMITTED" status, critical case details become **READ-ONLY** to prevent unauthorized tampering.
* **Evidence Integrity:** Verify hash values of all evidence linked to a case at any time.

### 2. **Advanced Evidence Handling**
* **Polymorphic Evidence Storage:** A unified system managing multiple evidence types with type-specific attributes:
  - **Video Evidence:** Tracks file format (MP4, AVI, etc.), resolution, and duration
  - **Audio Evidence:** Tracks sample rate (Hz), duration, and audio format (WAV, MP3, etc.)
  - **Image Evidence:** Tracks resolution, image format (JPG, PNG, etc.), and source capture device
* **Cryptographic Hash Integrity:** Each evidence file receives a unique SHA-256-style hash upon registration, serving as an immutable digital fingerprint.
* **File Metadata Tracking:** Maintains file size, name, and format information for every evidence item.

### 3. **Chain of Custody (Audit Trail)**
* **Automatic Event Logging:** Every significant action is automatically recorded:
  - User login/logout events
  - Case creation, status changes, and modifications
  - Evidence addition and integrity verification
  - Description and threat level edits
* **Comprehensive Logging:** Records **Who** (User Code), **What** (Action), **When** (Timestamp), and **Case Reference**.
* **System Log Persistence:** All audit records are written to `data/system_log.txt` with immediate disk flushing to prevent data loss.

### 4. **Role-Based Access Control (RBAC)**
* **Admin:** 
  - Full system access
  - Create and manage all cases
  - Reopen submitted cases for additional investigation
  - View complete system audit logs
  - Perform evidence integrity verification
  
* **Analyst:**
  - View assigned cases and associated evidence
  - Add evidence to open cases
  - Verify evidence integrity
  - Examine case details and evidence properties
  - Cannot modify locked/submitted cases
  
* **Intake Officer:**
  - Register and open new cases
  - Document initial case information
  - Cannot add or modify evidence
  - Cannot access other users' case logs

---

## Class Architecture & OOP Concepts

### Core Classes

#### **User Class Hierarchy**
```
User (Base Class)
├── Admin
├── Analyst  
└── IntakeOfficer
```

**User (Base Class)**
- `string name` – Full name of the user
- `string userID` – Unique identifier for authentication
- `string userCode` – Short code used in audit logs (e.g., "ADM001")
- `string userRole` – Role designation (Admin/Analyst/IntakeOfficer)
- `virtual void displayDetails()` – Polymorphic method for role-specific information display

**Admin (Derived Class)**
- Additional: `int caseVerified` – Count of cases verified by this admin

**Analyst (Derived Class)**
- Additional: `int caseInvestigated` – Count of cases investigated by this analyst

**IntakeOfficer (Derived Class)**
- Additional: `int caseOpened` – Count of cases registered by this officer

---

#### **Case Class**
- `const int cId` – Immutable case identifier (auto-incremented)
- `string victim` – Name/identifier of the victim
- `string discription` – Detailed case description
- `string crimeType` – Type of crime committed
- `int threatLevel` – Risk classification (1-5 scale)
- `int status` – Current case state (0=OPEN, 1=UNDER_INVESTIGATION, 2=SUBMITTED, 3=CLOSED)
- `bool isLocked` – Becomes true when case is submitted, preventing edits
- `string registerBy` – User code of the officer who registered the case
- `vector<Evidence*> evidenceList` – Collection of all evidence linked to this case
- `static int totalCases` – Class variable tracking total cases created

**Key Methods:**
- `bool addEvidence(Evidence* e)` – Add evidence (blocked if case is locked)
- `void displayEvidence()` – List all evidence for the case
- `void verifyEvidenceIntegrity()` – Verify hash values of all evidence
- `bool advanceStatus()` – Progress case through lifecycle
- `void lockCase() / void unLockCase()` – Manual lock control (Admin only)
- `void saveCase() / Case* loadCase()` – Persistence to disk

---

#### **Evidence Class Hierarchy (Abstract Base)**
```
Evidence (Abstract Base Class)
├── VideoEvidence
├── AudioEvidence
└── ImageEvidence
```

**Evidence (Abstract Base Class)**
- `string eId` – Evidence identifier (e.g., "1-E1", "2-E3")
- `const string hash` – Immutable SHA-256 style hash
- `string fileName` – Name of the evidence file
- `double sizeMB` – File size in megabytes
- `static int evidenceCount` – Total evidence items created
- `pure virtual void display()` – Must be implemented by derived classes
- `pure virtual string serializeToString()` – For file persistence

**VideoEvidence (Derived Class)**
- Additional fields: `string duration`, `string resolution`, `string format`
- Example: Videos with metadata like "1080p", "MP4", "2:30:45"

**AudioEvidence (Derived Class)**
- Additional fields: `string duration`, `int sampleRateHz`, `string format`
- Example: Audio files tracked with "44100 Hz", "WAV", "00:45:30"

**ImageEvidence (Derived Class)**
- Additional fields: `string resolution`, `string format`, `string captureDevice`
- Example: Photos from "Security Camera #2", "1920x1080", "JPG"

---

#### **Logger Class (Singleton Pattern)**
- `static Logger* instance` – Single instance throughout program lifetime
- `ofstream logFile` – File handle for `data/system_log.txt`
- `static Logger& get()` – Retrieve singleton instance
- Logging methods:
  - `void logCaseCreated(const User&, const Case&)` – Case registration
  - `void logCaseOpened(const User&, const Case&)` – Case access
  - `void logEvidenceAdded(const User&, const Evidence&, const Case&)` – Evidence addition
  - `void logEvidenceBlocked(const User&, const Evidence&, const Case&)` – Blocked attempts
  - `void logStatusChanged(const User&, const Case&, int oldStatus)` – Status transitions
  - `void logDescriptionEdit(const User&, const Case&, ...)` – Edits
  - `void logThreatEdit(const User&, const Case&, int oldLevel, int newLevel)` – Threat changes
  - `void logEvidenceVerified(const User&, const Case&)` – Integrity checks
  - `void logLogin(const User&) / void logLogout(const User&)` – Authentication

---

## OOP Principles Applied

| Principle | Implementation in DECMS |
| :--- | :--- |
| **Abstraction** | The `Evidence` class defines an abstract contract via `pure virtual` methods (`display()`, `serializeToString()`). Derived classes implement specific evidence handling without exposing internal details. |
| **Encapsulation** | Sensitive fields like `hash` (const), `cId` (const), and `isLocked` are declared `private`/`protected`. Access is controlled through getter/setter methods, preventing unauthorized modification. |
| **Inheritance** | `Admin`, `Analyst`, and `IntakeOfficer` inherit common user properties from `User` base class. `VideoEvidence`, `AudioEvidence`, and `ImageEvidence` inherit core evidence functionality from `Evidence`. |
| **Polymorphism** | `std::vector<Evidence*>` stores pointers to different evidence types. The `display()` method polymorphically calls the correct derived implementation. User roles override `displayDetails()` to show role-specific fields. |
| **Composition** | `Case` objects **contain** a `vector<Evidence*>` and a set of attributes. The Logger **contains** an ofstream for file I/O. Objects are composed to create complex behaviors. |
| **Singleton Pattern** | `Logger` uses the Singleton design pattern (`static Logger* instance`) to ensure only one logging stream exists throughout the program. |

---

## Technical Stack

* **Language:** C++ (C++11 or later recommended)
* **Paradigm:** Object-Oriented Programming (OOP) with inheritance and polymorphism
* **Data Persistence:** File I/O (`.txt` format) for case and system log storage
* **Interface:** Console-Based CLI (Command-Line Interface)
* **Platform:** Windows (uses `Windows.h` for cross-platform thread utilities)
* **Build System:** Direct compilation via C++ compiler (Visual Studio, MinGW, etc.)

---

## Project Directory Structure

```
Digital-Evidence-CMS/
├── src/
│   ├── main.cpp                 # Application entry point & main menu
│   ├── User.h                   # User base class and derived roles
│   ├── Case.h                   # Case management class
│   ├── Evidence.h               # Abstract Evidence class & derived types
│   ├── Logger.h                 # Singleton audit logging system
│   └── DECMS.exe                # Compiled executable
├── data/
│   ├── cases/                   # Case file storage (CASE_*.txt files)
│   └── system_log.txt           # Master audit trail
├── .gitignore                   # Git ignore configuration
└── README.md                    # This file
```

---

## Data Persistence

### Case Files
Cases are serialized to `data/cases/CASE_<ID>.txt` in the following format:


### Audit Logs
All actions are logged to `data/system_log.txt`:

---

## Workflow Example

1. **Intake Officer** logs in and registers a new case with suspect and crime details
2. **Case is created** with status `OPEN` and logged to audit trail
3. **Analyst** logs in and opens the case to investigate
4. **Analyst** catalogs video footage, audio recordings, and photos as evidence
5. Each evidence item receives a unique hash for integrity verification
6. **Admin** performs evidence integrity verification before case submission
7. **Case is advanced** to `SUBMITTED` status, automatically locking all details
8. If additional investigation is needed, **Admin** unlocks case for `UNDER_INVESTIGATION`
9. Final **case closure** records all actions in the audit trail

---

## Key Security Features

✓ **Immutable Hashes** – Evidence fingerprints cannot be altered after creation  
✓ **Case Locking** – Submitted cases become READ-ONLY to prevent tampering  
✓ **Audit Trails** – Every action is timestamped and logged with user identification  
✓ **Role-Based Access** – Users can only perform actions appropriate to their role  
✓ **Persistent Logs** – Audit records are immediately flushed to disk  
✓ **Status Progression** – Cases follow a defined lifecycle preventing invalid state transitions  

---


## Future Enhancements

- [ ] Database backend (SQLite/PostgreSQL) instead of file I/O
- [ ] GUI using Qt or wxWidgets
- [ ] Network support for multi-user simultaneous access
- [ ] Cryptographic hashing (actual SHA-256 implementation)
- [ ] User authentication with password hashing
- [ ] Advanced search and filtering capabilities
- [ ] PDF report generation
- [ ] Cloud backup for case files and logs

---

## License

This project is provided as-is for educational purposes in demonstrating OOP principles in C++.

---

## Author

**Sheraz Ali**  
GitHub: [@Sheraz-Ali403](https://github.com/Sheraz-Ali403)

---

## Notes

- The system uses simulated hash generation for demonstration purposes
- All timestamps follow the format: `YYYY-MM-DD HH:MM:SS`
- Case IDs and Evidence IDs are auto-incremented
- The application is single-user per session (login/logout cycle)
- File paths are relative to the working directory where DECMS.exe is executed

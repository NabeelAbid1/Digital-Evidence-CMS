#ifndef EVIDENCE_H
#define EVIDENCE_H
#pragma once

#include <iostream>
#include <string>
#include <cstdlib> //for rand(), srand()
#include <ctime> //for generating different seed for rand()
using namespace std;

//srand((unsigned)time(nullptr)); <---put this in int main()

class Evidence {
protected:
    string eId;
    const string hash; //unique identifier for the evidence, const so it can't be tampered with
    string fileName;
    double sizeMB;
    static int evidenceCount;
    
    static string generateHash(int cId) {
    return "SHA-" + to_string(rand() % 9000 + 1000)+ to_string(cId)+ to_string(evidenceCount)+ to_string(rand() % 9000 + 1000);
}

public:
    Evidence()
        : eId(" "), hash(" "), sizeMB(0){
        ++evidenceCount;
    }

    Evidence(double s, int cId, string fileName)
        : sizeMB(s),
          eId(to_string(cId) + "-E" + to_string(evidenceCount)),
          hash(generateHash(cId)), fileName(fileName) {
        ++evidenceCount;
        cout << "Ev created\n"; //for testing -> it is creating
    }

    //For getting existing data use by drived class
    Evidence(string id, string fileName, double fileSize, string hash): eId(id), fileName(fileName), sizeMB(fileSize), hash(hash) {}

    virtual string serializeToString() const = 0;   
    friend class Logger;

    //Getters
    string getId()          const { return eId; }
    string getHash()        const { return hash; }
    double getSize()        const { return sizeMB; }
    static int getCount()         { return evidenceCount; }

    virtual void display() const = 0; //virtual makes it late binding, calls the correct derived display()

    virtual ~Evidence() {}

public:
    //called inside derived display() to print shared fields
    void displayBase() const {
        cout << "\n  Evidence ID: " << eId        << "  Hash: " << hash        << "\n  Size: " << sizeMB << " MB"<< endl;
    }
};

int Evidence::evidenceCount = 0; //static initializer
class VideoEvidence : public Evidence {
private:
    string duration;
    string resolution;
    string format;

public:
    VideoEvidence(double s, int cId, string fileName,
                  string dur, string res, string fmt = "MP4")
        : Evidence(s, cId, fileName),duration(dur),resolution(res),format(fmt) {}

    VideoEvidence(string eid, string fileName, double fileSize, string hash, string format, string resolution, string duration): Evidence(eid, fileName, fileSize, hash), format(format), resolution(resolution), duration(duration) {}
    
    //Use for writing in file with Case 
    string serializeToString() const override {
        return "VIDEO|" + eId + "|" + fileName + "|" + to_string(sizeMB) 
               + "|" + format + "|" + resolution + "|" + duration + "|" + hash;
    }
    void display() const override { 
        cout << "\n  [VIDEO EVIDENCE]\n";
        displayBase();
        cout << "\n  Duration: " << duration<< "\n  Resolution: " << resolution<< "\n  Format: " << format     << "\n";
    }

    //getters
    string getDuration()   const { return duration; }
    string getResolution() const { return resolution; }
    string getFormat()     const { return format; }
};


class AudioEvidence : public Evidence {
private:
    string duration;
    int sampleRateHz;
    string format;

public:
    AudioEvidence(double s, int cId,  string fileName,
                  string dur, int sampleRate, string fmt = "WAV")
        : Evidence(s, cId, fileName),
          duration(dur),
          sampleRateHz(sampleRate),
          format(fmt) {}

    AudioEvidence(string eid, string fileName, double fileSize,string hash, string duration, int sampleRateHz, string format): Evidence(eid, fileName, fileSize, hash), duration(duration), sampleRateHz(sampleRateHz), format(format) {}

    //Use for writing in file with Case 
    string serializeToString() const override {
        return "AUDIO|" + eId + "|" + fileName + "|" + to_string(sizeMB)
               + "|" + duration + "|" + to_string(sampleRateHz) + "|" + format + "|" + hash;
    }

    void display() const override {
        cout << "\n  [AUDIO EVIDENCE]\n";
        displayBase();
        cout << "\n  Duration : " << duration     << "\n  Sample Rate: " << sampleRateHz << " Hz\n"<< "  Format: " << format<< "\n";
    }

        //getters
    string getDuration()   const { return duration; }
    int    getSampleRate() const { return sampleRateHz; }
    string getFormat()     const { return format; }
};


class ImageEvidence : public Evidence {
private:
    string resolution;
    string format;
    string captureDevice;

public:
    ImageEvidence(double s, int cId, string fileName,
                  string resolution, string format, string captureDevice)
        : Evidence(s, cId, fileName),
          resolution(resolution),
          format(format),
          captureDevice(captureDevice) {}

    ImageEvidence(string eid, string fileName, double fileSize,  string hash, string resolution, string format, string captureDevice): Evidence(eid, fileName, fileSize, hash), resolution(resolution), format(format), captureDevice(captureDevice) {}

    string serializeToString() const override {
        return "IMAGE|" + eId + "|" + fileName + "|" + to_string(sizeMB)
               + "|" + resolution + "|" + format + "|" + captureDevice + "|" + hash;
    }

    void display() const override {
        cout << "\n  [IMAGE EVIDENCE]\n";
        displayBase();
        cout << "\n  Resolution: " << resolution<< "\n  Format: " << format<< "\n  Capture Device: " << captureDevice << "\n";
    }

    //getters
    string getResolution()    const { return resolution; }
    string getFormat()        const { return format; }
    string getCaptureDevice() const { return captureDevice; }
};

#endif 
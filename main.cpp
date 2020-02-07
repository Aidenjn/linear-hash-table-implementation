#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>
#include <iterator>
#include <map>
#include <bitset>
#include <math.h>

using namespace std;

// Struct for holding records
struct record {
  string id; // Fixed at 8 bytes
  string name; // Max of 200 bytes
  string bio; // Max of 500 bytes
  string manager_id; // Fixed at 8 bytes
};

// Global Variables
vector<struct record> readRecords(string csvName); // Record list
map<string, vector<struct record>> lhTable; // Linear Hash Table
int buckets = 4; // amount of buckets currently in the index
int bitRep = 2; // amount of bits currently being used in the keys
int totalSpaceUsed = 0; // amount of bytes the records are taking up


// Function prototypes
void createIndex();
void lookupId(string id);
struct record* add_record(string id, string name, string bio, string manager_id); 
int binaryStrToDecimal(string str);
string idToBitString(string id);
string getLastIBits(string key, int i);
string flipBit(string key);
string toBinary(int n);
void reorderTable();
void insertRecord(struct record rec);
string getLineOfIndex(int lineNumber);
int countLinesOfIndex();
void changeLineOfIndex(int lineNumber, string newLine);
void addLineToIndex(string newLine);
int getIndexLineNumber(string index);
vector<struct record> readRecordsAtIndex(string index);
string idToIndexKey(string id);

int main(int argc, char* argv[])
{
    
    //readRecordsAtIndex("11");
    //cout << toBinary(3) << endl;
    //cout << binaryStrToDecimal("1001") << endl;
    //addLineToIndex("Whatttt");
    //changeLineOfIndex(2, "cars");

    if (argc < 2) {
        cerr << "Usage: " << endl;
        cerr << "For tuple lookup: " << argv[0] << " -L [ID]" << endl;
        cerr << "Tuple lookup example: " << argv[0] << " -L 4" << endl;
        cerr << "For index creation: " << argv[0] << " -C" << endl;
        return 1;
    }
    cout << "Correct number of arguments given" << endl;

    string mode = argv[1];

    if (mode ==  "-C") { // Create table mode
        createIndex();
    }
    else if (mode ==  "-L") { // Lookup mode 
        string id = argv[2];
        lookupId(id);
    }


idToBitString
    return 0;
}

void createIndex() {
    cout << "Creating index from csv file..." << endl;

    //struct record* newRecord = add_record("10000000", "Dan", "Likes dogs", "00000001");
    //cout << newRecord->bio << endl;


    vector<struct record> csvRecords = readRecords("Employee.csv");
    //cout << csvRecords[1].name << endl; 

    // Initialize lht with 1 bit
    lhTable.insert(pair<string, vector<struct record>>("0", vector<struct record>()));
    lhTable.insert(pair<string, vector<struct record>>("1", vector<struct record>()));

    for (int i=0; i < csvRecords.size(); i++) {
        insertRecord(csvRecords[i]);
    }

}

void lookupId(string id) {
    cout << "Looking up " << id << " in index data..." << endl;
    
    // Find amount of bits used to represent keys
    int keySize = 0;
    string firstLine = getLineOfIndex(1); // get first line of index
    char c;
    do {
        c = firstLine[keySize];
        keySize++;
    } while (c != '|');
    keySize--; // don't include delimeter in character count
    bitRep = keySize;

    // createkey from id
    string key = idToIndexKey(id);

    // Print info
    cout << "Key: " << key << endl;
    cout << "Line of EmployeeIndex: " << getIndexLineNumber(key) << endl;

    // lookup records with key
    vector<struct record> records = readRecordsAtIndex(key);
}

int getRecordSize(struct record r) {
    return r.id.length() + r.name.length() + r.bio.length() + r.manager_id.length();
}

struct record* add_record(string id, string name, string bio, string manager_id) {

    // Check to see if attributes are correct byte lengths
    // cout << "Length of id is " << id.length() << endl;
    if (id.length() != 8) {
        cerr << "ERROR: id value [" << id << "] not 8 bytes" << endl;
    }
    else if (name.length() < 1 || name.length() > 200) {
        cerr << "ERROR: name value [" << name << "] not within range of 1-200 bytes" << endl;
    }
    else if (bio.length() < 1 || bio.length() > 500) {
        cerr << "ERROR: bio value [" << bio << "] not within range of 1-200 bytes" << endl;
    }
    if (manager_id.length() != 8) {
        cerr << "ERROR: manager_id value [" << manager_id << "] not 8 bytes" << endl;
    }

    // Create new record
    struct record* returnRecord= new struct record;

    returnRecord->id = id;
    returnRecord->name = name;
    returnRecord->bio = bio;
    returnRecord->manager_id = manager_id;

    return returnRecord; 
}

vector<struct record> readRecords(string csvName){
    vector<struct record> records;
    vector<string> csvAttributes;
    string attribute; // For tokenizing csv lines
    ifstream infile(csvName);

    for(string line; getline(infile, line);)
    {
        //cout << line << endl;
        stringstream linestream(line);
        while (getline(linestream, attribute, ','))
        {
            //cout << attribute << endl;
            csvAttributes.push_back(attribute);
        }
        records.push_back(*add_record(csvAttributes[0], csvAttributes[1], csvAttributes[2], csvAttributes[3]));
        csvAttributes.clear();
    }
    return records;
}

vector<struct record> readRecordsAtIndex(string index){
    vector<struct record> records;
    vector<string> attributes;
    string record; // For tokenizing the line
    string attribute; // For tokenizing the line
    ifstream infile("EmployeeIndex");

    int linenumber = getIndexLineNumber(index);
    string line = getLineOfIndex(linenumber);
    line = line.substr(index.length() + 1, line.length()); // remove key from front of line

    stringstream linestream(line);

    while (getline(linestream, record, '|'))
    {
        stringstream recordstream(record);
        while (getline(recordstream, attribute, ',')) {
            cout << attribute << endl;
            attributes.push_back(attribute);
        }
        records.push_back(*add_record(attributes[0], attributes[1], attributes[2], attributes[3]));
        attributes.clear();
    }
    return records;
}

int binaryStrToDecimal(string str) {
    int num = 0;
    for (int i=0; i < str.length(); i++) {
        if (str[i] == '1') {
            num += pow(2, str.length() - i - 1);
        }
    }
    return num;
}

// Code taken from stack overflow user user3478487
// Reference link: https://stackoverflow.com/questions/22746429/c-decimal-to-binary-converting/22746526
string toBinary(int n)
{
    std::string r;
    while(n!=0) {r=(n%2==0 ?"0":"1")+r; n/=2;}
    return r;
}


string idToBitString(string id) {
    string bitstring = "";
    for (char& letter : id) {
        bitstring += bitset<8>(letter).to_string();
    }
    return bitstring;
}

string getLastIBits(string key, int i) {
    return key.substr(key.length()-i);
}

string flipBit(string key) {
    if (key[0] == '0') {
            key[0] = '1';
    }
    else if (key[0] == '1') {
        key[0] = '0';
    }
    /*
    for (int i=0; i < key.length(); i++) {
        if (key[i] == '0') {
            key[i] = '1';
        }
        else if (key[i] == '1') {
            key[i] = '0';
        }
    }
    */
    return key;
}

string idToIndexKey(string id) {
    string key = idToBitString(id);
    //cout << "bitstring: " << key << endl;
    key = getLastIBits(key, bitRep);

    // Flip bits if decimal rep is bigger than bucket amount
    if (binaryStrToDecimal(key) >= buckets) {
        key = flipBit(key);
    }

    return key;
}


void insertRecord(struct record rec) {
    string key = idToBitString(rec.id);
    key = getLastIBits(key, bitRep);
    
    // Flip bits if decimal rep is bigger than bucket amount
    if (binaryStrToDecimal(key) >= buckets) {
        key = flipBit(key);

    }

    cout << "ID: " << rec.id << ", key: " << key << endl;
    cout << "BinaryRep: " << binaryStrToDecimal(key) << endl;

    // Find bucket based on key
    vector <struct record> bucket = lhTable.find(key)->second;
    bucket.push_back(rec);

    // Add to space count
    totalSpaceUsed += getRecordSize(rec);

    // Trigger extension if overload occurs
    //if (totalSpaceUsed / 4096 > 0.8) {
    if (totalSpaceUsed / 40 > 0.8) {
       buckets++;
       if (buckets > pow(2, bitRep)) { // Extend key if necessary
           bitRep++;
           reorderTable();
       }

       // Add new bucket
       lhTable.insert(pair<string, vector<struct record>>(toBinary(buckets - 1), vector<struct record>()));
    }
}

void reorderTable() {
    map<string, vector<struct record>> iter;
    int count = 0;

    // Print old table
    /*
    for (itr = lhTable.begin(); itr != lhTable.end(); ++itr) { 
        cout << "BucketNumber: " << count << endl;
        cout << '\t' << itr->first 
             << '\t' << itr->second << '\n'; 
    } 

    // change keys 
    
    for (itr = lhTable.begin(); itr != lhTable.end(); ++itr) { 

    } 
    */
}

int countLinesOfIndex() {
    string line;
    int count = 0;
    ifstream indexfile {"EmployeeIndex"};

    while (getline(indexfile, line)) {
        count++;
    }
    return count;
}

string getLineOfIndex(int lineNumber) {
    string line;
    ifstream indexfile {"EmployeeIndex"};

    for (int i=0; i < lineNumber-1; i++) {
        if (!getline(indexfile, line)) {
            cerr << "ERROR: File does not extend to line " << lineNumber << endl;
        }
    }
    getline(indexfile, line);
    return line;
}

void changeLineOfIndex(int lineNumber, string newLine) {
    string line;
    ifstream indexfile {"EmployeeIndex"};
    ofstream newIndexFile {"NewEmployeeIndex"};

    for (int i=0; i < lineNumber-1; i++) {
        if (!getline(indexfile, line)) {
            cerr << "ERROR: File does not extend to line " << lineNumber << endl;
        }
        newIndexFile << line + "\n";
    }

    // insert new line
    newIndexFile << newLine + "\n";

    // discard old line
    getline(indexfile, line);

    // write remaining lines of old file
    while (getline(indexfile, line)) {
        newIndexFile << line + "\n";
    }

    // Delete old index
    if (remove("EmployeeIndex")!=0) {
        cerr << "ERROR: Could not delete old index file" << endl;
    }
    rename("NewEmployeeIndex", "EmployeeIndex");

    // Rename new index
}

void addLineToIndex(string newLine) {
    string line;
    ifstream indexfile {"EmployeeIndex"};
    ofstream newIndexFile {"NewEmployeeIndex"};

    // write lines of old file
    while (getline(indexfile, line)) {
        newIndexFile << line + "\n";
    }

    // insert new line
    newIndexFile << newLine + "\n";

    // Delete old index
    if (remove("EmployeeIndex")!=0) {
        cerr << "ERROR: Could not delete old index file" << endl;
    }
    rename("NewEmployeeIndex", "EmployeeIndex");

    // Rename new index
}

int getIndexLineNumber(string index) {
    string line;
    ifstream indexfile {"EmployeeIndex"};
    int lineNumber = 0;

    while (getline(indexfile, line)) {
        lineNumber++;
        if (index == line.substr(0, bitRep)) {
            return lineNumber;
        }
    }
    cerr << "ERROR: Could not find line number for index" << endl;
    return -5;
}



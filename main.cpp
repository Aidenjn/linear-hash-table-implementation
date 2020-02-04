#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <sstream>

using namespace std;

struct record {
  string id; // Fixed at 8 bytes
  string name; // Max of 200 bytes
  string bio; // Max of 500 bytes
  string manager_id; // Fixed at 8 bytes
};

struct record* add_record(string id, string name, string bio, string manager_id);
vector<struct record> readRecords(string csvName);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        cerr << "Usage: " << endl;
        cerr << "For tuple lookup: " << argv[0] << " -L [ID]" << endl;
        cerr << "Tuple lookup example: " << argv[0] << " -L 4" << endl;
        cerr << "For index creation: " << argv[0] << " -C" << endl;
        return 1;
    }
    cout << "Correct number of arguments given" << endl;


    //struct record* newRecord = add_record("10000000", "Dan", "Likes dogs", "00000001");
    //cout << newRecord->bio << endl;

    vector<struct record> csvRecords = readRecords("Employee.csv");

    cout << csvRecords[1].name << endl; 

    return 0;
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


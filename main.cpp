#include <iostream>
#include <string>

using namespace std;

struct record {
  string id; // Fixed at 8 bytes
  string name; // Max of 200 bytes
  string bio; // Max of 500 bytes
  string manager_id; // Fixed at 8 bytes
};

struct record* add_record(string id, string name, string bio, string manager_id);

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cerr << "Usage: " << std::endl;
        std::cerr << "For tuple lookup: " << argv[0] << " -L [ID]" << std::endl;
        std::cerr << "Tuple lookup example: " << argv[0] << " -L 4" << std::endl;
        std::cerr << "For index creation: " << argv[0] << " -C" << std::endl;
        return 1;
    }
    std::cout << "Correct number of arguments given" << std::endl;


    struct record* newRecord = add_record("10000000", "Dan", "Likes dogs", "00000001");

    std::cout << newRecord->bio << std::endl;

    return 0;
}

struct record* add_record(string id, string name, string bio, string manager_id) {

    // Check to see if attributes are correct byte lengths
    std::cout << "Length of id is " << id.length() << std::endl;
    if (id.length() != 8) {
        std::cerr << "ERROR: id value [" << id << "] not 8 bytes" << std::endl;
    }
    else if (name.length() < 1 || name.length() > 200) {
        std::cerr << "ERROR: name value [" << name << "] not within range of 1-200 bytes" << std::endl;
    }
    else if (bio.length() < 1 || bio.length() > 500) {
        std::cerr << "ERROR: bio value [" << bio << "] not within range of 1-200 bytes" << std::endl;
    }
    if (manager_id.length() != 8) {
        std::cerr << "ERROR: manager_id value [" << manager_id << "] not 8 bytes" << std::endl;
    }

    // Create new record
    struct record* returnRecord= new struct record;

    returnRecord->id = id;
    returnRecord->name = name;
    returnRecord->bio = bio;
    returnRecord->manager_id = manager_id;

    return returnRecord; 
}



#include <cstdlib>
#include <iostream>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "fileMang.h"

#define _SILENCE_EXPERIMENTAL_FILESYSTEM_DEPRECATION_WARNING
#define HAVE_STRUCT_TIMESPEC

#include <pthread.h>
#include <experimental/filesystem> // http://en.cppreference.com/w/cpp/experimental/fs

pthread_mutex_t mutex12 = PTHREAD_MUTEX_INITIALIZER;

using namespace std;
int totalFiles = 0;
int totalVectors = 100;
fileMang FM;

std::vector<std::string> get_filenames(std::experimental::filesystem::path path)
{
    namespace stdfs = std::experimental::filesystem;
    std::vector<std::string> filenames;
    const stdfs::directory_iterator end{};

    for (stdfs::directory_iterator iter{ path }; iter != end; ++iter)
    {
        if (stdfs::is_regular_file(*iter)) // comment out if all names (names of directories tc.) are required
        {
            filenames.push_back(iter->path().filename().string());
            //filenames.push_back(path.filename().string());
            totalFiles++;
        }
    }
    return filenames;
}

void mainRunFunction(string inputFromThread) {
	
	string flagCheck = "y";

    int command = stoi(inputFromThread.substr(0, inputFromThread.find("|")));
    string RemainingInput = inputFromThread.erase(0, inputFromThread.find("|") + 1);
    //cout << "command1-->" << command << endl;
	
    switch (command) {

        case 1: {
            string fileName = inputFromThread.substr(0, inputFromThread.find("|"));
            if (FM.fifFileExists(fileName) == 1) {
                cout << "The file '" << fileName << "' already prsent in the directory" << endl;
            }
            else {
                FM.fcreateFile(fileName);
            }
            break;
            
        }

        case 2: {
            string fileName = inputFromThread.substr(0, inputFromThread.find('|'));
            if (FM.fifFileExists(fileName)) {
                FM.fdeleteFile(fileName);
            }
            else {
                cout << "File with this name (" << fileName << ") is not available in the directory" << endl;
            }
            break;
        }

        case 3: {
            string fileName = inputFromThread.substr(0, inputFromThread.find("|"));
            inputFromThread = inputFromThread.erase(0, inputFromThread.find("|") + 1);
            if (FM.fifFileExists(fileName) == 1) {
                string opentype = inputFromThread.substr(0, inputFromThread.find("|"));         
                inputFromThread = inputFromThread.erase(0, inputFromThread.find("|") + 1);

                if (opentype == "r" || opentype == "w" || opentype == "a") {
                    cout << "here" << endl;
                    string text = inputFromThread.substr(0, inputFromThread.find("|"));
                    FM.fopenFile(fileName, opentype, text);
                }
                else if (opentype == "rf") {
                    int position = stoi(inputFromThread.substr(0, inputFromThread.find('|')));
                    inputFromThread = inputFromThread.erase(0, inputFromThread.find('|') + 1);

                    int length = stoi(inputFromThread.substr(0, inputFromThread.find('|')));
                    inputFromThread = inputFromThread.erase(0, inputFromThread.find('|') + 1);
                    FM.freadFrom(fileName, position, length);
                }
            }
            else {
                cout << "The file does (" << fileName << ") not exits in the directory:" << endl;
            }
            break;
        }

        case 4: {
            string sourcefilename, destinationfileName;
            sourcefilename = inputFromThread.substr(0, inputFromThread.find('|'));
            inputFromThread = inputFromThread.erase(0, inputFromThread.find('|') + 1);

            if (FM.fifFileExists(sourcefilename)) {
                destinationfileName = inputFromThread.substr(0, inputFromThread.find('|'));
                inputFromThread = inputFromThread.erase(0, inputFromThread.find('|') + 1);

                if (FM.fifFileExists(destinationfileName)) {
                    FM.fmoveFile(sourcefilename, destinationfileName);
                }
                else {
                    cout << "The target File does not exist: " << destinationfileName << endl;
                }
            }
            else {
                cout << "The source File does not exist: " << sourcefilename << endl;
            }

            break;
        }

        case 5: {
            FM.fprintToCheck();
            break;
        }

        case 6: {
            FM.fprintDeleteVector();
            break;
        }
        default:
            cout << "Invalid input" << endl;
            break;
    }
}

void* threadRoutine(void* arg) {

    string fNameT = *static_cast<string*>(arg);
    ifstream MyReadFile("./users/" + fNameT);
    string myText;

    //cout << "FileName --> " << fNameT << endl;
    while (getline(MyReadFile, myText)) {
		mainRunFunction(myText);
		//cout << myText << "---- by --->" << fNameT << endl;
    }
    return NULL;
}


int main() {

    vector<string>files;
    files = get_filenames("./users");
    pthread_t* threads = new pthread_t[files.size()];

    for (int i = 0; i < files.size(); i++) {
        cout << "Thread[" << i + 1 << "] is created" << endl;
        //string fName = files[i];
        pthread_create(&threads[i], NULL, threadRoutine, &files[i]);
    }

    for (int i = 0; i < files.size(); i++) {
        pthread_join(threads[i], NULL);
    }
    cout << endl;
    cout << "<-------THREAD END----------->" << endl;
    cout << endl;
    FM.fprintToCheck();
    cout << endl;
    

	return 0;

}
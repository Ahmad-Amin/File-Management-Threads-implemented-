#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class fileMang {

private:

	typedef struct file {
		std::string content;
		std::string name;
		int sl;
		int fl;
		bool filled = false;
		file* next;
	}*filePtr;

	typedef struct dataNode {
		std::string fileName;
		int fileSize = -1;
		file* pointToFile;
	};

	filePtr Filehead;
	filePtr Filecurr;

public:

	int count = 1;
	int startingMemLocation = 0;

	fileMang();

	std::vector<dataNode*> dataVector;
	std::vector<int> deleteInfoVector;
	void fcreateFile(std::string name);
	void fopenFile(std::string name, std::string mode, std::string text);
	void freadFrom(std::string fileName, int form, int size);
	void fdeleteFile(std::string fileName);
	void fmoveFile(std::string sName, std::string dName);
	void fprintDeleteVector();
	int fifFileExists(std::string fileName);

	void fprintToCheck();


	//void fwriteatt(std::string sName, int location);


};

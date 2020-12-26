#include <cstdlib>
#include <iostream>
#include "fileMang.h"
#include <iterator>
#include<vector>;

#define HAVE_STRUCT_TIMESPEC

#include <pthread.h>

using namespace std;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;


fileMang::fileMang() {
	Filehead = NULL;
	Filecurr = NULL;
}


void fileMang::fcreateFile(std::string name) {


	vector<dataNode*>::iterator FCF;

	if (dataVector.size() >= 1) {
		for (FCF = dataVector.begin(); FCF < dataVector.end(); FCF++) {
			//			cout << (*FCF)->fileSize << endl;
			if ((*FCF)->fileSize != 0) {
				cout << "Not Enterd";

				dataNode* DN = new dataNode;
				DN->fileName = name;
				DN->pointToFile = NULL;

				dataVector.push_back(DN);

				cout << "The file '" << name << "' created successfully" << endl;

				
				break;
			}
			if ((*FCF)->fileSize == 0) {
				cout << "Entered" << endl;
				(*FCF)->fileName = name;
				cout << "The file '" << name << "' created successfully" << endl;
			}
		}
	}
	else {
		dataNode* DN = new dataNode;
		DN->fileName = name;
		DN->pointToFile = NULL;

		dataVector.push_back(DN);

		cout << "The file '" << name << "' created successfully" << endl;


	}

}

void fileMang::freadFrom(std::string fileName, int from, int size) {
	cout << "Read-From mode" << endl;
	std::string content = "";
	vector<dataNode*>::iterator ptr;
	for (ptr = dataVector.begin(); ptr < dataVector.end(); ptr++) {
		if ((*ptr)->fileName == fileName) {
			if ((*ptr)->pointToFile != NULL) {
				//cout << "enterd" << endl;
				Filecurr = (*ptr)->pointToFile;
				while (Filecurr != NULL) {
					content = content + Filecurr->content;
					Filecurr = Filecurr->next;
				}
			}
		}
	}

	content = content.substr(from, size);
	cout << content << endl;
}

void fileMang::fopenFile(std::string name, std::string mode, std::string text) {

	if (mode == "a") {

		//pthread_mutex_lock(&mutex);

		cout << "Append-Mode\nStart-writing: " << endl;
		vector<dataNode*>::iterator ptr;

		std::string newContent;
		getline(cin, newContent);

		for (ptr = dataVector.begin(); ptr < dataVector.end(); ptr++) {

			if ((*ptr)->fileName == name) {
				if ((*ptr)->pointToFile != NULL) {
					//cout << "enterd" << endl;
					Filecurr = (*ptr)->pointToFile;
					while (Filecurr->next != NULL) {
						Filecurr = Filecurr->next;
					}
					file* newHead = new file;
					file* newCurr = new file;

					newHead = NULL;
					newCurr = NULL;

					newHead = Filecurr;
					std::string alreadyContent = Filecurr->content;
					newContent = alreadyContent + newContent;
					int i = 0;
					while (newContent.size() != 0) {

						std::string contentToAdd = newContent.substr(0, 100);

						if (i == 1) {
							file* fd = new file;
							fd->name = name;
							fd->content = contentToAdd;

							if (!deleteInfoVector.empty()) {
								fd->sl = deleteInfoVector.at(0);

								deleteInfoVector.erase(deleteInfoVector.begin());
								fd->fl = fd->sl + contentToAdd.size();
							}
							else {
								fd->sl = startingMemLocation;
								fd->fl = startingMemLocation + contentToAdd.size();
							}

							fd->filled = true;
							fd->next = NULL;

							if (newHead != NULL) {
								newCurr = newHead;
								while (newCurr->next != NULL) {
									newCurr = newCurr->next;
								}
								newCurr->next = fd;
							}
							else {
								newHead = fd;
							}
							newContent = newContent.erase(0, 100);
							if (contentToAdd.size() <= 100) {
								startingMemLocation = startingMemLocation + 100;
							}
							else {
								startingMemLocation = startingMemLocation + contentToAdd.size();
							}
						}
						else {
							Filecurr->content = contentToAdd;
							newContent = newContent.erase(0, 100);
							Filecurr->fl = Filecurr->sl + 100;
							i++;
						}

					}

					newHead = NULL;
					newCurr = NULL;
				}
			}

			
			cout << endl;
		}
		//pthread_mutex_unlock(&mutex);

	}

	if (mode == "w") {

		//pthread_mutex_lock(&mutex);

		Filehead = NULL;
		Filecurr = NULL;

		cout << "Write-Mode\nStart-Writing: " << endl;

		std::string content = text;


		vector<dataNode*>::iterator ptr;
		for (ptr = dataVector.begin(); ptr < dataVector.end(); ptr++) {
			if ((*ptr)->fileName == name) {

				int count = 0;

				(*ptr)->fileSize = content.size();

				while ((content.size() != 0)) {


					file* fp = new file;
					fp->name = name;
					fp->next = NULL;

					std::string ContentToAdd = content.substr(0, 100);
					fp->content = ContentToAdd;
					content = content.erase(0, 100);

					if (!deleteInfoVector.empty()) {
						fp->sl = deleteInfoVector.at(0);

						deleteInfoVector.erase(deleteInfoVector.begin());
						fp->fl = fp->sl + ContentToAdd.size();
					}
					else {
						fp->sl = startingMemLocation;
						fp->fl = startingMemLocation + ContentToAdd.size();
						if (ContentToAdd.size() <= 100) {
							startingMemLocation = startingMemLocation + 100;
						}
						else {
							startingMemLocation = startingMemLocation + ContentToAdd.size();
						}
					}

					fp->next = NULL;
					fp->filled = true;

					if (Filehead != NULL) {
						Filecurr = Filehead;
						while (Filecurr->next != NULL) {
							Filecurr = Filecurr->next;
						}
						Filecurr->next = fp;
					}
					else {
						Filehead = fp;
						(*ptr)->pointToFile = fp;
						cout << fp;
					}

				}
				
				//break;

			}
		}
		//pthread_mutex_unlock(&mutex);


		cout << "done" << endl;

	}

	if (mode == "r") {
		std::string content = "";
		cout << "read-mode" << endl;

		//pthread_mutex_lock(&mutex);
		vector<dataNode*>::iterator ptr;
		for (ptr = dataVector.begin(); ptr < dataVector.end(); ptr++) {
			if ((*ptr)->fileName == name) {
				if ((*ptr)->pointToFile != NULL) {
					//cout << "enterd" << endl;
					Filecurr = (*ptr)->pointToFile;
					while (Filecurr != NULL) {
						content = content + Filecurr->content;
						Filecurr = Filecurr->next;
					}
				}
			}
		}
		cout << content << endl;

		//pthread_mutex_lock(&mutex);

	}
}

void fileMang::fmoveFile(std::string sName, std::string dName) {
	
	vector<dataNode*>::iterator ptr;
	cout << "Source File-Name: " << sName << endl;
	cout << "Destination File-Name: " << dName << endl;
	string sContent = "";
	// Coding for the source File:
	for (ptr = dataVector.begin(); ptr < dataVector.end(); ptr++) {
		if ((*ptr)->fileName == sName) {
			file* fpcurr = new file;
			//file* fptemp = new file;
			fpcurr = (*ptr)->pointToFile;
			(*ptr)->pointToFile = NULL;
			while (fpcurr != NULL) {
				deleteInfoVector.push_back(fpcurr->sl);
				sContent = sContent + fpcurr->content;
				fpcurr = fpcurr->next;
			}
			break;
		}
	}

	for (ptr = dataVector.begin(); ptr < dataVector.end(); ptr++) {
		int count = 0;
		if ((*ptr)->fileName == dName) {
			file* fpcurr = new file;
			file* fptemp = new file;
			fpcurr = (*ptr)->pointToFile;

			cout << "Data To add to the file: ";
			cout << sContent << endl;
			while (fpcurr != NULL) {
				
				if (sContent.size() == 0) {
					count++;
					if (count == 1) {
						fptemp = fpcurr;
					}
					deleteInfoVector.push_back(fpcurr->sl);
				}

				fpcurr->content = sContent.substr(0, 100);
				sContent = sContent.erase(0, 100);
				// if the size of the destination file is More than the source file
				fpcurr = fpcurr->next;
			}
			fptemp->next = NULL;
			// if the content in the source file is More than the content in the destination file
			
			if (sContent.size() > 0) {
				Filecurr = NULL;
				Filehead = NULL;
				if ((*ptr)->pointToFile != NULL) {
					Filecurr = (*ptr)->pointToFile;
					while (Filecurr->next != NULL) {
						Filecurr = Filecurr->next;
					}
					file* newHead = new file;
					file* newCurr = new file;

					newHead = NULL;
					newCurr = NULL;

					newHead = Filecurr;
					string newContent = sContent;

					int i = 0;
					while (newContent.size() != 0) {

						std::string contentToAdd = newContent.substr(0, 100);

						
						file* fd = new file;
						fd->name = dName;
						fd->content = contentToAdd;

						if (!deleteInfoVector.empty()) {
							fd->sl = deleteInfoVector.at(0);

							deleteInfoVector.erase(deleteInfoVector.begin());
							fd->fl = fd->sl + contentToAdd.size();
						}
						else {
							fd->sl = startingMemLocation;
							fd->fl = startingMemLocation + contentToAdd.size();
						}

						fd->filled = true;
						fd->next = NULL;

						if (newHead != NULL) {
							newCurr = newHead;
							while (newCurr->next != NULL) {
								newCurr = newCurr->next;
							}
							newCurr->next = fd;
						}
						else {
							newHead = fd;
						}

						if (contentToAdd.size() <= 100) {
							startingMemLocation = startingMemLocation + 100;
						}
						else {
							startingMemLocation = startingMemLocation + contentToAdd.size();
						}

						newContent = newContent.erase(0, 100);
					}

					newHead = NULL;
					newCurr = NULL;
				}
			}
		}
	}
}

void fileMang::fdeleteFile(std::string name) {
	vector<dataNode*>::iterator ptr;
	for (ptr = dataVector.begin(); ptr < dataVector.end(); ptr++) {
		if ((*ptr)->fileName == name) {
			//dataVector.erase(ptr);
			if ((*ptr)->pointToFile != NULL) {
				Filecurr = (*ptr)->pointToFile;
				while (Filecurr != NULL) {
					cout << Filecurr->sl << "-" << Filecurr->fl << " | " << Filecurr->content << endl;
					deleteInfoVector.push_back(Filecurr->sl);
					Filecurr = Filecurr->next;
				}
			}
			dataVector.erase(ptr);
			cout << "File Deleted Successfully: " << name << endl;
			break;
		}
	}
}

void fileMang::fprintToCheck() {

	ofstream myFile("MemData.dat");
	vector<dataNode*>::iterator ptr;


	for (ptr = dataVector.begin(); ptr < dataVector.end(); ptr++) {
		file* Filecurr1 = new file;
		if ((*ptr)->pointToFile != NULL) {

			Filecurr1 = (*ptr)->pointToFile;
			//myFile << "FILNAME---> " << Filecurr1->name << endl;
			cout << "FILNAME---> " << Filecurr1->name << endl;
			while (Filecurr1 != NULL) {
				cout << Filecurr1->sl << "-" << Filecurr1->fl << " | " << Filecurr1->content << endl;
				//myFile << Filecurr1->sl << "-" << Filecurr1->fl << " | " << Filecurr1->content << endl;
				Filecurr1 = Filecurr1->next;
			}
			

		}
		cout << endl;
		//myFile << endl;
	}
	myFile.close();
}

void fileMang::fprintDeleteVector() {
	for (int i = 0; i < deleteInfoVector.size(); i++) {
		cout << deleteInfoVector.at(i) << " ";
	}
	cout << endl;
	cout << "Starting->" << startingMemLocation << endl;
}

int fileMang::fifFileExists(string fileName) {

	//pthread_mutex_lock(&mutex);
	vector<dataNode*>::iterator ptr;
	for (ptr = dataVector.begin(); ptr < dataVector.end(); ptr++) {
		if ((*ptr)->fileName == fileName) {
			return 1;
		}
	}
	//pthread_mutex_unlock(&mutex);
}



//void fileMang::fwriteatt(std::string name, int location) {
//	vector<dataNode*>::iterator ptr;
//	file* tail = new file;
//	string newContent, oldContent, oldContent2;
//	for (ptr = dataVector.begin(); ptr < dataVector.end(); ptr++) {
//		if ((*ptr)->fileName == name) {
//			cout << "Enter the text to write (at location): ";
//			cin >> newContent;
//			if ((*ptr)->pointToFile != NULL) {
//				//cout << "enterd" << endl;
//				Filecurr = (*ptr)->pointToFile;
//				while (Filecurr != NULL) {					
//					if (Filecurr->sl <= location) {
//						tail = Filecurr;
//					}
//					oldContent = oldContent + Filecurr->content;
//					Filecurr = Filecurr->next;
//				}
//			}
//			tail->next = NULL;
//		}
//	}
//
//	oldContent2 = oldContent.substr(location);
//	newContent = newContent + oldContent2;	
//}
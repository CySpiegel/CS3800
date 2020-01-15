#include <iostream>
#include <map>
#include <vector>
#include <string>
#include <sstream>
#include <ctime>



using std::cout;
using std::endl;
using std::cin;
#ifndef COMPUTER
#define COMPUTER


class files
{
    public:
		//stores file creation date
		std::time_t lastModified = std::time(nullptr);

        //what directory does it belong to
        std::string path;

		//user facing folder name
        std::string fileName;

		//folder permissions
        int userPermission;
        int groupPermission;
        int publicPermission;
		int size;

		files(std::string fileName);


};

class folder
{
    public:
		std::time_t lastModified = std::time(nullptr);
		std::string parrentFolderKey;
        std::string path;
		std::string folderName;
		int userPermission;
		int groupPermission;
		int publicPermission;
		int size;


        //Vector to hold all fo
        std::vector<files> dirFiles = {};

        std::vector<std::string> dirFolders = {}; 

        //Folder Default constructor user for root
        folder(std::string name);

		folder(std::string name, std::string parrentPath, std::string folderPath);
};

/*
    Main driver class for the terminal.
*/
class computer
{
		folder* currentLocation = nullptr;
        std::map<std::string, folder> directory;
    public:

        /*
            default constructure
            handels initalization 
        */
        computer();

		// starts computer and servs as a wraper for terminal
		void boot();

		std::string user;
		std::string group;

	private:
        /*
            creates new folders in the directory
        */
        void mkdir(const std::string newFolderName);

		//remove a directory
		void rmdir(const std::string folderToRemove);

		void rm(const std::string fileToRemove);

		//creates file in directory
		void touch(const std::string fileName);

		void ls(std::string arg = "");

		// return current working directory
		std::string pwd();

		//changes the current directory to another directory
		void cd(std::string argument);

		//permissions
		void chmod(std::string folderOrFile, std::string permissionNumber);

		//print folder permissions
		std::string printPermissionsFolders(std::string folder);

		//print file permissoin
		std::string printPermissionsFiles(const files & folder);

		//top level handler for output, everything left of $
		void terminal();

		//Input parsing turns incomming getline to vector of strings
		bool parseUserInput(std::string userInput);

		//takes vector of strings as command inputs
		bool terminalCommandProcessor(std::string cmd, std::vector<std::string> arguments);
};
#endif
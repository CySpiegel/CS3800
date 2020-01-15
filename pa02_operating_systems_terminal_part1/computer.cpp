#include "computer.h"


files::files(std::string newFileName)
{
	fileName = newFileName;
	// settings permissions on file
	userPermission = 6;
	groupPermission = 0;
	publicPermission = 0;
	size = 10;
	return;
}

folder::folder(std::string name)
{

	parrentFolderKey = name;
	// location in master map
	path = name;
	folderName = name;
	// settings permissions on folder
	userPermission = 6;
	groupPermission = 0;
	publicPermission = 0;
	//file size in bytes
	size = 0;
}
;

folder::folder(std::string name, std::string parrentPath, std::string folderPath)
{
	parrentFolderKey = parrentPath;
	path = folderPath;
	folderName = name;
	// settings permissions on folder
	userPermission = 6;
	groupPermission = 0;
	publicPermission = 0;
	//file size in bytes
	size = 0;

}
;


//------------------------------------------------------------------------------

computer::computer()
{
	//user and group
	user = "JohnyCache";
	group = "mst_user";

    //make root home folder
	folder* newFolder = nullptr;
	newFolder = new folder("root/");
	directory.insert(std::make_pair("root/", *newFolder));
	delete newFolder;
	//End of creation of root directory

	//Point current location to root folder
	currentLocation = &directory.find("root/")->second;

	//adding a new folder inside of root folder
	std::string newFolderName = "home";
	mkdir(newFolderName);

	currentLocation = &directory.find("root/home/")->second;

	newFolderName = "user";
	mkdir(newFolderName);
	touch("morejunk.txt");

	currentLocation = &directory.find("root/")->second;

	touch("Junk.txt");

	//Remove this before submitting this is ls functionality


}
void computer::boot()
{
	// starts terminal emulation
	terminal();
}
;


void computer::mkdir(const std::string newFolderName)
{

	std::string currentFolderPath = currentLocation->path;
	std::string newFolderPath = currentFolderPath + newFolderName + "/";

	if (directory.count(newFolderPath) == 0)
	{
		// creating new folder
		folder* newFolder = nullptr;
		newFolder = new folder(newFolderName, currentFolderPath, newFolderPath);

		//inserting new folder into master directory
		directory.insert(std::make_pair(newFolderPath, *newFolder));
		delete newFolder;

		//adding new folder to list of folders in this directory
		currentLocation->dirFolders.push_back(newFolderName);

	}
	else
	{
		cout << "Directory " << newFolderName << " already exists" << endl;
	}
	return;
}

void computer::rmdir(const std::string folderToRemove)
{
	//locating in master directory map
	std::string currentFolderPath = currentLocation->path;
	std::string removingFolder = currentFolderPath + folderToRemove + "/";
	bool foundDirectory = false;


	int count = 0;
	for (auto i = currentLocation->dirFolders.begin(); i != currentLocation->dirFolders.end(); ++i)
	{
		if (folderToRemove == *i)
		{
			////folder found in current working directory
			if (directory.find(removingFolder)->second.dirFolders.size() == 0 && directory.find(removingFolder)->second.dirFiles.size() == 0)
			{
				*currentLocation->dirFolders.erase(currentLocation->dirFolders.begin() + count);
				directory.erase(removingFolder);
				foundDirectory = true;
				break;
			}
			else
			{
				cout << "Cannot remove directory " << folderToRemove << " directory must be empty" << endl;
			}
		}
		count++;
	}

	if (foundDirectory == false)
	{
		cout << "Directory " + folderToRemove + " not found" << endl;
	}
}

void computer::rm(const std::string fileToRemove)
{
	int count = 0;
	for (auto i = currentLocation->dirFiles.begin(); i != currentLocation->dirFiles.end(); ++i)
	{
		
		if (fileToRemove == i->fileName)
		{
			//cout << "file found" <<  currentLocation->dirFiles.begin()->fileName <<endl;
			currentLocation->dirFiles.erase(currentLocation->dirFiles.begin() + count);
			//cout << "Not function: Dr Taylor ran off with this function" << endl;
			break;
		}
		count++;
	}
}

void computer::touch(const std::string fileName)
{

	bool fileFound = false;

	for (auto i = currentLocation->dirFiles.begin(); i != currentLocation->dirFiles.end(); ++i)
	{
		cout << i->fileName << ' ';
		if (i->fileName == fileName)
		{
			fileFound = true;
			//update timestamp
			i->lastModified = std::time(nullptr);
			cout << endl;
			break;
		}

	}



	// adding new file if it does not exist
	if (fileFound == false)
	{
		files* newFile = nullptr;
		newFile = new files(fileName);
		currentLocation->dirFiles.push_back(*newFile);
		delete newFile;
	}
}

void computer::ls(std::string arg)
{
	if (arg == "")
	{
		// Prints out a list of all folders
		for (auto i = currentLocation->dirFolders.begin(); i != currentLocation->dirFolders.end(); ++i)
		{
			cout << *i << ' ';
		}
		//Prints out all fileNames
		for (auto i = currentLocation->dirFiles.begin(); i != currentLocation->dirFiles.end(); ++i)
		{
			cout << i->fileName << ' ';
		}
		cout << endl;

	}
	else if (arg == "-l")
	{
		for (auto i = currentLocation->dirFolders.begin(); i != currentLocation->dirFolders.end(); ++i)
		{
			cout << printPermissionsFolders(*i) << endl;
		}
		//Prints out all fileNames
		for (auto i = currentLocation->dirFiles.begin(); i != currentLocation->dirFiles.end(); ++i)
		{
			cout << printPermissionsFiles(*i) << endl;;
		}
		cout << endl;
	}
}

std::string computer::pwd()
{
	// return current working directory
	//who is your daddy
	return currentLocation->path;
}

void computer::cd(std::string argument)
{	
	// Back up one level

	if (argument == "..")
	{
		// getting parrent directory
		std::string parrentDirectory = currentLocation->parrentFolderKey;
		//change current location to parrent folder
		currentLocation = currentLocation = &directory.find(parrentDirectory)->second;
	}
	else
	{
		bool foundDirectory = false;
		//check to see if directory exists
		for (auto i = currentLocation->dirFolders.begin(); i != currentLocation->dirFolders.end(); ++i)
		{
			if (argument == *i)
			{
				//build path for location in master directory map
				std::string currentFolderPath = currentLocation->path;
				std::string changeToDirectory = currentFolderPath + argument + "/";
				currentLocation = &directory.find(changeToDirectory)->second;
				foundDirectory = true;
				break;
			}
		}

		for (auto i = currentLocation->dirFiles.begin(); i != currentLocation->dirFiles.end(); ++i)
		{
			if (argument == i->fileName)
			{

				cout << "Cannot CD into a file, location must be a directory" << endl;
				return;
			}
		}

		if (foundDirectory == false)
		{
			cout << "Directory does not exist" << endl;
		}
	}
	return;
}

void computer::chmod(std::string folderOrFile, std::string permissionNumber)
{
	try
	{
		bool fileFound = false;
		//get permissions number
		int permissionSanityCheck = std::stoi(permissionNumber);
		if (permissionSanityCheck > 777 || permissionSanityCheck < 0)
		{
			cout << "Permissions Number invalid" << endl;
		}
		else
		{
			// check to see if all permissions are present
			if (permissionNumber.size() == 3)
			{

				for (auto i = currentLocation->dirFiles.begin(); i != currentLocation->dirFiles.end(); ++i)
				{
					if (i->fileName == folderOrFile)
					{
						//update permissions	
						i->userPermission = (permissionNumber[0] - 48);
						i->groupPermission = (permissionNumber[1] - 48);
						i->publicPermission = (permissionNumber[2] - 48);
						fileFound = true;
						break;
					}
				}


				
				if (fileFound == false)
				{
					cout << "File not found" << endl;
				}

			}
			else
			{
				cout << "chmod must have 3 digits to ssign" << endl;
			}
		}
	}
	catch (const std::exception&)
	{
		cout << "chmod: Attempted to use invalid permissions: " + permissionNumber << endl;
	}
}

std::string computer::printPermissionsFolders(std::string folder)
{
	std::string currentFolderPath = currentLocation->path;
	std::string folderAccessKey = currentFolderPath + folder + "/";
	std::string permissionSettings = "";
	
	std::string perm_list[8] = { "---","--x","-r-","-rx","w--","w-x","wr-","wrx" };

	// get permissions for folder
	int userPermission = directory.find(folderAccessKey)->second.userPermission;
	int groupPermission = directory.find(folderAccessKey)->second.groupPermission;
	int publicPermission = directory.find(folderAccessKey)->second.publicPermission;
	int size = directory.find(folderAccessKey)->second.size;
	std::time_t lastModified = directory.find(folderAccessKey)->second.lastModified;

	char* dt = ctime(&lastModified);
	tm* gmtm = gmtime(&lastModified);
	dt = asctime(gmtm);

	// building ls -l folder
	permissionSettings = "d" + perm_list[userPermission] + 
		perm_list[groupPermission] + perm_list[publicPermission];
	return permissionSettings + "  " + user + "  " + group + "  " + 
		std::to_string(size) + "  " + dt + "  " + folder;
}

std::string computer::printPermissionsFiles(const files & folder)
{
	std::string permissionSettings = "";
	std::string perm_list[8] = { "---","--x","-r-","-rx","w--","w-x","wr-","wrx" };

	// get permissions for folder
	int userPermission = folder.userPermission;
	int groupPermission = folder.groupPermission;
	int publicPermission = folder.publicPermission;
	int size = folder.size;
	std::time_t lastModified = folder.lastModified;

	char* dt = ctime(&lastModified);
	tm* gmtm = gmtime(&lastModified);
	dt = asctime(gmtm);
	
	// building ls -l folder
	permissionSettings = "-" + perm_list[userPermission] + 
		perm_list[groupPermission] + perm_list[publicPermission];
	return permissionSettings + " " + user + " " + group + " " + 
		std::to_string(size) + " " + dt + " " + folder.fileName;

}

void computer::terminal()
{
	// controls if the computer terminal exits
	bool running = true;
	
	// storing pwd output
	std::string currentWorkingDirectory = "";
	// storing user text input
	std::string userInput = "";
	std::stringstream stream(userInput);

	
	
	while (running)
	{
		currentWorkingDirectory = pwd();

		//show current working directory 
		cout << currentWorkingDirectory + "$ ";
		std::getline(cin, userInput);
		//std::stringstream stream(userInput);
		running = parseUserInput(userInput);
	}
}
bool computer::parseUserInput(std::string userInput)
{
	// variables needed to parse user input
	std::string argSequance = "";
	std::string cmd = "";
	std::stringstream streamingInput(userInput);
	std::vector<std::string> cmdArguments = {};
	bool exitNotRequested = true;


	// grab first input in stream for terminal command
	streamingInput >> cmd;
	while (streamingInput >> argSequance)
	{
		cmdArguments.push_back(argSequance);
	}

	exitNotRequested = terminalCommandProcessor(cmd, cmdArguments);

	return exitNotRequested;
}
bool computer::terminalCommandProcessor(std::string cmd, std::vector<std::string> arguments)
{
	//else if needed to detect invalid command catch all
	//ls command detection
	if (cmd == "ls")
	{
		if (arguments.size() > 0)
		{
			if (arguments.size() == 1 && arguments[0] == "-l")
			{
				ls(arguments[0]);
			}
			else
			{
				cout << "invalid Arguments for ls" << endl;
			}
		}
		else
		{
			ls();
		}
	}
	//cd command detection
	else if (cmd == "cd")
	{
		if (arguments.size() == 0)
		{
			currentLocation = &directory.find("root/")->second;
		}
		else if (arguments.size() == 1)
		{
			cd(arguments[0]);
		}
		else if (arguments.size() > 1)
		{
			cout << "CD... Too many arguments" << endl;
		}
	}
	//pwd detection
	else if (cmd == "pwd")
	{
		cout << pwd() << endl;;
	}
	//mkdir command detection
	else if (cmd == "mkdir")
	{
		if (arguments.size() >= 1)
		{
			for (auto i = arguments.begin(); i != arguments.end(); ++i)
			{
				mkdir(*i);
			}
		}
		else
		{
			cout << "Must have an argument to create directory" << endl;
		}
	}
	//rmdir command detection
	else if (cmd == "rmdir")
	{	
		if (arguments.size() < 1)
		{
			cout << "rmdir: Invalid amount of arguments" << endl;
		}
		else
		{
			for (auto i = arguments.begin(); i != arguments.end(); ++i)
			{
				rmdir(*i);
			}
		}
	}
	//rm command detection
	else if (cmd == "rm")
	{
		if (arguments.size() < 1)
		{
			cout << "rm: Invalid amount of arguments" << endl;
		}
		else
		{
			for (auto i = arguments.begin(); i != arguments.end(); ++i)
			{
				rm(*i);
			}
		}
	}
	//chmod command detection
	else if (cmd == "chmod")
	{
		if (arguments.size() == 2)
		{
			chmod(arguments[0], arguments[1]);
		}
		else if (arguments.size() < 2)
		{
			cout << "Too few arguments for chmod" << endl;
		}
		else if (arguments.size() > 2)
		{
			cout << "Too many arguments for chmod" << endl;
		}
	}
	//touch command detection
	else if (cmd == "touch")
	{
		if (arguments.size() < 1)
		{
			cout << "Touch needs arguments to operate" << endl;
		}
		else
		{
			for (auto i = arguments.begin(); i != arguments.end(); ++i)
			{
				touch(*i);
			}
		}
		
	}
	//exit/quit command detection
	else if (cmd == "exit" || cmd == "quit")
	{
		return false;
	}
	else
	{
		cout << "command not found" << endl;
	}

	return true;
}
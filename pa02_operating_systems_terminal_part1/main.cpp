#include "computer.h"
#include <string>
#include <iterator>

int main()
{
    computer MyComputer;

	MyComputer.boot();

    // std::map<std::string, folder> directory;

    // folder test("root/NewFolder", 0, 1);
    // //directory.insert(std::pair<std::string, folder>("root/", test));
    // folder *newFolder = nullptr;
    // newFolder = new folder("root/folder/", 1, 2);
    // directory.insert(std::make_pair("root/folder/", *newFolder));

    // cout << newFolder->path << endl;
    // delete newFolder;

    // cout << directory.find("root/folder/")->second.path << endl;

    return 0;
}
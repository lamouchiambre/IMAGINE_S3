#include <string>
#include <iostream>
#include <filesystem>
namespace fs = std::filesystem;

int main()
{
    
    std::string path = "../../../archive/Shoe_vs_Sandal_vs_Boot_Dataset/Boot";
    for (const auto & entry : fs::directory_iterator(path))
        std::cout << entry.path() << std::endl;
}
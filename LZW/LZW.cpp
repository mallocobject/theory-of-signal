#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>

const char HEX[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

void codingFile(const char *filename_in, const char *filename_out)
{
    std::unordered_map<std::string, unsigned short> ump;
    // for (char i = 32; i < 127; i++)
    // {
    //     std::string s = "" + i;
    //     ump.insert(std::pair<std::string, unsigned short>{s, i});
    // }
    // std::cout << ump.size() << std::endl;
    std::ifstream f_in;
    std::ofstream f_out;

    try
    {
        f_in.open(filename_in, std::ios::in);
        // if (!f_in)
        // {
        //     std::cout << "Error opening file" << std::endl;
        //     return false;
        // }
        f_out.open(filename_out, std::ios::out);
        // if (!f_out)
        // {
        //     std::cout << "Error opening file" << std::endl;
        //     return false;
        // }
        if (!f_in || !f_out)
            throw std::string("Error opening file");
    }
    catch (const std::string &e)
    {
        throw e;
    }

    // std::string coding_res = "";
    char ch = '\0';
    std::string key = "";

    unsigned short value = 0xFF;
    if (!f_in.eof())
    {
        f_in.get(ch);
        key += ch;
    }
    while (!f_in.eof())
    {
        f_in.get(ch);
        std::string temp = key;
        key += ch;
        if (key.size() == 1 || ump.find(key) != ump.end())
            continue;
        else if (ump.find(key) == ump.end())
        {
            unsigned short temp_value = temp.size() == 1 ? (unsigned short)temp[0] : ump[temp];
            if (key.size() != 1)
                ump[key] = ++value;
            // coding_res += HEX[(temp_value >> 8) & 0xF] + HEX[(temp_value >> 4) & 0xF] + HEX[(temp_value) & 0xF];
            f_out << HEX[(temp_value >> 8) & 0xF] << HEX[(temp_value >> 4) & 0xF] << HEX[(temp_value) & 0xF] << std::flush;
            key = ch;
        }
    }
    f_in.close();
    f_out.close();
    // std::cout << coding_res << std::endl;
}

int main(int argc, char *argv[])
{
    try
    {
        // for (int i = 1; i < argc; i++)
        // {
        //     // printf("%s", argv[i]);
        //     std::cout << argv[i] << std::endl;
        // }
        if (argc == 1)
        {
            std::string filename_in = "";
            std::string filename_out = "";
            std::cin >> filename_in >> filename_out;
            codingFile(filename_in.c_str(), filename_out.c_str());
        }
        else if (argc == 3)
        {
            std::cout << "source file: " << argv[1] << std::endl;
            std::cout << "destination file: " << argv[2] << std::endl;
            codingFile(argv[1], argv[2]);
        }
        else
        {
            std::cout << "if you want to run the program with arguments, you can follow the below:" << std::endl;
            std::cout << "Usage:<file.exe> <input_file.txt> <output_file.txt>" << std::endl;
            system("pause");
            return 0;
        }

        // // std::cout << argc << std::endl;
        // codingFile(argv[1], argv[2]);
        // codingFile("a.txt", "b.txt");
        std::cout << "coding successful." << std::endl;
    }
    catch (const std::string e)
    {
        std::cout << e << std::endl;
    }

    system("pause");
    return 0;
}
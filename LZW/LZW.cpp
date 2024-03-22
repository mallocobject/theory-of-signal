#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>

// HEX array for hexadecimal conversion
const char HEX[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/**
 * @brief Encodes a file using a custom encoding scheme.
 *
 * This function reads from an input file and writes the encoded output to another file.
 * The encoding scheme(方案) uses an unordered map to store unique keys and their corresponding values.
 * Each key is a string of characters from the input file, and the value is an unsigned short integer.
 * The function reads characters from the input file one by one, and appends each character to the current key.
 * If the key is found in the map, it continues to the next character.
 * If the key is not found, it adds the key to the map with a new value, and writes the value of the previous key to the output file.
 * The value is written as a hexadecimal number using the HEX array for conversion.
 *
 * @param filename_in The name of the input file.
 * @param filename_out The name of the output file.
 * @throws std::string If an error occurs while opening the files.
 */
void codingFile(const char *filename_in, const char *filename_out)
{
    // Unordered map to store the encoding
    std::unordered_map<std::string, unsigned short> ump;
    for (int c = 0; c < 256; c++)
    {
        std::string str(1, static_cast<unsigned char>(c)); // Convert the unsigned char to a string
        ump[str] = static_cast<unsigned short>(c);
    }
    std::cout << "ump size: " << ump.size() << std::endl;

    // File streams for input and output
    std::ifstream inputFile;
    std::ofstream outputFile;

    try
    {
        // Open the input and output files
        inputFile.open(filename_in, std::ios::in);
        outputFile.open(filename_out, std::ios::out);

        // Throw an error if the files cannot be opened
        if (!inputFile || !outputFile)
            throw std::string("Error opening file");
    }
    catch (const std::string &e)
    {
        // Re-throw the error to be caught in main
        throw e;
    }

    // Variables for the encoding process
    char current_char = '\0';
    std::string key = "";
    unsigned short value = 0xFF;

    // Read the first character from the input file
    if (!inputFile.eof())
    {
        inputFile.get(current_char);
        key += current_char;
    }

    // Continue reading until the end of the file
    while (!inputFile.eof())
    {
        inputFile.get(current_char);
        std::string temp = key;
        key += current_char;

        // If the key is found in the map, continue to the next iteration
        if (key.size() == 1 || ump.find(key) != ump.end())
            continue;

        // If the key is not found in the map, add it
        else if (ump.find(key) == ump.end())
        {
            unsigned short temp_value = temp.size() == 1 ? (unsigned short)temp[0] : ump[temp];
            if (key.size() != 1)
                ump[key] = ++value;

            // Write the encoded value to the output file
            outputFile << HEX[(temp_value >> 8) & 0xF] << HEX[(temp_value >> 4) & 0xF] << HEX[(temp_value) & 0xF] << std::flush;

            // Reset the key
            key = current_char;
        }
    }

    // Close the input and output files
    inputFile.close();
    outputFile.close();
}

// Main function
int main(int argc, char *argv[])
{
    try
    {
        // If no arguments are provided, read the filenames from the standard input
        if (argc == 1)
        {
            // std::string filename_in = "a.txt";
            // std::string filename_out = "b.txt";
            std::string filename_in, filename_out;
            std::cin >> filename_in >> filename_out;
            codingFile(filename_in.c_str(), filename_out.c_str());
        }
        // If two arguments are provided, use them as the filenames
        else if (argc == 3)
        {
            std::cout << "source file: " << argv[1] << std::endl;
            std::cout << "destination file: " << argv[2] << std::endl;
            codingFile(argv[1], argv[2]);
        }
        // If the wrong number of arguments are provided, print a usage message
        else
        {
            std::cout << "if you want to run the program with arguments, you can follow the below:" << std::endl;
            std::cout << "Usage:<file.exe> <input_file.txt> <output_file.txt>" << std::endl;
            std::cin.get();
            return 0;
        }

        // Print a success message
        std::cout << "coding successful." << std::endl;
    }
    // Catch any errors thrown during the encoding process
    catch (const std::string &e)
    {
        std::cout << e << std::endl;
    }

    // Pause before exiting
    std::cout << "Press enter to continue...";
    system("pause > nul"); // Pause the console(use for windows)
    return 0;
}
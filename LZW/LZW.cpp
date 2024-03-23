#include <iostream>
#include <unordered_map>
#include <string>
#include <fstream>
#include <stdexcept>

// HEX array for hexadecimal conversion
const char HEX[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'A', 'B', 'C', 'D', 'E', 'F'};

/**
 * @brief Encodes a file using a simple encoding algorithm.
 *
 * This function reads a file, encodes it using a simple encoding algorithm, and writes the encoded data to another file.
 * The encoding algorithm uses a map to store the encoding of each string of characters.
 * The map is initialized with single character strings, where the encoding of a single character string is the character itself.
 * The function reads characters from the input file until the end of the file is reached.
 * If the current string of characters is not in the map, it is added to the map and its encoded value is written to the output file.
 *
 * @param filename_in The name of the input file to be encoded.
 * @param filename_out The name of the output file where the encoded data will be written.
 * @throws std::runtime_error If the input file or the output file cannot be opened.
 */
void encodeFile(const std::string &filename_in, const std::string &filename_out)
{
    // Create a map to store the encoding
    std::unordered_map<std::string, unsigned short> ump;

    // Initialize the map with single character strings
    for (int c = 0; c < 256; c++)
    {
        // Convert the integer to a string of one character
        std::string str(1, static_cast<unsigned char>(c));

        // The encoding of a single character string is the character itself
        ump[str] = static_cast<unsigned short>(c);
    }

    // Create file streams for input and output
    std::ifstream inputFile;
    std::ofstream outputFile;

    // Open the input and output files
    inputFile.open(filename_in, std::ios::in);
    outputFile.open(filename_out, std::ios::out);

    // Throw an error if the input file cannot be opened
    if (!inputFile.is_open())
        throw std::runtime_error("Error opening input file: " + filename_in);

    // Throw an error if the output file cannot be opened
    if (!outputFile.is_open())
        throw std::runtime_error("Error opening output file: " + filename_out);

    // Initialize variables
    char current_char = '\0';
    std::string key = "";
    unsigned short value = 0xFF;

    // Read the first character from the input file
    if (!inputFile.eof())
    {
        inputFile.get(current_char);
        key += current_char;
    }

    // Continue reading characters from the input file until the end of the file is reached
    while (!inputFile.eof())
    {
        inputFile.get(current_char);
        std::string temp = key;
        key += current_char;

        // If the current string of characters is not in the map, add it
        if (ump.find(key) == ump.end())
        {
            unsigned short temp_value = ump[temp];
            ump[key] = ++value;

            // Write the encoded value to the output file
            outputFile << HEX[(temp_value >> 8) & 0xF] << HEX[(temp_value >> 4) & 0xF] << HEX[(temp_value) & 0xF] << std::flush;

            // Reset the key to the current character
            key = current_char;
        }
    }

    // Close the input and output files
    inputFile.close();
    outputFile.close();
}

/**
 * @brief Main function of the program.
 *
 * This function handles the command line arguments and calls the encodeFile function to encode the input file.
 * If there is one argument, it reads the input and output file names from the standard input.
 * If there are three arguments, it uses the second and third arguments as the input and output file names, respectively.
 * If the number of arguments is not one or three, it prints the usage of the program and returns.
 * After the encoding is done, it prints a success message.
 * If an error occurs during the process, it catches the exception and prints the error message.
 *
 * @param argc The number of command line arguments.
 * @param argv The array of command line arguments.
 * @return int Returns 0 after successful execution.
 */
int main(int argc, char *argv[])
{
    try
    {
        // If there is one argument, read the input and output file names from the standard input
        if (argc == 1)
        {
            std::string filename_in, filename_out;
            std::cin >> filename_in >> filename_out;

            // Encode the input file
            encodeFile(filename_in, filename_out);
        }
        // If there are three arguments, use the second and third arguments as the input and output file names
        else if (argc == 3)
        {
            std::cout << "source file: " << argv[1] << std::endl;
            std::cout << "destination file: " << argv[2] << std::endl;

            // Encode the input file
            encodeFile(argv[1], argv[2]);
        }
        // If the number of arguments is not one or three, print the usage of the program and return
        else
        {
            std::cout << "Usage:<file.exe> <input_file.txt> <output_file.txt>" << std::endl;
            return 0;
        }

        // Print a success message after the encoding is done
        std::cout << "coding successful." << std::endl;
    }
    // Catch any exceptions and print the error message
    catch (const std::runtime_error &e)
    {
        std::cout << e.what() << std::endl;
    }

    system("pause > nul"); // Pause the console to view the output
    // Return 0 after successful execution
    return 0;
}
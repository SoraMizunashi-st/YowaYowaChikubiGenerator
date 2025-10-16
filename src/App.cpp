// -------------------------------------------------------------------------------------------------------------------------------------//
// YYGC | App.cpp
// -------------------------------------------------------------------------------------------------------------------------------------//
#include "./App.hpp"


// -------------------------------------------------------------------------------------------------------------------------------------//
//
// YYGC CLI | Entry Point
//
// -------------------------------------------------------------------------------------------------------------------------------------//
// About the return value
// -------------------------------------------------------------------------------------------------------------------------------------//
// 0 : Normal termination
// 1 : Abnormal termination
// -------------------------------------------------------------------------------------------------------------------------------------//
int main(int p_Argc, char* p_Argv[]) 
{

    auto start = std::chrono::high_resolution_clock::now();

    std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++ " << std::endl;
    std::cout << "This is Test Module YYCG Programs " << std::endl;
    std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++ " << std::endl;

    // ---------------------------------------------------------------------------------------------------------------------------------//
    // Minimum check for number of arguments (must have at least the option itself)
    // ---------------------------------------------------------------------------------------------------------------------------------//
    if (p_Argc < 2) 
    {
        // Executable file name only
        std::cerr << "Error: No option provided." << std::endl;
        std::cerr << "Run '" << p_Argv[0] << " -h' for usage information." << std::endl;
        return 1; // abnormal termination
    }

    // Naming Convention: tPascalCase for local variables
    std::string t_Option(p_Argv[1]); // First argument (option)
    
    // ---------------------------------------------------------------------------------------------------------------------------------//
    // Check options (-h takes precedence first)
    // ---------------------------------------------------------------------------------------------------------------------------------//
    if (t_Option == "-h") 
    {
        // -h displays help without checking the number of arguments
        std::cout << "--------------------------------------------------------" << std::endl;
        std::cout << "YYCG (YowaYowaChikubiGenerator) Version 1.0.0" << std::endl;
        std::cout << "A generator for creating action chains. " << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;
        std::cout << "Usage: " << p_Argv[0] << " <option> [argument]" << std::endl;
        std::cout << "\nOptions:" << std::endl;
        std::cout << "  -f <filename>  : Specify the input file to process." << std::endl;
        std::cout << "  -h             : Display this help message and exit." << std::endl;
        std::cout << "--------------------------------------------------------" << std::endl;
        return 0; // normal termination
    }
    else if (t_Option == "-f") 
    {
        // -----------------------------------------------------------------------------------------------------------------------------//
        // In the case of -f, check the number of arguments (file names)
        // -----------------------------------------------------------------------------------------------------------------------------//
        if (p_Argc < 3) 
        {
            // If there is no file name after -f
            std::cerr << "Error: The '-f' option requires a filename argument." << std::endl;
            std::cerr << "Usage: " << p_Argv[0] << " -f <filename>" << std::endl;
            return 1;
        }

        // Naming Convention: tPascalCase. Use tFileName.
        std::string t_FileName(p_Argv[2]); // Now safely get the second argument

        // -----------------------------------------------------------------------------------------------------------------------------//
        // Call the main logic for file processing here
        // -----------------------------------------------------------------------------------------------------------------------------//
        std::cout << "YYCG: Processing file specified by '-f' option." << std::endl;
        std::cout << "Target File: " << t_FileName << std::endl;
        
        // my Core Logit insert this
        std::unique_ptr<yycgCore> core = std::make_unique<yycgCore>( t_FileName );
        core->Setting().Generate().Result();

        // temporary success message
        std::cout << "Successfully executed YYCG process." << std::endl;

        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);

        std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++ " << std::endl;
        std::cout << "Execution Time (Core Logic): " << duration.count() << " ms" << std::endl;
        std::cout << "+++++++++++++++++++++++++++++++++++++++++++++++ " << std::endl;

        return 0; // normal termination
    }
    else 
    {
        // Behavior when option is undefined
        std::cerr << "Error: Unknown option '" << t_Option << "'." << std::endl;
        std::cerr << "Run '" << p_Argv[0] << " -h' for usage information." << std::endl;
        return 1; // abnormal termination
    }
}
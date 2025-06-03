#include "scheduler.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <stdexcept>
#include "../shell/command_executor.h"
#include "../shell/input_parser.h"
namespace scheduler {


bool scheduleCommand(const std::vector<std::string> &args)
{
    if (args[1] != "s")
    {
        std::cerr << "Usage: after <number> s <command>" << std::endl;
        return false;
    }

    int delay;
    try
    {
        delay = std::stoi(args[0]);
    }
    catch (const std::exception &e)
    {
        std::cerr << "Invalid number: " << args[0] << std::endl;
        return  false;
    }
    
    

    std::string command = args[2];
    std::vector<std::string> commandArgs(args.begin() + 3, args.end());

                
                    std::this_thread::sleep_for(std::chrono::seconds(delay));
                    std::cout << "\nThe thread is completed!" << std::endl;

                    std::cout<< "Running command: " << command << std::endl;
    return executor::run(false, command, commandArgs);
                   
           

    std::cout << "Scheduled command '" << command << "' to run after " << delay << " seconds." << std::endl;

}
} // namespace scheduler
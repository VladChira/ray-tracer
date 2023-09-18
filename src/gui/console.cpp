#include "console.h"

Console *Console::console_instance{nullptr};
std::mutex Console::singleton_mutex;

Console::Console() {}

Console::~Console() {}

Console *Console::GetInstance()
{
    std::lock_guard<std::mutex> lock(singleton_mutex);
    if (console_instance == nullptr)
    {
        console_instance = new Console();
    }
    return console_instance;
}

void Console::DestroyInstance()
{
    delete console_instance;
    console_instance = NULL;
}


Console *Console::appendLine(const std::string &line)
{
    std::string final_line = ">  " + line;
    std::lock_guard<std::mutex> lock(text_mutex); // Acquire lock
    lines.push_back(final_line);                  // Append line to the array
    return this;
}


std::vector<std::string> Console::getContents()
{
    std::lock_guard<std::mutex> lock(text_mutex); // Acquire lock
    return lines;                                 // Return a copy of the array
}
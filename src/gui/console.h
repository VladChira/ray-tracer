#pragma once
#include <mutex>
#include <vector>


// TODO - Base abstract ConsoleEntry class with
// LogEntry, WarningEntry, ErrorEntry extending from it
// with colors

class Console
{
private:
    static Console *console_instance;
    static std::mutex singleton_mutex;

    std::vector<std::string> lines;
    std::mutex text_mutex;

protected:
    Console()
    {
    }
    ~Console() {}

public:
    Console(Console &other) = delete;
    void operator=(const Console &) = delete;

    static Console *GetInstance();

    static void DestroyInstance();

    
    // Append line to console
    Console *appendLine(const std::string &line)
    {
        std::string final_line = ">  " + line;
        std::lock_guard<std::mutex> lock(text_mutex); // Acquire lock
        lines.push_back(final_line);                     // Append line to the array
        return this;
    }

    // Retrieve the console's contents
    std::vector<std::string> getContents()
    {
        std::lock_guard<std::mutex> lock(text_mutex); // Acquire lock
        return lines;                          // Return a copy of the array
    }
};

Console *Console::console_instance{nullptr};
std::mutex Console::singleton_mutex;

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
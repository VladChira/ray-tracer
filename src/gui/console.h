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
    Console();
    ~Console();

public:
    Console(Console &other) = delete;
    void operator=(const Console &) = delete;

    static Console *GetInstance();

    static void DestroyInstance();

    
    // Append line to console
    Console *appendLine(const std::string &line);

    // Retrieve the console's contents
    std::vector<std::string> getContents();
};
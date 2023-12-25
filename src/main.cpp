#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <thread>
#include <nlohmann/json.hpp>
#include <algorithm>

struct LogEntry
{
    std::string timestamp;
    std::string severity;
    std::string origin;
    std::string message;

    LogEntry(){};
    LogEntry(std::string timestamp, std::string severity, std::string origin, std::string message) : timestamp(timestamp),
    severity(severity), origin(origin), message(message){};
};

std::vector<LogEntry> readLogsFromCSV(const std::filesystem::path &filePath)
{
    std::vector<LogEntry> logs;
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cout << "File: " << filePath << " not opened!" << std::endl;
        return logs;
    }

    std::string line;
    std::getline(file, line); // Headers
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        LogEntry log;

        std::string word;

        std::vector<std::string> store;
        while (std::getline(iss, word, ','))
            store.emplace_back(word);

        logs.push_back(LogEntry{store[0], store[1], store[2], store[3]});
    }

    return logs;
}

std::vector<LogEntry> readLogsFromText(const std::filesystem::path &filePath)
{
    std::vector<LogEntry> logs;
    std::ifstream file(filePath);

    if (!file.is_open())
    {
        std::cout << "File: " << filePath << " not opened!" << std::endl;
        return logs;
    }

    std::string line;
    while (std::getline(file, line))
    {
        std::istringstream iss(line);
        LogEntry log;
        iss >> log.timestamp;
        log.timestamp.append(" ");
        iss >> log.timestamp >> log.severity >> log.origin;
        std::getline(iss >> std::ws, log.message);
        
        logs.push_back(log);
    }

    return logs;
}

// Función para leer logs desde un archivo JSON
std::vector<LogEntry> readLogsFromJSON(const std::filesystem::path &filePath)
{
    std::vector<LogEntry> logs;

    std::ifstream file(filePath);
    if (!file.is_open())
    {
        std::cout << "File: " << filePath << " not opened!" << std::endl;
        return logs;
    }

    // Parsear el contenido JSON
    nlohmann::json jsonLogs;
    file >> jsonLogs;
    // Recorrer cada elemento del array JSON y convertirlo a LogEntry
    for (const auto &jsonEntry : jsonLogs)
    {
        LogEntry log;
        log.timestamp = jsonEntry["timestamp"];
        log.severity = jsonEntry["severity"];
        log.origin = jsonEntry["origin"];
        log.message = jsonEntry["message"];
        logs.push_back(log);
    }

    return logs;
}

// Función para leer logs en paralelo desde múltiples archivos
std::vector<std::vector<LogEntry>> readLogsInParallel(const std::vector<std::filesystem::path> &filePaths)
{
    std::vector<std::thread> threads;
    std::vector<std::vector<LogEntry>> results(filePaths.size());

    // Función para ser ejecutada por cada hilo
    auto threadFunction = [&](int index)
    {
        std::cout << "Thread created with id: " << std::this_thread::get_id() << std::endl;
        if (filePaths[index].extension() == ".csv")
        {
            results[index] = readLogsFromCSV(filePaths[index]);
        }
        else if (filePaths[index].extension() == ".txt")
        {
            results[index] = readLogsFromText(filePaths[index]);
        }
        else if (filePaths[index].extension() == ".json")
        {
            results[index] = readLogsFromJSON(filePaths[index]);
        }
    };

    // Crear y lanzar hilos
    for (int i = 0; i < filePaths.size(); ++i)
    {
        threads.emplace_back(threadFunction, i);
    }

    // Esperar a que todos los hilos terminen
    for (auto &thread : threads)
    {
        thread.join();
    }

    return results;
}

int main(int argc, char **argv)
{
    std::cout << "You have entered " << argc << " arguments:"
              << "\n";
    for (int i = 0; i < argc; ++i)
        std::cout << argv[i] << "\n";

    /* Log files list */
    std::string log_path{"/code/dev/documentation/log_templates/"};
    std::vector<std::string> log_files = {"log.csv", "log.json", "log.txt"};

    std::vector<std::filesystem::path> filePaths{};
    for (const auto &log_file : log_files)
        filePaths.push_back(log_path + log_file);

    // std::for_each(log_files.begin(), log_files.end(), [log_path, &filePaths] (std::string_view log_file){
    //     filePaths.push_back(log_path + std::string(log_file));
    // });

    std::vector<std::vector<LogEntry>> logsInParallel = readLogsInParallel(filePaths);

    // Display all logs
    for (const auto &logEntries : logsInParallel)
    {
        for (const auto &log : logEntries)
        {
            std::cout << "Timestamp: " << log.timestamp << "\n";
            std::cout << "Severity: " << log.severity << "\n";
            std::cout << "Origin: " << log.origin << "\n";
            std::cout << "Message: " << log.message << "\n";
            std::cout << "------------------------\n";
        }
    }

    return 0;
}
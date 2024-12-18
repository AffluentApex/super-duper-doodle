#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstring>
#include <fstream>
#include <map>
#include <filesystem>
#include <cstdlib>
#include <ctime>
#include <cmath>
#include <algorithm>
#include <iterator>
#include <chrono>
#include <thread>
#include <iomanip>
#include <sstream>
#include <cstdio>
#include <memory>
#include <array>
#include <regex>
#include <random>
#include <functional>

// Windows-specific includes and defines
#ifdef _WIN32
#include <windows.h>
#include <direct.h>
#define getcwd _getcwd
#define chdir _chdir
#endif

/*
 * SimpleShell - A friendly command line shell with lots of useful features!
 * Created with ❤️ to make command line tasks more fun and approachable
 */
class SimpleShell {
private:
    bool running;  // Keeps track of whether our shell is still running
    std::vector<std::string> command_history;  // Remember what commands were run before
    const size_t MAX_HISTORY = 1000;  // Don't let history get too big!

    // Show our cute ASCII art mascot and welcome message
    void showWelcome() {
        std::cout << R"(
                  .--.      .-'.      .--.      .--.      .--.      .--.      .`-.      .--.
              :::::.\::::::::.\::::::::.\::::::::.\::::::::.\::::::::.\::::::::.\::::::::.\
              '      `--'      `.-'      `--'      `--'      `--'      `-.'      `--'      `
                    /\
                   /  \
                  /    \
                 /      \
                /  /\    \         Welcome to SimpleShell!
               /  /  \    \        Your friendly neighborhood shell
              /  /    \    \       Type 'help' to see what I can do
             /  /    __\    \
            /__/____/   \____\     Let's make command line fun! ^_^
            
        )" << std::endl;
    }

    // Show all the cool things we can do!
    void showHelp() {
        std::cout << "\nHere's everything I can help you with:\n"
                  << "show [text]     - Want to see some text in the terminal?\n"
                  << "exit            - Time to say goodbye\n"
                  << "clear           - Let's tidy up the screen\n"
                  << "where           - Lost? I'll show you where you are\n"
                  << "goto [path]     - Let's go somewhere else!\n"
                  << "list            - See what's in this folder\n"
                  << "make [name]     - Create a shiny new file\n"
                  << "remove [name]   - Get rid of a file you don't need\n"
                  << "history         - See what commands you've used before\n"
                  << "copy [src] [dest] - Make a copy of a file\n"
                  << "move [src] [dest] - Move a file somewhere else\n"
                  << "rename [old] [new] - Give a file a new name\n"
                  << "read [name]     - Take a peek at what's in a file\n"
                  << "moveDir [src] [dest] - Move an entire folder\n"
                  << "copyDir [src] [dest] - Copy a whole folder\n"
                  << "rmdir [name]    - Remove a folder\n"
                  << "mkdir [name]    - Create a new folder\n"
                  << "search [text]   - Looking for something?\n"
                  << "calc [expr]     - Need to do some quick math?\n"
                  << "date            - What time is it anyway?\n"
                  << "cat [name]      - Another way to read files\n"
                  << "zip [src] [dest] - Compress files into a zip archive\n"
                  << "unzip [file]    - Extract files from a zip archive\n"
                  << "encrypt [file] [pass] - Encrypt a file with a password\n"
                  << "decrypt [file] [pass] - Decrypt an encrypted file\n"
                  << "backup [dir]    - Create a backup of a directory\n"
                  << "restore [backup] - Restore from a backup\n"
                  << "compress [file]  - Compress a file\n"
                  << "decompress [file] - Decompress a file\n"
                  << "checksum [file]  - Calculate file checksum\n"
                  << "permissions [file] [perms] - Set file permissions\n"
                  << "owner [file] [owner] - Change file owner\n"
                  << "group [file] [group] - Change file group\n"
                  << "find [pattern]  - Find files matching pattern\n"
                  << "grep [pat] [file] - Search for text in file\n"
                  << "sort [file]     - Sort file contents\n"
                  << "unique [file]   - Show unique lines in file\n"
                  << "count [file]    - Count lines in file\n"
                  << "tail [file]     - Show end of file\n"
                  << "head [file]     - Show start of file\n"
                  << "watch [file]    - Watch file for changes\n"
                  << "diff [f1] [f2]  - Compare two files\n"
                  << "merge [f1] [f2] - Merge two files\n"
                  << "split [file] [n] - Split file into n parts\n"
                  << "hash [file]     - Generate file hash\n"
                  << "base64 [file]   - Base64 encode file\n"
                  << "decode64 [file] - Base64 decode file\n"
                  << "hex [file]      - Show hex dump of file\n"
                  << "touch [file]    - Create empty file\n"
                  << "ln [src] [dest] - Create symbolic link\n"
                  << "chmod [file] [mode] - Change file mode\n"
                  << "chown [file] [owner] - Change file owner\n"
                  << "df              - Show disk free space\n"
                  << "du [path]       - Show disk usage\n"
                  << "ps              - List processes\n"
                  << "kill [pid]      - Kill a process\n"
                  << "top             - Show top processes\n"
                  << "netstat         - Show network status\n"
                  << "ping [host]     - Ping a host\n"
                  << "wget [url]      - Download file from URL\n"
                  << "curl [url]      - Make HTTP request\n"
                  << "tar [src] [dest] - Create tar archive\n"
                  << "untar [file]    - Extract tar archive\n"
                  << "gzip [file]     - Gzip compress file\n"
                  << "gunzip [file]   - Gzip decompress file\n"
                  << "bzip2 [file]    - Bzip2 compress file\n"
                  << "bunzip2 [file]  - Bzip2 decompress file\n"
                  << "xz [file]       - XZ compress file\n"
                  << "unxz [file]     - XZ decompress file\n"
                  << "help            - See this friendly message again!\n\n";
    }

    // Parse command line input into arguments
    std::vector<std::string> parseCommand(const std::string& cmdLine) {
        std::vector<std::string> args;
        std::string current;
        std::istringstream iss(cmdLine);

        while (iss >> current) {
            args.push_back(current);
        }
        return args;
    }

    // Execute a command based on the parsed arguments
    void executeCommand(const std::vector<std::string>& args) {
        if (args.empty()) return;

        std::string cmd = args[0];

        if (cmd == "show") {
            std::string text;
            for (size_t i = 1; i < args.size(); ++i) {
                text += args[i] + " ";
            }
            if (!text.empty()) {
                std::cout << text << std::endl;
            }
        }
        else if (cmd == "exit") {
            running = false;
        }
        else if (cmd == "clear") {
#ifdef _WIN32
            system("cls");
#else
            system("clear");
#endif
        }
        else if (cmd == "where") {
            char cwd[1024];
            if (getcwd(cwd, sizeof(cwd)) != nullptr) {
                std::cout << cwd << std::endl;
            }
        }
        else if (cmd == "goto" && args.size() > 1) {
            if (chdir(args[1].c_str()) != 0) {
                std::cout << "Cannot go to that location" << std::endl;
            }
        }
        else if (cmd == "list") {
#ifdef _WIN32
            system("dir");
#else
            system("ls");
#endif
        }
        else if (cmd == "make" && args.size() > 1) {
            std::ofstream file(args[1]);
            if (!file) {
                std::cout << "Cannot create file" << std::endl;
            }
        }
        else if (cmd == "remove" && args.size() > 1) {
            if (std::remove(args[1].c_str()) != 0) {
                std::cout << "Cannot remove file" << std::endl;
            }
        }
        else if (cmd == "history") {
            for (size_t i = 0; i < command_history.size(); ++i) {
                std::cout << (i + 1) << ": " << command_history[i] << std::endl;
            }
        }
        else if (cmd == "copy" && args.size() > 2) {
            try {
                std::filesystem::copy(args[1], args[2]);
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cout << "Error copying file: " << e.what() << std::endl;
            }
        }
        else if (cmd == "move" && args.size() > 2) {
            try {
                std::filesystem::rename(args[1], args[2]);
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cout << "Error moving file: " << e.what() << std::endl;
            }
        }
        else if (cmd == "rename" && args.size() > 2) {
            try {
                std::filesystem::rename(args[1], args[2]);
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cout << "Error renaming file: " << e.what() << std::endl;
            }
        }
        else if (cmd == "read" && args.size() > 1) {
            std::ifstream file(args[1]);
            if (file) {
                std::cout << file.rdbuf();
            } else {
                std::cout << "Cannot read file" << std::endl;
            }
        }
        else if (cmd == "moveDir" && args.size() > 2) {
            try {
                std::filesystem::rename(args[1], args[2]);
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cout << "Error moving directory: " << e.what() << std::endl;
            }
        }
        else if (cmd == "copyDir" && args.size() > 2) {
            try {
                std::filesystem::copy(args[1], args[2], std::filesystem::copy_options::recursive);
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cout << "Error copying directory: " << e.what() << std::endl;
            }
        }
        else if (cmd == "rmdir" && args.size() > 1) {
            try {
                std::filesystem::remove_all(args[1]);
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cout << "Error removing directory: " << e.what() << std::endl;
            }
        }
        else if (cmd == "mkdir" && args.size() > 1) {
            try {
                std::filesystem::create_directory(args[1]);
            }
            catch (const std::filesystem::filesystem_error& e) {
                std::cout << "Error creating directory: " << e.what() << std::endl;
            }
        }
        else if (cmd == "search" && args.size() > 1) {
            std::string searchText = args[1];
            for (const auto& entry : std::filesystem::recursive_directory_iterator(".")) {
                if (std::filesystem::is_regular_file(entry)) {
                    std::ifstream file(entry.path());
                    std::string line;
                    while (std::getline(file, line)) {
                        if (line.find(searchText) != std::string::npos) {
                            std::cout << "Found in " << entry.path() << ": " << line << std::endl;
                        }
                    }
                }
            }
        }
        else if (cmd == "calc" && args.size() > 1) {
            try {
                std::string expr;
                for (size_t i = 1; i < args.size(); ++i) {
                    expr += args[i];
                }
                // Basic calculator implementation
                // This is a simplified version - you might want to implement a proper expression parser
                std::cout << "Result: " << std::stod(expr) << std::endl;
            }
            catch (const std::exception& e) {
                std::cout << "Error in calculation: " << e.what() << std::endl;
            }
        }
        else if (cmd == "date") {
            time_t now = time(0);
            char* dt = ctime(&now);
            std::cout << "Current date & time: " << dt;
        }
        else if (cmd == "cat" && args.size() > 1) {
            std::ifstream file(args[1]);
            if (file) {
                std::cout << file.rdbuf();
            } else {
                std::cout << "Cannot read file" << std::endl;
            }
        }
        else if (cmd == "help") {
            showHelp();
        }
        else if (cmd == "zip" && args.size() > 2) {
            zipFiles(args[1], args[2]);
        }
        else if (cmd == "unzip" && args.size() > 1) {
            unzipFile(args[1]);
        }
        else if (cmd == "encrypt" && args.size() > 2) {
            encryptFile(args[1], args[2]);
        }
        else if (cmd == "decrypt" && args.size() > 2) {
            decryptFile(args[1], args[2]);
        }
        else if (cmd == "backup" && args.size() > 1) {
            backupDirectory(args[1]);
        }
        else if (cmd == "restore" && args.size() > 1) {
            restoreBackup(args[1]);
        }
        else if (cmd == "compress" && args.size() > 1) {
            compressFile(args[1]);
        }
        else if (cmd == "decompress" && args.size() > 1) {
            decompressFile(args[1]);
        }
        else if (cmd == "checksum" && args.size() > 1) {
            calculateChecksum(args[1]);
        }
        else if (cmd == "permissions" && args.size() > 2) {
            setFilePermissions(args[1], args[2]);
        }
        else if (cmd == "owner" && args.size() > 2) {
            changeFileOwner(args[1], args[2]);
        }
        else if (cmd == "group" && args.size() > 2) {
            changeFileGroup(args[1], args[2]);
        }
        else if (cmd == "find" && args.size() > 1) {
            findFiles(args[1]);
        }
        else if (cmd == "grep" && args.size() > 2) {
            grepInFile(args[1], args[2]);
        }
        else if (cmd == "sort" && args.size() > 1) {
            sortFile(args[1]);
        }
        else if (cmd == "unique" && args.size() > 1) {
            uniqueLines(args[1]);
        }
        else if (cmd == "count" && args.size() > 1) {
            countLines(args[1]);
        }
        else if (cmd == "tail" && args.size() > 1) {
            tailFile(args[1]);
        }
        else if (cmd == "head" && args.size() > 1) {
            headFile(args[1]);
        }
        else if (cmd == "watch" && args.size() > 1) {
            watchFile(args[1]);
        }
        else if (cmd == "diff" && args.size() > 2) {
            compareFiles(args[1], args[2]);
        }
        else if (cmd == "merge" && args.size() > 2) {
            mergeFiles(args[1], args[2]);
        }
        else if (cmd == "split" && args.size() > 2) {
            splitFile(args[1], std::stoi(args[2]));
        }
        else if (cmd == "hash" && args.size() > 1) {
            hashFile(args[1]);
        }
        else if (cmd == "base64" && args.size() > 1) {
            base64Encode(args[1]);
        }
        else if (cmd == "decode64" && args.size() > 1) {
            base64Decode(args[1]);
        }
        else if (cmd == "hex" && args.size() > 1) {
            hexDump(args[1]);
        }
        else if (cmd == "touch") {
            touchFile(args[1]);
        }
        else if (cmd == "ln" && args.size() > 2) {
            createLink(args[1], args[2]);
        }
        else if (cmd == "chmod" && args.size() > 2) {
            changeMode(args[1], args[2]);
        }
        else if (cmd == "chown" && args.size() > 2) {
            changeOwnership(args[1], args[2]);
        }
        else if (cmd == "df") {
            diskFree();
        }
        else if (cmd == "du" && args.size() > 1) {
            diskUsage(args[1]);
        }
        else if (cmd == "ps") {
            processList();
        }
        else if (cmd == "kill" && args.size() > 1) {
            killProcess(std::stoi(args[1]));
        }
        else if (cmd == "top") {
            showTopProcesses();
        }
        else if (cmd == "netstat") {
            networkStatus();
        }
        else if (cmd == "ping" && args.size() > 1) {
            pingHost(args[1]);
        }
        else if (cmd == "wget" && args.size() > 1) {
            downloadFile(args[1]);
        }
        else if (cmd == "curl" && args.size() > 1) {
            curlRequest(args[1]);
        }
        else if (cmd == "tar" && args.size() > 2) {
            createTarArchive(args[1], args[2]);
        }
        else if (cmd == "untar" && args.size() > 1) {
            extractTarArchive(args[1]);
        }
        else if (cmd == "gzip" && args.size() > 1) {
            gzipCompress(args[1]);
        }
        else if (cmd == "gunzip" && args.size() > 1) {
            gzipDecompress(args[1]);
        }
        else if (cmd == "bzip2" && args.size() > 1) {
            bzip2Compress(args[1]);
        }
        else if (cmd == "bunzip2" && args.size() > 1) {
            bzip2Decompress(args[1]);
        }
        else if (cmd == "xz" && args.size() > 1) {
            xzCompress(args[1]);
        }
        else if (cmd == "unxz" && args.size() > 1) {
            xzDecompress(args[1]);
        }
        else {
            std::cout << "Unknown command. Type 'help' for available commands." << std::endl;
        }

        // Add command to history
        if (!cmd.empty() && command_history.size() < MAX_HISTORY) {
            std::string fullCmd;
            for (const auto& arg : args) {
                fullCmd += arg + " ";
            }
            command_history.push_back(fullCmd);
        }
    }

    // Helper functions for new commands
    void zipFiles(const std::string& source, const std::string& dest) {
        std::string cmd = "zip -r " + dest + " " + source;
        system(cmd.c_str());
    }

    void unzipFile(const std::string& file) {
        std::string cmd = "unzip " + file;
        system(cmd.c_str());
    }

    void encryptFile(const std::string& source, const std::string& password) {
        std::string cmd = "openssl enc -aes-256-cbc -salt -in " + source +
                          " -out " + source + ".enc -k " + password;
        system(cmd.c_str());
    }

    void decryptFile(const std::string& source, const std::string& password) {
        std::string cmd = "openssl enc -d -aes-256-cbc -in " + source +
                          " -out " + source + ".dec -k " + password;
        system(cmd.c_str());
    }

    void backupDirectory(const std::string& dir) {
        std::string timestamp = std::to_string(std::time(nullptr));
        std::string backup_name = dir + "_backup_" + timestamp + ".tar.gz";
        std::string cmd = "tar -czf " + backup_name + " " + dir;
        system(cmd.c_str());
    }

    void restoreBackup(const std::string& backup) {
        std::string cmd = "tar -xzf " + backup;
        system(cmd.c_str());
    }

    void compressFile(const std::string& file) {
        std::string cmd = "gzip -k " + file;
        system(cmd.c_str());
    }

    void decompressFile(const std::string& file) {
        std::string cmd = "gzip -d -k " + file;
        system(cmd.c_str());
    }

    void calculateChecksum(const std::string& file) {
        std::string cmd = "md5sum " + file;
        system(cmd.c_str());
    }

    void setFilePermissions(const std::string& file, const std::string& perms) {
        std::string cmd = "chmod " + perms + " " + file;
        system(cmd.c_str());
    }

    void changeFileOwner(const std::string& file, const std::string& owner) {
        std::string cmd = "chown " + owner + " " + file;
        system(cmd.c_str());
    }

    void changeFileGroup(const std::string& file, const std::string& group) {
        std::string cmd = "chgrp " + group + " " + file;
        system(cmd.c_str());
    }

    void findFiles(const std::string& pattern) {
        std::string cmd = "find . -name \"" + pattern + "\"";
        system(cmd.c_str());
    }

    void grepInFile(const std::string& pattern, const std::string& file) {
        std::string cmd = "grep \"" + pattern + "\" " + file;
        system(cmd.c_str());
    }

    void sortFile(const std::string& file) {
        std::string cmd = "sort " + file;
        system(cmd.c_str());
    }

    void uniqueLines(const std::string& file) {
        std::string cmd = "sort " + file + " | uniq";
        system(cmd.c_str());
    }

    void countLines(const std::string& file) {
        std::string cmd = "wc -l " + file;
        system(cmd.c_str());
    }

    void tailFile(const std::string& file) {
        std::string cmd = "tail " + file;
        system(cmd.c_str());
    }

    void headFile(const std::string& file) {
        std::string cmd = "head " + file;
        system(cmd.c_str());
    }

    void watchFile(const std::string& file) {
        std::string cmd = "watch -n 1 cat " + file;
        system(cmd.c_str());
    }

    void compareFiles(const std::string& file1, const std::string& file2) {
        std::string cmd = "diff " + file1 + " " + file2;
        system(cmd.c_str());
    }

    void mergeFiles(const std::string& file1, const std::string& file2) {
        std::string cmd = "cat " + file1 + " " + file2 + " > merged.txt";
        system(cmd.c_str());
    }

    void splitFile(const std::string& file, int parts) {
        std::string cmd = "split -n " + std::to_string(parts) + " " + file;
        system(cmd.c_str());
    }

    void hashFile(const std::string& file) {
        std::string cmd = "sha256sum " + file;
        system(cmd.c_str());
    }

    void base64Encode(const std::string& file) {
        std::string cmd = "base64 " + file;
        system(cmd.c_str());
    }

    void base64Decode(const std::string& file) {
        std::string cmd = "base64 -d " + file;
        system(cmd.c_str());
    }

    void hexDump(const std::string& file) {
        std::string cmd = "hexdump -C " + file;
        system(cmd.c_str());
    }

    void touchFile(const std::string& file) {
        std::string cmd = "touch " + file;
        system(cmd.c_str());
    }

    void createLink(const std::string& source, const std::string& dest) {
        std::string cmd = "ln -s " + source + " " + dest;
        system(cmd.c_str());
    }

    void changeMode(const std::string& file, const std::string& mode) {
        std::string cmd = "chmod " + mode + " " + file;
        system(cmd.c_str());
    }

    void changeOwnership(const std::string& file, const std::string& owner) {
        std::string cmd = "chown " + owner + " " + file;
        system(cmd.c_str());
    }

    void diskFree() {
        system("df -h");
    }

    void diskUsage(const std::string& path) {
        std::string cmd = "du -sh " + path;
        system(cmd.c_str());
    }

    void processList() {
        system("ps aux");
    }

    void killProcess(int pid) {
        std::string cmd = "kill " + std::to_string(pid);
        system(cmd.c_str());
    }

    void showTopProcesses() {
        system("top -n 1");
    }

    void networkStatus() {
        system("netstat -tuln");
    }

    void pingHost(const std::string& host) {
        std::string cmd = "ping -c 4 " + host;
        system(cmd.c_str());
    }

    void downloadFile(const std::string& url) {
        std::string cmd = "wget " + url;
        system(cmd.c_str());
    }

    void curlRequest(const std::string& url) {
        std::string cmd = "curl " + url;
        system(cmd.c_str());
    }

    void createTarArchive(const std::string& source, const std::string& dest) {
        std::string cmd = "tar -czf " + dest + " " + source;
        system(cmd.c_str());
    }

    void extractTarArchive(const std::string& archive) {
        std::string cmd = "tar -xzf " + archive;
        system(cmd.c_str());
    }

    void gzipCompress(const std::string& file) {
        std::string cmd = "gzip -k " + file;
        system(cmd.c_str());
    }

    void gzipDecompress(const std::string& file) {
        std::string cmd = "gzip -d -k " + file;
        system(cmd.c_str());
    }

    void bzip2Compress(const std::string& file) {
        std::string cmd = "bzip2 -k " + file;
        system(cmd.c_str());
    }

    void bzip2Decompress(const std::string& file) {
        std::string cmd = "bzip2 -d -k " + file;
        system(cmd.c_str());
    }

    void xzCompress(const std::string& file) {
        std::string cmd = "xz -k " + file;
        system(cmd.c_str());
    }

    void xzDecompress(const std::string& file) {
        std::string cmd = "xz -d -k " + file;
        system(cmd.c_str());
    }

public:
    SimpleShell() : running(true) {}

    void run() {
        showWelcome();
        std::string input;
        std::cout << "SimpleScript Shell v1.0\n";
        std::cout << "Type 'help' for available commands\n\n";

        while (running) {
            std::cout << "> ";
            std::getline(std::cin, input);

            if (!input.empty()) {
                std::vector<std::string> args = parseCommand(input);
                executeCommand(args);
            }
        }
    }
};

int main() {
    SimpleShell shell;
    shell.run();
    return 0;
}

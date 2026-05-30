#include "blocker.h"
#include <fstream>
#include <iostream>
#include <tlhelp32.h>
#include <sstream>

Blocker::Blocker() : isActive(false) {
    // Get Windows directory path
    char winDir[MAX_PATH];
    GetWindowsDirectoryA(winDir, MAX_PATH);
    hostsFilePath = std::string(winDir) + "\\System32\\drivers\\etc\\hosts";
    hostsBackupPath = std::string(winDir) + "\\System32\\drivers\\etc\\hosts.backup";
    
    // Check admin privileges
    hasAdminRights = CheckAdminPrivileges();
    
    // Default blocked websites
    blockedWebsites = {
        "tiktok.com",
        "www.tiktok.com",
    };
    
    // Default blocked processes
    blockedProcesses = {
        "chrome.exe",      // Can be selective
        "msedge.exe",
        "discord.exe",
        "slack.exe",
        "telegram.exe",
        "spotify.exe"
    };
}

Blocker::~Blocker() {
    if (isActive) {
        Deactivate();
    }
}

bool Blocker::CheckAdminPrivileges() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = NULL;
    SID_IDENTIFIER_AUTHORITY ntAuthority = SECURITY_NT_AUTHORITY;
    
    if (AllocateAndInitializeSid(&ntAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID,
                                  DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0, 0, &adminGroup)) {
        CheckTokenMembership(NULL, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }
    
    return isAdmin == TRUE;
}

bool Blocker::BackupHostsFile() {
    std::ifstream src(hostsFilePath, std::ios::binary);
    if (!src.is_open()) return false;
    
    std::ofstream dst(hostsBackupPath, std::ios::binary);
    if (!dst.is_open()) {
        src.close();
        return false;
    }
    
    dst << src.rdbuf();
    src.close();
    dst.close();
    
    return true;
}

bool Blocker::RestoreHostsFile() {
    std::ifstream src(hostsBackupPath, std::ios::binary);
    if (!src.is_open()) return false;
    
    std::ofstream dst(hostsFilePath, std::ios::binary);
    if (!dst.is_open()) {
        src.close();
        return false;
    }
    
    dst << src.rdbuf();
    src.close();
    dst.close();
    
    return true;
}

bool Blocker::ModifyHostsFile(bool addEntries) {
    if (!hasAdminRights) return false;
    
    if (addEntries) {
        // Append blocking entries
        std::ofstream hostsFile(hostsFilePath, std::ios::app);
        if (!hostsFile.is_open()) return false;
        
        hostsFile << "\n# Focus App - Website Blocking (DO NOT EDIT MANUALLY)\n";
        for (const auto& site : blockedWebsites) {
            hostsFile << "127.0.0.1 " << site << "\n";
        }
        hostsFile << "# End Focus App Blocking\n";
        hostsFile.close();
        
        // Flush DNS cache
        system("ipconfig /flushdns > nul 2>&1");
        return true;
    } else {
        // Remove blocking entries
        return RestoreHostsFile();
    }
}

bool Blocker::EnableWebsiteBlocking() {
    if (!hasAdminRights) return false;
    
    // Backup current hosts file
    if (!BackupHostsFile()) return false;
    
    // Add blocking entries
    return ModifyHostsFile(true);
}

bool Blocker::DisableWebsiteBlocking() {
    if (!hasAdminRights) return false;
    
    // Try to restore from backup first
    bool success = RestoreHostsFile();
    
    // If restore fails (backup doesn't exist), manually remove blocking entries
    if (!success) {
        std::ifstream inFile(hostsFilePath);
        if (!inFile.is_open()) return false;
        
        std::string line;
        std::vector<std::string> cleanedLines;
        bool inBlockSection = false;
        
        // Read and filter out Focus App blocking entries
        while (std::getline(inFile, line)) {
            if (line.find("# Focus App - Website Blocking") != std::string::npos) {
                inBlockSection = true;
                continue;
            }
            if (line.find("# End Focus App Blocking") != std::string::npos) {
                inBlockSection = false;
                continue;
            }
            if (!inBlockSection) {
                cleanedLines.push_back(line);
            }
        }
        inFile.close();
        
        // Write cleaned content back
        std::ofstream outFile(hostsFilePath);
        if (!outFile.is_open()) return false;
        
        for (const auto& cleanLine : cleanedLines) {
            outFile << cleanLine << "\n";
        }
        outFile.close();
        success = true;
    }
    
    // Flush DNS cache
    system("ipconfig /flushdns > nul 2>&1");
    
    return success;
}

void Blocker::AddWebsite(const std::string& website) {
    blockedWebsites.push_back(website);
}

void Blocker::RemoveWebsite(const std::string& website) {
    for (auto it = blockedWebsites.begin(); it != blockedWebsites.end(); ++it) {
        if (*it == website) {
            blockedWebsites.erase(it);
            break;
        }
    }
}

void Blocker::AddProcess(const std::string& processName) {
    blockedProcesses.push_back(processName);
}

void Blocker::RemoveProcess(const std::string& processName) {
    for (auto it = blockedProcesses.begin(); it != blockedProcesses.end(); ++it) {
        if (*it == processName) {
            blockedProcesses.erase(it);
            break;
        }
    }
}

bool Blocker::IsProcessRunning(const std::string& processName) {
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) return false;
    
    PROCESSENTRY32 entry;
    entry.dwSize = sizeof(PROCESSENTRY32);
    
    bool found = false;
    if (Process32First(snapshot, &entry)) {
        do {
            if (processName == entry.szExeFile) {
                found = true;
                break;
            }
        } while (Process32Next(snapshot, &entry));
    }
    
    CloseHandle(snapshot);
    return found;
}

bool Blocker::TerminateBlockedProcesses() {
    if (!hasAdminRights) return false;
    
    bool anyTerminated = false;
    
    for (const auto& processName : blockedProcesses) {
        if (IsProcessRunning(processName)) {
            // Use taskkill command
            std::string command = "taskkill /F /IM " + processName + " > nul 2>&1";
            system(command.c_str());
            anyTerminated = true;
        }
    }
    
    return anyTerminated;
}

bool Blocker::Activate() {
    if (!hasAdminRights) {
        return false;
    }
    
    if (isActive) return true;
    
    // Enable website blocking
    if (!EnableWebsiteBlocking()) {
        return false;
    }
    
    // Terminate blocked processes
    TerminateBlockedProcesses();
    
    isActive = true;
    return true;
}

bool Blocker::Deactivate() {
    if (!isActive) return true;
    
    // Disable website blocking - ensure it succeeds
    bool success = DisableWebsiteBlocking();
    
    isActive = false;
    
    if (!success) {
        std::cerr << "Warning: Failed to fully disable website blocking. Check hosts file manually.\n";
    }
    
    return success;
}

void Blocker::SaveCustomWebsites(const std::string& filename) {
    std::ofstream file(filename);
    if (!file.is_open()) return;
    
    for (const auto& website : blockedWebsites) {
        file << website << "\n";
    }
    
    file.close();
}

void Blocker::LoadCustomWebsites(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) return;
    
    // Clear current list and reload
    blockedWebsites.clear();
    
    std::string line;
    while (std::getline(file, line)) {
        if (!line.empty()) {
            blockedWebsites.push_back(line);
        }
    }
    
    file.close();
}

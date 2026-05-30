#ifndef BLOCKER_H
#define BLOCKER_H

#include <string>
#include <vector>
#include <windows.h>

class Blocker {
private:
    bool isActive;
    bool hasAdminRights;
    std::string hostsFilePath;
    std::string hostsBackupPath;
    
    std::vector<std::string> blockedWebsites;
    std::vector<std::string> blockedProcesses;

public:
    Blocker();
    ~Blocker();

    // Admin checks
    bool CheckAdminPrivileges();
    bool IsActive() const { return isActive; }
    bool HasAdminRights() const { return hasAdminRights; }

    // Website blocking (via hosts file)
    bool EnableWebsiteBlocking();
    bool DisableWebsiteBlocking();
    void AddWebsite(const std::string& website);
    void RemoveWebsite(const std::string& website);
    const std::vector<std::string>& GetBlockedWebsites() const { return blockedWebsites; }

    // App blocking (via process termination)
    void AddProcess(const std::string& processName);
    void RemoveProcess(const std::string& processName);
    const std::vector<std::string>& GetBlockedProcesses() const { return blockedProcesses; }
    bool TerminateBlockedProcesses();

    // Main control
    bool Activate();
    bool Deactivate();

    // Save/Load custom websites
    void SaveCustomWebsites(const std::string& filename);
    void LoadCustomWebsites(const std::string& filename);

private:
    bool BackupHostsFile();
    bool RestoreHostsFile();
    bool ModifyHostsFile(bool addEntries);
    bool IsProcessRunning(const std::string& processName);
};

#endif // BLOCKER_H

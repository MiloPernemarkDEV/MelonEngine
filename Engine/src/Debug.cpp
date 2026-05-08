//
// Created by milos on 2026-05-08.
//

#include "../include/Debug.h"

auto Debug::Log(const LogLevel logLevel, const char *msg) -> void {
    switch (logLevel)
    {
        case LogLevel::INFO:
            std::cout << ASCI_BLUE_INFO << "[INFO] " << __FILE__ << " [LINE: " << __LINE__ << "] " << msg << ASCI_RESET << "\n";
            break;
        case LogLevel::WARNING:
            std::cout << ASCI_YELLOW_WARNING << "[WARNING] " << __FILE__ << " [LINE: " << __LINE__ << "] " << msg << ASCI_RESET << "\n";
            break;
        case LogLevel::ERROR:
            std::cout << ASCI_RED_ERROR << "[ERROR] " << __FILE__ << " [LINE: " << __LINE__ << "] " << msg << ASCI_RESET << "\n";
            break;
        default:
            std::cout << msg << ASCI_RESET << "\n";
    }
}

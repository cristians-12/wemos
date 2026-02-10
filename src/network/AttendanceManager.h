#ifndef ATTENDANCE_MANAGER_H
#define ATTENDANCE_MANAGER_H

#include <Arduino.h>

class NetworkManager;
class DisplayManager;

class AttendanceManager {
private:
    NetworkManager* _network;
    DisplayManager* _display;
    const char* _cedula;

public:
    AttendanceManager(const char* cedula);
    
    void setNetwork(NetworkManager* network);
    void setDisplay(DisplayManager* display);
    
    // Extraer URL del token desde HTML
    String extractValidationUrl(const String& html);
    
    // Marcar asistencia
    bool markAttendance();
};

#endif
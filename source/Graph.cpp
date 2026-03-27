#include "../headers/Graph.h"
#include <iostream>
#include <iomanip>

CampusGraph::CampusGraph() {
    locationCount = 0;
    
    // Initialize adjacency matrix with "infinity" (no direct path)
    for (int i = 0; i < MAX_LOCATIONS; i++) {
        for (int j = 0; j < MAX_LOCATIONS; j++) {
            if (i == j) {
                adjMatrix[i][j] = 0;  // Distance to self is 0
            } else {
                adjMatrix[i][j] = INT_MAX;  // No path initially
            }
        }
    }
}

int CampusGraph::addLocation(string name) {
    if (locationCount >= MAX_LOCATIONS) {
        cout << "Error: Maximum locations reached!\n";
        return -1;
    }
    
    locations[locationCount] = name;
    return locationCount++;
}

void CampusGraph::addPath(int loc1, int loc2, int distance) {
    if (loc1 < 0 || loc1 >= locationCount || loc2 < 0 || loc2 >= locationCount) {
        return;
    }
    
    // Undirected graph - add path in both directions
    adjMatrix[loc1][loc2] = distance;
    adjMatrix[loc2][loc1] = distance;
}

void CampusGraph::addPath(string name1, string name2, int distance) {
    int loc1 = findLocation(name1);
    int loc2 = findLocation(name2);
    
    if (loc1 != -1 && loc2 != -1) {
        addPath(loc1, loc2, distance);
    }
}

int CampusGraph::findLocation(string name) {
    for (int i = 0; i < locationCount; i++) {
        if (locations[i] == name) {
            return i;
        }
    }
    return -1;
}

void CampusGraph::initializeGIKICampus() {
    // Add all locations
    int raju = addLocation("Raju Hotel");
    int h1 = addLocation("Boys Hostel H1");
    int h2 = addLocation("Boys Hostel H2");
    int h3 = addLocation("Boys Hostel H3");
    int h4 = addLocation("Boys Hostel H4");
    int h5 = addLocation("Boys Hostel H5");
    int h6 = addLocation("Boys Hostel H6");
    int h7 = addLocation("Girls Hostel H7");
    int h8 = addLocation("Boys Hostel H8");
    int h9 = addLocation("Boys Hostel H9");
    int h10 = addLocation("Boys Hostel H10");
    int h11 = addLocation("Boys Hostel H11");
    int h12 = addLocation("Boys Hostel H12");
    int h13 = addLocation("Girls Hostel H13");
    int gate = addLocation("Main Gate");
    int medical = addLocation("Medical Center");
    
    // Add paths with approximate distances (in meters)
    // Raju Hotel connections
    addPath(raju, h1, 150);
    addPath(raju, h2, 200);
    addPath(raju, h3, 250);
    addPath(raju, gate, 300);
    
    // Boys Hostels - connected in sequence
    addPath(h1, h2, 100);
    addPath(h2, h3, 100);
    addPath(h3, h4, 100);
    addPath(h4, h5, 100);
    addPath(h5, h6, 100);
    addPath(h6, h8, 150);
    addPath(h8, h9, 100);
    addPath(h9, h10, 100);
    addPath(h10, h11, 100);
    addPath(h11, h12, 100);
    
    // Girls Hostels
    addPath(raju, h7, 400);
    addPath(raju, h13, 450);
    addPath(h7, h13, 150);
    
    // Medical Center connections
    addPath(medical, h7, 200);
    addPath(medical, h13, 250);
    addPath(medical, h6, 300);
    addPath(medical, gate, 250);
    
    // Main Gate connections
    addPath(gate, h1, 200);
    addPath(gate, h12, 350);
    addPath(gate, medical, 250);
    
    // Cross connections for efficiency
    addPath(h3, h7, 500);
    addPath(h6, medical, 300);
    addPath(h9, gate, 300);
}

void CampusGraph::displayMap() {
    cout << "\n";
    cout << "========================================================================\n";
    cout << "                    GIKI CAMPUS MAP - RAJU HOTEL                        \n";
    cout << "========================================================================\n\n";
    
    cout << "                         Main Gate (300m)\n";
    cout << "                              |\n";
    cout << "                              |\n";
    cout << "    Medical Center -------- Raju Hotel (Kitchen)\n";
    cout << "         |  (250m)            |    |    |\n";
    cout << "         |                    |    |    |\n";
    cout << "    Girls H7 (200m)      150m 200m 250m\n";
    cout << "         |                    |    |    |\n";
    cout << "         |                    v    v    v\n";
    cout << "    Girls H13              H1--H2--H3--H4--H5--H6\n";
    cout << "      (150m)                  |                |\n";
    cout << "                              |              H8--H9--H10--H11--H12\n";
    cout << "                         Main Gate\n\n";
    
    cout << "------------------------------------------------------------------------\n";
    cout << "DELIVERY LOCATIONS:\n";
    cout << "------------------------------------------------------------------------\n";
    cout << left << setw(25) << "Location" << "Type\n";
    cout << "------------------------------------------------------------------------\n";
    cout << left << setw(25) << "Raju Hotel" << "Kitchen/Pickup Point\n";
    cout << left << setw(25) << "Boys Hostels H1-H6" << "Delivery Zone A\n";
    cout << left << setw(25) << "Boys Hostels H8-H12" << "Delivery Zone B\n";
    cout << left << setw(25) << "Girls Hostels H7, H13" << "Delivery Zone C\n";
    cout << left << setw(25) << "Main Gate" << "Campus Entry\n";
    cout << left << setw(25) << "Medical Center" << "Medical Facility\n";
    cout << "------------------------------------------------------------------------\n";
    
    cout << "\nTOTAL LOCATIONS: " << locationCount << "\n";
    cout << "========================================================================\n";
}


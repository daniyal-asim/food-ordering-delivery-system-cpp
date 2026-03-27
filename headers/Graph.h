#ifndef GRAPH_H
#define GRAPH_H

#include <string>
#include <climits>
using namespace std;

const int MAX_LOCATIONS = 20;

// Graph class - Campus Map using Adjacency Matrix
class CampusGraph {
private:
    int adjMatrix[MAX_LOCATIONS][MAX_LOCATIONS];  // Adjacency Matrix
    string locations[MAX_LOCATIONS];              // Location names
    int locationCount;

public:
    CampusGraph();
    
    // Add locations and paths
    void initializeGIKICampus();
    int addLocation(string name);
    void addPath(int loc1, int loc2, int distance);
    void addPath(string name1, string name2, int distance);
    
    // Find location index
    int findLocation(string name);
    
    // Display functions
    void displayMap();
    
    // Get location count
    int getLocationCount() const { return locationCount; }
    string getLocationName(int idx) const { return locations[idx]; }
};

#endif
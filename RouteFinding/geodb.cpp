//
//  geodb.cpp
//  Project4
//
//  Created by Kevin Sun on 3/9/24.
//

#include <stdio.h>
#include "geodb.h"
#include <iostream> // needed for any I/O
#include <fstream>  // needed in addition to <iostream> for file I/O
#include <sstream>  // needed in addition to <iostream> for string stream I/O

using namespace std;

GeoDatabase::GeoDatabase() {
}

GeoDatabase::~GeoDatabase() {
}

bool GeoDatabase::load(const std::string& map_data_file) {
    ifstream infile(map_data_file);
    if (!infile){
        cerr << "Cannot open the file!" << endl;
        return false;
    }
    //Variable for getting file info stuff
    string street;
    string LongStart;
    string LatStart;
    string LongEnd;
    string LatEnd;
    //The street segment put together
    string start;
    string end;
    string seg;
    
    int num;
    string poiName;
    string LongPoi;
    string LatPoi;
    
    while (getline(infile, street)) {
        infile >> LongStart;
        infile >> LatStart;
        infile >> LongEnd;
        infile >> LatEnd;
        start = LongStart + " " + LatStart;
        end = LongEnd + " " + LatEnd;
        seg = start + " | " + end;
        
        m_streetSegments.insert(seg, street);
        
        //Connections to start
        if (m_connections.find(start) == nullptr) {
            m_connections.insert(start, vector<GeoPoint>());
        }
        (*m_connections.find(start)).push_back(GeoPoint(LongEnd, LatEnd));
        
        //Connections to end
        if (m_connections.find(end) == nullptr) {
            m_connections.insert(end, vector<GeoPoint>());
        }
        (*m_connections.find(end)).push_back(GeoPoint(LongStart, LatStart));
        
        
        infile >> num;
        infile.ignore(10000, '\n');
        if (num > 0) {
            string poiLine;
            //midpoint
            GeoPoint mid = midpoint(GeoPoint(LongStart, LatStart), GeoPoint(LongEnd, LatEnd));
            string midStr = mid.sLatitude + " " + mid.sLongitude;
            //street segments for midpoint
            m_streetSegments.insert(start + " | " + midStr, street);
            m_streetSegments.insert(end + " | " + midStr, street);
            
            //make midpoint a connection to start and end point
            (*m_connections.find(start)).push_back(mid);
            (*m_connections.find(end)).push_back(mid);
            
            //create midpoint key for connection
            if (m_connections.find(midStr) == nullptr) {
                m_connections.insert(midStr, vector<GeoPoint>());
            }
            (*m_connections.find(midStr)).push_back(GeoPoint(LongStart, LatStart));
            (*m_connections.find(midStr)).push_back(GeoPoint(LongEnd, LatEnd));
            
            for (int i = 0; i < num; i++) {
                getline(infile, poiLine);
                poiName = poiLine.substr(0, poiLine.find("|"));
                poiLine.erase(0, poiLine.find("|") + 1);
                LongPoi = poiLine.substr(0, poiLine.find(" "));
                LatPoi = poiLine.substr(poiLine.find(" ") + 1);
                string poiStr = LongPoi + " " + LatPoi;
                
                //POI insert
                m_poi.insert(poiName, GeoPoint(LongPoi, LatPoi));
                
                //Create Path for poi to mid
                m_streetSegments.insert(poiStr + " | " + midStr, "a path");
                
                //Connections for poi and mid
                (*m_connections.find(midStr)).push_back(GeoPoint(LongPoi, LatPoi));
                if (m_connections.find(poiStr) == nullptr) {
                    m_connections.insert(poiStr, vector<GeoPoint>());
                }
                (*m_connections.find(poiStr)).push_back(mid);
            }
        }
    }
    return true;
}

bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const {
    if (m_poi.find(poi) != nullptr) {
        point = *m_poi.find(poi);
        return true;
    }
    return false;
}

vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const {
    if (m_connections.find(pt.sLatitude + " " + pt.sLongitude) != nullptr) {
        return *m_connections.find(pt.sLatitude + " " + pt.sLongitude);
    }
    return vector<GeoPoint>();
}

string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const {
    if (m_streetSegments.find(pt1.sLatitude + " " + pt1.sLongitude + " | " + pt2.sLatitude + " " + pt2.sLongitude) != nullptr) {
        return *m_streetSegments.find(pt1.sLatitude + " " + pt1.sLongitude + " | " + pt2.sLatitude + " " + pt2.sLongitude);
    }
    if (m_streetSegments.find(pt2.sLatitude + " " + pt2.sLongitude + " | " + pt1.sLatitude + " " + pt1.sLongitude) != nullptr) {
        return *m_streetSegments.find(pt2.sLatitude + " " + pt2.sLongitude + " | " + pt1.sLatitude + " " + pt1.sLongitude);
    }
    return "";
}

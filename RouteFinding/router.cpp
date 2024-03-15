//
//  router.cpp
//  Project4
//
//  Created by Kevin Sun on 3/9/24.
//

#include <stdio.h>
#include "router.h"
#include <stack>
#include "geotools.h"
using namespace std;

Router::Router(const GeoDatabaseBase& geo_db)
: googleMaps(geo_db)
{}

Router::~Router() {}

GeoPoint Router::strToGeo(string geo) const {
    string latitude = geo.substr(0, geo.find(" "));
    string longtitude = geo.substr(geo.find(" ") + 1);
    return GeoPoint(latitude, longtitude);
}

string Router::geoToStr(GeoPoint geo) const {
    return geo.sLatitude + " " + geo.sLongitude;
}

vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
    vector<GeoPoint> v;
    map<std::string, mapInfo> m_openMap;
    map<std::string, mapInfo> m_closedMap;
    mapInfo start;
    
    //starting point struct info
    start.m_parent = "";
    start.m_fValue = 0;
    start.m_gValue = 0;
    
    string beginning = geoToStr(pt1);
    string destination = geoToStr(pt2);
    
    //if start and end are same
    if (beginning == destination) {
        v.push_back(pt1);
        return v;
    }
    m_openMap.insert(pair<string, mapInfo>(beginning, start));
    
    while (!m_openMap.empty()) {
        
        //find smallest F in open map
        map<string, mapInfo>::iterator it = m_openMap.begin();
        double minF;
        string minKey;
        minF = it->second.m_fValue;
        minKey = it->first;
        it++;
        for (;it != m_openMap.end(); it++) {
            if (it->second.m_fValue < minF) {
                minF = it->second.m_fValue;
                minKey = it->first;
            }
        }
        
        it = m_openMap.find(minKey);
        
        //store q (smallest F) info and erase from open map
        string q = it->first;
        double qFvalue = it->second.m_fValue;
        double qGvalue = it->second.m_gValue;
        string qParent = it->second.m_parent;
        m_openMap.erase(it);
        
        //get q's successors
        vector<GeoPoint> successors = googleMaps.get_connected_points(strToGeo(q));
        
        //push q into closed map
        mapInfo qInfo;
        qInfo.m_fValue = qFvalue;
        qInfo.m_gValue = qGvalue;
        qInfo.m_parent = qParent;
        m_closedMap.insert(pair<string, mapInfo>(q, qInfo));
        
        //Loop through successors
        for (int i = 0; i < successors.size(); i++) {
            
            //if successor is the destination
            if(geoToStr(successors[i]) == destination) {
                stack<GeoPoint> s;
                s.push(successors[i]);
                string sucParent = q;
                while (sucParent != "") {
                    string curr = m_closedMap.find(sucParent)->first;
                    s.push(strToGeo(curr));
                    sucParent = m_closedMap.find(curr)->second.m_parent;
                }
                while (!s.empty()) {
                    v.push_back(s.top());
                    s.pop();
                }
                return v;
            }
            
            //successor not destination
            else {
                string sucStr = geoToStr(successors[i]);
                //Distances
                double sucGvalue = qGvalue + distance_earth_miles(strToGeo(q), successors[i]);
                double sucHvalue = distance_earth_miles(successors[i], pt2);
                double sucFvalue = sucGvalue + sucHvalue;
                
                bool canInsert = true;
                //Check if successor in open map
                if (m_openMap.find(sucStr)->first == sucStr) {
                    if (m_openMap.find(sucStr)->second.m_fValue < sucFvalue) {
                        canInsert = false;
                    }
                }
                //check if successor in closed map
                else if (m_closedMap.find(sucStr)->first == sucStr) {
                    if (m_closedMap.find(sucStr)->second.m_fValue < sucFvalue) {
                        canInsert = false;
                    }
                }
                
                //Insert successor in open map if u can
                if (canInsert) {
                    mapInfo sucInfo;
                    sucInfo.m_fValue = sucFvalue;
                    sucInfo.m_gValue = sucGvalue;
                    sucInfo.m_parent = q;
                    m_openMap.insert(pair<string, mapInfo>(sucStr, sucInfo));
                }
            }
        }
    }
    
    return v;
}

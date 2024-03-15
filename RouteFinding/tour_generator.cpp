//
//  tour_generator.cpp
//  Project4
//
//  Created by Kevin Sun on 3/9/24.
//

#include <stdio.h>
#include "tour_generator.h"
#include "geotools.h"
using namespace std;

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) 
: m_googleMaps(geodb), m_route(router)
{}

TourGenerator::~TourGenerator() {}

//Proceed form p1 to p2
TourCommand TourGenerator::proceedFunc(GeoPoint p1, GeoPoint p2) const {
    double degree = angle_of_line(p1, p2);
    string direction;
    if (degree >= 0 && degree < 22.5)
        direction = "east";
    else if (degree < 67.6)
        direction = "northeast";
    else if (degree < 112.5)
        direction = "north";
    else if (degree < 157.6)
        direction = "northwest";
    else if (degree < 202.5)
        direction = "west";
    else if (degree < 247.5)
        direction = "southwest";
    else if (degree < 292.5)
        direction = "south";
    else if (degree < 337.5)
        direction = "southeast";
    else
        direction = "east";
    double distance = distance_earth_miles(p1, p2);
    string streetname = m_googleMaps.get_street_name(p1, p2);
    TourCommand t;
    t.init_proceed(direction, streetname, distance, p1, p2);
    return t;
}


//check if can turn
bool TourGenerator::turnFunc(GeoPoint p1, GeoPoint p2, GeoPoint p3, TourCommand& t) const {
    
    //check if streets r same
    string street1 = m_googleMaps.get_street_name(p1, p2);
    string street2 = m_googleMaps.get_street_name(p2, p3);
    if (street1 == street2) {
        return false;
    }
    
    //check degree of turn
    double degree = angle_of_turn(p1, p2, p3);
    if (degree >= 1 && degree < 180) {
        t.init_turn("left", street2);
    }
    else if (degree >= 180 && degree <= 359) {
        t.init_turn("right", street2);
    }
    else {
        return false;
    }
    return true;
}

vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const {
    const Stops& instructions = stops;
    
    vector<TourCommand> resultInstructions;
    
    //Starting point info
    string firstPoi;
    string firstTalkingPoints;
    GeoPoint firstPoiLocation;
    //check if stops is empty
    if (!instructions.get_poi_data(0, firstPoi, firstTalkingPoints)) {
        return resultInstructions;
    }
    TourCommand t;
    //Commentate and push to instruction results
    t.init_commentary(firstPoi, firstTalkingPoints);
    resultInstructions.push_back(t);
    //Get geopoint of first POI
    if (!(m_googleMaps.get_poi_location(firstPoi, firstPoiLocation))) {
        return vector<TourCommand>();
    }
    
    //Loop through stops
    int index = 1;
    string currPoi;
    string currTalkingPoints;
    GeoPoint currPoiLocation;
    vector<GeoPoint> currRoute;
    while (instructions.get_poi_data(index, currPoi, currTalkingPoints)) {
        //get current poi
        if (!(m_googleMaps.get_poi_location(currPoi, currPoiLocation))) {
            return vector<TourCommand>();
        }
        
        //get route from last poi to current poi
        currRoute = m_route.route(firstPoiLocation, currPoiLocation);
        
        if (currRoute.size() == 1) {
            index++;
            continue;
        }
        
        for (int i = 1; i < currRoute.size() - 1; i++) {
            resultInstructions.push_back(proceedFunc(currRoute[i-1], currRoute[i]));
            TourCommand turn;
            if (turnFunc(currRoute[i-1], currRoute[i], currRoute[i+1], turn)) {
                resultInstructions.push_back(turn);
            }
        }
        resultInstructions.push_back(proceedFunc(currRoute[currRoute.size()-2], currRoute[currRoute.size()-1]));
        TourCommand second;
        second.init_commentary(currPoi, currTalkingPoints);
        resultInstructions.push_back(second);
        index++;
        firstPoiLocation = currPoiLocation;
    }
    return resultInstructions;
}




//
//  geodb.h
//  Project4
//
//  Created by Kevin Sun on 3/9/24.
//

#ifndef geodb_h
#define geodb_h

#include "base_classes.h"
#include "HashMap.h"
#include "geotools.h"

class GeoDatabase: public GeoDatabaseBase
{
  public:
    GeoDatabase();
    virtual ~GeoDatabase();
    virtual bool load(const std::string& map_data_file);
    virtual bool get_poi_location(const std::string& poi, GeoPoint& point) const;
    virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt) const;
    virtual std::string get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const;
  private:
    HashMap<string> m_streetSegments; //Key: BegLong + " " + BegLat + " | " + EndLong + " " + EndLat
    HashMap<GeoPoint> m_poi; //Key: Point of interest name
    HashMap<vector<GeoPoint>> m_connections; //Key: Long + " " Lat
};
#endif /* geodb_h */

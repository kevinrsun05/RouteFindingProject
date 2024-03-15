//
//  router.h
//  Project4
//
//  Created by Kevin Sun on 3/9/24.
//

#ifndef router_h
#define router_h

#include "base_classes.h"
#include <map>

class Router: public RouterBase
{
  public:
    Router(const GeoDatabaseBase& geo_db);
    virtual ~Router();
    virtual std::vector<GeoPoint> route(const GeoPoint& pt1,
    const GeoPoint& pt2) const;
  private:
    struct mapInfo {
        std::string m_parent;
        double m_fValue;
        double m_gValue;
    };
    GeoPoint strToGeo(std::string geo) const;
    std::string geoToStr(GeoPoint geo) const;
    const GeoDatabaseBase& googleMaps;
    
};

#endif /* router_h */

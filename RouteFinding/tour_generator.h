//
//  tour_generator.h
//  Project4
//
//  Created by Kevin Sun on 3/9/24.
//

#ifndef tour_generator_h
#define tour_generator_h

#include "base_classes.h"

class TourGenerator: public TourGeneratorBase
{
  public:
    TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
    virtual ~TourGenerator();
    virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
  private:
    TourCommand proceedFunc(GeoPoint p1, GeoPoint p2) const;
    bool turnFunc(GeoPoint p1, GeoPoint p2, GeoPoint p3, TourCommand& t) const;
    const GeoDatabaseBase& m_googleMaps;
    const RouterBase& m_route;
};

#endif /* tour_generator_h */

#include <cstdio>
#include <iomanip>
#include <iostream>
#include <map>
#include <vector>

#include "geodb.h"
#include "router.h"
#include "stops.h"
#include "tourcmd.h"
#include "tour_generator.h"
#include "HashMap.h"

using namespace std;

void print_tour(vector<TourCommand>& tcs)
{
    double total_dist = 0;
    std::string direction;
    double street_distance = 0;

    cout << "Starting tour...\n";

    for (size_t i = 0; i < tcs.size(); ++i)
    {
        if (tcs[i].get_command_type() == TourCommand::commentary)
        {
            cout << "Welcome to " << tcs[i].get_poi() << "!\n";
            cout << tcs[i].get_commentary() << "\n";
        }
        else if (tcs[i].get_command_type() == TourCommand::turn)
        {
            cout << "Take a " << tcs[i].get_direction() << " turn on " << tcs[i].get_street() << endl;
        }
        else if (tcs[i].get_command_type() == TourCommand::proceed)
        {
            total_dist += tcs[i].get_distance();
            if (direction.empty())
                direction = tcs[i].get_direction();
            street_distance += tcs[i].get_distance();
            if (i+1 < tcs.size() && tcs[i+1].get_command_type() == TourCommand::proceed
                && tcs[i+1].get_street() == tcs[i].get_street() && tcs[i].get_street() != "a path")
            {
                continue;
            }

            cout << "Proceed " << std::fixed << std::setprecision(3) << street_distance << " miles " << direction << " on " << tcs[i].get_street() << endl;
            street_distance = 0;
            direction.clear();
        }
    }

    cout << "Your tour has finished!\n";
    cout << "Total tour distance: " << std::fixed << std::setprecision(3) << total_dist << " miles\n";
}

int main()
{
    //"/Users/kevinsun_05/Desktop/UCLA/Year1/Quarter2/CS32/Project4/Project4/mapdata.txt"

        GeoDatabase geodb;

        if (!geodb.load("/Users/kevinsun_05/Desktop/UCLA/Year1/Quarter2/CS32/Project4/Project4/mapdata.txt"))
        {
            cout << "Unable to load map data: " << "mapdata.txt" << endl;
            return 1;
        }
    
//    Router r(geodb);
//    GeoPoint p1 = GeoPoint("34.0630614", "-118.4468781");
//    GeoPoint p2 = GeoPoint("34.0614911", "-118.4464410");
//    vector<GeoPoint> v = r.route(p1, p2);
//
        Router router(geodb);
        

        
        TourGenerator tg(geodb, router);

        Stops stops;
        
        stops.load("/Users/kevinsun_05/Desktop/UCLA/Year1/Quarter2/CS32/Project4/Project4/stops.txt");
        
        vector<TourCommand> tcs = tg.generate_tour(stops);
        
        print_tour(tcs);
 
}

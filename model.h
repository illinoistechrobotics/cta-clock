//
// Created by tberg on 4/13/18.
//

#ifndef CTA_CLOCK_PROVIDER_H
#define CTA_CLOCK_PROVIDER_H

#include <vector>
#include "matrix/include/graphics.h"

using std::vector;
using rgb_matrix::Color;

namespace itr::cta_clock::model {

    class Direction {
    public:

        explicit Direction(const char *destination);

        ~Direction();

        /**
         * The current set of times
         */
        vector<int> Times;

        /**
         * The destination
         */
        const char *Destination;
    };


    class Line {
    public:
        /**
         * @param identifier A short identifier for the line (2-3 chars)
         * @param name A longer name for the line
         * @param directions A list of all directions for this line from this stop
         */
        Line(const char *identifier, const char *name, vector<Direction> directions);

        /**
         * @param color A color that identifies the line
         * @param name A longer name for the line
         * @param directions A list of all directions for the line from this stop
         */
        Line(Color color, const char *name, vector<Direction> directions);

        /**
         * @param identifier A short identifier for the line (2-3 chars)
         * @param color A color that identifies the line
         * @param name A longer name for the line
         * @param directions A list of all directions for the line from this stop
         */
        Line(const char *identifier, Color color, const char *name, vector<Direction> directions);

        ~Line();

        /**
         * A list of directions with service times
         */
        vector<Direction> Directions;

        /**
         * Time when this Line last got fresh data
         */
        time_t LastUpdate;

        /**
         * A short identifier for the line (2-3 chars)
         */
        const char *Identifier;

        /**
         * A longer name for the line
         */
        const char *Name;

        /**
         * An identifying color for the line
         */
        Color Color;

    };


    class Provider {
    public:
        vector<Line> Lines;

        virtual bool Update() =0;
    };
}


#endif //CTA_CLOCK_PROVIDER_H

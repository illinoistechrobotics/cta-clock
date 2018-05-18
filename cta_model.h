//
// Created by tberg on 4/20/18.
//

#ifndef CTA_CLOCK_CTA_MODEL_H
#define CTA_CLOCK_CTA_MODEL_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include "model.h"

using itr::cta_clock::model::Provider;
using itr::cta_clock::model::Line;
using itr::cta_clock::model::Direction;

using rgb_matrix::Color;

using std::vector;

namespace itr {
    namespace cta_clock {
        namespace providers {
            namespace cta_rail {

                class CTALine : public Line {
                public:
                    CTALine(int stop_id, const char *identifier, const char *name, const vector<Direction> &directions);

                    /**
                     * @param color A color that identifies the line
                     * @param name A longer name for the line
                     * @param directions A list of all directions for the line from this stop
                     */
                    CTALine(int stop_id, const Color *color, const char *name, vector<Direction> directions);

                    /**
                     * @param identifier A short identifier for the line (2-3 chars)
                     * @param color A color that identifies the line
                     * @param name A longer name for the line
                     * @param directions A list of all directions for the line from this stop
                     */
                    CTALine(int stop_id, const char *identifier, const Color *color, const char *name, vector<Direction> directions);

                    /**
                     * The station's Stop ID for requesting data from CTA's API
                     */
                    int StopID;
                };

                class CTARailProvider : public Provider {
                public:
                    CTARailProvider(const char *key, const char *endpoint);

                    ~CTARailProvider();

                    bool Update() override;

                    const char *Key, *Endpoint;

                    vector<CTALine> Lines;
                };

                CTARailProvider *get_itr_cta_rail_lines();
            }
        }
    }
}
#endif //CTA_CLOCK_CTA_MODEL_H

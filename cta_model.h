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

namespace itr::cta_clock::providers::cta_rail {

    class CTARailProvider : public Provider {
    public:
        CTARailProvider(const char *key, const char *endpoint);

        ~CTARailProvider();

        bool Update() override;

        const char *Key, *Endpoint;

        vector<CTALine> Lines;
    };

    class CTALine : public Line {
    public:
        CTALine(int stop_id, const char *identifier, const char *name, const vector<Direction> &directions);

        /**
         * The station's Stop ID for requesting data from CTA's API
         */
        int StopID;
    };

    CTARailProvider *get_itr_cta_rail_lines();

}

#endif //CTA_CLOCK_CTA_MODEL_H

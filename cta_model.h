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

    class CTARailProvider : Provider {
        CTARailProvider(const char *key, const char *endpoint);

        ~CTARailProvider();
    };

    class CTALine : Line {
    };

    CTARailProvider *get_itr_cta_rail_lines();

}

#endif //CTA_CLOCK_CTA_MODEL_H

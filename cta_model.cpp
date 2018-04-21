//
// Created by tberg on 4/20/18.
//

#include "cta_model.h"

#include <curlpp/Easy.hpp>


namespace itr::cta_clock::providers::cta_rail {

    CTARailProvider::CTARailProvider(const char *key, const char *endpoint) {

    }

    CTARailProvider::~CTARailProvider() = default;

    CTARailProvider *get_itr_cta_rail_lines() {
        CTARailProvider *provider = new CTARailProvider();
        vector<Line> lines;

        // Red Line
        vector<Direction> redLineDirs;
        redLineDirs.emplace_back("Howard");
        redLineDirs.emplace_back("95th/Dan Ryan");
        lines.emplace_back(Color(255, 0, 0), "Red Line", redLineDirs);

        // Green Line
        vector<Direction> greenLineDirs;
        greenLineDirs.emplace_back("Harlem/Lake");
        greenLineDirs.emplace_back("63rd/Cottage Grove");
        greenLineDirs.emplace_back("63rd/Ashland");
        lines.emplace_back(Color(0, 255, 0), "Green Line", greenLineDirs);

        return provider;
    }

}
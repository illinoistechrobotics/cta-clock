//
// Created by tberg on 4/20/18.
//

#include "cta_model.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#define URL_BUFFER_SIZE 512;
#define RESPONSE_BUFFER_SIZE 65535;

using std::cout;
using std::endl;

namespace itr::cta_clock::providers::cta_rail {

    CTARailProvider::CTARailProvider(const char *key, const char *endpoint) {
        this->Key = key;
        this->Endpoint = endpoint;
    }

    bool CTARailProvider::Update() {
        try {
            for (auto &i : this->Lines) {
                // Build the URL
                std::string url;
                std::stringstream response;

                sprintf(url, "%s?stpid=%d&key=%s&outputType=JSON", this->Endpoint, i.StopID, this->Key);

                // Make the request
                response << curlpp::options::Url(url);


            }
        } catch (curlpp::RuntimeError &e) {
            std::cout << e.what() << std::endl;
        } catch (curlpp::LogicError &e) {
            std::cout << e.what() << std::endl;
        }

    }

    CTARailProvider::~CTARailProvider() = default;

    CTARailProvider *get_itr_cta_rail_lines() {
        auto provider = new CTARailProvider(getenv("CTA_RAIL_KEY"),
                                            "http://lapi.transitchicago.com/api/1.0/ttarrivals.aspx");
        vector<CTALine> lines;

        // Red Line
        vector<Direction> redLineDirs;
        redLineDirs.emplace_back("Howard");
        redLineDirs.emplace_back("95th/Dan Ryan");
        lines.emplace_back(40190, new Color(255, 0, 0), "Red Line", redLineDirs);

        // Green Line
        vector<Direction> greenLineDirs;
        greenLineDirs.emplace_back("Harlem/Lake");
        greenLineDirs.emplace_back("63rd/Cottage Grove");
        greenLineDirs.emplace_back("63rd/Ashland");
        lines.emplace_back(41120, new Color(0, 255, 0), "Green Line", greenLineDirs);

        provider->Lines = lines;

        return provider;
    }

    CTALine::CTALine(const int map_id, const char *identifier, const char *name, const vector<Direction> &directions) :
            Line(identifier, name, directions) {
        this->StopID = map_id;
    }
}
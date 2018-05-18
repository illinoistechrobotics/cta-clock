//
// Created by tberg on 4/20/18.
//

#include <sstream>
#include <iostream>
#include "cta_model.h"

#include <curlpp/cURLpp.hpp>
#include <curlpp/Easy.hpp>
#include <curlpp/Options.hpp>

#define URL_BUFFER_SIZE 512;
#define RESPONSE_BUFFER_SIZE 65535;

using std::cout;
using std::endl;
using std::string;

using namespace itr::cta_clock::providers::cta_rail;

CTARailProvider::CTARailProvider(const char *key, const char *endpoint) {
    this->Key = key;
    this->Endpoint = endpoint;
}

bool CTARailProvider::Update() {
    try {
        for (auto &i : this->Lines) {
            // Build the URL
            std::ostringstream url_s, response;

            url_s << this->Endpoint << "?stpid=" << i.StopID << "&key=" << this->Key << "&outputType=JSON";

            // Make the request
            curlpp::Easy myRequest;
            curlpp::options::Url url(url_s.str());
            myRequest.setOpt(url);
            response << myRequest;
        }
    } catch (curlpp::RuntimeError &e) {
        std::cout << e.what() << std::endl;
    } catch (curlpp::LogicError &e) {
        std::cout << e.what() << std::endl;
    }

}

CTARailProvider::~CTARailProvider() = default;

CTARailProvider *itr::cta_clock::providers::cta_rail::get_itr_cta_rail_lines() {
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

CTALine::CTALine(int stop_id, const char *identifier, const char *name, const vector<Direction> &directions) :
        Line(identifier, name, directions) {
    this->StopID = stop_id;
}

CTALine::CTALine(int stop_id, const Color *color, const char *name, vector<Direction> directions) : Line(color, name, directions) {
    this->StopID = stop_id;
}

CTALine::CTALine(int stop_id, const char *identifier, const Color *color, const char *name,
                 vector<Direction> directions) : Line(identifier, color, name, directions) {
    this->StopID = stop_id;
}
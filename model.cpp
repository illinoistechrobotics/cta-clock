//
// Created by tberg on 4/13/18.
//

#include "model.h"

using rgb_matrix::Color;
using itr::cta_clock::model::Direction;
using itr::cta_clock::model::Line;

itr::cta_clock::model::Direction::Direction(const char *destination) {
    this->Destination = destination;
}

itr::cta_clock::model::Direction::~Direction() {
    delete this->Destination;
}

itr::cta_clock::model::Line::Line(const char *identifier, const char *name, vector<Direction> directions) {
    srandom((unsigned int)time(nullptr));
    this->Identifier = identifier;
    this->Name = name;
    this->LineColor = new Color((uint8_t)random(), (uint8_t)random(), (uint8_t)random());
    this->Directions = directions;
}

itr::cta_clock::model::Line::Line(const char *identifier, const Color *color, const char *name,
                                  vector<Direction> directions) {
    this->Identifier = identifier;
    this->Name = name;
    this->LineColor = color;
    this->Directions = directions;
}

itr::cta_clock::model::Line::Line(const Color *color, const char *name, vector<Direction> directions) {
    this->Identifier = nullptr;
    this->Name = name;
    this->LineColor = color;
    this->Directions = directions;
}

itr::cta_clock::model::Line::~Line() = default;
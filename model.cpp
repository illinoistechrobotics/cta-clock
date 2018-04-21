//
// Created by tberg on 4/13/18.
//

#include "model.h"

using itr::cta_clock::model::Direction
using itr::cta_clock::model::Line

itr::cta_clock::model::Direction::Direction(const char *destination) {
    this->Destination = destination;
    this->Times = vector<int>();
}

itr::cta_clock::model::Direction::~Direction() {
    delete this->Times;
    delete this->Destination;
}

itr::cta_clock::model::Line::Line(const char *identifier, const char *name, vector<Direction> directions) {
    this->Identifier = identifier;
    this->Name = name;
    this->Color = Color(255, 255, 255);
    this->Directions = directions;
}

itr::cta_clock::model::Line::Line(const char *identifier, Color color, const char *name, vector<Direction> directions) {
    this->Identifier = identifier;
    this->Name = name;
    this->Color = color;
    this->Directions = directions;
}

itr::cta_clock::model::Line::Line(const Color color, const char *name, vector<Direction> directions) {
    this->Identifier = nullptr;
    this->Name = name;
    this->Color = color;
    this->Directions = directions;
}

itr::cta_clock::model::Line::~Line() {
    delete this->Directions;
}
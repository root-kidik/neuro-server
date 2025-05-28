#pragma once

#include <string>

#include <opencv4/opencv2/core/types.hpp>

namespace yolo
{

enum class TargetType : std::uint8_t
{
    Person,
    Bicycle,
    Car,
    Motorcycle,
    Airplane,
    Bus,
    Train,
    Truck,
    Boat,
    TrafficLight,
    FireHydrant,
    StopSign,
    ParkingMeter,
    Bench,
    Bird,
    Cat,
    Dog,
    Horse,
    Sheep,
    Cow,
    Elephant,
    Bear,
    Zebra,
    Giraffe,
    Backpack,
    Umbrella,
    Handbag,
    Tie,
    Suitcase,
    Frisbee,
    Skis,
    Snowboard,
    SportsBall,
    Kite,
    BaseballBat,
    BaseballGlove,
    Skateboard,
    Surfboard,
    TennisRacket,
    Bottle,
    WineGlass,
    Cup,
    Fork,
    Knife,
    Spoon,
    Bowl,
    Banana,
    Apple,
    Sandwich,
    Orange,
    Broccoli,
    Carrot,
    HotDog,
    Pizza,
    Donut,
    Cake,
    Chair,
    Couch,
    PottedPlant,
    Bed,
    DiningTable,
    Toilet,
    TV,
    Laptop,
    Mouse,
    Remote,
    Keyboard,
    CellPhone,
    Microwave,
    Oven,
    Toaster,
    Sink,
    Refrigerator,
    Book,
    Clock,
    Vase,
    Scissors,
    TeddyBear,
    HairDrier,
    Toothbrush,
};

inline std::string TargetTypeToString(TargetType type)
{
    switch (type)
    {
        case TargetType::Person:
            return "Person";
        case TargetType::Bicycle:
            return "Bicycle";
        case TargetType::Car:
            return "Car";
        case TargetType::Motorcycle:
            return "Motorcycle";
        case TargetType::Airplane:
            return "Airplane";
        case TargetType::Bus:
            return "Bus";
        case TargetType::Train:
            return "Train";
        case TargetType::Truck:
            return "Truck";
        case TargetType::Boat:
            return "Boat";
        case TargetType::TrafficLight:
            return "TrafficLight";
        case TargetType::FireHydrant:
            return "FireHydrant";
        case TargetType::StopSign:
            return "StopSign";
        case TargetType::ParkingMeter:
            return "ParkingMeter";
        case TargetType::Bench:
            return "Bench";
        case TargetType::Bird:
            return "Bird";
        case TargetType::Cat:
            return "Cat";
        case TargetType::Dog:
            return "Dog";
        case TargetType::Horse:
            return "Horse";
        case TargetType::Sheep:
            return "Sheep";
        case TargetType::Cow:
            return "Cow";
        case TargetType::Elephant:
            return "Elephant";
        case TargetType::Bear:
            return "Bear";
        case TargetType::Zebra:
            return "Zebra";
        case TargetType::Giraffe:
            return "Giraffe";
        case TargetType::Backpack:
            return "Backpack";
        case TargetType::Umbrella:
            return "Umbrella";
        case TargetType::Handbag:
            return "Handbag";
        case TargetType::Tie:
            return "Tie";
        case TargetType::Suitcase:
            return "Suitcase";
        case TargetType::Frisbee:
            return "Frisbee";
        case TargetType::Skis:
            return "Skis";
        case TargetType::Snowboard:
            return "Snowboard";
        case TargetType::SportsBall:
            return "SportsBall";
        case TargetType::Kite:
            return "Kite";
        case TargetType::BaseballBat:
            return "BaseballBat";
        case TargetType::BaseballGlove:
            return "BaseballGlove";
        case TargetType::Skateboard:
            return "Skateboard";
        case TargetType::Surfboard:
            return "Surfboard";
        case TargetType::TennisRacket:
            return "TennisRacket";
        case TargetType::Bottle:
            return "Bottle";
        case TargetType::WineGlass:
            return "WineGlass";
        case TargetType::Cup:
            return "Cup";
        case TargetType::Fork:
            return "Fork";
        case TargetType::Knife:
            return "Knife";
        case TargetType::Spoon:
            return "Spoon";
        case TargetType::Bowl:
            return "Bowl";
        case TargetType::Banana:
            return "Banana";
        case TargetType::Apple:
            return "Apple";
        case TargetType::Sandwich:
            return "Sandwich";
        case TargetType::Orange:
            return "Orange";
        case TargetType::Broccoli:
            return "Broccoli";
        case TargetType::Carrot:
            return "Carrot";
        case TargetType::HotDog:
            return "HotDog";
        case TargetType::Pizza:
            return "Pizza";
        case TargetType::Donut:
            return "Donut";
        case TargetType::Cake:
            return "Cake";
        case TargetType::Chair:
            return "Chair";
        case TargetType::Couch:
            return "Couch";
        case TargetType::PottedPlant:
            return "PottedPlant";
        case TargetType::Bed:
            return "Bed";
        case TargetType::DiningTable:
            return "DiningTable";
        case TargetType::Toilet:
            return "Toilet";
        case TargetType::TV:
            return "TV";
        case TargetType::Laptop:
            return "Laptop";
        case TargetType::Mouse:
            return "Mouse";
        case TargetType::Remote:
            return "Remote";
        case TargetType::Keyboard:
            return "Keyboard";
        case TargetType::CellPhone:
            return "CellPhone";
        case TargetType::Microwave:
            return "Microwave";
        case TargetType::Oven:
            return "Oven";
        case TargetType::Toaster:
            return "Toaster";
        case TargetType::Sink:
            return "Sink";
        case TargetType::Refrigerator:
            return "Refrigerator";
        case TargetType::Book:
            return "Book";
        case TargetType::Clock:
            return "Clock";
        case TargetType::Vase:
            return "Vase";
        case TargetType::Scissors:
            return "Scissors";
        case TargetType::TeddyBear:
            return "TeddyBear";
        case TargetType::HairDrier:
            return "HairDrier";
        case TargetType::Toothbrush:
            return "Toothbrush";
        default:
            assert(false && "unknown target type");
    }

    return "";
}

struct YoloPrediction
{
    float      confidence;
    TargetType target_type;
    cv::Rect   box;
};

} // namespace yolo

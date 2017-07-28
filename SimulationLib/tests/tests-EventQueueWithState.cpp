#include "catch.hpp"

#include <map>
#include <string>

// Provides "compare_files" function for comparing output of CSVExporter's
//   to reference output
#include "../include/SimulationLib/EventQueue.h"
#include "./Person.h"

using namespace std;
using namespace SimulationLib;

vector<Person> Commmunity;

using EQ = EventQueue<int, bool>;
using EventFunc = EQ::EventFunc;
using SchedulerT = EQ::SchedulerT;

EventFunc genAdopt(int personIdx) {
    return [personIdx](int t, SchedulerT scheduler) {
        printf("[t=%2d] ", t);
        printf("Adopt a dog!\n");

        Person per;
        per = Commmunity.at(personIdx);
        printf("You started with %d dogs\n", per.nDogs);

        Commmunity[personIdx] = incrementDogs(per, 1);
        per = Commmunity.at(personIdx);

        printf("You now have %d dogs\n", per.nDogs);

        return true;
    };
}

EventFunc genColorChange(int personIdx) {
    return [personIdx](int t, SchedulerT scheduler) {
        printf("[t=%2d] ", t);
        printf("Change your favorite color!\n");

        Person per;
        per = Commmunity.at(personIdx);

        string oldFavorite;
        switch(per.color) {
            case FavoriteColor::Blue  :
                oldFavorite = "blue";
                break;
            case FavoriteColor::Red   :
                oldFavorite = "red";
                break;
            case FavoriteColor::Green :
                oldFavorite = "green";
                break;
            default :
                oldFavorite = "undefined";
                break;
        }

        printf("Your favorite color was: %s\n", oldFavorite.c_str());

        FavoriteColor newColor;
        switch(per.color) {
            case FavoriteColor::Blue  :
                newColor = FavoriteColor::Red;
                break;
            case FavoriteColor::Red   :
                newColor = FavoriteColor::Green;
                break;
            case FavoriteColor::Green :
                newColor = FavoriteColor::Blue;
                break;
        }

        Commmunity[personIdx] = changeFavoriteColor(per, newColor);
        per = Commmunity.at(personIdx);

        string newFavorite;
        switch(per.color) {
            case FavoriteColor::Blue  :
                newFavorite = "blue";
                break;
            case FavoriteColor::Red   :
                newFavorite = "red";
                break;
            case FavoriteColor::Green :
                newFavorite = "green";
                break;
        }

        printf("Your favorite is now: %s\n", newFavorite.c_str());

        return true;
    };
}

TEST_CASE("Check if object variables can get altered", "[csv]") {
    EQ eq{};

    Commmunity.push_back(newPerson(2,FavoriteColor::Blue));

    eq.Schedule(eq.MakeScheduledEvent(0, genAdopt(0)));
    eq.Schedule(eq.MakeScheduledEvent(1, genAdopt(0)));
    eq.Schedule(eq.MakeScheduledEvent(2, genColorChange(0)));
    eq.Schedule(eq.MakeScheduledEvent(3, genColorChange(0)));

    auto e1 = eq.Top();
    REQUIRE(e1.run());
    eq.Pop();

    auto e2 = eq.Top();
    REQUIRE(e2.run());
    eq.Pop();

    auto e3 = eq.Top();
    REQUIRE(e3.run());
    eq.Pop();

    auto e4 = eq.Top();
    REQUIRE(e4.run());
    eq.Pop();
}

TEST_CASE("Test while loop", "[csv]") {
    EQ eq{};

    Commmunity.push_back(newPerson(2,FavoriteColor::Blue));

    eq.Schedule(eq.MakeScheduledEvent(0, genAdopt(0)));
    eq.Schedule(eq.MakeScheduledEvent(1, genAdopt(0)));
    eq.Schedule(eq.MakeScheduledEvent(2, genColorChange(0)));
    eq.Schedule(eq.MakeScheduledEvent(3, genColorChange(0)));

    while(!eq.Empty()) {
        auto e = eq.Top();
        REQUIRE(e.run());
        eq.Pop();
    }
}

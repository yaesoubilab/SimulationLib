#include "catch.hpp"

#include <map>
#include <string>

// Provides "compare_files" function for comparing output of CSVExporter's
//   to reference output
#include "../include/SimulationLib/EventQueue.h"

using namespace std;
using namespace SimulationLib;

using EQ = EventQueue<int, bool>;
using SchedulerT = EQ::SchedulerT;
using EventFunc = EQ::EventFunc;

EventFunc genHello(int numHellos) {
    return [numHellos](int t, SchedulerT scheduler) {
        string msg = string("Hello!");
        int nHellos = numHellos;

        while (nHellos != 0) {
            msg += string(" Hello?");
            nHellos -= 1;
        }

        printf("[t=%2d] ", t);
        printf("%s", msg.c_str());
        printf("\n");

        return true;
    };
}

EventFunc genGoodbye(int numGoodbyes) {
    return [numGoodbyes](int t, SchedulerT scheduler) {
        string msg = string("Goodbye!");
        int nGoodbyes = numGoodbyes;

        while (nGoodbyes != 0) {
            msg += string(" Goodbye?");
            nGoodbyes -= 1;
        }

        printf("[t=%2d] ", t);
        printf("%s", msg.c_str());
        printf("\n");

        return true;
    };
}

TEST_CASE("Basic EventQueue: does it compile!?", "[csv]") {
    EQ eq{};

    eq.Schedule(eq.MakeScheduledEvent(0, genHello(1)));
    eq.Schedule(eq.MakeScheduledEvent(0, genGoodbye(1)));
}

TEST_CASE("Basic adding, running, and popping events", "[csv]") {
    EQ eq{};

    REQUIRE(eq.Empty() == true);

    eq.Schedule(eq.MakeScheduledEvent(0, genHello(2)));
    REQUIRE(eq.Empty() == false);

    eq.Schedule(eq.MakeScheduledEvent(1, genGoodbye(2)));

    auto e1 = eq.Top();
    REQUIRE(e1->run());
    eq.Pop();
    REQUIRE(eq.Empty() == false);

    auto e2 = eq.Top();
    REQUIRE(e2->run());
    eq.Pop();
    REQUIRE(eq.Empty() == true);
}

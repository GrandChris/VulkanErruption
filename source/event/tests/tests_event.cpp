//
// \file       testEvent.h
// \author     GrandChris
// \date       2020-10-24
// \brief      Test of the class Event
//

#include "event.h"

#include <gtest/gtest.h>

    class SimpleClass {
        public:
        void func(int const & val) {mVal = val;};

        void operator()(int val) {mVal = val;};

        int getVal() const {
            return mVal;
        }

        private:
            int mVal = 0;
    };

static int aVal = 0;
void func(int const & val) {aVal = val;};


TEST(Event, simple)
{
    

    SimpleClass simpleClass;
    Event<void(int)> event;
    event.add<&SimpleClass::func>(simpleClass);
    event.add(simpleClass);
    event.add<&func>();

    event(4711);

    EXPECT_EQ(simpleClass.getVal(), 4711);
    EXPECT_EQ(aVal, 4711);
}
#define BOOST_TEST_MODULE EntityTesting
#include <boost/test/unit_test.hpp>
#include <iostream>
#include <chrono>

#include "Signal.h"

bool gFlag = false;


owls::Signal<bool> emitter;


//free function test
void onFlagChanged(bool flag)
{
    gFlag = flag;
}

//remove observer from within callback
void onFlagChanged2(bool flag)
{
    gFlag = flag;
    emitter.disconnectAll();
}


//these methods are requested later
int giveVal1()
{
    return 4;
}

int giveVal2()
{
    return 10090;
}

BOOST_AUTO_TEST_CASE( signal_functional )
{
    auto link = emitter.connect(onFlagChanged);
    emitter(true);
    BOOST_REQUIRE(gFlag);
    emitter(false);
    BOOST_REQUIRE(!gFlag);
    link.disconnect();
    emitter(true);
    BOOST_REQUIRE(!gFlag);

    auto link1 = emitter.connect(onFlagChanged);
    auto link2 = emitter.connect(onFlagChanged2);
    BOOST_CHECK_EQUAL(emitter.callbackCount(), 2u);
    emitter(true);
    BOOST_REQUIRE(gFlag);
    link1.disconnect();
    link2.disconnect();
    BOOST_CHECK_EQUAL(emitter.callbackCount(), 1u);
    emitter(false);
    BOOST_CHECK_EQUAL(emitter.callbackCount(), 0u);
    BOOST_REQUIRE(gFlag);

    //emit from within callback
    gFlag = false;
    emitter.connect(onFlagChanged);
    emitter.connect([] (owls::SignalLink<void, bool> link, bool flag)
    {
         //disconnect to avoid permaloop
         link.disconnect();
         emitter(flag);
    });
    emitter(true);
    BOOST_REQUIRE(gFlag);
    BOOST_CHECK_EQUAL(emitter.callbackCount(), 1u);
    emitter.disconnectAll();
}

BOOST_AUTO_TEST_CASE( request_functional )
{
    owls::Request<int()> request;
    request.connect(giveVal1);
    request.connect(giveVal2);
    BOOST_CHECK_EQUAL(request([] (int a, int b) {return a<b;}), giveVal1());
    BOOST_CHECK_EQUAL(request([] (int a, int b) {return a>b;}), giveVal2());
}

BOOST_AUTO_TEST_CASE( signal_performance )
{
    //emitter.connect(onFlagChanged);
    std::cout << "Tesing signal speed............." << std::endl << std::endl;
    std::chrono::steady_clock::time_point begintime = std::chrono::steady_clock::now();
    emitter(true);
    std::chrono::steady_clock::time_point endtime = std::chrono::steady_clock::now();
    std::cout << "done (in " << std::chrono::duration_cast<std::chrono::nanoseconds>(endtime - begintime).count() << " ns)" << std::endl << std::endl;


    std::cout << "Tesing plain callback............." << std::endl << std::endl;
    std::function<void(bool)> callback = onFlagChanged;
    begintime = std::chrono::steady_clock::now();
    callback(true);
    endtime = std::chrono::steady_clock::now();
    std::cout << "done (in " << std::chrono::duration_cast<std::chrono::nanoseconds>(endtime - begintime).count() << " ns)" << std::endl << std::endl;
}

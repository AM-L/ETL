#define CATCH_CONFIG_MAIN
#include <catch.hpp>
#define ETLSTD etlstd

/*
namespace etlTest {
#define __Mock_Mock__
#include <etl/ioports.h>
#include <etl/architecture/uart_Mock.h>
#include <libstd/include/algorithm>
#include <libstd/include/thread>
} // namespace etlTest



using namespace etlTest::etl;
*/


/*
SCENARIO("GPIO directions", "[DDR]") {
    GIVEN("an MCU with default output gpios") {
        Device::initialize();
        Port0::setOutput(0b1111111111111111);
        REQUIRE(MockDevice::getInstance().readRegister(Device::DDR0) == 0b1111111111111111);
        WHEN("ports are set to input") {
            Pin0::setInput();
            Pin2::setInput();
            THEN("the direction register changes") {
                //REQUIRE(MockDevice::getInstance().readRegister(Device::DDR0) == 0b1111111111111010);
            }
        }
    }
}
*/
/*
template<typename Timer>
class TimerDutyCycle {
public:
    static void Init(uint8_t dutyCyclePercent) {
        Interrupts::Disable();
        Timer::
        Interrupts::Enable();
    }
}

SCENARIO("Timers") {
    
}



namespace etl {
struct FakePin {
    static void setOutput() {

    }

    static void set() {
        std::cout << '1';
    }

    static void clear() {
        std::cout << '0';
    }


};

struct FakePinReader {
    static void setInput() {

    }

   static void set() {
    }

    static void clear() {
    }


    static bool test() {
        if (index > 11) {
            index = 0;
        }
        return toRead[index++];
    }

   
    static const bool toRead[];
    static uint8_t index;
};

//const bool FakePinReader::toRead[] = { false,true,true,true,true,false,true,true,false,false,true };
const bool FakePinReader::toRead[] = { false,true,true,false,false,true,true,true,true };
uint8_t FakePinReader::index = 0;
template<> struct is_uart_txd_capable<FakePin> : std::true_type {};
template<> struct is_uart_rxd_capable<FakePinReader> : std::true_type {};
}

void ack1()
{
    using uart = etl::Uart<Pin0, etl::FakePin>;
    while (true) {
        uart::read();
    }
}

void ack2()
{
    using uart2 = etl::Uart<Pin0, FakePin, 14500, FrameFormat::_5E2, uint8_t>;
    while(true){
        uart2::read();
    }
}


SCENARIO("Uart") {
    
    using uart = etl::Uart<Pin0, etl::FakePin>;
    uart::start();
    std::thread t1(ack1);
    uart::write(2);
    uart::write(5);
    uart::write(7);
    t1.join();
   
}


SCENARIO("Uart 2") {

    using uart2 = etl::Uart<Pin0, FakePin, 14500, FrameFormat::_5E2, uint8_t>;
    uart2::start();
    std::thread t2(ack2);
    uart2::write(2);
    uart2::write(5);
    uart2::write(7);
    t2.join();
}



SCENARIO("Uart Read") {
    using uart = etl::Uart<FakePinReader, etl::Pin14>;
    uart::start();
    uint8_t value = uart::read();
    REQUIRE(value == 0b11110110);

}

SCENARIO("Uart Read2") {
    using uart = etl::Uart<FakePinReader, etl::Pin14, 14500, FrameFormat::_5E2, uint8_t>;
    uart::start();
    uint8_t value = uart::read();
    REQUIRE(value == 0b11001);

}*/
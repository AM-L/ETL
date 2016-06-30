/// @file ioports_ATmega32U4.h
/// @date 11/05/2014 19:28:16
/// @author Ambroise Leclerc and Cécile Gomes
/// @brief Atmel AVR 8-bit microcontrollers peripherals handling classes
//
// Copyright (c) 2016, Ambroise Leclerc and Cécile Gomes
//   All rights reserved.
//
//   Redistribution and use in source and binary forms, with or without
//   modification, are permitted provided that the following conditions are met:
//
//   * Redistributions of source code must retain the above copyright
//     notice, this list of conditions and the following disclaimer.
//   * Redistributions in binary form must reproduce the above copyright
//     notice, this list of conditions and the following disclaimer in
//     the documentation and/or other materials provided with the
//     distribution.
//   * Neither the name of the copyright holders nor the names of
//     contributors may be used to endorse or promote products derived
//     from this software without specific prior written permission.
//
//  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 'AS IS' 
//  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
//  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
//  ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE
//  LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
//  CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
//  SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
//  INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
//  CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
//  ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//  POSSIBILITY OF SUCH DAMAGE.
#pragma once

#include <util/delay.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <chrono>

extern void __builtin_avr_delay_cycles(unsigned long);

namespace etl {
#define IOPORTS_TO_STRING(name) #name
#define IOPORTS_IRQ_HANDLER(vector, type) asm(IOPORTS_TO_STRING(vector)) __attribute__ ((type, __INTR_ATTRS))

class Device {
public:
    static void delayTicks(uint32_t ticks)            { __builtin_avr_delay_cycles(ticks); }
    static const auto flashSize = 32768;
    static const auto eepromSize = 1024;
    static const auto sramSize = 2560;
    static const auto architectureWidth = 8;
    static const uint32_t McuFrequency = F_CPU;

    /// Enables interrupts by setting the global interrupt mask.
    /// This function generates a single 'sei' instruction with
    /// no overhead.
    static void enableInterrupts() { asm volatile("sei" ::: "memory"); }

    /// Disables interrupts by clearing the global interrupt mask.
    /// This function generates a single 'cli' instruction with
    /// no overhead.
    static void disableInterrupts() { asm volatile("cli" ::: "memory"); }
};

using clock_cycles = std::chrono::duration<unsigned long, std::ratio<1, Device::McuFrequency>>;
constexpr clock_cycles operator ""clks(unsigned long long c)     { return clock_cycles(static_cast<clock_cycles::rep>(c)); }
struct PinChangeIRQ0;

struct PortB {
    using PinChangeIRQ = PinChangeIRQ0;

    /// Assigns a value to PORTB
    /// @param[in] value value affected to PORTB
    static void assign(uint8_t value)   { PORTB = value; }

    /// Sets masked bits in PORTB
    /// @param[in] mask bits to set
    static void setBits(uint8_t mask)   { PORTB |= mask;}

    /// Clears masked bits in PORTB
    /// @param[in] mask bits to clear
    static void clearBits(uint8_t mask) { PORTB &= ~mask;} 

    /// Changes values of masked bits in PORTB
    /// @param[in] mask bits to change
    /// @param[in] value new bits values
    static void changeBits(uint8_t mask, uint8_t value) { uint8_t tmp = PORTB & ~mask; PORTB = tmp | value; } 

    /// Toggles masked bits in PORTB
    /// @param[in] mask bits to toggle
    static void toggleBits(uint8_t mask) { PORTB ^= mask;} 

    /// Pulses masked bits in PORTB with high state first.
    /// @param[in] mask bits to pulse
    static void pulseHigh(uint8_t mask) { PORTB |= mask; PORTB &= ~mask; }

    /// Pulses masked bits in PORTB with low state first.
    /// @param[in] mask bits to pulse
    static void pulseLow(uint8_t mask)  { PORTB &= ~mask; PORTB |= mask; }

    /// Set corresponding masked bits of PORTB to output direction.
    /// @param[in] mask bits
    static void setOutput(uint8_t mask)    { DDRB |= mask; }

    /// Set corresponding masked bits of PORTB to input direction.
    /// @param[in] mask bits
    static void setInput(uint8_t mask)  { DDRB &= ~mask; }

    /// Returns PINB register.
    static uint8_t getPIN()             { return PINB; }

    /// Tests masked bits of PORTB
    /// @param[in] mask bits
    /// @param[in] true if the corresponding bits are all set, false otherwise.
    static bool testBits(uint8_t mask)  { return (PINB & mask) == mask; }

    /// Returns the value of the bit at the position pos.
    /// @param[in] position of the bit to return
    /// @return true if the requested bit is set, false otherwise.
    static bool test(uint8_t pos) { return PINB & (1<<pos); }

};

struct PinB7 {
    /// Sets PinB7 to HIGH.
    static void set()       { PORTB |= (1<<7); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinB7 to LOW.
    static void clear()     { PORTB &= ~(1<<7); }

    /// Toggles PinB7 value.
    static void toggle()    { PINB |= (1<<7); }

    /// Configures PinB7  as an output pin.
    static void setOutput() { DDRB |= (1<<7); }

    /// Configures PinB7  as an input pin.
    static void setInput()  { DDRB &= ~(1<<7); }

    /// Pulses PinB7 with high state first.
    static void pulseHigh() { PORTB |= (1<<7); PORTB &= ~(1<<7); }

    /// Pulses PinB7 with low state first.
    static void pulseLow()  { PORTB &= ~(1<<7); PORTB |= (1<<7); }

    /// Reads PinB7  value.
    /// @return Port pin value.
    static bool test()      { return PINB & (1<<7); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<7)
    static constexpr uint8_t bitmask()               { return (1<<7); }

    /// Returns the bit corresponding to this pin.
    /// @return 7
    static constexpr uint8_t bit()                   { return 7; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortB;
};

struct PinB6 {
    /// Sets PinB6 to HIGH.
    static void set()       { PORTB |= (1<<6); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinB6 to LOW.
    static void clear()     { PORTB &= ~(1<<6); }

    /// Toggles PinB6 value.
    static void toggle()    { PINB |= (1<<6); }

    /// Configures PinB6  as an output pin.
    static void setOutput() { DDRB |= (1<<6); }

    /// Configures PinB6  as an input pin.
    static void setInput()  { DDRB &= ~(1<<6); }

    /// Pulses PinB6 with high state first.
    static void pulseHigh() { PORTB |= (1<<6); PORTB &= ~(1<<6); }

    /// Pulses PinB6 with low state first.
    static void pulseLow()  { PORTB &= ~(1<<6); PORTB |= (1<<6); }

    /// Reads PinB6  value.
    /// @return Port pin value.
    static bool test()      { return PINB & (1<<6); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<6)
    static constexpr uint8_t bitmask()               { return (1<<6); }

    /// Returns the bit corresponding to this pin.
    /// @return 6
    static constexpr uint8_t bit()                   { return 6; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortB;
};

struct PinB5 {
    /// Sets PinB5 to HIGH.
    static void set()       { PORTB |= (1<<5); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinB5 to LOW.
    static void clear()     { PORTB &= ~(1<<5); }

    /// Toggles PinB5 value.
    static void toggle()    { PINB |= (1<<5); }

    /// Configures PinB5  as an output pin.
    static void setOutput() { DDRB |= (1<<5); }

    /// Configures PinB5  as an input pin.
    static void setInput()  { DDRB &= ~(1<<5); }

    /// Pulses PinB5 with high state first.
    static void pulseHigh() { PORTB |= (1<<5); PORTB &= ~(1<<5); }

    /// Pulses PinB5 with low state first.
    static void pulseLow()  { PORTB &= ~(1<<5); PORTB |= (1<<5); }

    /// Reads PinB5  value.
    /// @return Port pin value.
    static bool test()      { return PINB & (1<<5); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<5)
    static constexpr uint8_t bitmask()               { return (1<<5); }

    /// Returns the bit corresponding to this pin.
    /// @return 5
    static constexpr uint8_t bit()                   { return 5; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortB;
};

struct PinB4 {
    /// Sets PinB4 to HIGH.
    static void set()       { PORTB |= (1<<4); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinB4 to LOW.
    static void clear()     { PORTB &= ~(1<<4); }

    /// Toggles PinB4 value.
    static void toggle()    { PINB |= (1<<4); }

    /// Configures PinB4  as an output pin.
    static void setOutput() { DDRB |= (1<<4); }

    /// Configures PinB4  as an input pin.
    static void setInput()  { DDRB &= ~(1<<4); }

    /// Pulses PinB4 with high state first.
    static void pulseHigh() { PORTB |= (1<<4); PORTB &= ~(1<<4); }

    /// Pulses PinB4 with low state first.
    static void pulseLow()  { PORTB &= ~(1<<4); PORTB |= (1<<4); }

    /// Reads PinB4  value.
    /// @return Port pin value.
    static bool test()      { return PINB & (1<<4); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<4)
    static constexpr uint8_t bitmask()               { return (1<<4); }

    /// Returns the bit corresponding to this pin.
    /// @return 4
    static constexpr uint8_t bit()                   { return 4; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortB;
};

struct PinB3 {
    /// Sets PinB3 to HIGH.
    static void set()       { PORTB |= (1<<3); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinB3 to LOW.
    static void clear()     { PORTB &= ~(1<<3); }

    /// Toggles PinB3 value.
    static void toggle()    { PINB |= (1<<3); }

    /// Configures PinB3  as an output pin.
    static void setOutput() { DDRB |= (1<<3); }

    /// Configures PinB3  as an input pin.
    static void setInput()  { DDRB &= ~(1<<3); }

    /// Pulses PinB3 with high state first.
    static void pulseHigh() { PORTB |= (1<<3); PORTB &= ~(1<<3); }

    /// Pulses PinB3 with low state first.
    static void pulseLow()  { PORTB &= ~(1<<3); PORTB |= (1<<3); }

    /// Reads PinB3  value.
    /// @return Port pin value.
    static bool test()      { return PINB & (1<<3); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<3)
    static constexpr uint8_t bitmask()               { return (1<<3); }

    /// Returns the bit corresponding to this pin.
    /// @return 3
    static constexpr uint8_t bit()                   { return 3; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortB;
};

struct PinB2 {
    /// Sets PinB2 to HIGH.
    static void set()       { PORTB |= (1<<2); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinB2 to LOW.
    static void clear()     { PORTB &= ~(1<<2); }

    /// Toggles PinB2 value.
    static void toggle()    { PINB |= (1<<2); }

    /// Configures PinB2  as an output pin.
    static void setOutput() { DDRB |= (1<<2); }

    /// Configures PinB2  as an input pin.
    static void setInput()  { DDRB &= ~(1<<2); }

    /// Pulses PinB2 with high state first.
    static void pulseHigh() { PORTB |= (1<<2); PORTB &= ~(1<<2); }

    /// Pulses PinB2 with low state first.
    static void pulseLow()  { PORTB &= ~(1<<2); PORTB |= (1<<2); }

    /// Reads PinB2  value.
    /// @return Port pin value.
    static bool test()      { return PINB & (1<<2); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<2)
    static constexpr uint8_t bitmask()               { return (1<<2); }

    /// Returns the bit corresponding to this pin.
    /// @return 2
    static constexpr uint8_t bit()                   { return 2; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortB;
};

struct PinB1 {
    /// Sets PinB1 to HIGH.
    static void set()       { PORTB |= (1<<1); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinB1 to LOW.
    static void clear()     { PORTB &= ~(1<<1); }

    /// Toggles PinB1 value.
    static void toggle()    { PINB |= (1<<1); }

    /// Configures PinB1  as an output pin.
    static void setOutput() { DDRB |= (1<<1); }

    /// Configures PinB1  as an input pin.
    static void setInput()  { DDRB &= ~(1<<1); }

    /// Pulses PinB1 with high state first.
    static void pulseHigh() { PORTB |= (1<<1); PORTB &= ~(1<<1); }

    /// Pulses PinB1 with low state first.
    static void pulseLow()  { PORTB &= ~(1<<1); PORTB |= (1<<1); }

    /// Reads PinB1  value.
    /// @return Port pin value.
    static bool test()      { return PINB & (1<<1); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<1)
    static constexpr uint8_t bitmask()               { return (1<<1); }

    /// Returns the bit corresponding to this pin.
    /// @return 1
    static constexpr uint8_t bit()                   { return 1; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortB;
};

struct PinB0 {
    /// Sets PinB0 to HIGH.
    static void set()       { PORTB |= (1<<0); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinB0 to LOW.
    static void clear()     { PORTB &= ~(1<<0); }

    /// Toggles PinB0 value.
    static void toggle()    { PINB |= (1<<0); }

    /// Configures PinB0  as an output pin.
    static void setOutput() { DDRB |= (1<<0); }

    /// Configures PinB0  as an input pin.
    static void setInput()  { DDRB &= ~(1<<0); }

    /// Pulses PinB0 with high state first.
    static void pulseHigh() { PORTB |= (1<<0); PORTB &= ~(1<<0); }

    /// Pulses PinB0 with low state first.
    static void pulseLow()  { PORTB &= ~(1<<0); PORTB |= (1<<0); }

    /// Reads PinB0  value.
    /// @return Port pin value.
    static bool test()      { return PINB & (1<<0); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<0)
    static constexpr uint8_t bitmask()               { return (1<<0); }

    /// Returns the bit corresponding to this pin.
    /// @return 0
    static constexpr uint8_t bit()                   { return 0; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortB;
};


struct PortC {

    /// Assigns a value to PORTC
    /// @param[in] value value affected to PORTC
    static void assign(uint8_t value)   { PORTC = value; }

    /// Sets masked bits in PORTC
    /// @param[in] mask bits to set
    static void setBits(uint8_t mask)   { PORTC |= mask;}

    /// Clears masked bits in PORTC
    /// @param[in] mask bits to clear
    static void clearBits(uint8_t mask) { PORTC &= ~mask;} 

    /// Changes values of masked bits in PORTC
    /// @param[in] mask bits to change
    /// @param[in] value new bits values
    static void changeBits(uint8_t mask, uint8_t value) { uint8_t tmp = PORTC & ~mask; PORTC = tmp | value; } 

    /// Toggles masked bits in PORTC
    /// @param[in] mask bits to toggle
    static void toggleBits(uint8_t mask) { PORTC ^= mask;} 

    /// Pulses masked bits in PORTC with high state first.
    /// @param[in] mask bits to pulse
    static void pulseHigh(uint8_t mask) { PORTC |= mask; PORTC &= ~mask; }

    /// Pulses masked bits in PORTC with low state first.
    /// @param[in] mask bits to pulse
    static void pulseLow(uint8_t mask)  { PORTC &= ~mask; PORTC |= mask; }

    /// Set corresponding masked bits of PORTC to output direction.
    /// @param[in] mask bits
    static void setOutput(uint8_t mask)    { DDRC |= mask; }

    /// Set corresponding masked bits of PORTC to input direction.
    /// @param[in] mask bits
    static void setInput(uint8_t mask)  { DDRC &= ~mask; }

    /// Returns PINC register.
    static uint8_t getPIN()             { return PINC; }

    /// Tests masked bits of PORTC
    /// @param[in] mask bits
    /// @param[in] true if the corresponding bits are all set, false otherwise.
    static bool testBits(uint8_t mask)  { return (PINC & mask) == mask; }

    /// Returns the value of the bit at the position pos.
    /// @param[in] position of the bit to return
    /// @return true if the requested bit is set, false otherwise.
    static bool test(uint8_t pos) { return PINC & (1<<pos); }

};

struct PinC7 {
    /// Sets PinC7 to HIGH.
    static void set()       { PORTC |= (1<<7); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinC7 to LOW.
    static void clear()     { PORTC &= ~(1<<7); }

    /// Toggles PinC7 value.
    static void toggle()    { PINC |= (1<<7); }

    /// Configures PinC7  as an output pin.
    static void setOutput() { DDRC |= (1<<7); }

    /// Configures PinC7  as an input pin.
    static void setInput()  { DDRC &= ~(1<<7); }

    /// Pulses PinC7 with high state first.
    static void pulseHigh() { PORTC |= (1<<7); PORTC &= ~(1<<7); }

    /// Pulses PinC7 with low state first.
    static void pulseLow()  { PORTC &= ~(1<<7); PORTC |= (1<<7); }

    /// Reads PinC7  value.
    /// @return Port pin value.
    static bool test()      { return PINC & (1<<7); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<7)
    static constexpr uint8_t bitmask()               { return (1<<7); }

    /// Returns the bit corresponding to this pin.
    /// @return 7
    static constexpr uint8_t bit()                   { return 7; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortC;
};

struct PinC6 {
    /// Sets PinC6 to HIGH.
    static void set()       { PORTC |= (1<<6); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinC6 to LOW.
    static void clear()     { PORTC &= ~(1<<6); }

    /// Toggles PinC6 value.
    static void toggle()    { PINC |= (1<<6); }

    /// Configures PinC6  as an output pin.
    static void setOutput() { DDRC |= (1<<6); }

    /// Configures PinC6  as an input pin.
    static void setInput()  { DDRC &= ~(1<<6); }

    /// Pulses PinC6 with high state first.
    static void pulseHigh() { PORTC |= (1<<6); PORTC &= ~(1<<6); }

    /// Pulses PinC6 with low state first.
    static void pulseLow()  { PORTC &= ~(1<<6); PORTC |= (1<<6); }

    /// Reads PinC6  value.
    /// @return Port pin value.
    static bool test()      { return PINC & (1<<6); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<6)
    static constexpr uint8_t bitmask()               { return (1<<6); }

    /// Returns the bit corresponding to this pin.
    /// @return 6
    static constexpr uint8_t bit()                   { return 6; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortC;
};


struct PortD {

    /// Assigns a value to PORTD
    /// @param[in] value value affected to PORTD
    static void assign(uint8_t value)   { PORTD = value; }

    /// Sets masked bits in PORTD
    /// @param[in] mask bits to set
    static void setBits(uint8_t mask)   { PORTD |= mask;}

    /// Clears masked bits in PORTD
    /// @param[in] mask bits to clear
    static void clearBits(uint8_t mask) { PORTD &= ~mask;} 

    /// Changes values of masked bits in PORTD
    /// @param[in] mask bits to change
    /// @param[in] value new bits values
    static void changeBits(uint8_t mask, uint8_t value) { uint8_t tmp = PORTD & ~mask; PORTD = tmp | value; } 

    /// Toggles masked bits in PORTD
    /// @param[in] mask bits to toggle
    static void toggleBits(uint8_t mask) { PORTD ^= mask;} 

    /// Pulses masked bits in PORTD with high state first.
    /// @param[in] mask bits to pulse
    static void pulseHigh(uint8_t mask) { PORTD |= mask; PORTD &= ~mask; }

    /// Pulses masked bits in PORTD with low state first.
    /// @param[in] mask bits to pulse
    static void pulseLow(uint8_t mask)  { PORTD &= ~mask; PORTD |= mask; }

    /// Set corresponding masked bits of PORTD to output direction.
    /// @param[in] mask bits
    static void setOutput(uint8_t mask)    { DDRD |= mask; }

    /// Set corresponding masked bits of PORTD to input direction.
    /// @param[in] mask bits
    static void setInput(uint8_t mask)  { DDRD &= ~mask; }

    /// Returns PIND register.
    static uint8_t getPIN()             { return PIND; }

    /// Tests masked bits of PORTD
    /// @param[in] mask bits
    /// @param[in] true if the corresponding bits are all set, false otherwise.
    static bool testBits(uint8_t mask)  { return (PIND & mask) == mask; }

    /// Returns the value of the bit at the position pos.
    /// @param[in] position of the bit to return
    /// @return true if the requested bit is set, false otherwise.
    static bool test(uint8_t pos) { return PIND & (1<<pos); }

};

struct PinD7 {
    /// Sets PinD7 to HIGH.
    static void set()       { PORTD |= (1<<7); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinD7 to LOW.
    static void clear()     { PORTD &= ~(1<<7); }

    /// Toggles PinD7 value.
    static void toggle()    { PIND |= (1<<7); }

    /// Configures PinD7  as an output pin.
    static void setOutput() { DDRD |= (1<<7); }

    /// Configures PinD7  as an input pin.
    static void setInput()  { DDRD &= ~(1<<7); }

    /// Pulses PinD7 with high state first.
    static void pulseHigh() { PORTD |= (1<<7); PORTD &= ~(1<<7); }

    /// Pulses PinD7 with low state first.
    static void pulseLow()  { PORTD &= ~(1<<7); PORTD |= (1<<7); }

    /// Reads PinD7  value.
    /// @return Port pin value.
    static bool test()      { return PIND & (1<<7); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<7)
    static constexpr uint8_t bitmask()               { return (1<<7); }

    /// Returns the bit corresponding to this pin.
    /// @return 7
    static constexpr uint8_t bit()                   { return 7; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortD;
};

struct PinD6 {
    /// Sets PinD6 to HIGH.
    static void set()       { PORTD |= (1<<6); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinD6 to LOW.
    static void clear()     { PORTD &= ~(1<<6); }

    /// Toggles PinD6 value.
    static void toggle()    { PIND |= (1<<6); }

    /// Configures PinD6  as an output pin.
    static void setOutput() { DDRD |= (1<<6); }

    /// Configures PinD6  as an input pin.
    static void setInput()  { DDRD &= ~(1<<6); }

    /// Pulses PinD6 with high state first.
    static void pulseHigh() { PORTD |= (1<<6); PORTD &= ~(1<<6); }

    /// Pulses PinD6 with low state first.
    static void pulseLow()  { PORTD &= ~(1<<6); PORTD |= (1<<6); }

    /// Reads PinD6  value.
    /// @return Port pin value.
    static bool test()      { return PIND & (1<<6); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<6)
    static constexpr uint8_t bitmask()               { return (1<<6); }

    /// Returns the bit corresponding to this pin.
    /// @return 6
    static constexpr uint8_t bit()                   { return 6; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortD;
};

struct PinD5 {
    /// Sets PinD5 to HIGH.
    static void set()       { PORTD |= (1<<5); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinD5 to LOW.
    static void clear()     { PORTD &= ~(1<<5); }

    /// Toggles PinD5 value.
    static void toggle()    { PIND |= (1<<5); }

    /// Configures PinD5  as an output pin.
    static void setOutput() { DDRD |= (1<<5); }

    /// Configures PinD5  as an input pin.
    static void setInput()  { DDRD &= ~(1<<5); }

    /// Pulses PinD5 with high state first.
    static void pulseHigh() { PORTD |= (1<<5); PORTD &= ~(1<<5); }

    /// Pulses PinD5 with low state first.
    static void pulseLow()  { PORTD &= ~(1<<5); PORTD |= (1<<5); }

    /// Reads PinD5  value.
    /// @return Port pin value.
    static bool test()      { return PIND & (1<<5); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<5)
    static constexpr uint8_t bitmask()               { return (1<<5); }

    /// Returns the bit corresponding to this pin.
    /// @return 5
    static constexpr uint8_t bit()                   { return 5; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortD;
};

struct PinD4 {
    /// Sets PinD4 to HIGH.
    static void set()       { PORTD |= (1<<4); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinD4 to LOW.
    static void clear()     { PORTD &= ~(1<<4); }

    /// Toggles PinD4 value.
    static void toggle()    { PIND |= (1<<4); }

    /// Configures PinD4  as an output pin.
    static void setOutput() { DDRD |= (1<<4); }

    /// Configures PinD4  as an input pin.
    static void setInput()  { DDRD &= ~(1<<4); }

    /// Pulses PinD4 with high state first.
    static void pulseHigh() { PORTD |= (1<<4); PORTD &= ~(1<<4); }

    /// Pulses PinD4 with low state first.
    static void pulseLow()  { PORTD &= ~(1<<4); PORTD |= (1<<4); }

    /// Reads PinD4  value.
    /// @return Port pin value.
    static bool test()      { return PIND & (1<<4); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<4)
    static constexpr uint8_t bitmask()               { return (1<<4); }

    /// Returns the bit corresponding to this pin.
    /// @return 4
    static constexpr uint8_t bit()                   { return 4; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortD;
};

struct PinD3 {
    /// Sets PinD3 to HIGH.
    static void set()       { PORTD |= (1<<3); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinD3 to LOW.
    static void clear()     { PORTD &= ~(1<<3); }

    /// Toggles PinD3 value.
    static void toggle()    { PIND |= (1<<3); }

    /// Configures PinD3  as an output pin.
    static void setOutput() { DDRD |= (1<<3); }

    /// Configures PinD3  as an input pin.
    static void setInput()  { DDRD &= ~(1<<3); }

    /// Pulses PinD3 with high state first.
    static void pulseHigh() { PORTD |= (1<<3); PORTD &= ~(1<<3); }

    /// Pulses PinD3 with low state first.
    static void pulseLow()  { PORTD &= ~(1<<3); PORTD |= (1<<3); }

    /// Reads PinD3  value.
    /// @return Port pin value.
    static bool test()      { return PIND & (1<<3); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<3)
    static constexpr uint8_t bitmask()               { return (1<<3); }

    /// Returns the bit corresponding to this pin.
    /// @return 3
    static constexpr uint8_t bit()                   { return 3; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortD;
};

struct PinD2 {
    /// Sets PinD2 to HIGH.
    static void set()       { PORTD |= (1<<2); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinD2 to LOW.
    static void clear()     { PORTD &= ~(1<<2); }

    /// Toggles PinD2 value.
    static void toggle()    { PIND |= (1<<2); }

    /// Configures PinD2  as an output pin.
    static void setOutput() { DDRD |= (1<<2); }

    /// Configures PinD2  as an input pin.
    static void setInput()  { DDRD &= ~(1<<2); }

    /// Pulses PinD2 with high state first.
    static void pulseHigh() { PORTD |= (1<<2); PORTD &= ~(1<<2); }

    /// Pulses PinD2 with low state first.
    static void pulseLow()  { PORTD &= ~(1<<2); PORTD |= (1<<2); }

    /// Reads PinD2  value.
    /// @return Port pin value.
    static bool test()      { return PIND & (1<<2); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<2)
    static constexpr uint8_t bitmask()               { return (1<<2); }

    /// Returns the bit corresponding to this pin.
    /// @return 2
    static constexpr uint8_t bit()                   { return 2; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortD;
};

struct PinD1 {
    /// Sets PinD1 to HIGH.
    static void set()       { PORTD |= (1<<1); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinD1 to LOW.
    static void clear()     { PORTD &= ~(1<<1); }

    /// Toggles PinD1 value.
    static void toggle()    { PIND |= (1<<1); }

    /// Configures PinD1  as an output pin.
    static void setOutput() { DDRD |= (1<<1); }

    /// Configures PinD1  as an input pin.
    static void setInput()  { DDRD &= ~(1<<1); }

    /// Pulses PinD1 with high state first.
    static void pulseHigh() { PORTD |= (1<<1); PORTD &= ~(1<<1); }

    /// Pulses PinD1 with low state first.
    static void pulseLow()  { PORTD &= ~(1<<1); PORTD |= (1<<1); }

    /// Reads PinD1  value.
    /// @return Port pin value.
    static bool test()      { return PIND & (1<<1); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<1)
    static constexpr uint8_t bitmask()               { return (1<<1); }

    /// Returns the bit corresponding to this pin.
    /// @return 1
    static constexpr uint8_t bit()                   { return 1; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortD;
};

struct PinD0 {
    /// Sets PinD0 to HIGH.
    static void set()       { PORTD |= (1<<0); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinD0 to LOW.
    static void clear()     { PORTD &= ~(1<<0); }

    /// Toggles PinD0 value.
    static void toggle()    { PIND |= (1<<0); }

    /// Configures PinD0  as an output pin.
    static void setOutput() { DDRD |= (1<<0); }

    /// Configures PinD0  as an input pin.
    static void setInput()  { DDRD &= ~(1<<0); }

    /// Pulses PinD0 with high state first.
    static void pulseHigh() { PORTD |= (1<<0); PORTD &= ~(1<<0); }

    /// Pulses PinD0 with low state first.
    static void pulseLow()  { PORTD &= ~(1<<0); PORTD |= (1<<0); }

    /// Reads PinD0  value.
    /// @return Port pin value.
    static bool test()      { return PIND & (1<<0); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<0)
    static constexpr uint8_t bitmask()               { return (1<<0); }

    /// Returns the bit corresponding to this pin.
    /// @return 0
    static constexpr uint8_t bit()                   { return 0; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortD;
};


struct PortE {

    /// Assigns a value to PORTE
    /// @param[in] value value affected to PORTE
    static void assign(uint8_t value)   { PORTE = value; }

    /// Sets masked bits in PORTE
    /// @param[in] mask bits to set
    static void setBits(uint8_t mask)   { PORTE |= mask;}

    /// Clears masked bits in PORTE
    /// @param[in] mask bits to clear
    static void clearBits(uint8_t mask) { PORTE &= ~mask;} 

    /// Changes values of masked bits in PORTE
    /// @param[in] mask bits to change
    /// @param[in] value new bits values
    static void changeBits(uint8_t mask, uint8_t value) { uint8_t tmp = PORTE & ~mask; PORTE = tmp | value; } 

    /// Toggles masked bits in PORTE
    /// @param[in] mask bits to toggle
    static void toggleBits(uint8_t mask) { PORTE ^= mask;} 

    /// Pulses masked bits in PORTE with high state first.
    /// @param[in] mask bits to pulse
    static void pulseHigh(uint8_t mask) { PORTE |= mask; PORTE &= ~mask; }

    /// Pulses masked bits in PORTE with low state first.
    /// @param[in] mask bits to pulse
    static void pulseLow(uint8_t mask)  { PORTE &= ~mask; PORTE |= mask; }

    /// Set corresponding masked bits of PORTE to output direction.
    /// @param[in] mask bits
    static void setOutput(uint8_t mask)    { DDRE |= mask; }

    /// Set corresponding masked bits of PORTE to input direction.
    /// @param[in] mask bits
    static void setInput(uint8_t mask)  { DDRE &= ~mask; }

    /// Returns PINE register.
    static uint8_t getPIN()             { return PINE; }

    /// Tests masked bits of PORTE
    /// @param[in] mask bits
    /// @param[in] true if the corresponding bits are all set, false otherwise.
    static bool testBits(uint8_t mask)  { return (PINE & mask) == mask; }

    /// Returns the value of the bit at the position pos.
    /// @param[in] position of the bit to return
    /// @return true if the requested bit is set, false otherwise.
    static bool test(uint8_t pos) { return PINE & (1<<pos); }

};

struct PinE6 {
    /// Sets PinE6 to HIGH.
    static void set()       { PORTE |= (1<<6); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinE6 to LOW.
    static void clear()     { PORTE &= ~(1<<6); }

    /// Toggles PinE6 value.
    static void toggle()    { PINE |= (1<<6); }

    /// Configures PinE6  as an output pin.
    static void setOutput() { DDRE |= (1<<6); }

    /// Configures PinE6  as an input pin.
    static void setInput()  { DDRE &= ~(1<<6); }

    /// Pulses PinE6 with high state first.
    static void pulseHigh() { PORTE |= (1<<6); PORTE &= ~(1<<6); }

    /// Pulses PinE6 with low state first.
    static void pulseLow()  { PORTE &= ~(1<<6); PORTE |= (1<<6); }

    /// Reads PinE6  value.
    /// @return Port pin value.
    static bool test()      { return PINE & (1<<6); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<6)
    static constexpr uint8_t bitmask()               { return (1<<6); }

    /// Returns the bit corresponding to this pin.
    /// @return 6
    static constexpr uint8_t bit()                   { return 6; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortE;
};

struct PinE2 {
    /// Sets PinE2 to HIGH.
    static void set()       { PORTE |= (1<<2); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinE2 to LOW.
    static void clear()     { PORTE &= ~(1<<2); }

    /// Toggles PinE2 value.
    static void toggle()    { PINE |= (1<<2); }

    /// Configures PinE2  as an output pin.
    static void setOutput() { DDRE |= (1<<2); }

    /// Configures PinE2  as an input pin.
    static void setInput()  { DDRE &= ~(1<<2); }

    /// Pulses PinE2 with high state first.
    static void pulseHigh() { PORTE |= (1<<2); PORTE &= ~(1<<2); }

    /// Pulses PinE2 with low state first.
    static void pulseLow()  { PORTE &= ~(1<<2); PORTE |= (1<<2); }

    /// Reads PinE2  value.
    /// @return Port pin value.
    static bool test()      { return PINE & (1<<2); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<2)
    static constexpr uint8_t bitmask()               { return (1<<2); }

    /// Returns the bit corresponding to this pin.
    /// @return 2
    static constexpr uint8_t bit()                   { return 2; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortE;
};


struct PortF {

    /// Assigns a value to PORTF
    /// @param[in] value value affected to PORTF
    static void assign(uint8_t value)   { PORTF = value; }

    /// Sets masked bits in PORTF
    /// @param[in] mask bits to set
    static void setBits(uint8_t mask)   { PORTF |= mask;}

    /// Clears masked bits in PORTF
    /// @param[in] mask bits to clear
    static void clearBits(uint8_t mask) { PORTF &= ~mask;} 

    /// Changes values of masked bits in PORTF
    /// @param[in] mask bits to change
    /// @param[in] value new bits values
    static void changeBits(uint8_t mask, uint8_t value) { uint8_t tmp = PORTF & ~mask; PORTF = tmp | value; } 

    /// Toggles masked bits in PORTF
    /// @param[in] mask bits to toggle
    static void toggleBits(uint8_t mask) { PORTF ^= mask;} 

    /// Pulses masked bits in PORTF with high state first.
    /// @param[in] mask bits to pulse
    static void pulseHigh(uint8_t mask) { PORTF |= mask; PORTF &= ~mask; }

    /// Pulses masked bits in PORTF with low state first.
    /// @param[in] mask bits to pulse
    static void pulseLow(uint8_t mask)  { PORTF &= ~mask; PORTF |= mask; }

    /// Set corresponding masked bits of PORTF to output direction.
    /// @param[in] mask bits
    static void setOutput(uint8_t mask)    { DDRF |= mask; }

    /// Set corresponding masked bits of PORTF to input direction.
    /// @param[in] mask bits
    static void setInput(uint8_t mask)  { DDRF &= ~mask; }

    /// Returns PINF register.
    static uint8_t getPIN()             { return PINF; }

    /// Tests masked bits of PORTF
    /// @param[in] mask bits
    /// @param[in] true if the corresponding bits are all set, false otherwise.
    static bool testBits(uint8_t mask)  { return (PINF & mask) == mask; }

    /// Returns the value of the bit at the position pos.
    /// @param[in] position of the bit to return
    /// @return true if the requested bit is set, false otherwise.
    static bool test(uint8_t pos) { return PINF & (1<<pos); }

};

struct PinF7 {
    /// Sets PinF7 to HIGH.
    static void set()       { PORTF |= (1<<7); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinF7 to LOW.
    static void clear()     { PORTF &= ~(1<<7); }

    /// Toggles PinF7 value.
    static void toggle()    { PINF |= (1<<7); }

    /// Configures PinF7  as an output pin.
    static void setOutput() { DDRF |= (1<<7); }

    /// Configures PinF7  as an input pin.
    static void setInput()  { DDRF &= ~(1<<7); }

    /// Pulses PinF7 with high state first.
    static void pulseHigh() { PORTF |= (1<<7); PORTF &= ~(1<<7); }

    /// Pulses PinF7 with low state first.
    static void pulseLow()  { PORTF &= ~(1<<7); PORTF |= (1<<7); }

    /// Reads PinF7  value.
    /// @return Port pin value.
    static bool test()      { return PINF & (1<<7); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<7)
    static constexpr uint8_t bitmask()               { return (1<<7); }

    /// Returns the bit corresponding to this pin.
    /// @return 7
    static constexpr uint8_t bit()                   { return 7; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortF;
};

struct PinF6 {
    /// Sets PinF6 to HIGH.
    static void set()       { PORTF |= (1<<6); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinF6 to LOW.
    static void clear()     { PORTF &= ~(1<<6); }

    /// Toggles PinF6 value.
    static void toggle()    { PINF |= (1<<6); }

    /// Configures PinF6  as an output pin.
    static void setOutput() { DDRF |= (1<<6); }

    /// Configures PinF6  as an input pin.
    static void setInput()  { DDRF &= ~(1<<6); }

    /// Pulses PinF6 with high state first.
    static void pulseHigh() { PORTF |= (1<<6); PORTF &= ~(1<<6); }

    /// Pulses PinF6 with low state first.
    static void pulseLow()  { PORTF &= ~(1<<6); PORTF |= (1<<6); }

    /// Reads PinF6  value.
    /// @return Port pin value.
    static bool test()      { return PINF & (1<<6); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<6)
    static constexpr uint8_t bitmask()               { return (1<<6); }

    /// Returns the bit corresponding to this pin.
    /// @return 6
    static constexpr uint8_t bit()                   { return 6; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortF;
};

struct PinF5 {
    /// Sets PinF5 to HIGH.
    static void set()       { PORTF |= (1<<5); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinF5 to LOW.
    static void clear()     { PORTF &= ~(1<<5); }

    /// Toggles PinF5 value.
    static void toggle()    { PINF |= (1<<5); }

    /// Configures PinF5  as an output pin.
    static void setOutput() { DDRF |= (1<<5); }

    /// Configures PinF5  as an input pin.
    static void setInput()  { DDRF &= ~(1<<5); }

    /// Pulses PinF5 with high state first.
    static void pulseHigh() { PORTF |= (1<<5); PORTF &= ~(1<<5); }

    /// Pulses PinF5 with low state first.
    static void pulseLow()  { PORTF &= ~(1<<5); PORTF |= (1<<5); }

    /// Reads PinF5  value.
    /// @return Port pin value.
    static bool test()      { return PINF & (1<<5); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<5)
    static constexpr uint8_t bitmask()               { return (1<<5); }

    /// Returns the bit corresponding to this pin.
    /// @return 5
    static constexpr uint8_t bit()                   { return 5; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortF;
};

struct PinF4 {
    /// Sets PinF4 to HIGH.
    static void set()       { PORTF |= (1<<4); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinF4 to LOW.
    static void clear()     { PORTF &= ~(1<<4); }

    /// Toggles PinF4 value.
    static void toggle()    { PINF |= (1<<4); }

    /// Configures PinF4  as an output pin.
    static void setOutput() { DDRF |= (1<<4); }

    /// Configures PinF4  as an input pin.
    static void setInput()  { DDRF &= ~(1<<4); }

    /// Pulses PinF4 with high state first.
    static void pulseHigh() { PORTF |= (1<<4); PORTF &= ~(1<<4); }

    /// Pulses PinF4 with low state first.
    static void pulseLow()  { PORTF &= ~(1<<4); PORTF |= (1<<4); }

    /// Reads PinF4  value.
    /// @return Port pin value.
    static bool test()      { return PINF & (1<<4); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<4)
    static constexpr uint8_t bitmask()               { return (1<<4); }

    /// Returns the bit corresponding to this pin.
    /// @return 4
    static constexpr uint8_t bit()                   { return 4; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortF;
};

struct PinF1 {
    /// Sets PinF1 to HIGH.
    static void set()       { PORTF |= (1<<1); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinF1 to LOW.
    static void clear()     { PORTF &= ~(1<<1); }

    /// Toggles PinF1 value.
    static void toggle()    { PINF |= (1<<1); }

    /// Configures PinF1  as an output pin.
    static void setOutput() { DDRF |= (1<<1); }

    /// Configures PinF1  as an input pin.
    static void setInput()  { DDRF &= ~(1<<1); }

    /// Pulses PinF1 with high state first.
    static void pulseHigh() { PORTF |= (1<<1); PORTF &= ~(1<<1); }

    /// Pulses PinF1 with low state first.
    static void pulseLow()  { PORTF &= ~(1<<1); PORTF |= (1<<1); }

    /// Reads PinF1  value.
    /// @return Port pin value.
    static bool test()      { return PINF & (1<<1); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<1)
    static constexpr uint8_t bitmask()               { return (1<<1); }

    /// Returns the bit corresponding to this pin.
    /// @return 1
    static constexpr uint8_t bit()                   { return 1; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortF;
};

struct PinF0 {
    /// Sets PinF0 to HIGH.
    static void set()       { PORTF |= (1<<0); }

    static void set(bool v) { v ? set() : clear(); }

    /// Sets PinF0 to LOW.
    static void clear()     { PORTF &= ~(1<<0); }

    /// Toggles PinF0 value.
    static void toggle()    { PINF |= (1<<0); }

    /// Configures PinF0  as an output pin.
    static void setOutput() { DDRF |= (1<<0); }

    /// Configures PinF0  as an input pin.
    static void setInput()  { DDRF &= ~(1<<0); }

    /// Pulses PinF0 with high state first.
    static void pulseHigh() { PORTF |= (1<<0); PORTF &= ~(1<<0); }

    /// Pulses PinF0 with low state first.
    static void pulseLow()  { PORTF &= ~(1<<0); PORTF |= (1<<0); }

    /// Reads PinF0  value.
    /// @return Port pin value.
    static bool test()      { return PINF & (1<<0); }

    /// Returns the bitmask corresponding to this pin.
    /// @return (1<<0)
    static constexpr uint8_t bitmask()               { return (1<<0); }

    /// Returns the bit corresponding to this pin.
    /// @return 0
    static constexpr uint8_t bit()                   { return 0; }

    /// Port is defined as the Port object to which this pin belongs.
    using Port = PortF;
};


struct SpiSpcr {

  /// Assigns a value to SPCR
  /// @param[in] value value affected to SPCR
  static void Assign(uint8_t value)  { SPCR = value; }

  /// Sets masked bits in SPCR
  /// @param[in] mask bits to set
  static void Set(uint8_t mask)      { SPCR |= mask; }

  /// Clears masked bits in SPCR
  /// @param[in] mask bits to clear
  static void Clear(uint8_t mask)    { SPCR &= ~mask; }
  static uint8_t Get()               { return SPCR; }
  static bool TestBits(uint8_t mask) { return SPCR & mask; }
  void operator=(uint8_t value)      { SPCR = value; }
};

struct SpiSpdr {

  /// Assigns a value to SPDR
  /// @param[in] value value affected to SPDR
  static void Assign(uint8_t value)  { SPDR = value; }

  /// Sets masked bits in SPDR
  /// @param[in] mask bits to set
  static void Set(uint8_t mask)      { SPDR |= mask; }

  /// Clears masked bits in SPDR
  /// @param[in] mask bits to clear
  static void Clear(uint8_t mask)    { SPDR &= ~mask; }
  static uint8_t Get()               { return SPDR; }
  static bool TestBits(uint8_t mask) { return SPDR & mask; }
  void operator=(uint8_t value)      { SPDR = value; }
};

struct SpiSpsr {

  /// Assigns a value to SPSR
  /// @param[in] value value affected to SPSR
  static void Assign(uint8_t value)  { SPSR = value; }

  /// Sets masked bits in SPSR
  /// @param[in] mask bits to set
  static void Set(uint8_t mask)      { SPSR |= mask; }

  /// Clears masked bits in SPSR
  /// @param[in] mask bits to clear
  static void Clear(uint8_t mask)    { SPSR &= ~mask; }
  static uint8_t Get()               { return SPSR; }
  static bool TestBits(uint8_t mask) { return SPSR & mask; }
  void operator=(uint8_t value)      { SPSR = value; }
};

struct Timer0 {
  using value_type = uint8_t;
  static const uint8_t timer_width = 8;

  enum Prescaler : uint8_t {
    STOP_TIMER = 0, NO_PRESCALER = (1<<CS00), CLK_DIV_8 = (1<<CS01),
    CLK_DIV_32 = (1<<CS01)|(1<<CS00), CLK_DIV_64 = (1<<CS02),
    CLK_DIV_128 = (1<<CS02)|(1<<CS00), CLK_DIV_256 = (1<<CS02)|(1<<CS01),
    CLK_DIV_1024 = (1<<CS02)|(1<<CS01)|(1<<CS00), BITS = (1<<CS02)|(1<<CS01)|(1<<CS00) };

  enum Interrupt : uint8_t { 
    OVERFLOW = (1<<TOIE0), COMPARE_MATCH_A = (1<<OCIE0A), COMPARE_MATCH_B = (1<<OCIE0B)};
  enum Constants : uint8_t { ALL_BITS = 0xFF };
  static value_type getValue()                 { return TCNT0; }
  static void setValue(value_type value)       { TCNT0 = value; }
  static void addValue(value_type value)       { TCNT0 += value; }
  static void subValue(value_type value)       { TCNT0 -= value; }
  static void setCtrlRegA(uint8_t mask)        { TCCR0A |= mask; }
  static void clearCtrlRegA(uint8_t mask)      { TCCR0A &= ~mask; }
  static void setCtrlRegB(uint8_t mask)        { TCCR0B |= mask; }
  static void clearCtrlRegB(uint8_t mask)      { TCCR0B &= ~mask; }
  static void setInterruptMask(uint8_t mask)   { TIMSK0 |= mask; }
  static void clearInterruptMask(uint8_t mask) { TIMSK0 &= ~mask; }
  static void setPrescaler(Prescaler val)      { TCCR0B &= ~Prescaler::BITS; TCCR0B |= val; }
  static void setOutputCompareValueA(value_type value) { OCR0A = value; }
  static value_type getOutputCompareValueA()           { return OCR0A; }
  static void setOutputCompareValueB(value_type value) { OCR0B = value; }
  static value_type getOutputCompareValueB()           { return OCR0B; }

  struct Isr {
    static void Overflow() IOPORTS_IRQ_HANDLER(TIMER0_OVF_vect, signal);
    static void CompareMatchA() IOPORTS_IRQ_HANDLER(TIMER0_COMPA_vect, signal);
    static void CompareMatchB() IOPORTS_IRQ_HANDLER(TIMER0_COMPB_vect, signal);
  };

  struct IsrNoBlock {
    static void Overflow() IOPORTS_IRQ_HANDLER(TIMER0_OVF_vect, interrupt);
    static void CompareMatchA() IOPORTS_IRQ_HANDLER(TIMER0_COMPA_vect, interrupt);
    static void CompareMatchB() IOPORTS_IRQ_HANDLER(TIMER0_COMPB_vect, interrupt);
  };

  struct IsrNaked {
    static void Overflow() IOPORTS_IRQ_HANDLER(TIMER0_OVF_vect, naked);
    static void CompareMatchA() IOPORTS_IRQ_HANDLER(TIMER0_COMPA_vect, naked);
    static void CompareMatchB() IOPORTS_IRQ_HANDLER(TIMER0_COMPB_vect, naked);
  };
};  

struct Timer1 {
  using value_type = uint16_t;
  static const uint8_t timer_width = 16;

  enum Prescaler : uint8_t {
    STOP_TIMER = 0, NO_PRESCALER = (1<<CS10), CLK_DIV_8 = (1<<CS11),
    CLK_DIV_32 = (1<<CS11)|(1<<CS10), CLK_DIV_64 = (1<<CS12),
    CLK_DIV_128 = (1<<CS12)|(1<<CS10), CLK_DIV_256 = (1<<CS12)|(1<<CS11),
    CLK_DIV_1024 = (1<<CS12)|(1<<CS11)|(1<<CS10), BITS = (1<<CS12)|(1<<CS11)|(1<<CS10) };

  enum Interrupt : uint8_t { 
    OVERFLOW = (1<<TOIE1), CAPTURE = (1<<ICIE1), COMPARE_MATCH_A = (1<<OCIE1A), COMPARE_MATCH_B = (1<<OCIE1B), COMPARE_MATCH_C = (1<<OCIE1C)};
  enum Constants : uint8_t { ALL_BITS = 0xFF };
  static value_type getValue()                 { return TCNT1; }
  static void setValue(value_type value)       { TCNT1 = value; }
  static void addValue(value_type value)       { TCNT1 += value; }
  static void subValue(value_type value)       { TCNT1 -= value; }
  static void setCtrlRegA(uint8_t mask)        { TCCR1A |= mask; }
  static void clearCtrlRegA(uint8_t mask)      { TCCR1A &= ~mask; }
  static void setCtrlRegB(uint8_t mask)        { TCCR1B |= mask; }
  static void clearCtrlRegB(uint8_t mask)      { TCCR1B &= ~mask; }
  static void setInterruptMask(uint8_t mask)   { TIMSK1 |= mask; }
  static void clearInterruptMask(uint8_t mask) { TIMSK1 &= ~mask; }
  static void setPrescaler(Prescaler val)      { TCCR1B &= ~Prescaler::BITS; TCCR1B |= val; }
  static void setOutputCompareValueA(value_type value) { OCR1A = value; }
  static value_type getOutputCompareValueA()           { return OCR1A; }
  static void setOutputCompareValueB(value_type value) { OCR1B = value; }
  static value_type getOutputCompareValueB()           { return OCR1B; }

  struct Isr {
    static void Overflow() IOPORTS_IRQ_HANDLER(TIMER1_OVF_vect, signal);
    static void Capture() IOPORTS_IRQ_HANDLER(TIMER1_CAPT_vect, signal);
    static void CompareMatchA() IOPORTS_IRQ_HANDLER(TIMER1_COMPA_vect, signal);
    static void CompareMatchB() IOPORTS_IRQ_HANDLER(TIMER1_COMPB_vect, signal);
    static void CompareMatchC() IOPORTS_IRQ_HANDLER(TIMER1_COMPC_vect, signal);
  };

  struct IsrNoBlock {
    static void Overflow() IOPORTS_IRQ_HANDLER(TIMER1_OVF_vect, interrupt);
    static void Capture() IOPORTS_IRQ_HANDLER(TIMER1_CAPT_vect, interrupt);
    static void CompareMatchA() IOPORTS_IRQ_HANDLER(TIMER1_COMPA_vect, interrupt);
    static void CompareMatchB() IOPORTS_IRQ_HANDLER(TIMER1_COMPB_vect, interrupt);
    static void CompareMatchC() IOPORTS_IRQ_HANDLER(TIMER1_COMPC_vect, interrupt);
  };

  struct IsrNaked {
    static void Overflow() IOPORTS_IRQ_HANDLER(TIMER1_OVF_vect, naked);
    static void Capture() IOPORTS_IRQ_HANDLER(TIMER1_CAPT_vect, naked);
    static void CompareMatchA() IOPORTS_IRQ_HANDLER(TIMER1_COMPA_vect, naked);
    static void CompareMatchB() IOPORTS_IRQ_HANDLER(TIMER1_COMPB_vect, naked);
    static void CompareMatchC() IOPORTS_IRQ_HANDLER(TIMER1_COMPC_vect, naked);
  };
};  

struct Timer3 {
  using value_type = uint16_t;
  static const uint8_t timer_width = 16;

  enum Prescaler : uint8_t {
    STOP_TIMER = 0, NO_PRESCALER = (1<<CS30), CLK_DIV_8 = (1<<CS31),
    CLK_DIV_32 = (1<<CS31)|(1<<CS30), CLK_DIV_64 = (1<<CS32),
    CLK_DIV_128 = (1<<CS32)|(1<<CS30), CLK_DIV_256 = (1<<CS32)|(1<<CS31),
    CLK_DIV_1024 = (1<<CS32)|(1<<CS31)|(1<<CS30), BITS = (1<<CS32)|(1<<CS31)|(1<<CS30) };

  enum Interrupt : uint8_t { 
    OVERFLOW = (1<<TOIE3), CAPTURE = (1<<ICIE3), COMPARE_MATCH_A = (1<<OCIE3A), COMPARE_MATCH_B = (1<<OCIE3B), COMPARE_MATCH_C = (1<<OCIE3C)};
  enum Constants : uint8_t { ALL_BITS = 0xFF };
  static value_type getValue()                 { return TCNT3; }
  static void setValue(value_type value)       { TCNT3 = value; }
  static void addValue(value_type value)       { TCNT3 += value; }
  static void subValue(value_type value)       { TCNT3 -= value; }
  static void setCtrlRegA(uint8_t mask)        { TCCR3A |= mask; }
  static void clearCtrlRegA(uint8_t mask)      { TCCR3A &= ~mask; }
  static void setCtrlRegB(uint8_t mask)        { TCCR3B |= mask; }
  static void clearCtrlRegB(uint8_t mask)      { TCCR3B &= ~mask; }
  static void setInterruptMask(uint8_t mask)   { TIMSK3 |= mask; }
  static void clearInterruptMask(uint8_t mask) { TIMSK3 &= ~mask; }
  static void setPrescaler(Prescaler val)      { TCCR3B &= ~Prescaler::BITS; TCCR3B |= val; }
  static void setOutputCompareValueA(value_type value) { OCR3A = value; }
  static value_type getOutputCompareValueA()           { return OCR3A; }
  static void setOutputCompareValueB(value_type value) { OCR3B = value; }
  static value_type getOutputCompareValueB()           { return OCR3B; }

  struct Isr {
    static void Overflow() IOPORTS_IRQ_HANDLER(TIMER3_OVF_vect, signal);
    static void Capture() IOPORTS_IRQ_HANDLER(TIMER3_CAPT_vect, signal);
    static void CompareMatchA() IOPORTS_IRQ_HANDLER(TIMER3_COMPA_vect, signal);
    static void CompareMatchB() IOPORTS_IRQ_HANDLER(TIMER3_COMPB_vect, signal);
    static void CompareMatchC() IOPORTS_IRQ_HANDLER(TIMER3_COMPC_vect, signal);
  };

  struct IsrNoBlock {
    static void Overflow() IOPORTS_IRQ_HANDLER(TIMER3_OVF_vect, interrupt);
    static void Capture() IOPORTS_IRQ_HANDLER(TIMER3_CAPT_vect, interrupt);
    static void CompareMatchA() IOPORTS_IRQ_HANDLER(TIMER3_COMPA_vect, interrupt);
    static void CompareMatchB() IOPORTS_IRQ_HANDLER(TIMER3_COMPB_vect, interrupt);
    static void CompareMatchC() IOPORTS_IRQ_HANDLER(TIMER3_COMPC_vect, interrupt);
  };

  struct IsrNaked {
    static void Overflow() IOPORTS_IRQ_HANDLER(TIMER3_OVF_vect, naked);
    static void Capture() IOPORTS_IRQ_HANDLER(TIMER3_CAPT_vect, naked);
    static void CompareMatchA() IOPORTS_IRQ_HANDLER(TIMER3_COMPA_vect, naked);
    static void CompareMatchB() IOPORTS_IRQ_HANDLER(TIMER3_COMPB_vect, naked);
    static void CompareMatchC() IOPORTS_IRQ_HANDLER(TIMER3_COMPC_vect, naked);
  };
};  

struct Timer4 {
  using value_type = uint16_t;
  static const uint8_t timer_width = 10;

  enum Prescaler : uint8_t {
    STOP_TIMER = 0, NO_PRESCALER = (1<<CS40), CLK_DIV_8 = (1<<CS41),
    CLK_DIV_64 = (1<<CS41)|(1<<CS40), CLK_DIV_256 = (1<<CS42),
    CLK_DIV_1024 = (1<<CS42)|(1<<CS40), BITS = (1<<CS42)|(1<<CS41)|(1<<CS40) };
  enum ClockSource : uint8_t {
    INC_ON_FALLING = (1<<CS42)|(1<<CS41), INC_ON_RISING = (1<<CS42)|(1<<CS41)|(1<<CS40) };

  enum Interrupt : uint8_t { 
    OVERFLOW = (1<<TOIE4), COMPARE_MATCH_A = (1<<OCIE4A), COMPARE_MATCH_B = (1<<OCIE4B), COMPARE_MATCH_D = (1<<OCIE4D)};
  enum Constants : uint8_t { ALL_BITS = 0xFF };
  static value_type getValue()                 { return TCNT4; }
  static void setValue(value_type value)       { TCNT4 = value; }
  static void addValue(value_type value)       { TCNT4 += value; }
  static void subValue(value_type value)       { TCNT4 -= value; }
  static void setCtrlRegA(uint8_t mask)        { TCCR4A |= mask; }
  static void clearCtrlRegA(uint8_t mask)      { TCCR4A &= ~mask; }
  static void setCtrlRegB(uint8_t mask)        { TCCR4B |= mask; }
  static void clearCtrlRegB(uint8_t mask)      { TCCR4B &= ~mask; }
  static void setInterruptMask(uint8_t mask)   { TIMSK4 |= mask; }
  static void clearInterruptMask(uint8_t mask) { TIMSK4 &= ~mask; }
  static void setPrescaler(Prescaler val)      { TCCR4B &= ~Prescaler::BITS; TCCR4B |= val; }
  static void setExternalClockSource(ClockSource val)  { TCCR4B &= ~Prescaler::BITS; TCCR4B |= val; }
  static void setOutputCompareValueA(value_type value) { OCR4A = value; }
  static value_type getOutputCompareValueA()           { return OCR4A; }
  static void setOutputCompareValueB(value_type value) { OCR4B = value; }
  static value_type getOutputCompareValueB()           { return OCR4B; }

  struct Isr {
    static void Overflow() IOPORTS_IRQ_HANDLER(TIMER4_OVF_vect, signal);
    static void CompareMatchA() IOPORTS_IRQ_HANDLER(TIMER4_COMPA_vect, signal);
    static void CompareMatchB() IOPORTS_IRQ_HANDLER(TIMER4_COMPB_vect, signal);
    static void CompareMatchD() IOPORTS_IRQ_HANDLER(TIMER4_COMPD_vect, signal);
  };

  struct IsrNoBlock {
    static void Overflow() IOPORTS_IRQ_HANDLER(TIMER4_OVF_vect, interrupt);
    static void CompareMatchA() IOPORTS_IRQ_HANDLER(TIMER4_COMPA_vect, interrupt);
    static void CompareMatchB() IOPORTS_IRQ_HANDLER(TIMER4_COMPB_vect, interrupt);
    static void CompareMatchD() IOPORTS_IRQ_HANDLER(TIMER4_COMPD_vect, interrupt);
  };

  struct IsrNaked {
    static void Overflow() IOPORTS_IRQ_HANDLER(TIMER4_OVF_vect, naked);
    static void CompareMatchA() IOPORTS_IRQ_HANDLER(TIMER4_COMPA_vect, naked);
    static void CompareMatchB() IOPORTS_IRQ_HANDLER(TIMER4_COMPB_vect, naked);
    static void CompareMatchD() IOPORTS_IRQ_HANDLER(TIMER4_COMPD_vect, naked);
  };
};  

#ifdef PCICR
struct PinChangeControlRegister {
  static void setBits(uint8_t mask)   { PCICR |= mask; }
  static void clearBits(uint8_t mask) { PCICR &= ~mask; }
};
#endif // PCICR

struct PinChangeMask0 {
  static void setBits(uint8_t mask)   { PCMSK0 |= mask; }
  static void clearBits(uint8_t mask) { PCMSK0 &= ~mask; }
  static uint8_t Get()                { return PCMSK0; }
};

struct PinChangeIRQ0 {
  static void enableSource(uint8_t PCINT) {
    PinChangeControlRegister::SetBits(1<<PCIE0);
    PinChangeMask0::SetBits(1<<PCINT);
  }

  static void disableSource(uint8_t PCINT) {
    PinChangeMask0::ClearBits(1<<PCINT);
    if (0 == PinChangeMask0::Get()) {
      PinChangeControlRegister::ClearBits(1<<PCIE0);
    }
  }

  struct Isr {
    static void trigger() IOPORTS_IRQ_HANDLER(PCINT0_vect, signal);
  };

  struct IsrNoBlock {
    static void trigger() IOPORTS_IRQ_HANDLER(PCINT0_vect, interrupt);
  };

  struct IsrNaked {
    static void trigger() IOPORTS_IRQ_HANDLER(PCINT0_vect, naked);
  };

};

#undef IOPORTS_TO_STRING
#undef IOPORTS_IRQ_HANDLER
} // namespace etl

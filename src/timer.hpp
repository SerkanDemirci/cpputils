// Copyright (c) 2018 <YOUR NAME>
//
// MIT License
//
// Permission is hereby granted, free of charge, to any person obtaining
// a copy of this software and associated documentation files (the
// "Software"), to deal in the Software without restriction, including
// without limitation the rights to use, copy, modify, merge, publish,
// distribute, sublicense, and/or sell copies of the Software, and to
// permit persons to whom the Software is furnished to do so, subject to
// the following conditions:
//
// The above copyright notice and this permission notice shall be
// included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
// EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
// MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
// NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
// LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
// OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
// WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

#pragma once
#ifndef TIMER_HPP
#define TIMER_HPP

#include <chrono>
#include <ctime>

enum ClockType{
    HighResolutionClock = 0, WallClock, CpuClock
};

template <ClockType C = HighResolutionClock>
class Timer{
private:
        using HRClock = std::chrono::high_resolution_clock;
        using WClock  = std::chrono::system_clock;

        using Clock = typename std::conditional<C == WallClock,
                                                WClock, HRClock>::type;

        using TimePoint = std::chrono::time_point<Clock>;
public:
    Timer(){
        m_running = false;
    }
    void start(){
        m_start_time = Clock::now();
        m_running = true;
    }
    void stop() {
        m_stop_time = Clock::now();
        m_running = false;
    }
    template <typename Rep, typename Period>
    Rep time(){
        if (m_running){
            return std::chrono::duration<Rep, Period>(Clock::now() - m_start_time).count();
        } else{
            return std::chrono::duration<Rep, Period>(m_stop_time - m_start_time).count();
        }
    }
    template <typename T> T milliseconds(){ return time<T, std::milli>(); }
    template <typename T> T seconds() { return time<T, std::ratio<1>>(); }
    template <typename T> T nanoseconds() { return time<T, std::nano>(); }
private:
    TimePoint m_start_time;
    TimePoint m_stop_time;
    bool m_running;
};

// CPU timer uses std::clock from time.h
//  it needs to be handled separately
template <>
class Timer<CpuClock>
{
public:
    Timer(){
        m_running = false;
    }
    void start(){
        m_start_time = std::clock();
        m_running = true;
    }
    void stop(){
        m_stop_time = std::clock();
        m_running = false;
    }
    template <typename Rep, typename Period>
    Rep time(){
        if (m_running){
            return time<Rep, Period>(m_start_time, std::clock());
        } else {
            return time<Rep, Period>(m_start_time, m_stop_time);
        }
    }
    template <typename T> T milliseconds(){ return time<T, std::milli>(); }
    template <typename T> T seconds() { return time<T, std::ratio<1>>(); }
    template <typename T> T nanoseconds() { return time<T, std::nano>(); }
private:
    std::clock_t m_start_time, m_stop_time;
    bool m_running;

    template <typename Rep, typename Period>
    Rep time(std::clock_t start, std::clock_t end){
        auto num   = static_cast<long double>(end - start) * Period::den;
        auto den = Period::num * CLOCKS_PER_SEC;
        return static_cast<Rep>( num / den );
    }
};
#endif /* end of include guard: TIMER_HPP */

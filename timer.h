/*******************************************************************************
 * timer.h
 *
 * A flexible wall-clock timer
 *
 *******************************************************************************
 * Copyright (C) 2016 Lorenz Hübschle-Schneider <lorenz@4z2.de>
 *
 * The MIT License (MIT)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.

 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 ******************************************************************************/


#pragma once

#include <chrono>

/// A flexible timer. TimeT is the precision of the timing, while scalingFactor
/// is the factor by which the output will be scaled. The default is to print
/// return milliseconds with microsecond precision.
template<typename TimeT = std::chrono::microseconds, int scalingFactor = 1000, typename return_type = double>
struct TimerT {
	TimerT() {
		reset();
	}

	void reset() {
		start = std::chrono::system_clock::now();
	}

        return_type get() const {
		TimeT duration = std::chrono::duration_cast<TimeT>(std::chrono::system_clock::now() - start);
		return (duration.count() * 1.0) / scalingFactor;
	}

        return_type get_and_reset() {
		auto t = get();
		reset();
		return t;
	}

private:
	std::chrono::system_clock::time_point start;
};

/// A timer that is accurate to microseconds, formatted as milliseconds
typedef TimerT<std::chrono::microseconds, 1000, double> Timer;

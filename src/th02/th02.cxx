/*
 * Author: Yevgeniy Kiveisha <yevgeniy.kiveisha@intel.com>
 * Contributions: Jon Trulson <jtlulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
 *
 * Credits to Seeed Studeo.
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include <iostream>
#include <unistd.h>
#include <stdlib.h>

#include "th02.h"

using namespace std;
using namespace upm;

struct TH02Exception : public std::exception {
    std::string message;
    TH02Exception (std::string msg) : message (msg) { }
    ~TH02Exception () throw () { }
    const char* what() const throw () { return message.c_str(); }
};

TH02::TH02 (int bus, uint8_t addr) : m_i2c(bus) {
    m_addr = addr;
    m_name = "TH02";

    mraa_result_t ret = m_i2c.address(m_addr);
    if (ret != MRAA_SUCCESS) {
        throw TH02Exception ("Couldn't initilize I2C.");
    }
}

TH02::~TH02 () {
}

float
TH02::getTemperature () {
    uint16_t temperature = 0;

    /* Start a new temperature conversion */
    if (m_i2c.writeReg(TH02_REG_CONFIG, TH02_CMD_MEASURE_TEMP)) {
        cerr << __FUNCTION__ << "@" << __LINE__ 
             << ": writeReg failed" << endl;
        return 0.0;
    }

    /* Wait until conversion is done */
    while (getStatus() == false);

    temperature = m_i2c.readReg(TH02_REG_DATA_H) << 8;
    temperature |= m_i2c.readReg(TH02_REG_DATA_L);
    temperature >>= 2;

    return ((float(temperature) / 32.0) - 50.0);
}

float
TH02::getHumidity () {
    uint16_t humidity = 0;

    /* Start a new humidity conversion */
    if (m_i2c.writeReg(TH02_REG_CONFIG, TH02_CMD_MEASURE_HUMI)) {
        cerr << __FUNCTION__ << "@" << __LINE__ 
             << ": writeReg failed" << endl;
        return 0.0;
    }

    /* Wait until conversion is done */
    while (getStatus() == false);

    humidity = m_i2c.readReg(TH02_REG_DATA_H) << 8;
    humidity |= m_i2c.readReg(TH02_REG_DATA_L);
    humidity >>= 4;

    return ((float(humidity) / 16.0) - 24.0);
}

bool
TH02::getStatus () {
    uint8_t status = m_i2c.readReg(TH02_REG_STATUS);

    if (status & TH02_STATUS_RDY_MASK)
        return false;           // NOT ready
    else
        return true;            // ready
}


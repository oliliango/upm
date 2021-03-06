/*
 * Author: Jon Trulson <jtrulson@ics.com>
 * Copyright (c) 2014 Intel Corporation.
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
#pragma once

#include <string>
#include <mraa/aio.h>

namespace upm {

  /**
   * @brief ENC03R Single Axis Gyro library
   * @defgroup enc03r libupm-enc03r
   * @ingroup seeed analog compass robok
   */

  /**
   * @library enc03r
   * @sensor enc03r
   * @comname ENC03R Single Axis Gyro
   * @altname Grove Single Axis Analog Gyro
   * @type compass
   * @man seeed
   * @con analog
   * @kit robok
   *
   * @brief API for the ENC03R Single Axis Analog Gyro
   *
   * UPM module for the ENC03R Single Axis Analog Gyro.
   * This gyroscope measures the x-axis angular velocity; that is,
   * how fast the sensor is rotating around the x-axis.
   * Calibration of the sensor is necessary for accurate readings.
   *
   * @image html enc03r.jpg 
   * @snippet enc03r.cxx Interesting
   */
  class ENC03R {
  public:

    /**
     * ENC03R sensor constructor
     *
     * @param pin analog pin to use
     * @param vref the voltage reference to use, default 5.0
     */
    ENC03R(int pin, float vref=5.0);

    /**
     * ENC03R Destructor
     */
    ~ENC03R();

    /**
     * Calibrate the sensor by determining an analog reading over many
     * samples with no movement of the sensor.  This must be done
     * before attempting to use the sensor.
     *
     * @param samples the number of samples to use for calibration
     */
    void calibrate(unsigned int samples);
    
    /**
     * Return the raw value of the sensor
     *
     * @return raw value of sensor
     */
    unsigned int value();

    /**
     * Return the currently stored calibration value
     *
     * @return current calibration value
     */
    float calibrationValue() { return m_calibrationValue; };

    /**
     * Compute angular velocity based on value and stored calibration
     * reference.
     *
     * @param val the value to use to compute the angular velocity
     * @return computed angular velocity
     */
    double angularVelocity(unsigned int val);

  private:
    // determined by calibrate();
    float m_calibrationValue;

    // reference voltage
    float m_vref;
    mraa_aio_context m_aio;
  };
}



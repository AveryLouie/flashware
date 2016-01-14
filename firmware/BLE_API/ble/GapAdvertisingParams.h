/* mbed Microcontroller Library
 * Copyright (c) 2006-2013 ARM Limited
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *     http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
*/

#ifndef __GAP_ADVERTISING_PARAMS_H__
#define __GAP_ADVERTISING_PARAMS_H__

/**
 *  This class provides a wrapper for the core advertising parameters,
 *  including the advertising type (Connectable Undirected,
 *  Non Connectable Undirected and so on), as well as the advertising and
 *  timeout intervals.
 */
class GapAdvertisingParams {
public:
    static const unsigned GAP_ADV_PARAMS_INTERVAL_MIN        = 0x0020;
    static const unsigned GAP_ADV_PARAMS_INTERVAL_MIN_NONCON = 0x00A0;
    static const unsigned GAP_ADV_PARAMS_INTERVAL_MAX        = 0x4000;
    static const unsigned GAP_ADV_PARAMS_TIMEOUT_MAX         = 0x3FFF;

    /*!
     * Encapsulates the peripheral advertising modes, which determine how
     * the device appears to other central devices in hearing range.
     */
    enum AdvertisingType_t {
        ADV_CONNECTABLE_UNDIRECTED,     /**< Vol 3, Part C, Section 9.3.4 and Vol 6, Part B, Section 2.3.1.1 */
        ADV_CONNECTABLE_DIRECTED,       /**< Vol 3, Part C, Section 9.3.3 and Vol 6, Part B, Section 2.3.1.2 */
        ADV_SCANNABLE_UNDIRECTED,       /**< Include support for Scan Response payloads, see Vol 6, Part B, Section 2.3.1.4 */
        ADV_NON_CONNECTABLE_UNDIRECTED  /**< Vol 3, Part C, Section 9.3.2 and Vol 6, Part B, Section 2.3.1.3 */
    };
    typedef enum AdvertisingType_t AdvertisingType; /* Deprecated type alias. */

public:
    GapAdvertisingParams(AdvertisingType_t advType  = ADV_CONNECTABLE_UNDIRECTED,
                         uint16_t          interval = GAP_ADV_PARAMS_INTERVAL_MIN_NONCON,
                         uint16_t          timeout  = 0) : _advType(advType), _interval(interval), _timeout(timeout) {
        /* Interval checks. */
        if (_advType == ADV_CONNECTABLE_DIRECTED) {
            /* Interval must be 0 in directed connectable mode. */
            _interval = 0;
        } else if (_advType == ADV_NON_CONNECTABLE_UNDIRECTED) {
            /* Min interval is slightly larger than in other modes. */
            if (_interval < GAP_ADV_PARAMS_INTERVAL_MIN_NONCON) {
                _interval = GAP_ADV_PARAMS_INTERVAL_MIN_NONCON;
            }
            if (_interval > GAP_ADV_PARAMS_INTERVAL_MAX) {
                _interval = GAP_ADV_PARAMS_INTERVAL_MAX;
            }
        } else {
            /* Stay within interval limits. */
            if (_interval < GAP_ADV_PARAMS_INTERVAL_MIN) {
                _interval = GAP_ADV_PARAMS_INTERVAL_MIN;
            }
            if (_interval > GAP_ADV_PARAMS_INTERVAL_MAX) {
                _interval = GAP_ADV_PARAMS_INTERVAL_MAX;
            }
        }

        /* Timeout checks. */
        if (timeout) {
            /* Stay within timeout limits. */
            if (_timeout > GAP_ADV_PARAMS_TIMEOUT_MAX) {
                _timeout = GAP_ADV_PARAMS_TIMEOUT_MAX;
            }
        }
    }

    static const uint16_t UNIT_0_625_MS = 625;  /**< Number of microseconds in 0.625 milliseconds. */
    static uint16_t MSEC_TO_ADVERTISEMENT_DURATION_UNITS(uint32_t durationInMillis) {
        return (durationInMillis * 1000) / UNIT_0_625_MS;
    }
    static uint16_t ADVERTISEMENT_DURATION_UNITS_TO_MS(uint16_t gapUnits) {
        return (gapUnits * UNIT_0_625_MS) / 1000;
    }

    AdvertisingType_t getAdvertisingType(void) const {
        return _advType;
    }

    /**
     * @return the advertisement interval (in milliseconds).
     */
    uint16_t getInterval(void) const {
        return ADVERTISEMENT_DURATION_UNITS_TO_MS(_interval);
    }

    /**
     * @return the advertisement interval in advertisement duration units (0.625ms units).
     */
    uint16_t getIntervalInADVUnits(void) const {
        return _interval;
    }

    uint16_t getTimeout(void) const {
        return _timeout;
    }

    void setAdvertisingType(AdvertisingType_t newAdvType) {_advType = newAdvType;  }
    void setInterval(uint16_t newInterval)                {_interval = MSEC_TO_ADVERTISEMENT_DURATION_UNITS(newInterval);}
    void setTimeout(uint16_t newTimeout)                  {_timeout = newTimeout;  }

private:
    AdvertisingType_t _advType;
    uint16_t          _interval; /* In ADV duration units (i.e. 0.625ms). */
    uint16_t          _timeout;  /* In seconds. */
};

#endif // ifndef __GAP_ADVERTISING_PARAMS_H__
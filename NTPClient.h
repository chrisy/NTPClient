#pragma once

#include "Arduino.h"

#include <Udp.h>

#define SEVENZYYEARS 2208988800UL
#define NTP_PACKET_SIZE 48
#define NTP_DEFAULT_LOCAL_PORT 1337

class NTPClient {
  private:
    UDP*          _udp;
    bool          _udpSetup       = false;

    const char*   _poolServerName = "pool.ntp.org"; // Default time server
    IPAddress     _poolServerIP;
    int           _port           = NTP_DEFAULT_LOCAL_PORT;
    long          _timeOffset     = 0;
    bool          _fractionalTime = false;

    unsigned long _updateInterval = 60000;  // In ms

    unsigned long _currentEpoc    = 0;      // In s
    unsigned long _lastUpdate     = 0;      // In ms
    unsigned long _fracOffset     = 0;      // In ms

    bool          _packetSent     = false;

    byte          _packetBuffer[NTP_PACKET_SIZE];

    void          sendNTPPacket();

  public:
    NTPClient(UDP& udp);
    NTPClient(UDP& udp, long timeOffset);
    NTPClient(UDP& udp, const char* poolServerName);
    NTPClient(UDP& udp, const char* poolServerName, long timeOffset);
    NTPClient(UDP& udp, const char* poolServerName, long timeOffset, unsigned long updateInterval);
    NTPClient(UDP& udp, IPAddress poolServerIP);
    NTPClient(UDP& udp, IPAddress poolServerIP, long timeOffset);
    NTPClient(UDP& udp, IPAddress poolServerIP, long timeOffset, unsigned long updateInterval);

    /**
     * Set time server name
     *
     * @param poolServerName
     */
    void setPoolServerName(const char* poolServerName);

    /**
     * Starts the underlying UDP client with the default local port
     */
    void begin();

    /**
     * Starts the underlying UDP client with the specified local port
     */
    void begin(int port);

    /**
     * This should be called in the main loop of your application. By default an update from the NTP Server is only
     * made every 60 seconds. This can be configured in the NTPClient constructor.
     *
     * @return true on success, false on failure
     */
    bool update();

    /**
     * This will force the update from the NTP Server.
     *
     * @return true on success, false on failure
     */
    bool forceUpdate();

    int getDay() const;
    int getHours() const;
    int getMinutes() const;
    int getSeconds() const;

    /**
     * Changes the time offset. Useful for changing timezones dynamically
     */
    void setTimeOffset(int timeOffset);

    /**
     * Set the update interval to another frequency. E.g. useful when the
     * timeOffset should not be set in the constructor
     */
    void setUpdateInterval(unsigned long updateInterval);

    /** Enable or disable tracking fractional seconds from the NTP response.
     * We don't return time with that precision, but we do account for it when
     * working out what the time now is.
     */
    void setFractionalTime(bool enable);

    /**
     * @return time formatted like `hh:mm:ss`
     */
    String getFormattedTime() const;

    /**
     * @return time in seconds since Jan. 1, 1970
     */
    unsigned long getEpochTime() const;

    /** If we track the fractional part of the NTP resopnse, return the offset
     * in milliseconds.
     */
    unsigned long getFractionalOffset() const;

    /** Whether an NTP packet was send since the last time @c update was called.
     * Can be used to disambiguate whether the @c false return from @c update was
     * because it was not yet time, or because no response was received.
     */
    bool getPacketSent() const;

    /**
     * Stops the underlying UDP client
     */
    void end();
};

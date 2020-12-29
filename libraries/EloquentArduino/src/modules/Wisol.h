#pragma once

#include <Arduino.h>


namespace Eloquent {

    /**
     * Use Sigfox module with ease
     */
    class Wisol {

    public:
        static const int SOFTWARE_RESET = 0;
        static const int SLEEP = 1;
        static const int DEEP_SLEEP = 2;

        /**
         * Construct Wisol from Stream (both Software or Hardware)
         */
        Wisol(Stream *stream) {
            _stream = stream;
        }

        /**
         * Check if module is connected
         */
        bool available() {
            return getID().length() > 0;
        }

        /**
         * Dump module info (ID and PAC)
         */
        String dumpInfo() {
            return String("ID :\t") + getID() + "\r\nPAC:\t" + getPAC();
        }

        /**
         *
         */
        void reset() {
            cmd("AT$P=0\r");
        }

        /**
         *
         */
        bool wakeup() {
            _stream->print('\n');

            return cmd("AT").indexOf("OK") >= 0;
        }

        /**
         *
         */
        String getID() {
            return cmd("AT$I=10\r");
        }

        /**
         *
         */
        String getPAC() {
            return cmd("AT$I=11\r");
        }

        /**
         *
         */
        uint16_t getTemp() {
            String temp = cmd("AT$T?\r");

            return temp.toInt();
        }

        /**
         *
         */
        String getVoltages() {
            String temp = cmd("AT$V?\r");

            return temp;
        }

        /**
         *
         */
        bool setPowerMode(uint8_t mode) {
            return set("AT$P", mode);
        }

        /**
         *
         */
        bool setOutputPower(uint8_t power) {
            return set("ATS302", power);
        }

        /**
         *
         */
        bool enableSoftwareResetMode() {
            return setPowerMode(SOFTWARE_RESET);
        }

        /**
         *
         */
        bool enableSleepMode() {
            return setPowerMode(SLEEP);
        }

        /**
         *
         */
        bool enableDeepSleepMode() {
            return setPowerMode(DEEP_SLEEP);
        }

        /**
         * Send command and get response
         */
        String cmd(String cmd) {
            _stream->print(cmd);

            return read();
        }

        /**
         * Get response
         */
        String read(uint16_t timeout = 2000) {
            uint32_t start = millis();
            String data = "";

            if (timeout == 0)
                timeout = 2000;

            while (millis() - start <= timeout && !_stream->available())
                delay(10);

            while (millis() - start <= timeout && _stream->available()) {
                data += (char) _stream->read();
            }

            data.trim();

            return data;
        }

        /**
         *
         * @param byte
         */
        void printByte(uint8_t byte) {
            if (byte < 16)
                _stream->print(0);
            _stream->print(byte, HEX);
        }

        /**
         * Send data.
         */
        void sendNoAck(const void *data, uint8_t size, bool ack = false) {
            uint8_t *bytes = (uint8_t *) data;

            _stream->print("AT$SF=");

            for (uint8_t i = 0; i < size; ++i)
                printByte(bytes[i]);

            if (ack)
                _stream->print(",1");

            _stream->print("\r");
        }

        bool send(const void *data, uint8_t size, bool ack = false) {
            sendNoAck(data, size, ack);

            return read(30000).indexOf("OK") >= 0;
        }


    protected:
        Stream *_stream;


        /**
         * Set key=value
         */
        String set(const char *key, uint8_t mode) {
            _stream->print(key);
            _stream->print('=');
            _stream->print(mode);
            _stream->print('\r');

            return read(3000) == "OK";
        }
    };
}

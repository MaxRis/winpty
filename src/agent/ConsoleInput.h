// Copyright (c) 2011-2015 Ryan Prichard
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to
// deal in the Software without restriction, including without limitation the
// rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
// sell copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
// FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
// IN THE SOFTWARE.

#ifndef CONSOLEINPUT_H
#define CONSOLEINPUT_H

#include <windows.h>
#include <stdint.h>

#include <string>
#include <vector>

#include "InputMap.h"

class Win32Console;
class DsrSender;

class ConsoleInput
{
public:
    ConsoleInput(DsrSender *dsrSender);
    ~ConsoleInput();
    void writeInput(const std::string &input);
    void flushIncompleteEscapeCode();

private:
    void doWrite(bool isEof);
    int scanKeyPress(std::vector<INPUT_RECORD> &records,
                     const char *input,
                     int inputSize,
                     bool isEof);
    void appendUtf8Char(std::vector<INPUT_RECORD> &records,
                        const char *charBuffer,
                        int charLen,
                        uint16_t keyState);
    void appendKeyPress(std::vector<INPUT_RECORD> &records,
                        uint16_t virtualKey,
                        uint16_t unicodeChar,
                        uint16_t keyState);
    void appendInputRecord(std::vector<INPUT_RECORD> &records,
                           BOOL keyDown,
                           uint16_t virtualKey,
                           uint16_t unicodeChar,
                           uint16_t keyState);
    static int utf8CharLength(char firstByte);
    static int matchDsr(const char *input, int inputSize);

private:
    Win32Console *m_console;
    DsrSender *m_dsrSender;
    bool m_dsrSent;
    std::string m_byteQueue;
    InputMap m_inputMap;
    DWORD lastWriteTick;
};

#endif // CONSOLEINPUT_H
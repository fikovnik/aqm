#ifndef DEBUG_H
#define DEBUG_H

#ifdef NDEBUG
#define DEBUG(...)
#else
#define DEBUG(...) Serial.printf(__VA_ARGS__)
#endif

#endif // DEBUG_H

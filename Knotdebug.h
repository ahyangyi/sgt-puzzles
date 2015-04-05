#ifndef Knotdebug_HEADER
#define Knotdebug_HEADER

#include <QString>

#ifdef KNOTPLASM_DEBUG
void knotDebugClear (QString category);
void knotDebugAppend (QString category, QString s);
void knotDebugFlush ();
#else
#define knotDebugClear(category)
#define knotDebugAppend(category, s)
#define knotDebugFlush()
#endif

#endif
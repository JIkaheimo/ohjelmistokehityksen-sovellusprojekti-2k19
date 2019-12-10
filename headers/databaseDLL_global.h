#ifndef DATABASEDLL_GLOBAL_H
#define DATABASEDLL_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(DATABASEDLL_LIBRARY)
#  define DATABASEDLL_EXPORT Q_DECL_EXPORT
#else
#  define DATABASEDLL_EXPORT Q_DECL_IMPORT
#endif

#endif // DATABASEDLL_GLOBAL_H
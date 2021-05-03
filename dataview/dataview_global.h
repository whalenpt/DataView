#ifndef DATAVIEW_GLOBAL_H
#define DATAVIEW_GLOBAL_H

//#include <QtCore/qglobal.h>
#include <QtCore/QtGlobal>

#if defined(DATAVIEW_LIBRARY)
#  define DATAVIEWSHARED_EXPORT Q_DECL_EXPORT
#else
#  define DATAVIEWSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // DATAVIEW_GLOBAL_H


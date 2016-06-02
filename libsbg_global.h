#ifndef LIBSBG_GLOBAL_H
#define LIBSBG_GLOBAL_H

#include <QtCore/qglobal.h>

#if defined(LIBSBG_LIBRARY)
#  define LIBSBGSHARED_EXPORT Q_DECL_EXPORT
#else
#  define LIBSBGSHARED_EXPORT Q_DECL_IMPORT
#endif

#endif // LIBSBG_GLOBAL_H

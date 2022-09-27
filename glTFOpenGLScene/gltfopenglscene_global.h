#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(GLTFOPENGLSCENE_LIB)
#  define GLTFOPENGLSCENE_EXPORT Q_DECL_EXPORT
# else
#  define GLTFOPENGLSCENE_EXPORT Q_DECL_IMPORT
# endif
#else
# define GLTFOPENGLSCENE_EXPORT
#endif

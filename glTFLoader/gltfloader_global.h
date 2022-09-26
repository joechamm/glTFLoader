#pragma once

#include <QtCore/qglobal.h>

#ifndef BUILD_STATIC
# if defined(GLTFLOADER_LIB)
#  define GLTFLOADER_EXPORT Q_DECL_EXPORT
# else
#  define GLTFLOADER_EXPORT Q_DECL_IMPORT
# endif
#else
# define GLTFLOADER_EXPORT
#endif

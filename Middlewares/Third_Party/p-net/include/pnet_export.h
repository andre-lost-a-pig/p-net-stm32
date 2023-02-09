
#ifndef PNET_EXPORT_H
#define PNET_EXPORT_H

#ifdef PNET_STATIC_DEFINE
#  define PNET_EXPORT
#  define PNET_NO_EXPORT
#else
#  ifndef PNET_EXPORT
#    ifdef profinet_EXPORTS
        /* We are building this library */
#      define PNET_EXPORT 
#    else
        /* We are using this library */
#      define PNET_EXPORT 
#    endif
#  endif

#  ifndef PNET_NO_EXPORT
#    define PNET_NO_EXPORT 
#  endif
#endif

#ifndef PNET_DEPRECATED
#  define PNET_DEPRECATED __attribute__ ((__deprecated__))
#endif

#ifndef PNET_DEPRECATED_EXPORT
#  define PNET_DEPRECATED_EXPORT PNET_EXPORT PNET_DEPRECATED
#endif

#ifndef PNET_DEPRECATED_NO_EXPORT
#  define PNET_DEPRECATED_NO_EXPORT PNET_NO_EXPORT PNET_DEPRECATED
#endif

#if 0 /* DEFINE_NO_DEPRECATED */
#  ifndef PNET_NO_DEPRECATED
#    define PNET_NO_DEPRECATED
#  endif
#endif

#endif /* PNET_EXPORT_H */

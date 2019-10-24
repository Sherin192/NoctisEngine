

#if defined(_WIN32)
#define NOCTIS_MSVC_ONLY(...) __VA_ARGS__
#else
#define NOCTIS_MSVC_ONLY(…)
#endif 
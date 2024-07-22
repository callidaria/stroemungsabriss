#ifndef CCB_FEATURE_LOGGER
#define CCB_FEATURE_LOGGER

#define DEBUG
// TODO: move debug definition flag


// text colour definition
constexpr const char* LOG_RED = "\033[1;31m";
constexpr const char* LOG_BLUE = "\033[1;36m";
constexpr const char* LOG_CLEAR = "\033[0m";


// logger macro definition to include component in debug build
#ifdef DEBUG

#define COMM_MSG(msg,col) printf("%s%s%s\n",col,msg,LOG_CLEAR);
#define COMM_ERR(msg) printf("%s%s%s\n",LOG_RED,msg,LOG_CLEAR);

// logger macro definition to exclude component in release build
#else

#define COMM_MSG(msg,col)
#define COMM_ERR(msg)

#endif

#endif

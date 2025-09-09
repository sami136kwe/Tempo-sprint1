#include <time.h>

/**
 * A portable version of the strdup function
 *
 * Note: The returned string must be freed when no longer necessary.
 *
 * @param s  The string to duplicate
 * @return   The duplicated string
 */
char* utils_strdup(const char *s);

/**
 * A portable version of the timegm function
 *
 * Note: This function is not thread-safe.
 *
 * @param tm  The 'struct tm' instance to convert
 * @return    The resulting 'time_t' timestamp
 */
time_t utils_timegm(struct tm *tm);

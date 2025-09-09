#include "utils.h"

#include <stdlib.h>
#include <string.h>

// Prototypes for preventing warnings
int setenv(const char* name, const char* value, int overwrite);
int unsetenv(const char* name);
void tzset(void);

char* utils_strdup(const char *s) {
  if (s == NULL) {
    return NULL;
  }
  size_t len = strlen(s);
  char *new_string = (char *)malloc(len + 1);
  if (new_string == NULL) {
    return NULL;
  }
  memcpy(new_string, s, len + 1);
  return new_string;
}

time_t utils_timegm(struct tm *tm) {
  time_t ret;
  char *tz;
  tz = getenv("TZ");
  if (tz)
    tz = utils_strdup(tz);
  setenv("TZ", "", 1);
  tzset();
  ret = mktime(tm);
  if (tz) {
    setenv("TZ", tz, 1);
    free(tz);
  } else
    unsetenv("TZ");
  tzset();
  return ret;
}

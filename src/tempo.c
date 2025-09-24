#include <stdio.h>
#include <time.h>
#include <string.h>    

// Constants
// ---------

// The usage
#define HELP "Usage: tempo SUBCOMMAND\n\
Displays information about a timeseries.\n\
\n\
SUBCOMMAND is mandatory and must take one of the following values:\n\
  describe: describes the timeseries\n\
  help: shows this message\n\
  show: list the observations of the timeseries\n\
\n\
A timeseries is a text stream that must satisfy the following syntax:\n\
\n\
  1. The first line must contain a valid reference datetime with format\n\
       YYYY-mm-DDTHH:MM:SS\n\
     where YYYY are the 4 digits for the year,\n\
           mm are the 2 digits for the month,\n\
           DD are the 2 digits for the day,\n\
           HH are the 2 digits for the hour,\n\
           MM are the 2 digits for the minutes and\n\
           SS are the 2 digits for the seconds\n\
  2. Each of the remaining line must contain an observation, given with the\n\
     format\n\
       OFFSET VALUE\n\
     where OFFSET is a positive integer indicating the number of seconds offset\n\
                  with respect to the reference datetime and\n\
           VALUE is an integer\n"
// The length of a datetime string
#define DATETIME_LENGTH 19
// The maximum size of a timeseries
#define MAX_SIZE 1000

// Types
// -----

// A datetime
struct Datetime {
  // The 'time_t' representation of the datetime
  time_t t;
  // The RFC3339 string representation of the datetime
  char rfc3339_string[DATETIME_LENGTH + 1];
};

// A timeseries
struct Timeseries {
  // The start datetime of the timeseries
  struct Datetime start_datetime;
  // The last datetime of the timeseries
  struct Datetime last_datetime;
  // The size of the timeseries
  unsigned int size;
  // The offsets of the observations
  int offsets[MAX_SIZE];
  // The observed values
  int values[MAX_SIZE];
};



/**
 * Validates and parses command line arguments
 * @param argc Number of arguments
 * @param argv Array of arguments
 * @return Command type (0=help, 1=show, 2=describe) or -1 for error
 */
int parse_command(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "error: subcommand is mandatory\n");
        return -1;
    }
    
    if (strcmp(argv[1], "help") == 0) {
        return 0;
    } else if (strcmp(argv[1], "show") == 0) {
        return 1;
    } else if (strcmp(argv[1], "describe") == 0) {
        return 2;
    } else {
        fprintf(stderr, "error: unrecognized subcommand '%s'\n", argv[1]);
        return -1;
    }
}
/**
 * Parses a datetime string in YYYY-mm-DDTHH:MM:SS format
 * @param datetime_str String to parse
 * @param datetime Output datetime structure
 * @return 0 on success, -1 on format error, -2 on invalid datetime
 */
int parse_datetime(const char *datetime_str, struct Datetime *datetime);

/**
 * Parses an observation line (offset value)
 * @param line Line to parse
 * @param offset Output offset
 * @param value Output value
 * @return 0 on success, -1 on format error, -2 on negative offset
 */
int parse_observation(const char *line, int *offset, int *value);

/**
 * Reads and validates a timeseries from stdin
 * @param ts Output timeseries structure
 * @return 0 on success, error code otherwise
 */
int read_timeseries(struct Timeseries *ts);

/**
 * Shows the timeseries (show command)
 * @param ts Timeseries to display
 */
void show_timeseries(const struct Timeseries *ts);

/**
 * Describes the timeseries (describe command)
 * @param ts Timeseries to describe
 */
void describe_timeseries(const struct Timeseries *ts);

/**
 * Converts offset to datetime
 * @param start_time Reference time
 * @param offset Offset in seconds
 * @param result Output datetime
 */
void offset_to_datetime(time_t start_time, int offset, struct Datetime *result);

// Main
// ----

int main(int argc, char *argv[]) {
    int cmd = parse_command(argc, argv);
    
    if (cmd == -1) {
        return 1;
    }
    
    switch (cmd) {
        case 0: // help
            printf("%s", HELP);
            break;
        case 1: // show
            printf("TODO: implement show command\n");
            break;
        case 2: // describe
            printf("TODO: implement describe command\n");
            break;
    }
    
    return 0;
}

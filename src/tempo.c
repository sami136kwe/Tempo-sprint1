/**
 * @file tempo.c
 * @brief A command-line application to read and analyze time series data.
 *
 * This program reads a time series from standard input, including a reference
 * timestamp and a sequence of (offset, value) observations. It can display the
 * time series in chronological order (`show`) or describe its characteristics
 * (`describe`). It also provides a usage guide (`help`).
 *
 * Time is parsed using the RFC3339 format (YYYY-mm-DDTHH:MM:SS), and observations
 * consist of non-negative integer offsets (in seconds) and integer values.
 *
 * Example usage:
 *   $ ./tempo show < examples/6.ts
 *   $ ./tempo describe < examples/6.ts
 *   $ ./tempo help
 * 
 * @author SAMI GEAGEA GEAS72090507
 * @version 1.0
 */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include "utils.h"

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
// The maximum length of a line read from input


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


// struct used for datetime parsing
struct DateAndTime {
    int verify_sec;
    int verify_min;
    int verify_hours;
    int verify_day;
    int verify_month;
    int verify_year;
};

// Prototypes
int  parse_command(int argc, char *argv[]);
int  parse_datetime(const char *datetime_str, struct Datetime *datetime);
int  parse_observation(const char *line, int *offset, int *value);
int  read_timeseries(struct Timeseries *ts);
void show_timeseries(const struct Timeseries *ts);
void describe_timeseries(const struct Timeseries *ts);
void offset_to_datetime(time_t start_time, int offset, struct Datetime *result);


/**
 * @brief Parses the command-line argument to determine the subcommand.
 *
 * This function validates the presence and validity of the subcommand (`help`, `show`, or `describe`).
 * Returns an integer code representing the parsed command.
 *
 * @param argc Number of arguments passed to the program.
 * @param argv Array of argument strings.
 *
 * @return 0 if the command is "help"  
 *         1 if the command is "show"  
 *         2 if the command is "describe"  
 *         -1 if the command is missing or unrecognized
 *
 * @exception Prints error message to stderr if command is missing or invalid.
 *
 *  
 * @version 1.0
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
 * @brief Parses a datetime string in RFC3339 format and populates a Datetime struct.
 *
 * This function checks the format of the string "YYYY-mm-DDTHH:MM:SS", validates
 * its components, and fills a `struct Datetime` with both `time_t` and formatted string.
 *
 * @param datetime_str Input string to parse (must match the RFC3339 format).
 * @param datetime Output structure containing the parsed datetime.
 *
 * @return 0 on success  
 *         -1 if the format is incorrect  
 *         -2 if the datetime components are invalid
 *
 * @exception None
 *
 *  
 * @version 1.0
 */
int parse_datetime(const char *datetime_str, struct Datetime *datetime) {
    if (strlen(datetime_str) != DATETIME_LENGTH) return -1;

    struct DateAndTime verify;

    if (sscanf(datetime_str, "%4d-%2d-%2dT%2d:%2d:%2d",
               &verify.verify_year, &verify.verify_month, &verify.verify_day,
               &verify.verify_hours, &verify.verify_min, &verify.verify_sec) != 6) {
        return -1;
    }

    if (datetime_str[4] != '-' || datetime_str[7] != '-' ||
        datetime_str[10] != 'T' || datetime_str[13] != ':' ||
        datetime_str[16] != ':') return -1;

    if (verify.verify_sec   < 0 || verify.verify_sec   > 59) return -2;
    if (verify.verify_min   < 0 || verify.verify_min   > 59) return -2;
    if (verify.verify_hours < 0 || verify.verify_hours > 23) return -2;
    if (verify.verify_month < 1 || verify.verify_month > 12) return -2;
    if (verify.verify_year  < 1900 || verify.verify_year > 3000) return -2;

    int days_in_month[13] = {0,31,28,31,30,31,30,31,31,30,31,30,31};
    int leap = (verify.verify_year % 4 == 0 &&
               (verify.verify_year % 100 != 0 || verify.verify_year % 400 == 0));
    if (leap) days_in_month[2] = 29;

    if (verify.verify_day < 1 || verify.verify_day > days_in_month[verify.verify_month]) return -2;

    struct tm tm_time;
    tm_time.tm_year = verify.verify_year - 1900;
    tm_time.tm_mon  = verify.verify_month - 1;
    tm_time.tm_mday = verify.verify_day;
    tm_time.tm_hour = verify.verify_hours;
    tm_time.tm_min  = verify.verify_min;
    tm_time.tm_sec  = verify.verify_sec;
    tm_time.tm_isdst = -1;

    datetime->t = utils_timegm(&tm_time);
    strncpy(datetime->rfc3339_string, datetime_str, DATETIME_LENGTH);
    datetime->rfc3339_string[DATETIME_LENGTH] = '\0';

    return 0;
}

/**
 * @brief Parses a line containing a time offset and an integer value.
 *
 * This function expects the format: `<offset> <value>`, where offset is a
 * non-negative integer and value is a valid integer. It also validates input.
 *
 * @param line The line to parse (e.g., "3600 42").
 * @param offset Output variable for the offset in seconds.
 * @param value Output variable for the observed value.
 *
 * @return 0 on success  
 *         -1 if the format is incorrect  
 *         -2 if the offset is negative
 *
 * @exception None
 *
 *  
 * @version 1.0
 */
int parse_observation(const char *line, int *offset, int *value) {
    char value_str[64];
    char extra;

    if (sscanf(line, "%d %63s", offset, value_str) != 2) return -1;
    if (*offset < 0) return -2;
    if (sscanf(value_str, "%d%c", value, &extra) != 1) return -1;

    return 0;
}

/**
 * @brief Reads and validates a timeseries from standard input.
 *
 * This function reads a reference datetime from the first line,
 * then parses each subsequent line as an observation (offset and value).
 * Observations must follow the format described in the TP1 specification.
 *
 * @param ts Pointer to a Timeseries structure that will be filled with parsed data.
 *
 * @return 0 if the timeseries is successfully read  
 *         -3 if an error occurs (invalid datetime, format error, negative offset, etc.)
 *
 * @exception Prints detailed error messages to stderr for invalid input format.
 *
 *  
 * @version 1.0
 */
int read_timeseries(struct Timeseries *ts) {
    char buffer[256];
    memset(ts, 0, sizeof(*ts));

    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        ts->size = 0;
        return 0;
    }

    buffer[strcspn(buffer, "\r\n")] = 0;

    int dt_result = parse_datetime(buffer, &ts->start_datetime);
    if (dt_result == -1) {
        fprintf(stderr, "error: invalid datetime format (should be YYYY-mm-DDTHH:MM:SS)\n");
        return -3;
    } else if (dt_result == -2) {
        fprintf(stderr, "error: invalid datetime (%s)\n", buffer);
        return -3;
    }

    unsigned int i = 0;
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {
        buffer[strcspn(buffer, "\r\n")] = 0;
        if (i >= MAX_SIZE) return 2;

        int result = parse_observation(buffer, &ts->offsets[i], &ts->values[i]);
        if (result == -1) {
            fprintf(stderr, "error: invalid observation format (%s)\n", buffer);
            return -3;
        } else if (result == -2) {
            fprintf(stderr, "error: invalid offset (%s)\n", buffer);
            return -3;
        }
        i++;
    }

    ts->size = i;
    return 0;
}

/**
 * @brief Displays the timeseries in chronological order (show command).
 *
 * Converts each offset to an absolute datetime, sorts the observations by time,
 * and prints each (datetime, value) pair. Duplicate timestamps are removed, keeping
 * only the latest observation for each time.
 *
 * @param ts Pointer to a Timeseries structure to be displayed.
 *
 * @return None (void)
 *
 * @exception None
 *
 *  
 * @version 1.0
 */
void show_timeseries(const struct Timeseries *ts) {
    if (ts->size == 0) return;

    struct Datetime datetimes[MAX_SIZE];
    int values[MAX_SIZE];

    for (unsigned int i = 0; i < ts->size; i++) {
        offset_to_datetime(ts->start_datetime.t, ts->offsets[i], &datetimes[i]);
        values[i] = ts->values[i];
    }

    for (unsigned int i = 0; i < ts->size; i++) {
        for (unsigned int j = i + 1; j < ts->size; j++) {
            if (datetimes[i].t > datetimes[j].t) {
                struct Datetime tmp_dt = datetimes[i];
                datetimes[i] = datetimes[j];
                datetimes[j] = tmp_dt;

                int tmp_val = values[i];
                values[i] = values[j];
                values[j] = tmp_val;
            }
        }
    }

    for (unsigned int i = 0; i < ts->size; i++) {
        if (i + 1 < ts->size && datetimes[i].t == datetimes[i + 1].t) {
            continue;
        }
        printf("%s %d\n", datetimes[i].rfc3339_string, values[i]);
    }
}

/**
 * @brief Describes key properties of the timeseries (describe command).
 *
 * Computes and displays:
 *   - Domain: earliest to latest datetime of observations
 *   - Codomain: smallest to largest value
 *   - Size: number of unique observations
 *   - Duration: seconds between first and last observation
 *   - Amplitude: difference between max and min observed values
 *
 * If the timeseries is empty, appropriate default values are shown.
 *
 * @param ts Pointer to a Timeseries structure to analyze.
 *
 * @return None (void)
 *
 * @exception None
 *
 *  
 * @version 1.0
 */
void describe_timeseries(const struct Timeseries *ts) {
    if (ts->size == 0) {
        printf("Domain: [%s, %s]\n", ts->start_datetime.rfc3339_string, ts->start_datetime.rfc3339_string);
        printf("Codomain: none\n");
        printf("Size: 0\n");
        printf("Duration: 0\n");
        printf("Amplitude: 0\n");
        return;
    }

    struct Datetime datetimes[MAX_SIZE];
    int values[MAX_SIZE];
    int unique_size = 0;

    for (unsigned int i = 0; i < ts->size; i++) {
        struct Datetime dt;
        offset_to_datetime(ts->start_datetime.t, ts->offsets[i], &dt);

        int found = -1;
        for (int j = 0; j < unique_size; j++) {
            if (datetimes[j].t == dt.t) {
                found = j;
                break;
            }
        }

        if (found >= 0) {
            datetimes[found] = dt;
            values[found] = ts->values[i];
        } else {
            datetimes[unique_size] = dt;
            values[unique_size] = ts->values[i];
            unique_size++;
        }
    }

    struct Datetime min_dt = datetimes[0];
    struct Datetime max_dt = datetimes[0];
    for (int i = 1; i < unique_size; i++) {
        if (datetimes[i].t < min_dt.t) min_dt = datetimes[i];
        if (datetimes[i].t > max_dt.t) max_dt = datetimes[i];
    }

    int min_val = values[0];
    int max_val = values[0];
    for (int i = 1; i < unique_size; i++) {
        if (values[i] < min_val) min_val = values[i];
        if (values[i] > max_val) max_val = values[i];
    }

    printf("Domain: [%s, %s]\n", min_dt.rfc3339_string, max_dt.rfc3339_string);
    printf("Codomain: [%d, %d]\n", min_val, max_val);
    printf("Size: %u\n", unique_size);
    printf("Duration: %ld\n", max_dt.t - min_dt.t);
    printf("Amplitude: %d\n", max_val - min_val);
}

/**
 * @brief Converts an offset (in seconds) into an absolute datetime.
 *
 * Adds the given offset to the reference datetime (in `time_t` format) and
 * converts the result into an RFC3339 formatted string stored in a Datetime struct.
 *
 * @param start_time The reference time (usually from the start of the timeseries).
 * @param offset Offset in seconds to be added to the reference time.
 * @param result Pointer to a Datetime struct to store the resulting absolute datetime.
 *
 * @return None (void)
 *
 * @exception None
 *
 * @see parse_datetime
 * @see read_timeseries
 *
 *  
 * @version 1.0
 */
void offset_to_datetime(time_t start_time, int offset, struct Datetime *result) {
    result->t = start_time + offset;

    struct tm tm = {0};
    gmtime_r(&result->t, &tm);

    strftime(result->rfc3339_string, sizeof(result->rfc3339_string),
             "%Y-%m-%dT%H:%M:%S", &tm);
}

/**
 * @brief Entry point of the program.
 *
 * Parses the subcommand and delegates execution to the corresponding function:
 *   - `help`: prints usage guide
 *   - `show`: reads and displays the timeseries
 *   - `describe`: computes and prints statistics of the timeseries
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of argument strings.
 *
 * @return 0 on successful execution  
 *         1 on usage error (invalid or missing subcommand)  
 *         2 on input error (invalid format, data error)
 *
 * @exception Prints error messages to stderr as specified in TP1 instructions.
 *
 * @see parse_command
 * @see read_timeseries
 * @see show_timeseries
 * @see describe_timeseries
 *
 *  
 * @version 1.0
 */
int main(int argc, char *argv[]) {
    int cmd = parse_command(argc, argv);
    if (cmd == -1) return 1;

    switch (cmd) {
        case 0:
            printf("%s", HELP);
            return 0;
        case 1: {
            struct Timeseries ts;
            int err = read_timeseries(&ts);
            if (err != 0) return 2;
            show_timeseries(&ts);
            return 0;
        }
        case 2: {
            struct Timeseries ts;
            int err = read_timeseries(&ts);
            if (err != 0) return 2;
            describe_timeseries(&ts);
            return 0;
        }
        default:
            return 1;
    }
}

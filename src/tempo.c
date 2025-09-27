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
int parse_datetime(const char *datetime_str, struct Datetime *datetime) {
    
    if ( strlen(datetime_str)!=DATETIME_LENGTH) {
         return -1; // error
    }
    
    //struct type variable suivie nom exemple Points2d type de la varible et p cest le nom de la variable 
    //struct Point2d p;
    
    struct DateAndTime {
        int verify_sec;
        int verify_min;
        int verify_hours;
        int verify_day;
        int verify_month;
        int verify_year;
    };
    
    struct DateAndTime verify;
    
    if (sscanf(datetime_str, "%4d-%2d-%2dT%2d:%2d:%2d",
           &verify.verify_year,
           &verify.verify_month,
           &verify.verify_day,
           &verify.verify_hours,
           &verify.verify_min,
           &verify.verify_sec) != 6) {
    return -1;
}
    // The scanf function returns an integer value which indicates the number of 
    // input items successfully matched and assigned.  
    // si scanf ne lit pas exactement 6 champs error
    
    if (datetime_str[4]  != '-') return -1;
    if (datetime_str[7]  != '-') return -1;
    if (datetime_str[10] != 'T') return -1;
    if (datetime_str[13] != ':') return -1;
    if (datetime_str[16] != ':') return -1;
    
    if (verify.verify_sec < 0 || verify.verify_sec > 59) return -2;
    if (verify.verify_min < 0 || verify.verify_min > 59) return -2;
    if (verify.verify_hours < 0 || verify.verify_hours > 23) return -2;
    if (verify.verify_month < 1 || verify.verify_month > 12) return -2;
    
     // number of days in a month
    
    int days_in_month [13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    
    
    // int leap = (verify.verify_year % 4 == 0 &&
    //            (verify.verify_year % 100 != 0 || verify.verify_year % 400 == 0));
    // if (leap) {
    //     days_in_month[2] = 29;
    // }

    if (verify.verify_day < 1 || verify.verify_day > days_in_month[verify.verify_month]) {
        return -2;
    }
    
    
  
    
 // build struct tm of from the standard structure of the library <time.h>
    
    struct tm tm_time;
    tm_time.tm_year = verify.verify_year - 1900;
    tm_time.tm_mon  = verify.verify_month - 1;
    tm_time.tm_mday = verify.verify_day;
    tm_time.tm_hour = verify.verify_hours;
    tm_time.tm_min  = verify.verify_min;
    tm_time.tm_sec  = verify.verify_sec;
    tm_time.tm_isdst = -1;

    // Convertir en time_t avec utils_timegm
    
    datetime->t = utils_timegm(&tm_time);

    // Copier la chaîne d’origine
    strncpy(datetime->rfc3339_string, datetime_str, DATETIME_LENGTH);
    datetime->rfc3339_string[DATETIME_LENGTH] = '\0';

    return 0; // succès
}




/**
 * Parses an observation line (offset value)
 * @param line Line to parse
 * @param offset Output offset
 * @param value Output value
 * @return 0 on success, -1 on format error, -2 on negative offset
 */
int parse_observation(const char *line, int *offset, int *value){
    
    if ( sscanf(line, "%d %d",offset,value)!=2) return -1;
    
    if (*offset < 0)return -2;
    
    return 0;
}

/**
 * Reads and validates a timeseries from stdin
 * @param ts Output timeseries structure
 * @return 0 on success, error code otherwise
 */
int read_timeseries(struct Timeseries *ts) {
    // TODO: implémenter lecture complète
    return 0;
}


/**
 * Shows the timeseries (show command)
 * @param ts Timeseries to display
 */
void show_timeseries(const struct Timeseries *ts) {
    // TODO: implémenter affichage

}
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
    
    
    // test 
    int o, v;
    char line[100];

    while (fgets(line, sizeof(line), stdin)) {
        int res = parse_observation(line, &o, &v);
        printf("res=%d offset=%d value=%d\n", res, o, v);
        
        
     int cmd = parse_command(argc, argv);

    if (cmd == -1) {
        return 1; // error usage
    }

    switch (cmd) {
        case 0: // help
            printf("%s", HELP);
            return 0;
        case 1: // show
            struct Timeseries ts;
            int err = read_timeseries(&ts);
            if (err != 0) return 2;
            show_timeseries(&ts);
            return 0;
        case 2: // describe
            // TODO: implement tempo_describe()
            return 0;
        default:
            return 1;
    }
    
    
    
    
    } 
}

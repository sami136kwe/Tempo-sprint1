#include <stdio.h>
#include <time.h>
#include <string.h>
#include "utils.h"

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

// A datetime
struct Datetime {
    time_t t;                                     // The 'time_t' representation of the datetime
    char   rfc3339_string[DATETIME_LENGTH + 1];   // The RFC3339 string representation of the datetime
};

// A timeseries
struct Timeseries {
    struct Datetime start_datetime;               // The start datetime of the timeseries
    struct Datetime last_datetime;                // The last datetime of the timeseries
    unsigned int    size;                         // The size of the timeseries
    int             offsets[MAX_SIZE];            // The offsets of the observations
    int             values[MAX_SIZE];             // The observed values
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

    if (strlen(datetime_str) != DATETIME_LENGTH) {
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

    if (verify.verify_sec   < 0 || verify.verify_sec   > 59) return -2;
    if (verify.verify_min   < 0 || verify.verify_min   > 59) return -2;
    if (verify.verify_hours < 0 || verify.verify_hours > 23) return -2;
    if (verify.verify_month < 1 || verify.verify_month > 12) return -2;
    if (verify.verify_year  < 1900 || verify.verify_year > 3000) return -2;

    // number of days in a month
    int days_in_month[13] = {0, 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

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
    tm_time.tm_year  = verify.verify_year - 1900;
    tm_time.tm_mon   = verify.verify_month - 1;
    tm_time.tm_mday  = verify.verify_day;
    tm_time.tm_hour  = verify.verify_hours;
    tm_time.tm_min   = verify.verify_min;
    tm_time.tm_sec   = verify.verify_sec;
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
int parse_observation(const char *line, int *offset, int *value) {

    char value_str[64];   // buffer temporaire pour lire la valeur
    char extra;           // pour détecter les caractères en trop

    // Lire offset et la "value" en tant que chaîne
    if (sscanf(line, "%d %63s", offset, value_str) != 2) {
        return -1; // format invalide
    }

    // Vérifier que l'offset est positif
    if (*offset < 0) {
        return -2; // offset invalide
    }

    // Vérifier que la valeur est un entier pur
    if (sscanf(value_str, "%d%c", value, &extra) != 1) {
        return -1; // la valeur contient autre chose qu'un entier
    }

    return 0; // succès
}


/**
 * Reads and validates a timeseries from stdin
 * @param ts Output timeseries structure
 * @return 0 on success, error code otherwise
 */
int read_timeseries(struct Timeseries *ts) {

    char buffer[256];

    // Initialize the entire structure to zero to prevent uninitialized memory issues
    memset(ts, 0, sizeof(*ts));

    // Read the first line (reference datetime)
    if (fgets(buffer, sizeof(buffer), stdin) == NULL) {
        ts->size = 0;
        return 0; // empty timeseries
    }

    // Remove trailing newline or carriage return
    buffer[strcspn(buffer, "\r\n")] = 0;

    // Parse and validate the datetime
    int dt_result = parse_datetime(buffer, &ts->start_datetime);
    if (dt_result == -1) {
        printf("error: invalid datetime format (should be YYYY-mm-DDTHH:MM:SS)\n");
        return 2;
    } else if (dt_result == -2) {
        printf("error: invalid datetime (%s)\n", buffer);
        return 2;
    }

    // Read and parse observations
    unsigned int i = 0;
    while (fgets(buffer, sizeof(buffer), stdin) != NULL) {

        // Remove trailing newline or carriage return
        buffer[strcspn(buffer, "\r\n")] = 0;

        // Check array bounds to avoid overflow
        if (i >= MAX_SIZE) {
            return 2; // too many observations
        }

        int result = parse_observation(buffer, &ts->offsets[i], &ts->values[i]);
        if (result == -1) {
            printf("error: invalid observation format (%s)\n", buffer);
            return 2;
        } else if (result == -2) {
            printf("error: invalid offset (%s)\n", buffer);
            return 2;
        }
        i++;
    }

    // Store the number of valid observations
    ts->size = i;
    return 0;
}


/**
 * Shows the timeseries (show command)
 * @param ts Timeseries to display
 */
void show_timeseries(const struct Timeseries *ts) {

    if (ts->size == 0) {
        return; // rien à afficher
    }

    // Tableau temporaire pour stocker datetimes + valeurs
    struct Datetime datetimes[MAX_SIZE];
    int values[MAX_SIZE];

    memset(datetimes, 0, sizeof(datetimes));
    memset(values, 0, sizeof(values));

    // Convertir offsets -> datetimes
    for (unsigned int i = 0; i < ts->size; i++) {
        offset_to_datetime(ts->start_datetime.t, ts->offsets[i], &datetimes[i]);
        values[i] = ts->values[i];
    }

    // Trier (algorithme à bulles pour débutant)
    for (unsigned int i = 0; i < ts->size; i++) {
        for (unsigned int j = i + 1; j < ts->size; j++) {
            if (datetimes[i].t > datetimes[j].t) {
                // échanger datetimes
                struct Datetime tmp_dt = datetimes[i];
                datetimes[i] = datetimes[j];
                datetimes[j] = tmp_dt;

                // échanger valeurs
                int tmp_val = values[i];
                values[i] = values[j];
                values[j] = tmp_val;
            }
        }
    }

    // Afficher en supprimant les doublons
    for (unsigned int i = 0; i < ts->size; i++) {
        // Si c’est un doublon → garder seulement le dernier
        if (i + 1 < ts->size && datetimes[i].t == datetimes[i + 1].t) {
            continue; // on saute l’ancien doublon
        }
        printf("%s %d\n", datetimes[i].rfc3339_string, values[i]);
    }
}


/**
 * Describes the timeseries (describe command)
 * @param ts Timeseries to describe
 */
void describe_timeseries(const struct Timeseries *ts) {

    if (ts->size == 0) {
        // Cas particulier : série vide
        printf("Domain: [%s, %s]\n", ts->start_datetime.rfc3339_string, ts->start_datetime.rfc3339_string);
        printf("Codomain: none\n");
        printf("Size: 0\n");
        printf("Duration: 0\n");
        printf("Amplitude: 0\n");
        return;
    }

    // Tableaux temporaires pour observations uniques
    struct Datetime datetimes[MAX_SIZE];
    int values[MAX_SIZE];
    int unique_size = 0;

    memset(datetimes, 0, sizeof(datetimes));
    memset(values, 0, sizeof(values));

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
            // Doublon → garder la dernière valeur
            datetimes[found] = dt;
            values[found] = ts->values[i];
        } else {
            // Nouvelle observation
            datetimes[unique_size] = dt;
            values[unique_size] = ts->values[i];
            unique_size++;
        }
    }

    // Trouver min et max datetime
    struct Datetime min_dt = datetimes[0];
    struct Datetime max_dt = datetimes[0];
    for (int i = 1; i < unique_size; i++) {
        if (datetimes[i].t < min_dt.t) min_dt = datetimes[i];
        if (datetimes[i].t > max_dt.t) max_dt = datetimes[i];
    }

    // Trouver min et max valeurs
    int min_val = values[0];
    int max_val = values[0];
    for (int i = 1; i < unique_size; i++) {
        if (values[i] < min_val) min_val = values[i];
        if (values[i] > max_val) max_val = values[i];
    }

    // Afficher résultats
    printf("Domain: [%s, %s]\n", min_dt.rfc3339_string, max_dt.rfc3339_string);
    printf("Codomain: [%d, %d]\n", min_val, max_val);
    printf("Size: %u\n", unique_size);
    printf("Duration: %ld\n", max_dt.t - min_dt.t);
    printf("Amplitude: %d\n", max_val - min_val);
}


/**
 * Converts offset to datetime
 * @param start_time Reference time
 * @param offset Offset in seconds
 * @param result Output datetime
 */
void offset_to_datetime(time_t start_time, int offset, struct Datetime *result) {

    // Calculer le temps
    result->t = start_time + offset;

    // Convertir en struct tm
    struct tm tm = {0};
    gmtime_r(&result->t, &tm);

    // Construire la string RFC3339
    strftime(result->rfc3339_string, sizeof(result->rfc3339_string),
             "%Y-%m-%dT%H:%M:%S", &tm);
}


/**
 * Main
 */
int main(int argc, char *argv[]) {

    int cmd = parse_command(argc, argv);
    if (cmd == -1) {
        return 1; // error usage
    }

    switch (cmd) {
        case 0: // help
            printf("%s", HELP);
            return 0;

        case 1: { // show
            struct Timeseries ts;
            int err = read_timeseries(&ts);
            if (err != 0) return 2;
            show_timeseries(&ts);
            return 0;
        }

        case 2: { // describe
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
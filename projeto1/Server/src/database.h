#include <sqlite3.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define MAX_PROFILE_INFO 50
#define MAX_PROFILES 10
#define MAX_SKILLS 200

typedef struct Profile {
    char email[MAX_PROFILE_INFO];
    char first_name[MAX_PROFILE_INFO];
    char last_name[MAX_PROFILE_INFO];
    char city[MAX_PROFILE_INFO];
    char course[MAX_PROFILE_INFO];
    char year[MAX_PROFILE_INFO];
    char skills[MAX_SKILLS];
}Profile; 

typedef struct ListProfile {
    Profile *list;
    int count;
}ListProfile; 


sqlite3 *init_database(char *database_name);

int add_profile(sqlite3 *database, char *email, char *name, char *city, char *course, char *year, char *skills);
ListProfile *find_by_skill(sqlite3 *database, char *skill);
ListProfile *find_by_course(sqlite3 *database, char *course);
ListProfile *find_by_year(sqlite3 *database, char *year);
ListProfile *find_by_email(sqlite3 *database, char *email);
ListProfile *get_all(sqlite3 *database);
int remove_by_email(sqlite3 *database, char *email);
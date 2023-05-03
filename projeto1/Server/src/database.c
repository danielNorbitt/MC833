#include "database.h"

sqlite3 *init_database(char *database_name){
    sqlite3 *database;
    int result = sqlite3_open(database_name, &database);

    if (result != SQLITE_OK) {
        printf("Could not open database: %s\n", sqlite3_errmsg(database));
        sqlite3_close(database);
        return NULL;
    }

    char *create_table_sql = "CREATE TABLE IF NOT EXISTS Profile ("
            "email TEXT PRIMARY KEY,"
            "first_name TEXT,"
            "last_name TEXT,"
            "city TEXT,"
            "course TEXT,"
            "year TEXT,"
            "skills TEXT);";

    char *err_msg;
    result = sqlite3_exec(database, create_table_sql, 0, 0, &err_msg);

    if (result != SQLITE_OK) {
        printf("Error: %s\n", err_msg);
        sqlite3_free(err_msg);        
        sqlite3_close(database);
        return NULL;
    } 

    printf("Database %s loaded successfully.\n", database_name);
    return database;
}

int add_profile(sqlite3 *database, Profile profile) {
    char *sql = "INSERT INTO Profile (email, first_name,last_name, city, course, year, skills) VALUES (?, ?, ?, ?, ?, ?, ?);";

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);

    rc = sqlite3_bind_text(stmt, 1, profile.email, -1, NULL);
    rc = sqlite3_bind_text(stmt, 2, profile.first_name, -1, NULL);
    rc = sqlite3_bind_text(stmt, 3, profile.last_name, -1, NULL);
    rc = sqlite3_bind_text(stmt, 4, profile.city, -1, NULL);
    rc = sqlite3_bind_text(stmt, 5, profile.course, -1, NULL);
    rc = sqlite3_bind_text(stmt, 6, profile.year, -1, NULL);
    rc = sqlite3_bind_text(stmt, 7, profile.skills, -1, NULL);

    if (rc != SQLITE_OK) {
        printf("Error: %s\n", sqlite3_errmsg(database));
        return 0;
    }

    sqlite3_step(stmt);

    sqlite3_finalize(stmt);
    return 1;
}

ListProfile *get_all(sqlite3 *database){
    ListProfile *profiles = malloc(sizeof(ListProfile));
    profiles->list = malloc(MAX_PROFILES * sizeof(Profile));

    char *sql = "SELECT * FROM Profile;";

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        printf("Error: %s\n", sqlite3_errmsg(database));
        return NULL;
    }

    for (int i = 0; (sqlite3_step(stmt) == SQLITE_ROW && i < MAX_PROFILES); i++) {
        strcpy(profiles->list[i].email,       (char*)sqlite3_column_text(stmt,0));
        strcpy(profiles->list[i].first_name,  (char*)sqlite3_column_text(stmt,1));
        strcpy(profiles->list[i].last_name,   (char*)sqlite3_column_text(stmt,2));
        strcpy(profiles->list[i].city,        (char*)sqlite3_column_text(stmt,3));
        strcpy(profiles->list[i].course,      (char*)sqlite3_column_text(stmt,4));
        strcpy(profiles->list[i].year,        (char*)sqlite3_column_text(stmt,5));
        strcpy(profiles->list[i].skills,      (char*)sqlite3_column_text(stmt,6));
        profiles->count = i + 1;
    }

    sqlite3_finalize(stmt);

    return profiles;
}

ListProfile *find_by_course(sqlite3 *database, char *course) {
    ListProfile *profiles = malloc(sizeof(ListProfile));
    profiles->list = malloc(MAX_PROFILES * sizeof(Profile));

    char *sql = "SELECT email,first_name,last_name FROM Profile WHERE course = ?;";

     sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        printf("Error: %s\n", sqlite3_errmsg(database));
        return NULL;
    }

    rc = sqlite3_bind_text(stmt, 1, course, -1, NULL);

    for (int i = 0; (sqlite3_step(stmt) == SQLITE_ROW && i < MAX_PROFILES); i++) {
        strcpy(profiles->list[i].email,       (char*)sqlite3_column_text(stmt,0));
        strcpy(profiles->list[i].first_name,  (char*)sqlite3_column_text(stmt,1));
        strcpy(profiles->list[i].last_name,  (char*)sqlite3_column_text(stmt,2));
        profiles->list[i].city[0]   = '\0';
        profiles->list[i].course[0] = '\0';
        profiles->list[i].year[0]   = '\0';
        profiles->list[i].skills[0] = '\0';
        profiles->count = i + 1;
    }

    sqlite3_finalize(stmt);

    return profiles;
}

ListProfile *find_by_skill(sqlite3 *database, char *skill) {
    ListProfile *profiles = malloc(sizeof(ListProfile));
    profiles->list = malloc(MAX_PROFILES * sizeof(Profile));

    char *sql = "SELECT email,first_name, last_name FROM Profile WHERE skills LIKE '%'|| ? ||'%';";

     sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        printf("Error: %s\n", sqlite3_errmsg(database));
        return NULL;
    }

    rc = sqlite3_bind_text(stmt, 1, skill, -1, NULL);

    for (int i = 0; (sqlite3_step(stmt) == SQLITE_ROW && i < MAX_PROFILES); i++) {
        strcpy(profiles->list[i].email,       (char*)sqlite3_column_text(stmt,0));
        strcpy(profiles->list[i].first_name,  (char*)sqlite3_column_text(stmt,1));
        strcpy(profiles->list[i].last_name,  (char*)sqlite3_column_text(stmt,1));
        profiles->list[i].city[0]   = '\0';
        profiles->list[i].course[0] = '\0';
        profiles->list[i].year[0]   = '\0';
        profiles->list[i].skills[0] = '\0';
        profiles->count = i + 1;
    }

    sqlite3_finalize(stmt);

    return profiles;
}

ListProfile *find_by_year(sqlite3 *database, char *year) {
    ListProfile *profiles = malloc(sizeof(ListProfile));
    profiles->list = malloc(MAX_PROFILES * sizeof(Profile));

    char *sql = "SELECT email,first_name,last_name,course FROM Profile WHERE year = ?;";

     sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        printf("Error: %s\n", sqlite3_errmsg(database));
        return NULL;
    }

    rc = sqlite3_bind_text(stmt, 1, year, -1, NULL);

    for (int i = 0; (sqlite3_step(stmt) == SQLITE_ROW && i < MAX_PROFILES); i++) {
        strcpy(profiles->list[i].email,       (char*)sqlite3_column_text(stmt,0));
        strcpy(profiles->list[i].first_name,  (char*)sqlite3_column_text(stmt,1));
        strcpy(profiles->list[i].last_name,   (char*)sqlite3_column_text(stmt,2));
        strcpy(profiles->list[i].course,      (char*)sqlite3_column_text(stmt,3));
        profiles->list[i].city[0]   = '\0';
        profiles->list[i].year[0]   = '\0';
        profiles->list[i].skills[0] = '\0';
        profiles->count = i + 1;
    }

    sqlite3_finalize(stmt);

    return profiles;
}

ListProfile *find_by_email(sqlite3 *database, char *email) {
    ListProfile *profiles = malloc(sizeof(ListProfile));
    profiles->list = malloc(MAX_PROFILES * sizeof(Profile));

    char *sql = "SELECT * FROM Profile WHERE email = ?;";

     sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);

    if (rc != SQLITE_OK) {
        printf("Error: %s\n", sqlite3_errmsg(database));
        return NULL;
    }

    rc = sqlite3_bind_text(stmt, 1, email, -1, NULL);

    for (int i = 0; (sqlite3_step(stmt) == SQLITE_ROW && i < MAX_PROFILES); i++) {
        strcpy(profiles->list[i].email,       (char*)sqlite3_column_text(stmt,0));
        strcpy(profiles->list[i].first_name,  (char*)sqlite3_column_text(stmt,1));
        strcpy(profiles->list[i].last_name,   (char*)sqlite3_column_text(stmt,2));
        strcpy(profiles->list[i].city,        (char*)sqlite3_column_text(stmt,3));
        strcpy(profiles->list[i].course,      (char*)sqlite3_column_text(stmt,4));
        strcpy(profiles->list[i].year,        (char*)sqlite3_column_text(stmt,5));
        strcpy(profiles->list[i].skills,      (char*)sqlite3_column_text(stmt,6));
        profiles->count = i + 1;
    }

    sqlite3_finalize(stmt);

    return profiles;
}

int remove_by_email(sqlite3 *database, char *email){
    char *sql = "DELETE FROM Profile WHERE email = ?;";

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(database, sql, -1, &stmt, NULL);

    rc = sqlite3_bind_text(stmt, 1, email, -1, NULL);

    if (rc != SQLITE_OK) {
        printf("Error: %s\n", sqlite3_errmsg(database));
        return 0;
    }

    sqlite3_step(stmt);
    
    return 1;
}
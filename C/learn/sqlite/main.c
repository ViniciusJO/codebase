#include <stdio.h>
#include <./sqlite3.h>

int main(void) {
  sqlite3 *db;
  char *err_msg = 0;
  sqlite3_stmt *res;

  // 1. Open database (creates file if it doesn't exist)
  int rc = sqlite3_open("test.db", &db);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
    sqlite3_close(db);
    return 1;
  }

  // 2. Execute a simple SQL command (e.g., Create Table)
  char *sql = "CREATE TABLE IF NOT EXISTS Friends(Id INTEGER PRIMARY KEY, Name TEXT);";
  rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
  if (rc != SQLITE_OK) {
    fprintf(stderr, "SQL error: %s\n", err_msg);
    sqlite3_free(err_msg);
  }

  // 3. Use Prepared Statements (Safer for inserting data)
  char *insert_sql = "INSERT INTO Friends(Name) VALUES(?);";
  rc = sqlite3_prepare_v2(db, insert_sql, -1, &res, 0);
  if (rc == SQLITE_OK) {
    sqlite3_bind_text(res, 1, "Alice", -1, SQLITE_STATIC);
    sqlite3_step(res);
  }

  sqlite3_finalize(res);

  // 4. Close connection
  sqlite3_close(db);
  return 0;
}

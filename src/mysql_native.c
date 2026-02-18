// mysql_native.c — C glue between Wyn and libmysqlclient
// Compile: cc -c mysql_native.c $(mysql_config --cflags) -o mysql_native.o
// Link:    $(mysql_config --libs)

#include <mysql/mysql.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static MYSQL* connections[64] = {0};
static int conn_count = 0;

long long mysql_native_connect(const char* host, const char* user, const char* pass, const char* db) {
    if (conn_count >= 64) return -1;
    MYSQL* conn = mysql_init(NULL);
    if (!conn) return -1;
    if (!mysql_real_connect(conn, host, user, pass, db, 0, NULL, 0)) {
        fprintf(stderr, "mysql: %s\n", mysql_error(conn));
        mysql_close(conn);
        return -1;
    }
    int id = conn_count++;
    connections[id] = conn;
    return id;
}

const char* mysql_native_query(long long id, const char* sql) {
    if (id < 0 || id >= conn_count || !connections[id]) return "";
    if (mysql_query(connections[id], sql)) {
        static char errbuf[1024];
        snprintf(errbuf, sizeof(errbuf), "ERROR: %s", mysql_error(connections[id]));
        return errbuf;
    }
    MYSQL_RES* res = mysql_store_result(connections[id]);
    if (!res) return "OK";
    
    int nrows = mysql_num_rows(res);
    int ncols = mysql_num_fields(res);
    static char result[65536];
    int pos = 0;
    
    MYSQL_ROW row;
    int r = 0;
    while ((row = mysql_fetch_row(res)) && pos < 65000) {
        if (r > 0) pos += snprintf(result + pos, sizeof(result) - pos, "\n");
        for (int c = 0; c < ncols && pos < 65000; c++) {
            if (c > 0) pos += snprintf(result + pos, sizeof(result) - pos, ",");
            pos += snprintf(result + pos, sizeof(result) - pos, "%s", row[c] ? row[c] : "NULL");
        }
        r++;
    }
    
    mysql_free_result(res);
    return result;
}

void mysql_native_close(long long id) {
    if (id >= 0 && id < conn_count && connections[id]) {
        mysql_close(connections[id]);
        connections[id] = NULL;
    }
}

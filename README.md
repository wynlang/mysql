# mysql - Official Wyn Package

MySQL/MariaDB driver for Wyn. Wraps libmysqlclient.

## Install

```bash
# Install libmysqlclient first
brew install mysql              # macOS
apt install libmysqlclient-dev  # Ubuntu/Debian

# Then install the package
wyn pkg install github.com/wynlang/mysql
```

## Usage

```wyn
import mysql

var db = mysql.Mysql_connect("localhost", "root", "", "mydb")
mysql.Mysql_exec(db, "INSERT INTO users (name) VALUES (?)", ["Alice"])
var rows = mysql.Mysql_query(db, "SELECT name FROM users")
print(rows)
mysql.Mysql_close(db)
```

## API

| Function | Description |
|----------|-------------|
| `Mysql_connect(host, user, pass, dbname)` | Connect, returns handle |
| `Mysql_query(conn, sql)` | Run SQL, returns rows as newline/tab-separated string |
| `Mysql_exec(conn, sql, params)` | Run SQL with `?` placeholders substituted from `params` |
| `Mysql_close(conn)` | Close connection |

## Test

```bash
wyn run tests/test_mysql.wyn
```

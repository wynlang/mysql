# mysql — Official Wyn Package

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
var db = Mysql_connect("localhost", "root", "", "mydb")
var rows = Mysql_query(db, "SELECT name FROM users")
Mysql_exec(db, "INSERT INTO users (name) VALUES (?)", ["Alice"])
Mysql_close(db)
```

## Test

```bash
wyn run tests/test_mysql.wyn
```

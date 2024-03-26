# ANSI/ISO SQL

## Transaction

- Atomicity
- Consistency
- Isolation
- Durability

## Session

```sql
-- Connect database
CONNECT user@database

-- Disconnect database
DISCONNECT

-- Exit database
EXIT
```

## Data type

```sql
-- Data Type                    MYSQL
-- BIT(n)                       BIT(n)
-- BIT VARYING(n)               -
-- DECIMAL(p, s)                DECIMAL(p, s); DEC(p, s)
-- INTEGER                      INTEGER; TINYINT; BOOLEAN; BOOL
-- SMALLINT                     SMALLINT
-- BIGINT                       BIGINT
-- FLOAT(p, s)                  FLOAT(p, s); DOUBLE(p, s)
-- DOUBLE PRECISION(p, s)       DOUBLE PRECISION(p, s)
-- REAL(s)                      REAL(s)

-- CHARACTER(n)                 CHAR(n); BINARY(n)
-- CHARACTER VARYING(n)         VARCHAR(n); VARBINARY(n)
-- BLOB                         TINYBLOB; BLOB; MEDIUMBLOB; LONGBLOB
-- TEXT                         TINYTEXT; TEXT; MEDIUMTEXT; LONGTEXT

-- DATE                         DATE
-- TIME                         TIME
-- DATETIME                     DATETIME
-- TIMESTAMP                    TIMESTAMP
```

## View

```sql
CREATE VIEW myview AS
    SELECT c0, c1 FROM t WHERE c0 = 'v';

SELECT * FROM myview;
```

## Data Definition

```sql
-- Create table
CREATE TABLE t (
  product_no SERIAL,
  name text,
  price numeric DEFAULT 0.0
);

-- equivalent to

CREATE SEQUENCE t_product_no_seq AS integer;
CREATE TABLE t (
  product_no integer NOT NULL DEFAULT nextval('t_product_no_seq'),
  name text,
  price numeric DEFAULT 0.0
);
ALTER SEQUENCE t_product_no_seq OWNED BY t.product_no;

-- Sequences may be "holes" or gaps in the sequence of values which appears in the column,
-- even if no rows are ever deleted. A value allocated from the sequence is still "used up"
-- even if a row containing that value is never successfully inserted into the table column.
-- This may happen, for example, if the inserting transaction rolls back.

-- Drop table
DROP TABLE t;

-- Generated column
CREATE TABLE t (
  hight_cm numeric,
  hight_m  numeric GENERATED ALWAYS AS (hight / 100) STORED
);
```

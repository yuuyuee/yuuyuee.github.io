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
  name TEXT,
  price NUMERIC DEFAULT 0.0
);

-- equivalent to

CREATE SEQUENCE t_product_no_seq AS INTEGER;
CREATE TABLE t (
  product_no INTEGER NOT NULL DEFAULT nextval('t_product_no_seq'),
  name TEXT,
  price NUMERIC DEFAULT 0.0
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
  hight_cm NUMERIC,
  hight_m  NUMERIC GENERATED ALWAYS AS (hight / 100) STORED
);

-- Constraints

-- Check constraints
CREATE TABLE t (
  product_no SERIAL,
  name TEXT,
  price NUMERIC CHECK (price > 0)
);

-- Give the constraint a separate name

CREATE TABLE t (
  product_no SERIAL,
  name TEXT,
  price NUMERIC CONSTRAINT positive_price CHECK (price > 0)
);

CREATE TABLE t (
  product_no SERIAL,
  name TEXT,
  price NUMERIC CHECK (price > 0)
  discounted_price NUMERIC CHECK (discounted_price > 0), CHECK (discounted_price < price)
);

CREATE TABLE t (
  product_no SERIAL,
  name TEXT,
  price NUMERIC,
    CHECK (price > 0)
  discounted_price NUMERIC,
    CHECK (discounted_price > 0),
    CONSTRAINT valid_discount CHECK (discounted_price < price)
);

CREATE TABLE t (
  product_no SERIAL,
  name TEXT,
  price NUMERIC,
    CHECK (price > 0)
  discounted_price NUMERIC,
    CHECK (discounted_price > 0 AND discounted_price < price)
);

-- Not-Null constraints
CREATE TABLE t (
  product_no SERIAL NOT NULL,
  name TEXT,
  price NUMERIC
);

-- Unique constraints
CREATE TABLE t (
  product_no serial UNIQUE,
  name text UNIQUE
);

CREATE TABLE t (
  product_no serial,
  name text,
  UNIQUE (product_no, name)  -- combination unique constraints
);

CREATE TABLE t (
  product_no serial CONSTRAINT must_be_different UNIQUE
);

-- By default, two null values are not considered equal in this comparison.
CREATE TABLE t (
  product_no serial UNIQUE NULLS NOT DISTINCT
);

CREATE TABLE t (
  product_no serial,
  UNIQUE NULLS NOT DISTINCT (product_no)
);

-- Primary key constraints
-- Adding a primary key will automatically create unique B-tree index
-- and will force the column to be marked NOT NULL.
CREATE TABLE t (
  product_no serial PRIMARY KEY, -- product_no serial UNIQUE NOT NULL
);

CREATE TABLE t1 (
  product_no serial,
  order_no integer,
  PRIMARY KEY (product_no, order_no)  -- combination primary key constraints
);

-- Foregin constraints
CREATE TABLE orders (
  order_id integer PRIMARY KEY,
  product_no integer REFERENCES t (product_no),  -- product_no integer REFERENCES products
  quantity integer
);

CREATE TABLE t1 (
  a integer PRIMARY KEY,
  b integer,
  c integer,
  FOREIGN KEY (b, c) REFERENCES other_table (c1, c2)
);

-- Policy on the many-to-many relationship
-- Disallow to remove a product that is still referenced by an order
-- Remove an order, the order items are removed as well
CREATE TABLE products (
    product_no integer PRIMARY KEY,
    name text,
    price numeric
);

CREATE TABLE orders (
    order_id integer PRIMARY KEY,
    shipping_address text
);

-- RESTRICT     - Prevents deletion of a referenced row.
-- SET DEFAULT  - Referencing column in the referencing row to be set default value.
-- SET NULL     - Referencing column in the referencing row to be set null.
-- CASCADE      - When a referenced row is deleted, referencing row should be automatically deleted as well.

CREATE TABLE order_items (
    product_no integer REFERENCES products ON DELETE SET NULL, -- ON DELETE SET DEFAULT | RESTRICT
    order_id integer REFERENCES orders ON DELETE CASCADE,  -- ON UPDATE CASCADE
    quantity integer,
    PRIMARY KEY (product_no, order_id)
);

-- Adding column
ALTER TABLE t ADD COLUMN c text CHECK (c <> '');

-- Removing column
ALTER TABLE t DROP COLUMN c;
ALTER TABLE t DROP COLUMN c CASCADE; -- dropping everything depends

-- Adding constraint
ALTER TABLE t ADD CHECK(c <> '');
ALTER TABLE t ADD CONSTRAINT must_be_different UNIQUE (c);
ALTER TABLE t ADD FOREIGN KEY (c) REFERENCES t2;

ALTER TABLE t ALTER COLUMN c SET NOT NULL;

-- Dropping constraint
ALTER TABLE t DROP CONSTRAINT some_name;
ALTER TABLE t ALTER COLUMN c DROP NOT NULL;

-- Changing column's default value
ALTER TABLE t ALTER COLUMN c SET DEFAULT 0;
ALTER TABLE t ALTER COLUMN c DROP DEFAULT;

-- Changing column's data type
ALTER TABLE t ALTER COLUMN c TYPE numeric(10, 2);

-- Renaming column
ALTER TABLE t RENAME COLUMN c TO c1;

-- Renaming table
ALTER TABLE t RENAME TO t1;

-- Privileges

-- Privilege      | Abbreviation    | Applicable Object Type
-- =============== ================= ========================
-- SELECT         | r ('read')      | LARGE OBJECT, SEQUENCE, TABLE (and table-like objects), table column
-- INSERT         | a ('append')    | TABLE, table column
-- UPDATE         | w ('write')     | LARGE OBJECT, SEQUENCE, TABLE, table column
-- DELETE         | d               | TABLE
-- TRUNCATE       | D               | TABLE
-- REFERENCES     | x               | TABLE, table column
-- TRIGGER        | t               | TABLE
-- CREATE         | C               | DATABASE, SCHEMA, TABLESPACE
-- CONNECT        | c               | DATABASE
-- TEMPORARY      | T               | DATABASE
-- EXECUTE        | X               | FUNCTION, PROCEDURE
-- USAGE          | U               | DOMAIN, FOREIGN DATA WRAPPER, FOREIGN SERVER, LANGUAGE, SCHEMA, SEQUENCE, TYPE
-- SET            | s               | PARAMETER
-- ALTER SYSTEM   | A               | PARAMETER
-- ALL            |                 |


-- Object Type                    | All Privileges  | Default PUBLIC Privileges | psql Command
-- =============================== ================= =========================== ==============
-- DATABASE                       | CTc             | Tc                        | \l
-- DOMAIN                         | U               | U                         | \dD+
-- FUNCTION or PROCEDURE          | X               | X                         | \df+
-- FOREIGN DATA WRAPPER           | U               | none                      | \dew+
-- FOREIGN SERVER                 | U               | none                      | \des+
-- LANGUAGE                       | U               | U                         | \dL+
-- LARGE OBJECT                   | rw              | none                      | \dl+
-- PARAMETER                      | sA              | none                      | \dconfig+
-- SCHEMA                         | UC              | none                      | \dn+
-- SEQUENCE                       | rwU             | none                      | \dp
-- TABLE (and table-like objects) | arwdDxt         | none                      | \dp
-- Table column                   | arwx            | none                      | \dp
-- TABLESPACE                     | C               | none                      | \db+
-- TYPE                           | U               | U                         | \dT+

-- Assigning new owner
ALTER TABLE t OWNER TO new_owner;

-- Granting privileges
GRANT UPDATE ON t TO role_name; -- granting the privilege to UPDATE the table for role_name
GRANT ALL ON t TO PUBLIC; -- granting the privilege to UPDATE the table for all role


```

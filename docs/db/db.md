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

-- Copy table (structure & data, but no indexes, constraints and others).
CREATE TABLE new_table AS TABLE existing_table;
-- Copy table (structure only, but no indexes, constraints and others).
CREATE TABLE new_table AS TABLE existing_table WITH NO DATA;
-- Copy table (data only).
INSERT INTO new_table SELECT * FROM existing_table;
-- Copy table (structure, index, constraints, .e.g)
-- ALL (COMMENTS, COMPRESSION, CONSTRAINTS, DEFAULTS, GENERATED
--      IDENTITY, INDEXES, STATISTICS, STORAGE)
CREATE TABLE new_table (LIKE existing_table INCLUDING ALL);
-- Copy table (structure, index, constraints, .e.g, but no default values)
CREATE TABLE new_table
  (LIKE existing_table INCLUDING ALL EXCLUDING DEFAULTS);

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

-- Inheritance

CREATE TABLE person (
  name text PRIMARY KEY,
  age  int,
  sex  text
);

CREATE TABLE stutent (
  class text
) INHERITS (persion);


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


-- Table partitioning
-- Although all partitions must have the same columns as their partitioned
-- parent, partitions may have their own indexes, constraints and default
-- values, distinct from those of other partitions.

-- Declarative Partitioning

-- 1. Create partition table
CREATE TABLE measurement (
  city_id   int NOT NULL,
  logdate   date NOT NULL,
  peaktemp  int,
  unitsales int
) PARTITION BY RANGE (logdate);

-- 2. Create partition
CREATE TABLE measurement_y2006m02 PARTITION OF measurement
  FOR VALUES FROM ('2006-02-01') TO ('2006-03-01');

CREATE TABLE measurement_y2006m03 PARTITION OF measurement
  FOR VALUES FROM ('2006-03-01') TO ('2006-04-01');

CREATE TABLE measurement_y2006m04 PARTITION OF measurement
  FOR VALUES FROM ('2006-04-01') TO ('2006-05-01')
  TABLESPACE fasttablespace;

CREATE TABLE measurement_y2006m05 PARTITION OF measurement
  FOR VALUES FROM ('2006-05-01') TO ('2006-06-01')
  WITH (parallel_workers = 4)
  TABLESPACE fasttablespace;

-- Sub-partition
CREATE TABLE measurement_y2006m06 PARTITION OF measurement
  FOR VALUES FROM ('2006-06-01') TO ('2006-07-01')
  PARTITION BY RANGE (peaktemp);

-- Inserting data into the parent table that does not map to one of
-- the existing partitions will cause an error; an appropriate partition
-- must be added manually.

-- 3. Create index
-- Not possible to use the CONCURRENTLY qualifier
CREATE INDEX ON measurement (logdate);

-- 4. Ensure that the enable_partition_pruning configuration parameter
-- is not disabled in postgresql.conf. If it is, queries will not be
-- optimized as desired.

-- Partition Maintenance

-- Remove partition
-- require ACCESS EXCLUSIVE lock
DROP TABLE measurement_y2006m02;
-- or remove partition but retain access it as a table
-- require ACCESS EXCLUSIVE lock on the partition table
ALTER TABLE measurement DETACH PARTITION measurement_y2006m02;
-- require SHARE UPDATE EXCLUSIVE lock on the partition table
ALTER TABLE measurement DETACH PARTITION measurement_y2006m02 CONCURRENTLY;

-- Create partition
CREATE TABLE measurement_y2006m06 PARTITION OF measurement
  FOR VALUES FROM ('2006-06-01') TO ('2006-07-01');
-- As an alternative
CREATE TABLE measurement_y2006m06
  (LIKE measurement INCLUDING DEFAULTS INCLUDING CONSTRAINTS);
-- Add CHECK constriant to avoid the scan which is otherwise needed to
-- validate the implicit partition constraint.
ALTER TABLE measurement_y2006m06 ADD CONSTRAINT y2006m06
  CHECK (logdate >= '2006-06-01' AND logdate < '2006-07-01');
-- Some data preparation work
-- require ACCESS EXCLUSIVE lock on the partition
ALTER TABLE measurement ATTACH PARTITION measurement_y2006m06
  FOR VALUES FROM ('2006-06-01') TO ('2006-07-01');
-- Remove redundant CHECK constraint
ALTER TABLE measurement_y2006m06 DROP CONSTRAINT y2006m06;

-- To avoid long lock time
CREATE INDEX measurement_unitsales_idx ON ONLY measurement (unitsales);
CREATE INDEX measurement_y2006m06_unitsales_idx
  ON measurement_y2006m06 (unitsales) CONCURRENTLY;
ALTER INDEX measurement_unitsales_idx
  ATTACH PARTITION measurement_y2006m06_unitsales_idx;
-- This technique can be used with UNIQUE and PRIMARY KEY constraint too
ALTER TABLE ONLY measurement ADD UNIQUE (logdate);
ALTER TABLE measurement_y2006m06 ADD UNIQUE (logdate);
ALTER INDEX measurement_logdate_idx
  ATTACH PARTITION measurement_y2006m06_logdate_idx;
```

## Data manipulation

```sql
-- Queries
-- [WITH with_queries] SELECT select_list FROM table_expression [sort_specification]

-- Joined table
-- T1 { [INNER] | { LEFT | RIGHT | FULL } [OUTER] } JOIN T2 ON boolean_expression
-- T1 { [INNER] | { LEFT | RIGHT | FULL } [OUTER] } JOIN T2 USING (join column list )
-- T1 NATURAL { [INNER] | { LEFT | RIGHT | FULL } [OUTER] } JOIN T2
-- The words INNER and OUTER are optional in all forms.
-- INNER is the default; LEFT, RIGHT, and FULL imply an outer join.

-- T1 CROSS JOIN T2 = T1 INNER JOIN T2 ON TRUE = FROM T1, T2
-- T1 JOIN T2 USING (a, b) = T1 JOIN T2 ON T1.a = T2.a AND T1.b = T2.b

--  ----- --- -----
-- |     |   |     |
-- |  A  | B |  C  |
-- |     |   |     |
--  ----- --- -----

-- T1 = A + B; T2 = B + C

-- T1 INNER JOIN T2; RESULT = B
-- T1 INNER JOIN T2 ON TRUE;
-- T1 CROSS JOIN T2;
-- T1, T2; RESULT = T1 * T2

-- T1 LEFT JOIN T2; RESULT = A + B
-- T1 RIGHT JOIN T2; RESULT = B + C
-- T1 FULL JOIN T2; RESULT = A + B + C

-- Table and column aliase
-- table_reference [AS] alias

-- Sub-queries
-- FROM (SELECT * FROM table1) AS alias_name
-- FROM (VALUES ('anne', 'smith'), ('bob', 'jones')) AS names(first, last)

-- Table function
-- function_call [WITH ORDINALITY] [[AS] table_alias [(column_alias [, ... ])]]
-- ROWS FROM( function_call [, ... ] ) [WITH ORDINALITY] [[AS] table_alias [(column_alias [, ... ])]]
-- UNNEST( array_expression [, ... ] ) [WITH ORDINALITY] [[AS] table_alias [(column_alias [, ... ])]]

CREATE TABLE t (
  id int,
  subid int,
  name text;
);

CREATE FUNCTION gett(int) RETURNS SETOF foo AS $$
  SELECT * FROM t WHERE id = $1;
$$;

SELECT * FROM gett(1) AS t1;

SELECT * FROM t
  WHERE subid IN (
    SELECT subid FROM gett(t.id) AS t1 WHERE t.id = t1.id
  );

CREATE VIEW t_view AS SELECT * FROM gett(1);

SELECT *
  FROM dblink('dbname=mydb', 'SELECT proname, prosrc FROM pg_proc')
    AS t1(proname name, prosrc text) WHERE proname LIKE 'bytea%';

SELECT *
FROM ROWS FROM
    (
        json_to_recordset('[{"a":40,"b":"foo"},{"a":"100","b":"bar"}]')
            AS (a INTEGER, b TEXT),
        generate_series(1, 3)
    ) AS x (p, q, s)
ORDER BY p;

--   p  |  q  | s
-- -----+-----+---
--   40 | foo | 1
--  100 | bar | 2
--      |     | 3

-- GROUP BY & HAVING
-- SELECT select_list
--   FROM ...
--   [WHERE ...]
--   GROUP BY grouping_column_reference [, grouping_column_reference]...
--   HAVING boolean_expression
-- In general, if a table is grouped, columns that are not listed
-- in GROUP BY cannot be referenced except in aggregate expressions.
-- If a query contains aggregate function calls, but no GROUP BY clause,
-- grouping still occurs: the result is a single group row (or perhaps no
-- rows at all, if the single row is then eliminated by HAVING).
-- The same is true if it contains a HAVING clause, even without any
-- aggregate function calls or GROUP BY clause.

-- PARTITION BY
SELECT
  product_id,
  category,
  date,
  sales,
  sum(sales) AS (PARTITION BY category) AS total_sales
FROM products
ORDER BY date;
```

# PostgreSQL

## Transaction

- Atomicity
- Consistency
- Isolation
- Durability

## Data type

```sql
-- Data Type                    MYSQL                                     PostgreSQL
-- BIT(n)                       BIT(n)                                    BIT; BIT(n)
-- BIT VARYING(n)               -                                         BIT VARYING [(n)] | VARBIT(n)
-- DECIMAL(p, s)                DECIMAL(p, s); DEC(p, s)                  NUMERIC(p, s) | DECIMAL(p, s)
-- BOOLEAN                      BOOLEAN; BOOL                             BOOLEAN; BOOL
-- INTEGER                      INTEGER; TINYINT                          INTEGER | INT; INT4
-- SMALLINT                     SMALLINT                                  SAMLLINT | INT2
-- BIGINT                       BIGINT                                    BITINT | INT8
-- FLOAT(p, s)                  FLOAT(p, s); DOUBLE(p, s)                 REAL | FLOAT4
-- DOUBLE PRECISION(p, s)       DOUBLE PRECISION(p, s)                    DOUBLE PERCISION | FLOAT8
-- REAL(s)                      REAL(s)                                   REAL | FLOAT4

-- CHARACTER(n)                 CHAR(n);                                  CHARACTER[(n)] | CHAR(n)
-- CHARACTER VARYING(n)         VARCHAR(n); VARBINARY(n)                  CHARACTER VARYING[(n)] | VCHAR (n)
-- BINARY(n)                    BINARY(n)                                 BYTEA
-- VARBINARY(n)                 VARBINARY(n)                              BYTEA
-- BLOB                         TINYBLOB; BLOB; MEDIUMBLOB; LONGBLOB      BYTEA
-- TEXT                         TINYTEXT; TEXT; MEDIUMTEXT; LONGTEXT      TEXT

-- DATE                         DATE                                      DATE
-- TIME                         TIME                                      TIME [(p)] [WITHOUT TIME ZONE]
-- DATETIME                     DATETIME                                  -
-- TIMESTAMP                    TIMESTAMP                                 TIMESTAMP [(p)] [WITHOUT TIME ZONE]
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

-- Combining queries (UNION & INTERSECT & EXCEPT)
-- query1 UNION [ALL] query2
-- query1 INTERSECT [ALL] query2
-- query1 EXCEPT [ALL] query2
-- In order to calculate the union, intersection, or difference of two
-- queries, the two queries must be “union compatible”, which means that
-- they return the same number of columns and the corresponding columns
-- have compatible data types.
-- If `ALL` is not used, queries has eliminates duplicate rows from its
-- result, in the same way as DISTINCT.

-- Sorting rows
-- SELECT select_list
-- FROM table_expression
-- ORDER BY sort_expression1 [ASC | DESC] [NULLS { FIRST | LAST }]
--   [, sort_expression2 [ASC | DESC] [NULLS { FIRST | LAST }] ...

-- Limit & OFFSET
SELECT select_list
FROM table_expression
  [ ORDER BY ... ]
  [ LIMIT { number | ALL } ] [ OFFSET number ]
-- LIMIT: No more than that many rows will be returned.
-- OFFSET: skip many results before beginning to return rows.

-- Values lists
-- VALUES ( expression [, ...] ) [, ...]
SELECT 1 AS c1, 'one' AS c2
UNION ALL SELECT 2, 'two'
UNION ALL SELECT 3, 'three';
-- or
SELECT * FROM (VALUES (1, 'one'), (2, 'two'), (3, 'three')) AS t (c1, c2);

-- WITH queries (common table expressions)
-- WITH regional_sales AS (
--   SELECT region, SUM(amount) AS total_sales
--   FROM orders
--   GROUP BY region
-- ), top_regions AS (
--   SELECT region
--   FROM regional_sales
--   WHERE total_sales > (SELECT SUM(total_sales)/10 FROM regional_sales)
-- )
-- SELECT
--   region,
--   product,
--   SUM(quantity) AS product_units,
--   SUM(amount) AS product_sales
-- FROM orders
-- WHERE region IN (SELECT region FROM top_regions)
-- GROUP BY region, product;

-- Declaring enumerated types
CREATE TYPE mood AS ENUM ('sad', 'ok', 'happy');
CREATE TABLE person (
  name text,
  current_mood mood
);
INSERT INTO person VALUES ('Moe', 'happy');
SELECT * FROM person WHERE current_mood = 'happy';
--  name | current_mood
-- ------+--------------
--  Moe  | happy

-- JSON type
-- @>: containment
-- ? : existance

-- Simple scalar/primitive value
-- Primitive values can be numbers, quoted strings, true, false, or null
SELECT '5'::json;

-- Array of zero or more elements (elements need not be of same type)
SELECT '[1, 2, "foo", null]'::json;

-- Object containing pairs of keys and values
-- Note that object keys must always be quoted strings
SELECT '{"bar": "baz", "balance": 7.77, "active": false}'::json;

-- Arrays and objects can be nested arbitrarily
SELECT '{"foo": [true, "bar"], "tags": {"a": 1, "b": null}}'::json;

-- This array contains the primitive string value:
SELECT '["foo", "bar"]'::jsonb @> '"bar"'::jsonb;

-- This exception is not reciprocal -- non-containment is reported here:
SELECT '"bar"'::jsonb @> '["bar"]'::jsonb;  -- yields false

-- String exists as array element:
SELECT '["foo", "bar", "baz"]'::jsonb ? 'bar';

-- String exists as object key:
SELECT '{"foo": "bar"}'::jsonb ? 'foo';

-- Object values are not considered:
SELECT '{"foo": "bar"}'::jsonb ? 'bar';  -- yields false

-- As with containment, existence must match at the top level:
SELECT '{"foo": {"bar": "baz"}}'::jsonb ? 'bar'; -- yields false

-- A string is considered to exist if it matches a primitive JSON string:
SELECT '"foo"'::jsonb ? 'foo';

-- jsonb index
CREATE INDEX idxgin ON api USING GIN (jdoc);
CREATE INDEX idxginp ON api USING GIN (jdoc jsonb_path_ops);
-- {
--   "guid": "9c36adc1-7fb5-4d5b-83b4-90356a46061a",
--   "name": "Angela Barton",
--   "is_active": true,
--   "company": "Magnafone",
--   "address": "178 Howard Place, Gulf, Washington, 702",
--   "registered": "2009-11-07T08:53:22 +08:00",
--   "latitude": 19.793713,
--   "longitude": 86.513373,
--   "tags": [
--     "enim",
--     "aliquip",
--     "qui"
--   ]
-- }

-- Find documents in which the key "company" has value "Magnafone"
SELECT jdoc->'guid', jdoc->'name' FROM api WHERE jdoc @> '{"company": "Magnafone"}';
-- Find documents in which the key "tags" contains key or array element "qui"
SELECT jdoc->'guid', jdoc->'name' FROM api WHERE jdoc -> 'tags' ? 'qui';

CREATE INDEX idxgintags ON api USING GIN ((jdoc -> 'tags'));
-- Find documents in which the key "tags" contains array element "qui"
SELECT jdoc->'guid', jdoc->'name' FROM api WHERE jdoc @> '{"tags": ["qui"]}';
SELECT jdoc->'guid', jdoc->'name' FROM api WHERE jdoc @? '$.tags[*] ? (@ == "qui")';
SELECT jdoc->'guid', jdoc->'name' FROM api WHERE jdoc @@ '$.tags[*] == "qui"';

-- jsonb subscripting

-- Extract object value by key
SELECT ('{"a": 1}'::jsonb)['a'];

-- Extract nested object value by key path
SELECT ('{"a": {"b": {"c": 1}}}'::jsonb)['a']['b']['c'];

-- Extract array element by index
SELECT ('[1, "2", null]'::jsonb)[1];

-- Update object value by key. Note the quotes around '1': the assigned
-- value must be of the jsonb type as well
UPDATE table_name SET jsonb_field['key'] = '1';

-- This will raise an error if any record's jsonb_field['a']['b'] is something
-- other than an object. For example, the value {"a": 1} has a numeric value
-- of the key 'a'.
UPDATE table_name SET jsonb_field['a']['b']['c'] = '1';

-- Filter records using a WHERE clause with subscripting. Since the result of
-- subscripting is jsonb, the value we compare it against must also be jsonb.
-- The double quotes make "value" also a valid jsonb string.
SELECT * FROM table_name WHERE jsonb_field['key'] = '"value"';

-- Where jsonb_field was NULL, it is now {"a": 1}
UPDATE table_name SET jsonb_field['a'] = '1';

-- Where jsonb_field was NULL, it is now [1]
UPDATE table_name SET jsonb_field[0] = '1';

-- Where jsonb_field was [], it is now [null, null, 2];
-- where jsonb_field was [0], it is now [0, null, 2]
UPDATE table_name SET jsonb_field[2] = '2';

-- Where jsonb_field was {}, it is now {"a": [{"b": 1}]}
UPDATE table_name SET jsonb_field['a'][0]['b'] = '1';

-- Where jsonb_field was [], it is now [null, {"a": 1}]
UPDATE table_name SET jsonb_field[1]['a'] = '1';

-- Declaring arrary types
CREATE TABLE t (
  c1 integer[],
  c2 text[][],
  c3 integer[3][3]
  -- ignores any supplied array size limits,
  -- the behavior is the same as for arrays of unspecified length.
);
--  SQL standard
CREATE TABLE t (
  c1 integer ARRAY[],
  c2 text ARRAY[][],
  c3 integer ARRAY[3][3]
);

-- '{{1,2,3}, {4,5,6}, {7,8,9}}'
INSERT INTO t
  VALUES ('{1, 2}', '{{1, 2}, {1, 2}}', '{{1, 2}, {1, 2}, {1, 2}}');
SELECT * FROM t WHERE c1[0] == 1;
SELECT c2[0] FROM t;
SELECT c3[1:2][2] FROM t;
SELECT c3[:2][2:] FROM t;
-- array_dims
-- array_upper
-- array_length
-- cardinality
UPDATE t SET c1 = '{1,2,3}';
UPDATE t SET c1 = ARRAY[1,2,3];
UPDATE t SET c1[0] = 100;
```

## Transaction isolation

- dirty read
  A transaction reads data written by a concurrent uncommitted transaction.

- nonrepeatable read
  A transaction re-reads data it has previously read and finds that data has been modified by another transaction (that committed since the initial read).

- phantom read
  A transaction re-executes a query returning a set of rows that satisfy a search condition and finds that the set of rows satisfying the condition has changed due to another recently-committed transaction.

- serialization anomaly
  The result of successfully committing a group of transactions is inconsistent with all possible orderings of running those transactions one at a time.

### Transaction isolation levels

| Isolation Level       | Dirty Read              | Nonrepeatable Read  | Phantom Read            | Serialization Anomaly |
| --------------------- | ------------------------|---------------------|-------------------------|---------------------- |
| **Read uncommitted**  | Allowed, but not in PG  | Possible            | Possible                | Possible              |
| **Read committed**    | Not possible            | Possible            | Possible                | Possible              |
| **Repeatable read**   | Not possible            | Not possible        | Allowed, but not in PG  | Possible              |
| **Serializable**      | Not possible            | Not possible        | Not possible            | Not possible          |

- Read committed
  在PostgreSQL中Read committed是默认隔离级别。
  Read committed事务隔离级别以新的快照启动每个命令，快照包括截止到该命令前的所有事务提交的内容。
  对于SELECT查询，无法看到其他未提交的数据和在**查询执行期间**其他并发事务提交的数据，因此单个事务内连续查询可能看到不同的数据（第一个查询开始后且第二个查询开始前其他事务提交了数据）。
  对于UPDATE, DELETE, SELECT FOR UPDATE, SELECT FOR SHARE在查询方面类似于SELECT，但是当查询行正在被其他并发事务更新（删除或锁住）时，会等待前者提交或回滚，如果数据被更新，后者将应用这些更新内容。

  ```sql
  BEGIN;
  UPDATE t SET c = c + 1;
  -- run from another session:  DELETE FROM t WHERE c = 10;
  -- DELETE will wait for first updating transaction to commit or roll back.
  COMMIT;
  ```

- Repeatable read
  Repeatable read仅能看到事务开始前提交的数据，无法看到其他未提交的数据和在**事务执行期间**其他并发事务提交的数据。
  对于SELECT查询，与Read committed事务隔离级别不同在于快照创建于事务的第一个非事务控制语句（BEGIN, COMMIT）而不是当前语句，因此单个事务内的连续查询看到的是相同的数据，看不到事务开始后其他事务提交的数据。
  对于UPDATE, DELETE, SELECT FOR UPDATE, SELECT FOR SHARE在查询方面类似于SELECT，但是当查询行正在被其他并发事务更新（删除或锁住）时，会等待前者提交或回滚，如果数据被更新，后者将回滚事务并发出错误信息*ERROR:  could not serialize access due to concurrent update*，因为在Repeatable read事务开始后，不能修改或锁住被其他事务修改的行，因此应用程序在使用这个事务隔离级别是必须重试执行事务。

- Serializable
  Serializable对所有提交的事务逐个执行，串行的而非并发的。然而，就像Repeatable read，应用程序需要准备重试执行由于序列化失败的事务。实际上，这个隔离级别与Repeatable read完全相同，不同之处在于它还监视可能使可串行化事务的并发集的执行行为与这些事务的所有可能的串行（一次一个）执行不一致的条件。除了可重复读取之外，这种监视不会引入任何阻塞，但监视会有一些开销，并且检测到可能导致序列化异常的条件会触发序列化失败。

  ```sql
  -- Table t
  -- class | value
  -- -------+-------
  --     1 |    10
  --     1 |    20
  --     2 |   100
  --     2 |   200

  -- Transaction A
  BEGIN;
  SELECT SUM(value) FROM t where class = 1;
  INSERT INTO t VALUE (2, 30);
  COMMIT;

  -- Transaction B Concurrently
  BEGIN;
  SELECT SUM(value) FROM t where class = 2;
  INSERT INTO t VALUE (1, 300);
  COMMIT;
  ```

  上述2个并发事务在Repeatable read事务隔离级别下都可以成功提交事务，但是由于没有结果一致的连续（逐个）执行顺序（即事务执行顺序AB与BA结果不一致），在Serializable事务隔离级别下只有一个可以成功提交事务，另一个则失败并且发出错误信息*ERROR:  could not serialize access due to read/write dependencies among transactions*。

## Setup & Operation

### Environments

- PGDATA - Default data directory location (The use of this environment variable is deprecated)
- PGDATABASE - Default database
- PGUSER - Default user name
- PGPASSWORD - Default user password
- PGHOST - Default host name
- PGHOSTADDR - Same as PGHOST but not DNS lookup overhead
- PGPORT - Default port number (preferably set in the configuration file)

### Create database cluster

```bash
mkdir /usr/local/pgsql
chown postgres /usr/local/pgsql
su postgres

initdb -D /usr/local/pgsql # pg_ctl -D /usr/local/pgsql initdb
                           # or
                           # PGDATA=/usr/local/pgsql initdb
```

### Start the database server

```bash
postgres -D /usr/local/pgsql >logfile 2>&1
# or
pg_ctl -D /usr/local/pgsql -l logfile start
```

## Role

```sql
-- CREATE ROLE <name> [privilege]
-- privilege:
-- LOGIN - login privilege
-- SUPERUSER - superuser status that can passby all permission checks
-- CREATEDB - database creation
-- CREATEROLE - role creation
-- REPLICATION LOGIN - initiating replication
-- PASSWORD 'string' - password
-- WITH INHERIT <TRUE|FALSE> - inheritance of privileges
-- CONNECTION LIMIT 'integer' - connection limit
CREATE ROLE name;
DROP ROLE name;
SELECT rolname FROM pg_roles WHERE rolcanlogin; -- \du for psql program
```

or

```bash
createuser name
dropuser name
```

## Database

```sql
CREATE DATABASE dbname OWNER rolename;
DROP DATABASE dbname;
ALTER DATABASE dbname SET {option} TO {value};
```

or

```bash
createdb -O rolename dbname
```

### Table space

```sql
CREATE TABLESPACE tablespacename LOCATION '/path';
CREATE TABLE foo(i integer) TABLESPACE tablespacename;
```

## Backup and Restore

```bash
# pg_dump dumps only a single database at a time, and it does not dump information
# about roles or tablespaces (because those are cluster-wide rather than per-database)

# backup
pg_dump dbname > dumpfile

# restore
psql dbname < dumpfile

# backup dbname in the host1 to the host2
pg_dump -h host1 dbname | psql -h host2 dbname
```

or

```bash
pg_dumpall > dumpfile
psql -f dumpfile postgres
```

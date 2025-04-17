# MYSQL

## Partition Table

### Creation

```sql
CREATE TABLE `table_name`  (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `device_id` varchar(36),
  `create_time` datetime NOT NULL DEFAULT CURRENT_TIMESTAMP,
  PRIMARY KEY (`id`, `create_time`) USING BTREE
) PARTITION BY RANGE (TO_DAYS(create_time))
PARTITIONS 4
(PARTITION `p20250401` VALUES LESS THAN (739707),
PARTITION `p20250402` VALUES LESS THAN (739708),
PARTITION `p20250403` VALUES LESS THAN (739709),
PARTITION `p_default` VALUES LESS THAN (MAXVALUE));
```

### Insertion/Delete

```sql
SELECT * FROM table_name WHERE create_time = '2025-04-01';
SELECT * FROM table_name PARTITION (p20250401);

```

## Repartition

```sql
-- 重建分区
ALTER TABLE table_name PARTITION BY RANGE(TO_DAYS(create_time)) PARTITIONS 5;
```

## Add/Delete partition

```sql
ALTER TABLE table_name ADD PARTITION (PARTITION P20250404 VALUES LESS THAN (TO_DAYS('2025-04-04')));
ALTER TABLE table_name
    REORGANIZE PARTITION p_default INTO (
        PARTITION p20250404 VALUES LESS THAN (TO_DAYS('2025-04-04')),
        PARTITION p20250405 VALUES LESS THAN (TO_DAYS('2025-04-05')),
        PARTITION p_default VALUES LESS THAN (MAXVALUE));

-- remove partition p20250401
ALTER TABLE table_name DROP PARTITION p20250401;
-- remove all partition
ALTER TABLE table_name DROP PARTITION;

```

```bash
#!/bin/bash

# Add partition automatically
NEXT_DAY=$(date -d "+1 day" +"%Y-%m-%d")
PARTITION_NAME="p$(date -d "+1 day" +"%Y%m%d")"

mysql -uuser -P"password" <<EOF
USE database_name;
ALTER TABLE table_name
ADD PARTITION (
  PARTITION ${PARTITION_NAME} VALUES LESS THAN (TO_DAYS('${NEXT_DAY}'))
);
EOF
```

## Analyze/Repair partition

```sql
ALTER TABLE t1 ANALYZE PARTITION p1;
ALTER TABLE t1 REPAIRE PARTITION p1, p2;
```

## QPS/TPS

```bash
# QPS based questions
mysqladmin -uroot -p extended-status --relative --sleep=5 | grep "Questions"

# TPS based Com_commit and Com_rollback
mysqladmin -uroot -p extended-status --relative --sleep=5 | grep "Com_commit|Com_rollback"
```

```sql
SELECT * FROM performance_schema.events_statements_summary_global_by_event_name;
SELECT * FROM performance_schema.events_transactions_summary_global_by_event_name;
```

## Partition monitor

```sql
-- check partition usage
SELECT partition_name, table_rows, data_length
FROM information_schema.PARTITIONS
WHERE table_name = 'table name';

-- check partition available
EXPLAIN PARTITIONS
SELECT * FROM table_name
WHERE partition_column = 'volumn value';
```

## Periodic perform

```sql
ALTER TABLE table_name OPTIMIZE PARTITION partition_name;
ALTER TABLE table_name ANALYZE PARTITION partition_name;
```

## Performance optimization

```sql
-- The partition key works with the index
ALTER TABLE orders ADD INDEX idx_customer_date (customer_id, order_date);

-- Local partition index (automatically created)
ALTER TABLE orders PARTITION BY RANGE (YEAR(order_date)) (
    PARTITION p_2022 VALUES LESS THAN (2023),
    PARTITION p_2023 VALUES LESS THAN (2024)
);

--- Query optimization
-- Specifying partition
SELECT * FROM orders PARTITION (p_2023) WHERE customer_id = 100;

-- Aggregate optimization across partitions
SELECT customer_id, SUM(amount)
FROM orders
WHERE order_date BETWEEN '2023-01-01' AND '2023-12-31'
GROUP BY customer_id;

-- Rebuild partition
ALTER TABLE table_name REBUILD PARTITION partition_name;

-- Defragment
ALTER TABLE table_name DEFRAGMENT PARTITION partition_name;
```


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
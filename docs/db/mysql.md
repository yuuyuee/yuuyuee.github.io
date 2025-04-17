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

## Daily partition maintenance

```bash
#!/bin/bash
# 文件名: advanced_partition_maintenance.sh
# 功能: 自动添加新分区 + 自动清理旧分区

# 配置区
MYSQL_CONN="-h127.0.0.1 -uroot -p'your_password'"
DB_NAME="your_database"
TABLE_NAME="logs"
KEEP_DAYS=30  # 保留最近30天的分区

# 日期计算
CURRENT_TS=$(date +%s)
TOMORROW=$(date -d "+1 day" +"%Y-%m-%d")
NEW_PARTITION_NAME="p$(date -d "+1 day" +"%Y%m%d")"

# 1. 添加新分区
ADD_PARTITION_SQL="ALTER TABLE \`${DB_NAME}\`.\`${TABLE_NAME}\`
ADD PARTITION (PARTITION ${NEW_PARTITION_NAME} VALUES LESS THAN (TO_DAYS('${TOMORROW}'))"

mysql ${MYSQL_CONN} -e "${ADD_PARTITION_SQL}"

# 2. 清理旧分区
# 获取所有分区列表
PARTITION_LIST=$(mysql ${MYSQL_CONN} -N -e "
SELECT partition_name
FROM information_schema.partitions
WHERE table_schema='${DB_NAME}'
AND table_name='${TABLE_NAME}'
AND partition_name LIKE 'p%'
AND partition_name != 'pmax'")

for PARTITION in $PARTITION_LIST; do
    # 从分区名提取日期 (p20230101 -> 20230101)
    PARTITION_DATE=${PARTITION:1}

    # 转换为时间戳
    PARTITION_TS=$(date -d "${PARTITION_DATE:0:4}-${PARTITION_DATE:4:2}-${PARTITION_DATE:6:2}" +%s 2>/dev/null)

    # 计算分区天数差
    if [[ -n "$PARTITION_TS" ]]; then
        DAYS_DIFF=$(( (CURRENT_TS - PARTITION_TS) / 86400 ))

        if [ $DAYS_DIFF -gt $KEEP_DAYS ]; then
            # 执行分区删除
            mysql ${MYSQL_CONN} -e "
            ALTER TABLE \`${DB_NAME}\`.\`${TABLE_NAME}\`
            DROP PARTITION ${PARTITION}"

            echo "已删除旧分区: ${PARTITION}"
        fi
    fi
done
```

```bash
# 每天凌晨1点执行
0 1 * * * /path/to/daily_partition_maintenance.sh >> /var/log/partition_maintenance.log 2>&1
```

```sql
-- 检查未来分区是否已创建
SELECT partition_name, partition_description
FROM information_schema.partitions
WHERE table_schema = 'your_database'
AND table_name = 'your_table'
ORDER BY partition_description DESC LIMIT 5;
```


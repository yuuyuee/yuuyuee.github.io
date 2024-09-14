# Redis

## Introduction

### Cache

### Vector

### Database

## Data type

### Strings

### JSON

### Lists

### Sets

### Hashes

### Sorted sets

### Streams

### Geospatial

### Bitfields

### Probabilistic

### Time seriesf

## Others

### Connect

#### redis-cli

```bash
redis-cli -h localhost -p 6379
```

#### Python

To install redis-py:

```bash
pip install redis
```

Connect to Redis:

```python
import reids

client = redis.Redis(host="localhost", port=6379, decode_responses=True)
client.set('key', 'value")
client.get('key')
```

Connect to Redis cluster:

```python
import redis

cluster = redis.cluster.RedisCluster(host="localhost", port=6379)
print(cluster.get_nodes())
cluster.set('key', 'value")
cluster.get('key')
```

Connect to Redis with TLS:

```python
import reids

client = redis.Redis(
    host="localhost",
    port=6379,
    username="default",
    password="secret",
    ssl=True,
    ssl_certfile="./redis_user.crt",
    ssl_keyfile="./redis_user_private.key",
    ssl_ca_certs="./redis_ca.pem",
    decode_responses=True)

client.set('key', 'value")
client.get('key')
```

#### C/C++

```cpp
// TODO
```

#### Redis Insight

```bash
# TODO
```

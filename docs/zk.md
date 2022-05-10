# ZK

zk节点被称为znode；znode的数据上的读写时原子的；

空字符不能作为路径名的一部分。

不可见字符不能作为路径名的一部分。

点可以是节点一部分，但不能用于指示路劲，zk不使用相对路径。

zookeeper是保留节点。

ACL不是递归的。

事务日志目录放在独立设备上而不是IO繁忙的设备上是最佳事件，zk对性能最关键的部分就是事务日志。

zk并非设计为通用数据库或大型对象存储，它管理协调数据，数据可以是配置，状态信息，集合点等，它们有个共同属性就是相对较小，以k为单位。

临时znode创建于会话活动状态，在会话结束后被删除；临时节点不允许有子节点。

队列节点

容器节点（3.6.0新增）

TTL节点（3.6.0新增）

本地会话（3.5.0新增）

zkid：zk事务ID，状态的每次变更都会收到一个zxid表示变更总顺序，变更越早zxid值越小

版本号

1. version znode数据变更次数
2. cversion znode子节点变更次数
3. aversion znode ACL变更次数

## zk znode Stat

czxid：创建znode的更改zxid

mzxid：最后修改znode的更改zxid

pzxid：最后修改znode子节点的更改zxid

ctime：创建znode的EPOCH时间，单位毫秒

mtime：上次修改znode的EPOCH时间，单位毫秒

version：znode数据更改次数

cversion：znode的子节点更改次数

aversion：znode的ACL更改次数

ephemeralOwner：如果是临时节点则是所有者的会话ID否则为零

dataLength：znode数据长度

numChildren：znode子节点数量

## zk会话

![](zk\state_dia.jpg)

## 如何处理CONNECTION_LOSS错误

CONNECTION_LOSS表示客户端与服务端之间的连接已经断开。这并不一定意味着请求失败。例如当创建请求发送到服务器且在返回响应之前连接断开，则创建请求成功；如果创建请求发送到服务器之前连接断开，则创建请求失败。客户端无法感知哪种情况发生，所以返回CONNECTION_LOSS，使用者必须弄清楚请求时成功还是失败，通常可以通过特定检测方法完成，比如检查某个节点是否创建或某个节点值是否修改。

## 如何处理SESSION_EXPIRED

SESSION_EXPIRED自动关闭zk句柄。在正常运行的集群中永远不会看到SESSION_EXPIRED。

SESSION_EXPIRED意味着客户端与zk服务断开时间超过会话超时时间，客户端死亡并进入恢复状态，这里恢复状态可能仅仅时重连也可能需要创建临时节点，重建已发布状态等等，具体以实际需求为准。

使用者应该返回一个致命错误而不是恢复。

会话超时后，zk集群将删除此会话拥有的所有临时节点，并立即将更改通知所有监视这些节点的客户端。会话只有在重新连接到zk集群才会获得会话过期通知。

## 如何测试SESSION_EXPIRED

zk句柄可以使用会话ID和密码，其用于在应用错误时恢复会话。例如 当连接到zk时保持会话ID和密码到文件中，恢复时重新从文件中读取会话ID和密码，使用会话ID和密码重连到zk不会丢失会话和对应的临时节点。

使用者需要注意不要不会话ID和密码传递给多个实例否则可能出现问题。

## 如何确定ZK集群大小?

在部署ZK集群首要考虑的是可靠性而非性能。

单台本质上没有可靠性；因为zk基于多数投票选举Leader，至少3台才能实现单点故障情况下服务的可用性；

生产环境建议集群中至少5台服务器，可保证单台停机维护情况下的服务可用性。

## 性能

zk在读多写少（协调服务通常如此）的应用程序中具有高性能。

因为写入设计同步所有服务器状态，集群中增加节点时写入性能实际上会降低，而读取性能适度提高。

zk单机单核约15k/qps。

## 配置

tickTime：zk使用的基本时间单位，以毫秒为单位；它用于心跳，会话超时（最小会话超时等于tickTime*2）等。

dataDir：存储内存数据库快照的位置，除非另有说明，否则也存储数据库更新的事务日志。

dataLogDir：存储数据库更新事务日志位置

clientPort：监听客户端连接端口

initLimit：仲裁期间连接到leader的超时时间

syncLimit：节点和leader间过期时间

server.A=B:C:D ：follower单节点定义，A等于节点myid，B等于节点地址，C表示节点与leader数据同步端口，D表示选举leader端口。

admin.enableServer=[true|false]：启用/禁用管理页面

admin.serverPort=8080：管理页面端口

zookeeper-env.sh

​ZOO_LOG_DIR：日志目录

​ZOO_LOG_FILE：日志文件名

java-env.sh

## 启动zk服务

```bash
./zkServer.sh [--config <conf-dir>] {start|stop|version|restart|status|print-cmd}
```

## 启动zk客户端

```bash
./zkCli.sh [-server <host:port>] [-client-configuration <properties-file>] <cmd> <args>
```

## zk集群设置

zk集群主要目的时获得可靠的zk服务，只要多数服务器可用，该服务就可用；集群数量最好使用奇数。通常一个集群最少3台服务器，推荐5台服务器。

1. 安装Java JDK，可以通过http://java.sun.com/javase/downloads/index.jsp处下载，zk在Java 1.8或更高版本运行，不支持JDK8 LTS, JDK11 LTS, JDK 12 - java9和java10。

2. 设置Java堆大小，这对于避免swap非常重要，swap会严重降低zk的性能。推荐使用负载测试并确保远低于导致swap的使用限制。

3. 安装Zookeeper，可以通过http://zookeeper.apache.org/releases.html处下载。

4. 创建配置文件，文件名可以为任意文件名。

   ```conf
   tickTime=2000
   dataDir=/var/lib/zookeeper/
   clientPort=2181
   intLimit=5
   syncLimit=2
   
   # format: server.<myid>=<address>:<quorum-port>:<election-port>
   server.1=zoo1:2888:3888
   server.2=zoo2:2888:3888
   server.3=zoo3:2888:3888
   ```

   在dataDir目录内创建名为myid的文件，其文件内容包含该服务器的唯一ID，ID有效值为1-255，如果开启了扩展功能，由于内部限制，有效值为1-254。

5. 在与myid相同目录下创建初始化文件initialize。该文件表明需要一个空的数据目录。当存在时将创建一个空数据库并删除标记文件。如果不存在一个空的数据目录则意味着该对等方没有投票权，并且与活动leader通讯之前不会填充数据目录。预期用途仅在启动新集群式创建此文件。

6. 启动zookeeper服务

   ```bash
   zkServer.sh --config <config-dir> start
   ```

## 日志清理

1. 自动清理

   zookeeper 3.4.0及更高版本可以通过配置autopurge.snapRetainCount和autopurge.purgeInterval启动自动快照和事务日志清理。

2. 手动清理

```bash
zkCleanup.sh --config <config-dir> -n <count>
```

## 故障恢复

当遇到文件损坏等故障导致服务无法启动时，**在确认整体的所有其他服务器都已启动**后，可以删除datadir/version-2和datalogdir/version-2中所有文件并重启服务。

## 配置参数

### 基本配置

| 配置               | Java系统属性 | 启用版本 | 注释                                                         |      |
| :----------------- | ------------ | -------- | ------------------------------------------------------------ | ---- |
| clientPort         |              | -        | 客户端连接端口                                               |      |
| secureClientPort   |              |          | 安全客户端连接端口（SSL），指定clientPort和secureClientPort将启用混合模式，忽略任意配置将禁用此模式。当插入zookeeper.serverCnxnFactory, zookeeper.clientCnxnSocket作为Netty时将启用SSL特性 |      |
| observerMasterPort |              |          | 观察者连接端口，除了leader模式，在follower模式下托管观察者连接，在观察者模式下连接到任何follower |      |
| dataDir            |              |          | 内存数据库快照存储目录，如果未指定dataLogDir则事务日志也保存在此目录 |      |
| tickTime           |              |          | 单个tick长度，基本时间单位（毫秒）通常用于心跳，超时等       |      |

### 高级配置

| 配置                                                | Java系统属性                                      | 启用版本 | 注释                                                         |
| --------------------------------------------------- | ------------------------------------------------- | -------- | ------------------------------------------------------------ |
| dataLogDir                                          |                                                   |          | 事务日志存储目录，推荐单独设备用于此                         |
| globalOutstandingLimit                              | zookeeper.globalOutstandingLimit                  |          | 排队的请求最大数量，默认值1000                               |
| preAllocSize                                        | zookeeper.preAllocSize                            |          | 事务日志大小，默认值64M                                      |
| snapCount                                           | zookeeper.snapCount                               |          | 默认值100000，当达到[snapCount/2+1, snapCount]范围内的随机值进行事务日志滚动 |
| commitLogCount                                      | zookeeper.commitLogCount                          |          | 内存最后提交请求列表，以便不太落后的follower快速同步，如果快照很大（》100000）可以提高同步性能 |
| snapSizeLimitInKb                                   | zookeeper.snapSizeLimitInKb                       |          | 默认值4G，非正值禁用该功能                                   |
| txnLogSizeLimitInKb                                 | zookeeper.txnLogSizeLimitInKb                     |          | 默认关闭                                                     |
| maxClientCnxns                                      |                                                   |          | 限制与服务端的并发连接总数，默认值0即不限制                  |
| clientPortAddress                                   |                                                   | 3.3.0    | 监听客户端连接地址（IPv4,IPv6,主机名），可选项               |
| minSessionTimeout                                   |                                                   | 3.3.0    | 最小会话超时时间（毫秒）默认值tickTime * 2                   |
| maxSessionTimeout                                   |                                                   | 3.3.0    | 最大会话超时时间（毫秒）默认值tickTime * 20                  |
| fsync.warningthresholdms                            | zookeeper.fsync.warningthresholdms                | 3.3.4    | 事务日志fsync花费时间超过此值输出警告日志，默认值1000        |
| maxResponseCacheSize                                | zookeeper.maxResponseCacheSize                    |          | 保存最近读取的记录的序列化形式的缓存大小默认值400，<=0关闭该特性 |
| maxGetChildrenResponseCaheSize                      |                                                   |          | 同上，仅作用于获取子请求                                     |
| autopurge.snapRetainCount                           |                                                   | 3.4.0    | 自动清除dataDir和dataLogDir中超过数量的日志部分，默认值为3   |
| autopurge.purgeInterval                             |                                                   | 3.4.0    | 清除任务间隔（小时）默认为0                                  |
| syncEnabled                                         | zookeeper.observer.syncEnabled                    | 3.5.0    | 使观察者像follower一样记录事务并写快照以减少观察者重启恢复时间，默认为true |
| fastleader.minNotificationInterval                  | zookeeper.fastleader.minNotificationInterval      |          | leader选举2此连续通知检查之间最小间隔长度，间隔遵循最小值和最大值的退避策略，用于长选举 |
| fastleader.maxNotificationInterval                  | zookeeper.fastleader.maxNotificationInterval      |          |                                                              |
| connectionMaxTokens                                 | zookeeper.connection_throttle_tokens              | 3.6.0    | 调整服务端节流器参数；定义令牌桶最大令牌数，默认值0          |
| connectionTokenFillTime                             | zookeeper.connection_throttle_fill_count          | 3.6.0    | 整服务端节流器参数；重新填充令牌桶时间间隔（毫秒）默认值1    |
| connectionTokenFillCount                            | zookeeper.connection_throttle_fill_count          | 3.6.0    | 每次添加到令牌桶的令牌数，默认值1                            |
| connectionFreezeTime                                | zookeeper.connection_throttle_freeze_time         | 3.6.0    | 增加的令牌丢弃概率默认值0.002                                |
| connectionDecreaseRatio                             |                                                   |          |                                                              |
| zookeeper.connection_throttle_weight_enabled        |                                                   |          |                                                              |
| zookeeper.connection_throttle_global_session_weight |                                                   |          |                                                              |
| zookeeper.connection_throttle_local_session_weight  |                                                   |          |                                                              |
| zookeeper.connection_throttle_renew_session_weight  |                                                   |          |                                                              |
| clientPortListenBacklog                             |                                                   | 3.6.0    | 套接字积压长度，-1未设置Linux监听默认50                      |
| serverCnxnFactory                                   | zookeeper.serverCnxnFactory                       |          | ServerCnxnFactory实现，默认NIOServerCnxnFactory,应该设置为NettyServerCnxnFactory以便基于TLS通讯 |
| flushDelay                                          | zookeeper.flushDelay                              |          | 延迟提交日志刷新时间（毫秒）默认值0禁用，高写入的集群可以提高吞吐量，每个值提高10-20毫秒 |
| maxWriteQueuePollTime                               | zookeeper.maxWriteQueuePollTime                   |          | 如果启用flushDelay默认值flushDelay/3                         |
| maxBatchSize                                        | zookeeper.maxBatchSize                            |          | 出发提交日志刷新前允许的事务数，默认值1000                   |
| requestThrottleLimit                                | zookeeper.request_throttle_max_requests           | 3.6.0    | 默认值0禁用节流器                                            |
| requestThrottleStallTime                            |                                                   |          |                                                              |
| requestThrottleDropStale                            |                                                   |          |                                                              |
| requestStaleLatencyCheck                            |                                                   |          |                                                              |
| requestStaleConnectionCheck                         |                                                   |          |                                                              |
| zookeeper.request_throttler.shutdownTimeout         |                                                   |          |                                                              |
| advancedFlowControlEnabled                          | zookeeper.netty.adcancedFlowControl.enabled       |          |                                                              |
| enableEagerACLCheck                                 | zookeeper.enableEagerACLCheck                     |          | 设置为true将请求发送到法定人数之前对本地服务器上写入请求启用预先ACL检查，默认值false |
| maxConcurrentSnapSyncs                              | zookeeper.leader.maxConcurrentSnapSyncs           |          | leader、follower同时服务的最大快照同步数默认值10             |
| maxConcurrentDiffSyncs                              | zookeeper.leader.maxConcurrentDiffSyncs           |          | leader、follower同时服务的最大差异同步数，默认值100          |
| digest.enabled                                      | zookeeper.digest.enabled                          | 3.6.0    | 默认启用                                                     |
| snapshot.compression.me                             |                                                   | 3.6.0    | ""=无压缩（默认） gz=gzip snappy=snappy                      |
| audit.enable                                        | zookeeper.audit.enable                            |          | 默认值false                                                  |
| audit.impl.class                                    | zookeeper.audit.impl.class                        |          | org.apache.zookeeper.audit .Log4jAuditLogger                 |
| largeRequestMaxBytes                                | zookeeper.largeRequestMaxBytes                    |          | 大请求最大字节数，默认值100M                                 |
| largeRequestThreshold                               | zookeeper.largeRequestThreshold                   |          | 默认值-1，全部都是小请求                                     |
| outstandingHandshake.limit                          | zookeeper.netty.server.outstandingHandshake.limit |          | 最大TLS并发性，250足以避免羊群效应                           |
| learner.asyncSending                                | zookeeper.learner.asyncSending                    | 3.7.0    | 启用独立异步发送线程，默认false                              |
|                                                     |                                                   |          |                                                              |

### 集群配置

| 配置                              | Java系统属性                          | 启用版本 | 注释                                                         |
| --------------------------------- | ------------------------------------- | -------- | ------------------------------------------------------------ |
| electionAlg                       |                                       |          | 3.6.0只有FastLeaderElection可用即electionAlg=3               |
| initLimit                         |                                       |          | 允许追随者连接并同步到领导者的时间量,如果集群管理的数量很大，则需要增加此值 |
| connectToLearnerMasterLimit       | zookeeper.connectToLearnerMasterLimit |          | 默认值initLimit                                              |
| leaderServes                      | zookeeper.leaderServes                |          | leader是否接受客户端连接，默认值yes；当集群超过3台时强烈建议打开 |
| server.x=[hostname]:nnnnn[:nnnnn] |                                       |          | ```server.<myid>=<address>:<quorum-port>:<election-port>```；动态配置 |
| syncLimit                         |                                       |          | follower与leader同步时间量；如果落后太久会被淘汰             |
| group.x=nnnn[:nnnn]               |                                       |          | 分层仲裁构造；x=组标识符 nnnn=服务器标识符列表               |
| weight.x=nnnn                     |                                       |          | 与group一起使用，定义服务器权重                              |
| cnxtimeout                        | zookeeper.cnxTimeout                  |          | leader选举通知连接超时值，用于electionAlg=3，默认值5         |
| quorumCnxnTimeoutMs               | zookeeper.quorumCnxnTimeoutMs         |          | leader选举通知连接读取超时值，用于electionAlg=3默认值-1然后使用syncLimit * tickTime作为超时值 |
| StandaloneEnabled                 |                                       | 3.5.0    | 默认值true                                                   |
| reconfigEnabled                   |                                       | 3.5.3    |                                                              |
| 4lw.commands.whitelist            | zookeeper.4lw.commands.whitelist      |          | 4lw.commands.whitelist=a,b;4lw.commands.whitelist=*          |
| tcpKeepAlive                      | zookeeper.tcpKeepAlive                | 3.5.4    | 是否为用于仲裁的套接字上设置TCP keepAlive，默认false         |
| observer.reconnectDelayMs         | zookeeper.observer.reconnectDelayMs   |          | observer与leader重连间隔，默认0                              |
| observer.election.DelayMs         | zookeeper.election.DelayMs            |          | observer与leader断开连接时会等待时间；默认值200              |
| localSessionsEnabled              |                                       |          | 默认值false；是否启用本地会话                                |
| localSessionsUpgradingEnabled     |                                       |          | 默认值false；是否自动将本地会话升级为全局会话                |

### 加密/认证配置

### 实验性配置

| 配置     | Java系统属性        | 启用版本 | 注释      |
| -------- | ------------------- | -------- | --------- |
| 只读模式 | readonlymode.enable | 3.4.0    | 默认false |

### 危险配置

| 配置                                  | Java系统属性                                                 | 启用版本 | 注释                                                         |
| ------------------------------------- | ------------------------------------------------------------ | -------- | ------------------------------------------------------------ |
| forceSync                             | zookeeper.forceSync                                          |          | 是否要求在完成更新处理之前将更新同步到事务日志的媒体中       |
| jute.maxbuffer                        | jute.maxbuffer(仅java选项)                                   |          | znode数据最大字节数默认值0xfffff(1048575)略低于1M            |
| jute.maxbuffer.extrasize              | zookeeper.jute.maxbuffer.extrasize                           | 3.5.7    | 通常不配置；默认值最佳                                       |
| skipACL                               | zookeeper.skipACL                                            |          | 跳过ACL检查，这意味着对每个人开放数据数的完全访问            |
| quorumListenOnAllIPs                  |                                                              |          | 是否在所有可用IP地址上监听来自对等方的连接而不仅仅是服务器列表中的地址；默认false |
| multiAddress.reachabilityCheckEnabled | zookeeper.multiAddress.reachabilityCheckEnabled              |          | 默认值false                                                  |
|                                       | 环境变量：ZOO_DATADIR_AUTOCREATE_DISABLE=1  zookeeper.datadir.autocreate=false | 3.5.0    | 如果启用则在日志目录为空时拒绝启动服务                       |
|                                       | zookeeper.db.autocreate=false                                | 3.6.0    | 如果启用则在数据树为空时拒绝启动服务                         |
|                                       |                                                              |          |                                                              |
|                                       |                                                              |          |                                                              |

### 调试配置

| 配置                | Java系统属性                        | 启用版本 | 注释            |
| ------------------- | ----------------------------------- | -------- | --------------- |
|                     | zookeeper.messageTracker.BufferSize | 3.6.0    | 默认值10        |
|                     | zookeeper.messageTracker.Enabled    | 3.6.0    | 默认值false     |
| admin.enableServer  | zookeeper.admin.enableServer        | 3.5.0    | 默认值false     |
| admin.serverAddress | zookeeper.admin.serverAddress       |          | 默认值0.0.0.0   |
| admin.serverPort    | zookeeper.admin.serverPort          |          | 默认值8080      |
| admin.idleTimeout   | zookeeper.admin.idleTimeout         |          | 默认值30000毫秒 |
| admin.commandURL    | zookeeper.admin.commandURL          |          | 默认值/commands |

### 指标提供者

| 配置                          | Java系统属性 | 启用版本 | 注释                                                         |
| ----------------------------- | ------------ | -------- | ------------------------------------------------------------ |
| metricsProvider.className     |              |          | org.apache.zookeeper.metrics.prometheus.PrometheusMetricsProvider |
| metricsProvider.httpPort      |              |          | 默认值7000；```http://hostname:httPort/metrics```            |
| metricsProvider.exportJvmInfo |              |          | 默认值true                                                   |

## 四字命令

| 命令 | 注释 |
| ---- | ---- |
| conf |      |
| cons |      |
| crst |      |
| dump |      |
| envi |      |
| ruok |      |
| srst |      |
| srvr |      |
| stat |      |
| wchs |      |
| wchc |      |
| dirs |      |
| wchp |      |
| mntr |      |
| isro |      |
| hash |      |
| gtmk |      |
| stmk |      |

## AdminServer

| 命令                                  | 注释 |
| ------------------------------------- | ---- |
| *connection_stat_reset/crst*          |      |
| *configuration/conf/config*           |      |
| *connection/cons*                     |      |
| *hash*                                |      |
| *dirs*                                |      |
| *dump*                                |      |
| *environment/env/envi*                |      |
| *get_trace_mask/gtmk*                 |      |
| *initial_configuration/icfg*          |      |
| *is_read_only/isro*                   |      |
| *last_snapshot/lsnp*                  |      |
| *leader/lead*                         |      |
| *monitor/mntr*                        |      |
| *observer_connection_stat_reset/orst* |      |
| *ruok*                                |      |
| *set_trace_mask/stmk*                 |      |
| *server_stats/srvr*                   |      |
| *stats/stat*                          |      |
| *stat_reset/srst*                     |      |
| *observers/obsr*                      |      |
| *system_properties/sysp*              |      |
| *voting_view*                         |      |
| *watches/wchc*                        |      |
| *watches_by_path/wchp*                |      |
| *watch_summary/wchs*                  |      |
| *zabstate*                            |      |

## Observer

observer致力于进一步提高zk的可扩展性。它与follower一样接受读写请求并转发给leader，不同的时它无投票权，仅接受投票结果，因而达到不损害集群写入性能的前提下扩展集群数量。

此外，因为它没有投票权，所以它不是zk集群的关键部分，进而失败也不会损害集群可用性。从用户角度来看相较于follower可以使用更不可靠的网络进行连接通讯。事实上它可用于与来自另一个数据中心的zk服务器通讯。因为读取都在本地提供，所以连接到observer的客户端可以快速读取结果，并且使用更小的网络流量。

### 配置observer

```bash
# 指示zk服务器将是一个observer
peerType=observer

# 在集群中所有服务器配置中指示哪些服务器是observer
server.<myid>=<address>:<quorum-port>:<election-port>:observer

# 指示observer连接到对等节点（follower or leader）的端口2191，并指示follower创建一个ObserverMaster线程且在此端口上监听并提供服务
observerMasterPort=2191
```

* 使2个数据中心形成zk集群的方案因延迟差可能导致误报，故障和分区等问题，而如果一个数据中心运行集群另一个运行观察者则不会有分区问题。
* 如果使用zk作为持久可靠的消息总线，observer可以使用插件机制把看到的提案附加到发布订阅系统，同样无需加载核心集成。

## 动态重新配置（3.5.0+）

从3.5.0开始，不应该使用clientPort和clientPortAddress配置参数，此信息现在是服务器关键字规范的一部分：server，group，weight

```bash
# server.<myid>=<address>:<quorum-port>:<election-port>[:role];[<client-port-address>:]<client-port>
server.1=0.0.0.0:1234:1235;1236
server.1=0.0.0.0:1234:1235:participant;1236 # follower(默认值)
server.1=0.0.0.0:1234:1235:observer;1236 # observer
server.5=0.0.0.0:1234:1235;x.x.x.x:1236
server.5=0.0.0.0:1234:1235:participant;x.x.x.x:1236
```

启用集群模式

```bash
standaloneEnabled=false # 静态配置
```

[3.5.0, 3.5.3)无法禁用动态重新配置功能，3.5.3+版本可通过一下配置启用/禁用动态重新配置功能。

```bash
reconfigEnabled=true
```

动态配置文件

```bash
# zoo_replicated1.cfg <静态配置>
tickTime=2000
dataDir=/zookeeper/data/zookeeper1
initLimit=5
syncLimit=2
# 指定动态配置文件路径
dynamicConfigFile=/zookeeper/conf/zoo_replicated1.cfg.dynamic

# zoo_replicated1.cfg.dynamic <动态配置>
server.1=x.x.x.x:1234:1235:participant;1236
server.2=x.x.x.x:1234:1235:participant;1236
```

检索动态配置

```bash
sync
config

zk.sync(zooDefs.CONFIG_NODE, callback, context);
zk.getConfig(watcher, callback, context);
```

修改动态配置

> 增量重新配置模式：指定对当前配置的更改。

```bash
reconfig -remove 3 -add
  server.5=x.x.x.x:2111:2112,6=localhost:2114:2115:observer;2116
```

> 批量重新配置模式：仅指定系统的新动态配置。

```bash
reconfig -file newconfig.cfg // newconfig.cfg 是动态配置文件

reconfig -member server.1=x.x.x.x:1234:1235:participant;1236,server.2=x.x.x.x:1237:1238:observer;1239
```

## 审计日志

```bash
# zoo.cfg
audit.enable=true
```

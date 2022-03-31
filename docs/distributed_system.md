## 分布式系统理论

### 基础课程

1. CMU-15-440 (```http://www.cs.cmu.edu/~dga/15-440/S14/```)

2. CMU-15-712 (```http://www.cs.cmu.edu/afs/cs.cmu.edu/academic/class/15712-s12/www/syllabus.html```)

3. MIT-6.824 (```https://pdos.csail.mit.edu/6.824/schedule.html```)


### 论文

1. Leslie Lamport

   - Time, Clocks and the Ordering of Events in a Distributed System
   - Reaching agreement in the presence of faults
   - Distributed Snapshots: Determining Global States of a Distributed System
   - The Part-Time Parliament

2. Edsger Wybe Dijkstra

   - Solution of a Problem in Concurrent Programming Control
   - Self-stabilizing Systems in Spite of Distributed Control

3. Butler W.Lampson

   - Distributed System - Architecture and Implementation: an Advanced Course, Springer, 1981
   - How to Build a Highly Availability System using Consensus 1996

4. Barbara Liskov

5. Viewstamped Replicatioin: A New Primary Copy Method to Support Highly-Available Distributed Systems

### 阅读列表

```https://dancres.github.io/Pages/```

```https://martinfowler.com/articles/patterns-of-distributed-systems/```

### 分布式理论导读

1. 起源 （互斥执行/面包店问题）

   - Solution of a Problem in Concurrent Programming Control(Dijkstra 1965)

   - A New Solution of Dijkstra’s Concurrent Programming Problem(Lamport 1974)
2. 容错&故障处理
   - Self-stabilizing Systems in Spite of Distributed Control(Dijkstra 1974)
   - Why Do Computers Stop and What Can be Done About It?(Jim Gary 1985)
3. Time Clock&HappenBefore&分布式快照
   - Time Clocks and the Ordering of Events in a Distributed System(Lamport 1978)
   - Distributed Snapshots-Determining Global States of a Distributed System(Chandy & Lamport 1985)
4. 共识
   - A brief history of Consensus, 2PC and Transaction
   - The Byzantine General Problem(Lamport etc 1982) 
   - Impossibility of Distributed Consensus with One Faulty Process(Michael J. Fischer , Nancy A. Lynch , Michael S. Paterson 1983)
   - Paxos
   - How to Build a Highly Availability System using Consensus(Lampson 1996)
   - Paxos Made Simple(Lamport 2001)
   - Paxos Made Live - An Engineering Perspective(Google 2007)
   - transactional
   - Concurrency Control and Recovery in Database Systems
   - Two Phase Commit(Lampson 1976/Jim Gary 1979)
   - NonBlocking Commit Protocols (Dale Skeen 1981)
   - Consensus on Transaction Commit(Lamport & Jim Gray 2004)
5. 一致性
   - 顺序一致性
   - How to Make a Multiprocessor Computer That Correctly Executes Multiprocess Programs(Lamport 1979)
   - 线性一致性
   -  Linearizability: A Correctness Condition for Concurrent Objects(Maurice Herlihy · Jeannette M Wing 1990)
   - 最终一致性
   -  Eventually Consistent(Werner Vogels 2008)
6. LSM-Tree
   - The Log-Structured Merge-Tree(Patrick O’Neil &Edward Cheng etc. 1996)
7. Leases
   - Leases: An efficient fault-tolerant mechanism for distributed file cache consistency(Cary G.Gray and David R. Cheriton 1989)
8. 网络
   - Single-Message Communication(DAG BELSNES 1976)
9. 延迟处理
   - The Tail at Scale(Jeff Dean, Luiz André Barroso 2013)
10. 部署运维
    - On Designing and Deploying Internet-Scale Services(James Hamilton 2007)
11. 系统设计
    - End-To-End Arguments in System Design(J.H. Saltzer, D.P. Reed and D.D. Clark 1981)
    - Hints for Computer System Design(Lampson 1983)
    - The Design Philosophy of the DARPA Internet Protocols(David D. Clark 1988)
    - Rethinking the Design of the Internet: The End-to-End Arguments vs. the Brave New World(David D. Clark 2000)
12. CAP&BASE
    - CAP
    
      C - Consistency
    
      A - Availability
    
      P - Partition tolerance
    
    - BASE
    
      Basically Available - 系统在出现不可预知的故障时，允许损失部分可用性
    
      Soft state - 允许系统中数据存在中间状态，其中间状态不会影响系统整体可用性
    
      Eventually consistent - 不保证数据强一致性
    
    - BASE: An Acid Alternative(Dan Pritchett@Ebay 2008)

## 引用

[^1]:[Harvest, Yield, and Scalable Tolerant Systems](docs/ds/10.1.1.33.411.pdf)
[^2]:[In Search of an Understandable Consensus Algorithm](docs/ds/raft.pdf)
[^3]:[ZooKeeper: Wait-free coordination for Internet-scale systems](docs/ds/zookeeper.pdf)


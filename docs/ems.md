# 场站级EMS

## 实时监控与数据分析

### 全景数据采集

通过边缘计算网关或智能设备，实时采集市电、光伏、储能、充电桩等设备的运行数据，支持7×24小时监控和可视化展示

### 多维度分析

统计能耗与收益数据，生成运行报表，并通过云端实现多电站的远程管理

## 智能调度优化

### 储能调度

根据电网价格波动、负荷需求及光伏发电预测，制定峰谷充放电策略，降低需量电费，提升投资回报率

### 光储协同控制

优化光伏与储能的出力比例，实现一体化运行，提升新能源消纳效率

### 负荷预测与容量优化

基于历史数据预测用电需求，动态调整储能容量配置，避免变压器超容问题

## 安全与预警管理

### 电池健康管理

利用AI技术监测电芯状态，预测热失控、内短路等故障，延长电池寿命

### 故障预警与应急响应

通过传感器实时监测设备异常，触发断电、喷淋等应急措施，并通知运维人员或消防系统

### 电能质量监测

实时分析电压、频率等参数，触发故障解列或紧急控制，保障系统安全运行

## 电网互动与辅助服务

### 快速响应电网指令

支持一次调频（响应时间＜20ms）、动态电压调节、调峰调频等功能，提升电网稳定性

### 黑启动与构网能力

在电网故障时，通过储能系统实现黑启动，恢复局部供电

## 远程运维与高效管理

### 远程控制

通过云端平台下发控制指令，实现场站的并网/离网切换、有功/无功功率调整等操作

### 智能运维

采用远程故障诊断和运维方案设计，减少人工巡检需求，降低运维成本

## 效能优化与经济性提升

### 需量控制

通过实时跟踪市电需量，减少需量电费，优化用能成本

### 峰谷套利与柔性扩容

利用储能参与电力市场需求响应，实现峰谷电价差套利，并在负载突增时提供柔性扩容支持

## 多场景与复杂环境适应

### 高海拔适应性

针对高海拔地区的散热难题，优化系统散热设计，并增强远程运维能力，保障设备稳定运行

### 多场景兼容

覆盖新能源配储、独立储能、火电调频等多种应用场景，支持虚拟电厂和电力交易

## 微电网与虚拟电厂整合

### 微网协调管理

聚合分布式电源、储能、充电桩等资源，通过本地协调控制器实现微电网的自治运行

### 虚拟电厂支持

整合分散的能源资源，参与电力市场交易，提升能源收益

## 友商产品

### 电享科技iEMS

### 阳光电源EMS3000

### 安科瑞AcrelEMS3.0

## RPEMS

### Weather watch

This will enable the cloud to monitor local weather and charge the batteries for backup in case of any extreme future weather events.
This mode is equivalent to the backup only with the largest target SOC.
这将使云端监测当地天气，并为电池充电，以备未来任何极端天气事件。
这种方式相当于只备份最大目标SOC。

### Backup-reserve

- The persentage of battery power is reserved for grid failure event at all times.
- Drag the button to adjust the percentage of priority backup to battery power.
- Click 'Allow Chage From Grid' button, allow charge from battery.
- 始终为电网故障事件保留一定比例的电池电量。
- 拖动该按钮，调整优先备份电池电量的百分比。
- 点击'Allow Chage From Grid'按钮，允许从电池充电。

### Self-power

Ensure that you are using the power you generated even when the utility grid power is available. If the production is higher than the consumption, the surplus will be stored in the battery modules. Excess power will be fed into the grid after the battery is full. When production is lower than consumption, electric power will be released from the battery modules to power the house.
确保即使在公用电网供电的情况下，你也在使用自己产生的电力。如果产量高于消耗量，剩余部分将存储在电池模块中。电池充满后，多余的电力将被送入电网。当产量低于消耗量时，电能将从电池模块中释放出来，为房屋供电。

### TOU

- Can maximize your cost savings by using your stored battery power during the high-cost part of the day and recharging from solar and optionally with electricity purchased from the grid at the lowest offered rates.
- The inverter runs according to the set plan:
  - Select the enable status and enable time;
  - Select work mode;
  - Select work date;
  - Save data.
- Wrok mode
  - Charge
    - You can set the charging power and choose whether to allow charging from the grid.
  - Discharge
    - You can set the discharging power.
  - Backup-only
    - You can set the target SOC and choose whether to allow charging from the grid.
  - Self-power
    - /

## 场站级EMS vs RPEMS

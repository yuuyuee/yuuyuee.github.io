# The syntax of configuration in Extended Backus-Naur form

```conf
### 老版本配置

# 代码开始标签直到文件结尾或下一个开始标签
# 格式: \[[0-1a-z]{32}\]
[00000000000000000000000000000000]

## 界面生成的注释
## areaCode=320100
## packetType=HTTP
## host=qq.com
## applyUserNo=admin
## applyUserName=Manager
## srcIp=0.0.0.0
## srcPort=0
## dstIp=0.0.0.0
## dstPort=0
## label=0

# 运行版本检查, 默认值5.2.0
# 0                           - 停用, 不在任何版本上运行
# 1                           - 启用, 可在任何版本上运行
# 8.3.0                       - 该功能仅支持8.3.0
# 8.3.0-8.3.1                 - 该功能仅8.3.0到8.3.1支持
# 8.3.0-                      - 该功能在8.3.0开始支持
# -8.3.0                      - 该功能仅仅支持到8.3.0
# NULL                        - 等于默认值
# {version}|{upareaid[,...]}  - 开启上传地控制, 仅在指定地市生效
RUN_VERSION=8.0.2-

HOST=qq.com
URL=/topic

# 输出路径控制
# DESTINATION - 输出目的地
# TOPIC       - DESTINATION = prepare, 表名
#             - DESTINATION = kafka, 主题
# DESTINATION=prepare
# TOPIC=NB_XXL
# 扩展为以下
# N_(OEP_ARBI_DESTINATION)=NONE|STRING("prepare")
# N_(OEP_ARBI_TOPIC)=NONE|STRING("NB_XXL")
DESTINATION=prepare
TOPIC=NB_XXL

# IP IP地址
IP=all

# REGISTERDATA 注册规则
# 格式: rule1|rule2|...
REGISTERDATA=xxx

# PROTO 协议
# 不正确不会加载
PROTO=98

# ID 配置ID
ID=123456789

# KEY 关键词
KEY=82.157.13.125

# POSTRESPONSE
# 仅反诈加载
POSTRESPONSE=1

# NEWMASS_ERROR
# 等于2将不会被加载
NEWMASS_ERROR=1

# SPECIAL_FLAG 特殊标签
# 1 - 仅精细化启用
# 2 - 仅多媒体启用
# 3 - 仅PK使用
# 其他 - 禁用
SPECIAL_FLAG=1

# 字段格式
# [NMTF]_(name|number)=expression
# N - 正常输出字段
# M - 正常输出字段 (输出多次)
# T - 临时字段, 供其他字段关联
# F - 标识打包下载

# 非输出字段
# 格式: T_(name|number)=<source>\|<type|function>(<args[,...]>)[\|...]
# 数字定义:
# 998 - 配置ID
T_(5)   = NONE|STRING(USERID)
T_(6)   = NONE|STRING(103)
T_(7)   = NONE|STRING(103)
T_(20)  = NONE|STRING(7)
T_(998) = NONE|STRING(2222222222222222222)
T_(999) = NONE|STRING(1)

# 输出字段
# 格式: N_(name|number)=<source>\|<type|function>(<args[,...]>)[\|...]
N_(RESTORE_TYPE)=NONE|STRING(1021)
N_(ACTION_TYPE)=NONE|STRING(1021)
N_(RESOURCES_TAG)=NONE|STRING(1021)
N_(ENTITY_TYPE)=NONE|STRING(1021)
N_(MAIN_PROTO_TAG)=NONE|STRING(1021)
N_(APP_TYPE)=NONE|STRING(1021)
N_(OLD_APP_TYPE)=NONE|STRING(1021)
N_(OLD_ACTION)=NONE|STRING(1021)
N_(TOOL_TYPE)=NONE|STRING(1021)
N_(TOOL_NAME)=NONE|STRING(1021)
N_(FILE_MD5_S)=NONE|STRING(1021)
N_(USERID)=NONE|STRING(1021)
N_(URL)=NONE|STRING(1021)
N_(DOMAIN)=NONE|STRING(1021)
```

```conf
### 老版本配置 - 转换成配置语言

# 代码开始标签直到文件结尾或下一个开始标签
# 格式: \[[0-1a-z]{32}\]
[00000000000000000000000000000000]
RUN_VERSION = "8.0.2-"
DESTINATION="prepare"
TOPIC="NB_XXL"
IP="all"


```

```ebnf

```

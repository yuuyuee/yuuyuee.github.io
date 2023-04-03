# The syntax of configuration in Extended Backus-Naur form

```
## 仅行注释
\[[a-zA-z0-9]{32}\] # 代码开始标签直到文件结尾或下一个开始标签

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
#             - DESTINATION = prepare, 主题
# IP          - 输出地址
DESTINATION=prepare|kafka
TOPIC=NB_XXL
IP=all

# 非输出字段
# 格式: T_(name|number)=<source>\|<type|function>(<args[,...]>)[\|...]
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

```ebnf

```

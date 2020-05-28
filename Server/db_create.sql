DROP DATABASE IF EXISTS `db_gm`;
CREATE DATABASE IF NOT EXISTS `db_gm`;


DROP DATABASE IF EXISTS `db_log`;
CREATE DATABASE IF NOT EXISTS `db_log` ;


DROP DATABASE IF EXISTS `db_game`;
CREATE DATABASE IF NOT EXISTS `db_game` ;


DROP DATABASE IF EXISTS `db_account`;
CREATE DATABASE IF NOT EXISTS `db_account` ;

use db_account;
-- ----------------------------
-- Table structure for account
-- ----------------------------
DROP TABLE IF EXISTS `account`;
CREATE TABLE `account` (
  `id` bigint(20) NOT NULL,
  `name` char(255) NOT NULL DEFAULT '0' ,
  `password` char(20) NOT NULL DEFAULT '0' ,
  `lastsvrid` int(11) NOT NULL DEFAULT '0' ,
  `channel` int(20) NOT NULL DEFAULT '0',
  `logincount` int(11) unsigned NOT NULL DEFAULT '0' ,
  `create_time` int(11) NOT NULL DEFAULT '0',
  `seal_end_time` int(11) NOT NULL DEFAULT '0',
  `seal_describle` varchar(255) DEFAULT '' ,
  PRIMARY KEY (`id`),
  KEY `id` (`id`) USING BTREE,
  KEY `name` (`name`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ;

use db_game;
-- ----------------------------
-- Table structure for activity
-- ----------------------------
DROP TABLE IF EXISTS `activity`;
CREATE TABLE `activity`  (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `type` int(11) NOT NULL,
  `roleid` bigint(255) NOT NULL,
  `join_time` varchar(1024) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `data_len` int(11) NOT NULL,
  `data` blob NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for bag
-- ----------------------------
DROP TABLE IF EXISTS `bag`;
CREATE TABLE `bag`  (
  `guid` bigint(20) NOT NULL,
  `roleid` bigint(20) NOT NULL,
  `itemguid` bigint(20) NULL DEFAULT NULL,
  `itemid` int(11) NULL DEFAULT NULL,
  `count` bigint(11) NULL DEFAULT NULL,
  `status` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`guid`, `roleid`) USING BTREE,
  UNIQUE INDEX `index_guid`(`guid`) USING BTREE,
  INDEX `index_roleid`(`roleid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for chapter
-- ----------------------------
DROP TABLE IF EXISTS `chapter`;
CREATE TABLE `chapter`  (
  `chapterid` int(20) NOT NULL,
  `roleid` bigint(20) NOT NULL,
  `staraward` int(11) NULL DEFAULT NULL,
  `sceneaward` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`chapterid`, `roleid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for copy
-- ----------------------------
DROP TABLE IF EXISTS `copy`;
CREATE TABLE `copy`  (
  `copyid` int(20) NOT NULL,
  `roleid` bigint(20) NOT NULL,
  `star` int(11) NULL DEFAULT NULL,
  `battlecnt` int(11) NULL DEFAULT NULL,
  `resetcnt` int(11) NULL DEFAULT NULL,
  `battletime` bigint(20) NULL DEFAULT NULL,
  `resettime` bigint(20) NULL DEFAULT NULL,
  PRIMARY KEY (`copyid`, `roleid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for counter
-- ----------------------------
DROP TABLE IF EXISTS `counter`;
CREATE TABLE `counter`  (
  `id` int(11) NOT NULL,
  `roleid` bigint(20) NOT NULL,
  `index` int(11) NOT NULL,
  `time` bigint(20) NULL DEFAULT NULL,
  `value` bigint(20) NULL DEFAULT NULL,
  PRIMARY KEY (`id`, `roleid`, `index`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for equip
-- ----------------------------
DROP TABLE IF EXISTS `equip`;
CREATE TABLE `equip`  (
  `guid` bigint(20) NOT NULL,
  `roleid` bigint(20) NOT NULL,
  `equipid` int(11) NULL DEFAULT NULL,
  `isuse` int(11) NULL DEFAULT NULL,
  `strengthlvl` int(11) NULL DEFAULT NULL,
  `refinelvl` int(11) NULL DEFAULT NULL,
  `starlvl` int(11) NULL DEFAULT NULL,
  `status` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`guid`, `roleid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for gem
-- ----------------------------
DROP TABLE IF EXISTS `gem`;
CREATE TABLE `gem`  (
  `guid` bigint(20) NOT NULL,
  `roleid` bigint(20) NOT NULL,
  `gemid` int(11) NULL DEFAULT NULL,
  `pos` int(11) NULL DEFAULT NULL,
  `strengthlvl` int(11) NULL DEFAULT NULL,
  `refinelvl` int(11) NULL DEFAULT NULL,
  `starlvl` int(11) NULL DEFAULT NULL,
  `status` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`guid`, `roleid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for globaldata
-- ----------------------------
DROP TABLE IF EXISTS `globaldata`;
CREATE TABLE `globaldata`  (
  `serverid` int(11) NOT NULL,
  `maxguid` bigint(20) NULL DEFAULT NULL,
  `maxonline` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`serverid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for guild
-- ----------------------------
DROP TABLE IF EXISTS `guild`;
CREATE TABLE `guild`  (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `name` char(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `level` int(11) NOT NULL,
  `notice` varchar(1024) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `exp` int(11) NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for guild_member
-- ----------------------------
DROP TABLE IF EXISTS `guild_member`;
CREATE TABLE `guild_member`  (
  `guildid` bigint(20) NOT NULL AUTO_INCREMENT COMMENT '自增id',
  `roleid` bigint(20) NOT NULL,
  `join_time` bigint(20) NOT NULL,
  `pose` int(11) NOT NULL,
  PRIMARY KEY (`guildid`) USING BTREE,
  INDEX `role_id_gang_role_idx`(`roleid`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for mail
-- ----------------------------
DROP TABLE IF EXISTS `mail`;
CREATE TABLE `mail`  (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `roleid` bigint(20) NOT NULL,
  `senderid` bigint(20) NOT NULL,
  `sendername` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `mail_time` bigint(20) NOT NULL,
  `title` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `content` varchar(1024) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `itemdata` tinyblob NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for mail_group
-- ----------------------------
DROP TABLE IF EXISTS `mail_group`;
CREATE TABLE `mail_group`  (
  `id` bigint(20) NOT NULL,
  `mail_time` bigint(20) NOT NULL,
  `mail_type` int(11) NULL DEFAULT NULL,
  `channel` int(11) NULL DEFAULT NULL,
  `title` varchar(255) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `content` varchar(1024) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `itemdata` tinyblob NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for mount
-- ----------------------------
DROP TABLE IF EXISTS `mount`;
CREATE TABLE `mount`  (
  `guid` bigint(20) NOT NULL,
  `roleid` bigint(20) NOT NULL,
  `mountid` int(11) NULL DEFAULT NULL,
  `isuse` int(11) NULL DEFAULT NULL,
  `strengthlvl` int(11) NULL DEFAULT NULL,
  `refinelvl` int(11) NULL DEFAULT NULL,
  `starlvl` int(11) NULL DEFAULT NULL,
  `status` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`guid`, `roleid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for partner
-- ----------------------------
DROP TABLE IF EXISTS `partner`;
CREATE TABLE `partner`  (
  `guid` bigint(20) NOT NULL,
  `roleid` bigint(20) NOT NULL,
  `partnerid` int(11) NULL DEFAULT NULL,
  `setpos` int(11) NULL DEFAULT NULL,
  `strengthlvl` int(11) NULL DEFAULT NULL,
  `refinelvl` int(11) NULL DEFAULT NULL,
  `starlvl` int(11) NULL DEFAULT NULL,
  `status` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`guid`, `roleid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for pet
-- ----------------------------
DROP TABLE IF EXISTS `pet`;
CREATE TABLE `pet`  (
  `guid` bigint(20) NOT NULL,
  `roleid` bigint(20) NOT NULL,
  `petid` int(11) NULL DEFAULT NULL,
  `isuse` int(11) NULL DEFAULT NULL,
  `strengthlvl` int(11) NULL DEFAULT NULL,
  `refinelvl` int(11) NULL DEFAULT NULL,
  `starlvl` int(11) NULL DEFAULT NULL,
  `status` int(11) NULL DEFAULT NULL,
  PRIMARY KEY (`guid`, `roleid`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for player
-- ----------------------------
DROP TABLE IF EXISTS `player`;
CREATE TABLE `player`  (
  `id` bigint(20) NOT NULL,
  `account_id` bigint(20) NULL DEFAULT NULL,
  `name` varchar(256) CHARACTER SET utf8 COLLATE utf8_general_ci NULL DEFAULT NULL,
  `carrerid` int(11) NULL DEFAULT NULL,
  `level` int(11) NULL DEFAULT NULL,
  `exp` bigint(20) NULL DEFAULT NULL,
  `langid` int(11) NULL DEFAULT NULL,
  `viplevel` int(11) NULL DEFAULT NULL,
  `vipexp` int(11) NULL DEFAULT NULL,
  `citycopyid` int(11) NULL DEFAULT NULL,
  `action1` bigint(20) NULL DEFAULT NULL,
  `action2` bigint(20) NULL DEFAULT NULL,
  `action3` bigint(20) NULL DEFAULT NULL,
  `action4` bigint(20) NULL DEFAULT NULL,
  `actime1` bigint(20) NULL DEFAULT NULL,
  `actime2` bigint(20) NULL DEFAULT NULL,
  `actime3` bigint(20) NULL DEFAULT NULL,
  `actime4` bigint(20) NULL DEFAULT NULL,
  `fightvalue` bigint(20) NULL DEFAULT NULL,
  `createtime` bigint(20) NULL DEFAULT NULL,
  `logontime` bigint(20) NULL DEFAULT NULL,
  `logofftime` bigint(20) NULL DEFAULT NULL,
  `guildid` bigint(20) NULL DEFAULT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for relationship
-- ----------------------------
DROP TABLE IF EXISTS `relationship`;
CREATE TABLE `relationship`  (
  `id` bigint(20) NOT NULL AUTO_INCREMENT,
  `other_id` char(255) CHARACTER SET utf8 COLLATE utf8_general_ci NOT NULL,
  `ship_type` int(11) NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB AUTO_INCREMENT = 1 CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for skill
-- ----------------------------
DROP TABLE IF EXISTS `skill`;
CREATE TABLE `skill`  (
  `id` bigint(20) NOT NULL,
  `roleid` bigint(20) NOT NULL,
  `level` int(20) NOT NULL,
  `key_pos` int(255) NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

-- ----------------------------
-- Table structure for task
-- ----------------------------
DROP TABLE IF EXISTS `task`;
CREATE TABLE `task`  (
  `id` bigint(20) NOT NULL,
  `roleid` bigint(20) NOT NULL,
  `task_status` int(11) NOT NULL,
  `progress` int(11) NOT NULL,
  PRIMARY KEY (`id`) USING BTREE
) ENGINE = InnoDB CHARACTER SET = utf8 COLLATE = utf8_general_ci ROW_FORMAT = Dynamic;

use db_log;

-- ----------------------------
-- Table structure for accountcreate
-- ----------------------------
DROP TABLE IF EXISTS `accountcreate`;
CREATE TABLE `accountcreate` (
  `accountid` varchar(50) DEFAULT NULL COMMENT '帐号',
  `channel` varchar(50) DEFAULT NULL COMMENT '平台ID',
  `optime` datetime DEFAULT NULL COMMENT '创建帐号时间',
  `model` varchar(50) DEFAULT NULL COMMENT '机型',
  `mei` varchar(50) DEFAULT NULL,
  `mac` varchar(50) DEFAULT NULL,
  `ip` varchar(50) DEFAULT NULL,
  `openid` varchar(50) DEFAULT NULL,
  UNIQUE KEY `account_index` (`accountid`) USING BTREE,
  KEY `operation_time_index` (`optime`) USING BTREE
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

-- ----------------------------
-- Table structure for accountlogin
-- ----------------------------
DROP TABLE IF EXISTS `accountlogin`;
CREATE TABLE `accountlogin` (
  `account_id` bigint(20) NOT NULL COMMENT '帐号ID',
  `channel` int(20) NOT NULL COMMENT '平台ID',
  `version` char(20) NOT NULL COMMENT '版本ID',
  `optime` datetime NOT NULL COMMENT '登出时间',
  `login_ip` char(50) NOT NULL COMMENT '登入IP',
  `mac_address` char(50) NOT NULL COMMENT '登入设备MAC地址',
  `device_uuid` char(50) NOT NULL COMMENT '登入设备UUID',
  `idfa` char(50) DEFAULT NULL,
  `networktype` char(20) DEFAULT NULL,
  `imei` varchar(50) DEFAULT NULL,
  `imodel` varchar(50) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8 ROW_FORMAT=COMPACT;

use db_gm;
-- ----------------------------
-- Table structure for review_client
-- ----------------------------
DROP TABLE IF EXISTS `review_client`;
CREATE TABLE `review_client` (
  `id` int(11) NOT NULL AUTO_INCREMENT,
  `client_version` varchar(255) DEFAULT NULL,
  PRIMARY KEY (`id`)
) ENGINE=InnoDB AUTO_INCREMENT=2 DEFAULT CHARSET=utf8;

-- ----------------------------
-- Table structure for server_list
-- ----------------------------
DROP TABLE IF EXISTS `server_list`;
CREATE TABLE `server_list` (
  `id` int(11) NOT NULL ,
  `name` varchar(255) NOT NULL ,
  `ip` varchar(255) DEFAULT NULL ,
  `port` int(11) DEFAULT '0',
  `http_port` int(11) DEFAULT NULL,
  `watch_port` int(11) DEFAULT NULL,
  `opentime` bigint(20) DEFAULT '0' ,
  `state` int(11) DEFAULT '0' ,
  `flag` int(11) DEFAULT '0' ,
  `min_version` varchar(255) DEFAULT '' ,
  `max_version` varchar(255) DEFAULT '' ,
  `check_chan` varchar(1024) DEFAULT NULL,
  `check_ip` varchar(1024) DEFAULT NULL ,
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;

-- ----------------------------
-- Table structure for server_status
-- ----------------------------
DROP TABLE IF EXISTS `server_status`;
CREATE TABLE `server_status` (
  `id` int(11) NOT NULL,
  `name` varchar(255) DEFAULT NULL,
  `curr_online` int(11) NULL DEFAULT NULL,
  `max_online` int(11) NULL DEFAULT 0,
  `total_cnt` int(11) NULL DEFAULT 0,
  `cache_cnt` int(11) NULL DEFAULT NULL COMMENT '缓存人数',
  `update_time` bigint(11) DEFAULT '0',
  `status` int(11) DEFAULT '0' COMMENT '',
  `file_version` int(11) DEFAULT '0',
  PRIMARY KEY (`id`)
) ENGINE=MyISAM DEFAULT CHARSET=utf8 ROW_FORMAT=DYNAMIC;




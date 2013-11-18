/*
 Navicat Premium Data Transfer

 Source Server         : localhost
 Source Server Type    : MySQL
 Source Server Version : 50613
 Source Host           : localhost
 Source Database       : jjaxc

 Target Server Type    : MySQL
 Target Server Version : 50613
 File Encoding         : utf-8

 Date: 11/18/2013 18:36:38 PM
*/

SET NAMES utf8;
SET FOREIGN_KEY_CHECKS = 0;

-- ----------------------------
--  Table structure for `accept_friend`
-- ----------------------------
DROP TABLE IF EXISTS `accept_friend`;
CREATE TABLE `accept_friend` (
  `userid` int(11) NOT NULL DEFAULT '0',
  `friendid` int(11) DEFAULT NULL,
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `add_friend`
-- ----------------------------
DROP TABLE IF EXISTS `add_friend`;
CREATE TABLE `add_friend` (
  `userid` int(11) NOT NULL DEFAULT '0',
  `friendid` int(11) DEFAULT NULL,
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `buyitem_data`
-- ----------------------------
DROP TABLE IF EXISTS `buyitem_data`;
CREATE TABLE `buyitem_data` (
  `type` int(11) DEFAULT NULL,
  `itemname` varchar(255) DEFAULT NULL,
  `itemdes` varchar(255) DEFAULT NULL,
  `itemid` varchar(255) DEFAULT NULL,
  `itemprice` int(11) DEFAULT NULL,
  `itemnum` int(11) DEFAULT NULL,
  `itemallnum` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `daily_login`
-- ----------------------------
DROP TABLE IF EXISTS `daily_login`;
CREATE TABLE `daily_login` (
  `num` int(11) DEFAULT NULL,
  `silver` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `get_action`
-- ----------------------------
DROP TABLE IF EXISTS `get_action`;
CREATE TABLE `get_action` (
  `userid` int(11) DEFAULT NULL,
  `starttime` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `get_datetime`
-- ----------------------------
DROP TABLE IF EXISTS `get_datetime`;
CREATE TABLE `get_datetime` (
  `userid` int(11) DEFAULT NULL,
  `datetime` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `get_friendlist`
-- ----------------------------
DROP TABLE IF EXISTS `get_friendlist`;
CREATE TABLE `get_friendlist` (
  `friendid` int(11) DEFAULT NULL,
  `otherid` int(11) DEFAULT NULL,
  `friendname` varchar(255) DEFAULT NULL,
  `friendscore` int(11) DEFAULT NULL,
  `maxscore` int(11) DEFAULT NULL,
  `goldnum` int(11) DEFAULT NULL,
  `silvernum` int(11) DEFAULT NULL,
  `bronzenum` int(11) DEFAULT NULL,
  `time` int(11) DEFAULT NULL,
  `time2` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `get_itemlist`
-- ----------------------------
DROP TABLE IF EXISTS `get_itemlist`;
CREATE TABLE `get_itemlist` (
  `itemid` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `des` varchar(255) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `get_userhis`
-- ----------------------------
DROP TABLE IF EXISTS `get_userhis`;
CREATE TABLE `get_userhis` (
  `hisid` int(11) DEFAULT NULL,
  `itemid` int(11) DEFAULT NULL,
  `histime` varchar(255) DEFAULT NULL,
  `award` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `get_userinfo`
-- ----------------------------
DROP TABLE IF EXISTS `get_userinfo`;
CREATE TABLE `get_userinfo` (
  `user` varchar(255) DEFAULT NULL,
  `userid` int(11) DEFAULT NULL,
  `otherid` varchar(255) DEFAULT NULL,
  `othername` varchar(255) DEFAULT NULL,
  `email` varchar(255) DEFAULT NULL,
  `age` int(11) DEFAULT NULL,
  `sex` int(11) DEFAULT NULL,
  `curScore` int(11) DEFAULT NULL,
  `curSliver` int(11) DEFAULT NULL,
  `maxScore` int(11) DEFAULT NULL,
  `lv` int(11) DEFAULT NULL,
  `ani` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `get_useritemlist`
-- ----------------------------
DROP TABLE IF EXISTS `get_useritemlist`;
CREATE TABLE `get_useritemlist` (
  `itemid` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `des` varchar(255) DEFAULT NULL,
  `name` varchar(255) DEFAULT NULL,
  `itemlv` int(11) DEFAULT NULL,
  `price` int(11) DEFAULT NULL,
  `num` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `getmessage_list`
-- ----------------------------
DROP TABLE IF EXISTS `getmessage_list`;
CREATE TABLE `getmessage_list` (
  `notifyid` int(11) DEFAULT NULL,
  `notifytype` int(11) DEFAULT NULL,
  `friendid` int(11) DEFAULT NULL,
  `otherid` varchar(255) DEFAULT NULL,
  `othername` varchar(255) DEFAULT NULL,
  `datetime` int(11) DEFAULT NULL,
  `content` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `map_raw`
-- ----------------------------
DROP TABLE IF EXISTS `map_raw`;
CREATE TABLE `map_raw` (
  `name` varchar(255) DEFAULT NULL,
  `binData` blob,
  `url` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `missionlist`
-- ----------------------------
DROP TABLE IF EXISTS `missionlist`;
CREATE TABLE `missionlist` (
  `week` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `num` int(11) DEFAULT NULL,
  `description` varchar(255) DEFAULT NULL,
  `award_num` int(11) DEFAULT NULL,
  `award_type` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `playlist`
-- ----------------------------
DROP TABLE IF EXISTS `playlist`;
CREATE TABLE `playlist` (
  `playid` int(11) DEFAULT NULL,
  `url` varchar(255) DEFAULT NULL,
  `diamond` int(11) DEFAULT NULL,
  `isjoin` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `request_addfriend`
-- ----------------------------
DROP TABLE IF EXISTS `request_addfriend`;
CREATE TABLE `request_addfriend` (
  `userid` int(11) DEFAULT NULL,
  `friendid` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `set_data`
-- ----------------------------
DROP TABLE IF EXISTS `set_data`;
CREATE TABLE `set_data` (
  `sound` int(11) DEFAULT NULL,
  `music` int(11) DEFAULT NULL,
  `culture` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `share_data`
-- ----------------------------
DROP TABLE IF EXISTS `share_data`;
CREATE TABLE `share_data` (
  `type` int(11) DEFAULT NULL,
  `game` varchar(255) DEFAULT NULL,
  `weibo` varchar(255) DEFAULT NULL,
  `url` varchar(255) DEFAULT NULL,
  `mark` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `ungame_data`
-- ----------------------------
DROP TABLE IF EXISTS `ungame_data`;
CREATE TABLE `ungame_data` (
  `username` varchar(255) DEFAULT NULL,
  `otherid` varchar(255) DEFAULT NULL,
  `othername` varchar(255) DEFAULT NULL,
  `sex` int(11) DEFAULT NULL,
  `pic` varchar(255) DEFAULT NULL,
  `lang` varchar(255) DEFAULT NULL,
  `time` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `update_userhis`
-- ----------------------------
DROP TABLE IF EXISTS `update_userhis`;
CREATE TABLE `update_userhis` (
  `userid` int(11) DEFAULT NULL,
  `hisid` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `update_useritem`
-- ----------------------------
DROP TABLE IF EXISTS `update_useritem`;
CREATE TABLE `update_useritem` (
  `userid` int(11) DEFAULT NULL,
  `itemid` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `uplist`
-- ----------------------------
DROP TABLE IF EXISTS `uplist`;
CREATE TABLE `uplist` (
  `lv` int(11) DEFAULT NULL,
  `levupex` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `percent` int(11) DEFAULT NULL,
  `sliver` int(11) DEFAULT NULL,
  `rule` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Records of `uplist`
-- ----------------------------
BEGIN;
INSERT INTO `uplist` VALUES ('1', '300', '1', '0', '1', '300'), ('2', '600', '2', '1', '1', '300'), ('3', '600', '2', '2', '1', '300'), ('4', '900', '3', '2', '1', '300'), ('5', '900', '3', '3', '1', '300'), ('6', '900', '3', '3', '1', '300'), ('7', '1200', '4', '4', '10', '300'), ('8', '1200', '4', '4', '1', '300'), ('9', '1200', '4', '5', '1', '300'), ('10', '1800', '6', '5', '1', '300'), ('11', '1800', '6', '6', '1', '300'), ('12', '1800', '6', '6', '1', '300'), ('13', '2400', '8', '7', '1', '300'), ('14', '2400', '8', '7', '1', '300'), ('15', '2400', '8', '8', '10', '300'), ('16', '3000', '10', '8', '1', '300'), ('17', '3000', '10', '9', '1', '300'), ('18', '3000', '10', '9', '1', '300'), ('19', '3900', '13', '10', '1', '300'), ('20', '3900', '13', '10', '1', '300'), ('21', '3900', '13', '11', '1', '300');
COMMIT;

-- ----------------------------
--  Table structure for `user`
-- ----------------------------
DROP TABLE IF EXISTS `user`;
CREATE TABLE `user` (
  `userid` int(11) NOT NULL AUTO_INCREMENT,
  `username` varchar(255) DEFAULT NULL,
  `otherid` varchar(255) DEFAULT NULL,
  `othername` varchar(255) DEFAULT NULL,
  `sex` int(11) DEFAULT '1',
  `email` varchar(255) DEFAULT NULL,
  `lang` varchar(255) DEFAULT NULL,
  `datetime` int(11) DEFAULT NULL,
  `starttime` int(11) DEFAULT NULL,
  `age` int(11) DEFAULT NULL,
  `score` int(11) DEFAULT NULL,
  `diamond` int(11) DEFAULT NULL,
  `action` int(11) DEFAULT NULL,
  `highscore` int(11) DEFAULT NULL,
  `lv` int(11) DEFAULT NULL,
  `curex` int(11) DEFAULT NULL,
  `goldnum` int(11) DEFAULT NULL,
  `silvernum` int(11) DEFAULT NULL,
  `bronzenum` int(11) DEFAULT NULL,
  `day` int(11) DEFAULT NULL,
  `week` int(11) DEFAULT NULL,
  `num` int(11) DEFAULT NULL,
  PRIMARY KEY (`userid`)
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `user_exdata`
-- ----------------------------
DROP TABLE IF EXISTS `user_exdata`;
CREATE TABLE `user_exdata` (
  `lv` int(11) DEFAULT NULL,
  `levupex` int(11) DEFAULT NULL,
  `type` int(11) DEFAULT NULL,
  `bonuscore` int(11) DEFAULT NULL,
  `reward` int(11) DEFAULT NULL,
  `getex` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `user_login`
-- ----------------------------
DROP TABLE IF EXISTS `user_login`;
CREATE TABLE `user_login` (
  `username` varchar(255) DEFAULT NULL,
  `otherid` varchar(255) DEFAULT NULL,
  `othername` varchar(255) DEFAULT NULL,
  `sex` int(11) DEFAULT NULL,
  `pic` varchar(255) DEFAULT NULL,
  `lang` varchar(255) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

-- ----------------------------
--  Table structure for `week_rank_data`
-- ----------------------------
DROP TABLE IF EXISTS `week_rank_data`;
CREATE TABLE `week_rank_data` (
  `friendid` int(11) DEFAULT NULL,
  `friendname` varchar(255) DEFAULT NULL,
  `otherid` varchar(255) DEFAULT NULL,
  `friendscore` int(11) DEFAULT NULL
) ENGINE=InnoDB DEFAULT CHARSET=utf8;

SET FOREIGN_KEY_CHECKS = 1;

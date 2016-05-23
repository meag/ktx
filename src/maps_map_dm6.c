// Converted from .qc on 05/02/2016

#include "g_local.h"
#include "fb_globals.h"

static gedict_t* dm6_door = 0;

qbool CheckNewWeapon (int weapon);

void map_dm6() {
	CreateMarker(152, -841, 171);
	CreateMarker(1522, -1090, 40);
	CreateMarker(1145, -1090, -40);
	CreateMarker(1503, -555, 88);
	CreateMarker(1072, -453, 40);
	CreateMarker(710, -1089, 56);
	CreateMarker(299, -1088, 112);
	CreateMarker(1025, -706, 40);
	CreateMarker(1278, -436, 40);
	CreateMarker(1515, -287, 40);
	CreateMarker(1726, -318, 40);
	CreateMarker(1786, -577, 40);
	CreateMarker(1638, -566, 40);
	CreateMarker(1733, -724, 40);
	CreateMarker(1664, -1023, 40);
	CreateMarker(1354, -342, 168);
	CreateMarker(1627, -313, 168);
	CreateMarker(1740, -793, 168);
	CreateMarker(1392, -1089, 247);
	CreateMarker(1229, -951, 256);
	CreateMarker(1220, -1225, 256);
	CreateMarker(694, -1087, 256);
	CreateMarker(809, -1171, 256);
	CreateMarker(943, -1303, 256);
	CreateMarker(1031, -1472, 256);
	CreateMarker(1038, -1305, 256);
	CreateMarker(1716, -1016, 168);
	CreateMarker(809, -983, 256);
	CreateMarker(481, -1119, 256);
	CreateMarker(431, -1362, 256);
	CreateMarker(424, -1508, 256);
	CreateMarker(429, -1786, 184);
	CreateMarker(87, -1516, 40);
	CreateMarker(87, -1621, 40);
	CreateMarker(797, -1711, 40);
	CreateMarker(74, -1780, 40);
	CreateMarker(596, -1670, 40);
	CreateMarker(998, -1643, 40);
	CreateMarker(1027, -1481, 40);
	CreateMarker(202, -736, 136);
	CreateMarker(304, -736, 136);
	CreateMarker(96, -736, 136);
	CreateMarker(304, -894, 136);
	CreateMarker(96, -888, 136);
	CreateMarker(1022, -1295, -24);
	CreateMarker(1025, -885, -24);
	CreateMarker(925, -1088, -40);
	CreateMarker(1894, -419, 168);
	CreateMarker(1630, -149, 168);
	CreateMarker(424, -1653, 232);
	CreateMarker(1358, -1090, -8);
	CreateMarker(1189, -1202, -40);
	CreateMarker(1194, -991, -40);
	CreateMarker(1300, -538, 168);
	CreateMarker(144, -1966, 88);
	CreateMarker(411, -1945, 168);
	CreateMarker(194, -1786, 56);
	CreateMarker(923, -1199, -40);
	CreateMarker(1751, -417, 168);
	CreateMarker(1648, -889, 40);
	CreateMarker(888, -1090, -280);
	CreateMarker(248, -1854, 56);
	CreateMarker(247, -1719, 56);
	CreateMarker(1025, -1015, -32);
	CreateMarker(1024, -1173, -32);
	CreateMarker(512, -1938, 40);
	CreateMarker(314, -2064, 40);
	CreateMarker(1383, -438, 40);
	CreateMarker(395, -1087, 128);
	CreateMarker(776, -1085, -40);
	CreateMarker(554, -1089, 113);
	CreateMarker(410, -1676, 40);
	CreateMarker(248, -843, 168);
	CreateMarker(1528, -1085, 207);
	CreateMarker(1750, -573, 168);
	CreateMarker(1497, -308, 168);
	CreateMarker(1023, -1093, 96);

	SetGoal(1, 51);
	SetGoal(10, 20);
	SetGoal(10, 26);
	SetGoal(11, 23);
	SetGoal(11, 29);
	SetGoal(12, 16);
	SetGoal(12, 17);
	SetGoal(13, 14);
	SetGoal(14, 10);
	SetGoal(14, 11);
	SetGoal(15, 13);
	SetGoal(16, 21);
	SetGoal(2, 39);
	SetGoal(3, 35);
	SetGoal(4, 12);
	SetGoal(5, 44);
	SetGoal(6, 47);
	SetGoal(7, 33);
	SetGoal(8, 38);
	SetGoal(9, 40);

	SetZone(1, 112);
	SetZone(1, 44);
	SetZone(1, 45);
	SetZone(1, 46);
	SetZone(1, 47);
	SetZone(10, 16);
	SetZone(10, 17);
	SetZone(10, 56);
	SetZone(10, 59);
	SetZone(10, 6);
	SetZone(11, 100);
	SetZone(11, 110);
	SetZone(11, 126);
	SetZone(11, 127);
	SetZone(11, 31);
	SetZone(11, 51);
	SetZone(11, 68);
	SetZone(11, 69);
	SetZone(11, 99);
	SetZone(12, 12);
	SetZone(12, 120);
	SetZone(12, 122);
	SetZone(12, 124);
	SetZone(12, 3);
	SetZone(12, 35);
	SetZone(12, 4);
	SetZone(12, 52);
	SetZone(12, 57);
	SetZone(12, 58);
	SetZone(12, 7);
	SetZone(12, 91);
	SetZone(12, 92);
	SetZone(12, 93);
	SetZone(12, 94);
	SetZone(12, 95);
	SetZone(13, 13);
	SetZone(13, 53);
	SetZone(13, 66);
	SetZone(14, 128);
	SetZone(2, 105);
	SetZone(2, 14);
	SetZone(2, 15);
	SetZone(2, 25);
	SetZone(2, 55);
	SetZone(2, 67);
	SetZone(3, 111);
	SetZone(3, 119);
	SetZone(3, 36);
	SetZone(3, 39);
	SetZone(3, 60);
	SetZone(3, 61);
	SetZone(3, 62);
	SetZone(3, 63);
	SetZone(3, 64);
	SetZone(3, 65);
	SetZone(4, 125);
	SetZone(4, 37);
	SetZone(4, 70);
	SetZone(4, 78);
	SetZone(5, 10);
	SetZone(5, 11);
	SetZone(5, 30);
	SetZone(5, 33);
	SetZone(5, 71);
	SetZone(5, 72);
	SetZone(5, 73);
	SetZone(5, 74);
	SetZone(5, 75);
	SetZone(5, 76);
	SetZone(5, 77);
	SetZone(5, 79);
	SetZone(6, 101);
	SetZone(6, 107);
	SetZone(6, 21);
	SetZone(6, 34);
	SetZone(6, 48);
	SetZone(6, 49);
	SetZone(6, 8);
	SetZone(6, 80);
	SetZone(6, 81);
	SetZone(6, 82);
	SetZone(6, 83);
	SetZone(6, 9);
	SetZone(7, 1);
	SetZone(7, 106);
	SetZone(7, 108);
	SetZone(7, 113);
	SetZone(7, 114);
	SetZone(7, 117);
	SetZone(7, 118);
	SetZone(7, 123);
	SetZone(7, 19);
	SetZone(7, 2);
	SetZone(7, 20);
	SetZone(7, 22);
	SetZone(7, 26);
	SetZone(7, 38);
	SetZone(7, 40);
	SetZone(7, 5);
	SetZone(7, 84);
	SetZone(7, 85);
	SetZone(7, 87);
	SetZone(8, 18);
	SetZone(8, 28);
	SetZone(8, 86);
	SetZone(8, 88);
	SetZone(8, 89);
	SetZone(8, 90);
	SetZone(9, 102);
	SetZone(9, 103);
	SetZone(9, 104);
	SetZone(9, 109);
	SetZone(9, 115);
	SetZone(9, 116);
	SetZone(9, 121);
	SetZone(9, 23);
	SetZone(9, 24);
	SetZone(9, 27);
	SetZone(9, 29);
	SetZone(9, 32);
	SetZone(9, 41);
	SetZone(9, 42);
	SetZone(9, 43);
	SetZone(9, 54);
	SetZone(9, 96);
	SetZone(9, 97);
	SetZone(9, 98);

	SetMarkerFlag(124, 1);
	SetMarkerFlag(52, 1);
	SetMarkerFlag(7, 1);

	SetMarkerPath(1, 0, 4);
	SetMarkerPath(1, 1, 38);
	SetMarkerPath(1, 2, 123);
	SetMarkerPath(1, 3, 26);
	SetMarkerPath(10, 0, 79);
	SetMarkerPath(10, 1, 11);
	SetMarkerPath(10, 2, 103);
	SetMarkerPath(100, 0, 68);
	SetMarkerPath(100, 1, 31);
	SetMarkerPath(101, 0, 82);
	SetMarkerPath(101, 1, 83);
	SetMarkerPath(101, 2, 85);
	SetMarkerPath(101, 3, 38);
	SetMarkerPath(101, 4, 1);
	SetMarkerPath(101, 5, 114);
	SetMarkerPath(102, 0, 53);
	SetMarkerPath(102, 1, 103);
	SetMarkerPath(102, 2, 104);
	SetMarkerPath(102, 3, 32);
	SetMarkerPath(102, 4, 27);
	SetMarkerPath(103, 0, 102);
	SetMarkerPath(103, 1, 96);
	SetMarkerPath(103, 2, 54);
	SetMarkerPath(103, 3, 24);
	SetMarkerPath(103, 4, 32);
	SetMarkerPath(103, 5, 116);
	SetMarkerPath(104, 0, 32);
	SetMarkerPath(104, 1, 102);
	SetMarkerPath(104, 2, 97);
	SetMarkerPath(104, 3, 54);
	SetMarkerPath(104, 4, 115);
	SetMarkerPath(105, 0, 67);
	SetMarkerPath(105, 1, 55);
	SetMarkerPath(105, 2, 14);
	SetMarkerPath(105, 3, 39);
	SetMarkerPath(106, 0, 40);
	SetMarkerPath(106, 1, 87);
	SetMarkerPath(106, 2, 107);
	SetMarkerPath(106, 3, 108);
	SetMarkerPath(106, 4, 101);
	SetMarkerPath(107, 0, 106);
	SetMarkerPath(107, 1, 34);
	SetMarkerPath(107, 2, 83);
	SetMarkerPath(107, 3, 20);
	SetMarkerPath(107, 4, 1);
	SetMarkerPath(107, 5, 113);
	SetMarkerPath(108, 0, 38);
	SetMarkerPath(108, 1, 20);
	SetMarkerPath(108, 2, 87);
	SetMarkerPath(108, 3, 4);
	SetMarkerPath(108, 4, 85);
	SetMarkerPath(109, 0, 41);
	SetMarkerPath(109, 1, 98);
	SetMarkerPath(109, 2, 96);
	SetMarkerPath(109, 3, 116);
	SetMarkerPath(109, 4, 112);
	SetMarkerPath(11, 0, 10);
	SetMarkerPath(11, 1, 71);
	SetMarkerPath(11, 2, 103);
	SetMarkerPath(11, 3, 109);
	SetMarkerPath(11, 4, 79);
	SetMarkerPath(11, 5, 115);
	SetMarkerPath(11, 6, 128);
	SetMarkerPath(110, 0, 51);
	SetMarkerPath(110, 1, 126);
	SetMarkerPath(110, 2, 55);
	SetMarkerPath(110, 3, 39);
	SetMarkerPath(110, 4, 99);
	SetMarkerPath(110, 5, 64);
	SetMarkerPath(111, 0, 66);
	SetMarkerPath(111, 1, 65);
	SetMarkerPath(111, 2, 78);
	SetMarkerPath(111, 3, 69);
	SetMarkerPath(112, 0, 44);
	SetMarkerPath(113, 0, 20);
	SetMarkerPath(113, 1, 26);
	SetMarkerPath(113, 2, 1);
	SetMarkerPath(113, 3, 108);
	SetMarkerPath(113, 4, 87);
	SetMarkerPath(114, 0, 38);
	SetMarkerPath(114, 1, 108);
	SetMarkerPath(114, 2, 1);
	SetMarkerPath(114, 3, 87);
	SetMarkerPath(114, 4, 88);
	SetMarkerPath(114, 5, 85);
	SetMarkerPath(115, 0, 98);
	SetMarkerPath(115, 1, 29);
	SetMarkerPath(115, 2, 43);
	SetMarkerPath(115, 3, 104);
	SetMarkerPath(115, 4, 97);
	SetMarkerPath(116, 0, 96);
	SetMarkerPath(116, 1, 109);
	SetMarkerPath(116, 2, 43);
	SetMarkerPath(116, 3, 98);
	SetMarkerPath(116, 4, 103);
	SetMarkerPath(117, 0, 26);
	SetMarkerPath(117, 1, 88);
	SetMarkerPath(118, 0, 19);
	SetMarkerPath(118, 1, 26);
	SetMarkerPath(119, 0, 60);
	SetMarkerPath(119, 1, 61);
	SetMarkerPath(119, 2, 39);
	SetMarkerPath(119, 3, 36);
	SetMarkerPath(12, 0, 35);
	SetMarkerPath(12, 1, 2);
	SetMarkerPath(120, 0, 122);
	SetMarkerPath(120, 1, 58);
	SetMarkerPath(121, 0, 57);
	SetMarkerPath(121, 1, 41);
	SetMarkerPath(121, 2, 23);
	SetMarkerPath(121, 3, 112);
	SetMarkerPath(122, 0, 57);
	SetMarkerPath(122, 1, 120);
	SetMarkerPath(123, 0, 1);
	SetMarkerPath(123, 1, 38);
	SetMarkerPath(123, 2, 114);
	SetMarkerPath(123, 3, 88);
	SetMarkerPath(123, 4, 26);
	SetMarkerPath(124, 0, 94);
	SetMarkerPath(125, 0, 70);
	SetMarkerPath(125, 1, 78);
	SetMarkerPath(126, 0, 69);
	SetMarkerPath(126, 1, 110);
	SetMarkerPath(126, 2, 39);
	SetMarkerPath(126, 3, 55);
	SetMarkerPath(126, 4, 111);
	SetMarkerPath(126, 5, 64);
	SetMarkerPath(127, 0, 64);
	SetMarkerPath(127, 1, 39);
	SetMarkerPath(127, 2, 119);
	SetMarkerPath(127, 3, 55);
	SetMarkerPath(127, 4, 68);
	SetMarkerPath(127, 5, 67);
	SetMarkerPath(128, 0, 43);
	SetMarkerPath(128, 1, 98);
	SetMarkerPath(128, 2, 32);
	SetMarkerPath(128, 3, 96);
	SetMarkerPath(128, 4, 97);
	SetMarkerPath(128, 5, 54);
	SetMarkerPath(128, 6, 29);
	SetMarkerPath(13, 0, 65);
	SetMarkerPath(13, 1, 66);
	SetMarkerPath(14, 0, 105);
	SetMarkerPath(14, 1, 67);
	SetMarkerPath(14, 2, 55);
	SetMarkerPath(15, 0, 25);
	SetMarkerPath(16, 0, 17);
	SetMarkerPath(16, 1, 56);
	SetMarkerPath(16, 2, 59);
	SetMarkerPath(17, 0, 56);
	SetMarkerPath(17, 1, 59);
	SetMarkerPath(17, 2, 16);
	SetMarkerPath(18, 0, 88);
	SetMarkerPath(18, 1, 28);
	SetMarkerPath(18, 2, 86);
	SetMarkerPath(19, 0, 117);
	SetMarkerPath(19, 1, 118);
	SetMarkerPath(2, 0, 1);
	SetMarkerPath(20, 0, 26);
	SetMarkerPath(20, 1, 88);
	SetMarkerPath(20, 2, 108);
	SetMarkerPath(20, 3, 87);
	SetMarkerPath(20, 4, 113);
	SetMarkerPath(21, 0, 73);
	SetMarkerPath(21, 1, 81);
	SetMarkerPath(21, 2, 49);
	SetMarkerPath(22, 0, 1);
	SetMarkerPath(23, 0, 41);
	SetMarkerPath(23, 1, 97);
	SetMarkerPath(23, 2, 104);
	SetMarkerPath(23, 3, 29);
	SetMarkerPath(24, 0, 27);
	SetMarkerPath(24, 1, 96);
	SetMarkerPath(24, 2, 43);
	SetMarkerPath(24, 3, 32);
	SetMarkerPath(25, 0, 15);
	SetMarkerPath(25, 1, 67);
	SetMarkerPath(25, 2, 67);
	SetMarkerPath(25, 3, 39);
	SetMarkerPath(25, 4, 127);
	SetMarkerPath(26, 0, 20);
	SetMarkerPath(26, 1, 1);
	SetMarkerPath(26, 2, 118);
	SetMarkerPath(26, 3, 113);
	SetMarkerPath(26, 4, 117);
	SetMarkerPath(26, 5, 123);
	SetMarkerPath(27, 0, 24);
	SetMarkerPath(27, 1, 32);
	SetMarkerPath(27, 2, 43);
	SetMarkerPath(27, 3, 96);
	SetMarkerPath(28, 0, 18);
	SetMarkerPath(28, 1, 89);
	SetMarkerPath(28, 2, 86);
	SetMarkerPath(29, 0, 23);
	SetMarkerPath(29, 1, 98);
	SetMarkerPath(29, 2, 41);
	SetMarkerPath(29, 3, 115);
	SetMarkerPath(29, 4, 97);
	SetMarkerPath(29, 5, 112);
	SetMarkerPath(3, 0, 2);
	SetMarkerPath(3, 1, 95);
	SetMarkerPath(3, 2, 94);
	SetMarkerPath(3, 3, 35);
	SetMarkerPath(3, 4, 58);
	SetMarkerPath(30, 0, 76);
	SetMarkerPath(31, 0, 99);
	SetMarkerPath(31, 1, 100);
	SetMarkerPath(32, 0, 103);
	SetMarkerPath(32, 1, 104);
	SetMarkerPath(32, 2, 54);
	SetMarkerPath(32, 3, 102);
	SetMarkerPath(33, 0, 71);
	SetMarkerPath(33, 1, 70);
	SetMarkerPath(33, 2, 72);
	SetMarkerPath(33, 3, 23);
	SetMarkerPath(33, 4, 109);
	SetMarkerPath(33, 5, 128);
	SetMarkerPath(33, 6, 43);
	SetMarkerPath(33, 7, 97);
	SetMarkerPath(34, 0, 40);
	SetMarkerPath(34, 1, 4);
	SetMarkerPath(34, 2, 83);
	SetMarkerPath(35, 0, 12);
	SetMarkerPath(35, 1, 58);
	SetMarkerPath(35, 2, 95);
	SetMarkerPath(35, 3, 3);
	SetMarkerPath(35, 4, 94);
	SetMarkerPath(36, 0, 60);
	SetMarkerPath(36, 1, 39);
	SetMarkerPath(36, 2, 61);
	SetMarkerPath(37, 0, 69);
	SetMarkerPath(37, 1, 78);
	SetMarkerPath(37, 2, 125);
	SetMarkerPath(38, 0, 123);
	SetMarkerPath(38, 1, 114);
	SetMarkerPath(38, 2, 87);
	SetMarkerPath(38, 3, 1);
	SetMarkerPath(38, 4, 85);
	SetMarkerPath(38, 5, 101);
	SetMarkerPath(38, 6, 83);
	SetMarkerPath(39, 0, 119);
	SetMarkerPath(39, 1, 64);
	SetMarkerPath(39, 2, 61);
	SetMarkerPath(39, 3, 62);
	SetMarkerPath(39, 4, 51);
	SetMarkerPath(39, 5, 126);
	SetMarkerPath(39, 6, 110);
	SetMarkerPath(4, 0, 3);
	SetMarkerPath(40, 0, 87);
	SetMarkerPath(40, 1, 106);
	SetMarkerPath(41, 0, 98);
	SetMarkerPath(41, 1, 29);
	SetMarkerPath(41, 2, 121);
	SetMarkerPath(41, 3, 96);
	SetMarkerPath(41, 4, 109);
	SetMarkerPath(41, 5, 112);
	SetMarkerPath(42, 0, 43);
	SetMarkerPath(43, 0, 22);
	SetMarkerPath(43, 1, 54);
	SetMarkerPath(43, 2, 96);
	SetMarkerPath(43, 3, 97);
	SetMarkerPath(44, 0, 46);
	SetMarkerPath(44, 1, 112);
	SetMarkerPath(45, 0, 46);
	SetMarkerPath(45, 1, 47);
	SetMarkerPath(46, 0, 44);
	SetMarkerPath(46, 1, 45);
	SetMarkerPath(47, 0, 42);
	SetMarkerPath(47, 1, 45);
	SetMarkerPath(48, 0, 49);
	SetMarkerPath(48, 1, 83);
	SetMarkerPath(48, 2, 82);
	SetMarkerPath(48, 3, 81);
	SetMarkerPath(48, 4, 80);
	SetMarkerPath(49, 0, 48);
	SetMarkerPath(49, 1, 81);
	SetMarkerPath(49, 2, 80);
	SetMarkerPath(49, 3, 82);
	SetMarkerPath(5, 0, 84);
	SetMarkerPath(51, 0, 110);
	SetMarkerPath(51, 1, 68);
	SetMarkerPath(51, 2, 39);
	SetMarkerPath(52, 0, 95);
	SetMarkerPath(53, 0, 102);
	SetMarkerPath(53, 1, 66);
	SetMarkerPath(53, 2, 13);
	SetMarkerPath(54, 0, 96);
	SetMarkerPath(54, 1, 97);
	SetMarkerPath(54, 2, 43);
	SetMarkerPath(54, 3, 103);
	SetMarkerPath(54, 4, 104);
	SetMarkerPath(54, 5, 32);
	SetMarkerPath(55, 0, 64);
	SetMarkerPath(55, 1, 39);
	SetMarkerPath(55, 2, 119);
	SetMarkerPath(55, 3, 105);
	SetMarkerPath(56, 0, 16);
	SetMarkerPath(56, 1, 60);
	SetMarkerPath(56, 2, 59);
	SetMarkerPath(56, 3, 17);
	SetMarkerPath(56, 4, 6);
	SetMarkerPath(57, 0, 122);
	SetMarkerPath(57, 1, 23);
	SetMarkerPath(57, 2, 121);
	SetMarkerPath(57, 3, 109);
	SetMarkerPath(57, 4, 41);
	SetMarkerPath(58, 0, 35);
	SetMarkerPath(58, 1, 94);
	SetMarkerPath(58, 2, 12);
	SetMarkerPath(58, 3, 3);
	SetMarkerPath(58, 4, 120);
	SetMarkerPath(58, 5, 95);
	SetMarkerPath(59, 0, 17);
	SetMarkerPath(59, 1, 56);
	SetMarkerPath(59, 2, 97);
	SetMarkerPath(6, 0, 16);
	SetMarkerPath(6, 1, 59);
	SetMarkerPath(6, 2, 60);
	SetMarkerPath(60, 0, 56);
	SetMarkerPath(60, 1, 119);
	SetMarkerPath(61, 0, 39);
	SetMarkerPath(61, 1, 62);
	SetMarkerPath(61, 2, 64);
	SetMarkerPath(61, 3, 60);
	SetMarkerPath(62, 0, 61);
	SetMarkerPath(62, 1, 63);
	SetMarkerPath(62, 2, 64);
	SetMarkerPath(62, 3, 39);
	SetMarkerPath(63, 0, 62);
	SetMarkerPath(63, 1, 64);
	SetMarkerPath(63, 2, 65);
	SetMarkerPath(63, 3, 39);
	SetMarkerPath(64, 0, 65);
	SetMarkerPath(64, 1, 55);
	SetMarkerPath(64, 2, 39);
	SetMarkerPath(64, 3, 63);
	SetMarkerPath(64, 4, 62);
	SetMarkerPath(64, 5, 61);
	SetMarkerPath(64, 6, 68);
	SetMarkerPath(64, 7, 127);
	SetMarkerPath(65, 0, 64);
	SetMarkerPath(65, 1, 13);
	SetMarkerPath(65, 2, 66);
	SetMarkerPath(65, 3, 63);
	SetMarkerPath(65, 4, 111);
	SetMarkerPath(66, 0, 65);
	SetMarkerPath(66, 1, 102);
	SetMarkerPath(66, 2, 13);
	SetMarkerPath(66, 3, 111);
	SetMarkerPath(67, 0, 127);
	SetMarkerPath(67, 1, 105);
	SetMarkerPath(67, 2, 39);
	SetMarkerPath(67, 3, 15);
	SetMarkerPath(67, 4, 55);
	SetMarkerPath(67, 5, 25);
	SetMarkerPath(68, 0, 51);
	SetMarkerPath(68, 1, 127);
	SetMarkerPath(68, 2, 100);
	SetMarkerPath(68, 3, 64);
	SetMarkerPath(68, 4, 39);
	SetMarkerPath(68, 5, 55);
	SetMarkerPath(69, 0, 78);
	SetMarkerPath(69, 1, 126);
	SetMarkerPath(69, 2, 64);
	SetMarkerPath(69, 3, 111);
	SetMarkerPath(7, 0, 35);
	SetMarkerPath(70, 0, 33);
	SetMarkerPath(70, 1, 71);
	SetMarkerPath(70, 2, 72);
	SetMarkerPath(70, 3, 125);
	SetMarkerPath(71, 0, 70);
	SetMarkerPath(71, 1, 33);
	SetMarkerPath(71, 2, 11);
	SetMarkerPath(71, 3, 96);
	SetMarkerPath(71, 4, 128);
	SetMarkerPath(71, 5, 54);
	SetMarkerPath(71, 6, 43);
	SetMarkerPath(72, 0, 77);
	SetMarkerPath(72, 1, 33);
	SetMarkerPath(72, 2, 70);
	SetMarkerPath(72, 3, 97);
	SetMarkerPath(72, 4, 128);
	SetMarkerPath(72, 5, 43);
	SetMarkerPath(73, 0, 21);
	SetMarkerPath(73, 1, 74);
	SetMarkerPath(73, 2, 79);
	SetMarkerPath(73, 3, 80);
	SetMarkerPath(73, 4, 128);
	SetMarkerPath(73, 5, 98);
	SetMarkerPath(74, 0, 73);
	SetMarkerPath(74, 1, 75);
	SetMarkerPath(74, 2, 79);
	SetMarkerPath(74, 3, 97);
	SetMarkerPath(74, 4, 128);
	SetMarkerPath(75, 0, 74);
	SetMarkerPath(75, 1, 72);
	SetMarkerPath(75, 2, 77);
	SetMarkerPath(75, 3, 104);
	SetMarkerPath(76, 0, 77);
	SetMarkerPath(76, 1, 30);
	SetMarkerPath(77, 0, 75);
	SetMarkerPath(77, 1, 72);
	SetMarkerPath(77, 2, 76);
	SetMarkerPath(77, 3, 29);
	SetMarkerPath(77, 4, 104);
	SetMarkerPath(77, 5, 116);
	SetMarkerPath(77, 6, 128);
	SetMarkerPath(78, 0, 37);
	SetMarkerPath(78, 1, 125);
	SetMarkerPath(78, 2, 69);
	SetMarkerPath(78, 3, 111);
	SetMarkerPath(79, 0, 73);
	SetMarkerPath(79, 1, 74);
	SetMarkerPath(79, 2, 10);
	SetMarkerPath(79, 3, 96);
	SetMarkerPath(79, 4, 11);
	SetMarkerPath(79, 5, 128);
	SetMarkerPath(8, 0, 81);
	SetMarkerPath(8, 1, 48);
	SetMarkerPath(8, 2, 101);
	SetMarkerPath(8, 3, 82);
	SetMarkerPath(80, 0, 73);
	SetMarkerPath(80, 1, 21);
	SetMarkerPath(80, 2, 49);
	SetMarkerPath(80, 3, 81);
	SetMarkerPath(80, 4, 9);
	SetMarkerPath(81, 0, 80);
	SetMarkerPath(81, 1, 21);
	SetMarkerPath(81, 2, 82);
	SetMarkerPath(81, 3, 49);
	SetMarkerPath(81, 4, 48);
	SetMarkerPath(82, 0, 101);
	SetMarkerPath(82, 1, 81);
	SetMarkerPath(82, 2, 48);
	SetMarkerPath(82, 3, 8);
	SetMarkerPath(83, 0, 107);
	SetMarkerPath(83, 1, 101);
	SetMarkerPath(83, 2, 20);
	SetMarkerPath(83, 3, 4);
	SetMarkerPath(83, 4, 1);
	SetMarkerPath(84, 0, 85);
	SetMarkerPath(84, 1, 5);
	SetMarkerPath(85, 0, 84);
	SetMarkerPath(85, 1, 38);
	SetMarkerPath(85, 2, 87);
	SetMarkerPath(85, 3, 108);
	SetMarkerPath(86, 0, 38);
	SetMarkerPath(86, 1, 88);
	SetMarkerPath(86, 2, 89);
	SetMarkerPath(86, 3, 18);
	SetMarkerPath(86, 4, 28);
	SetMarkerPath(87, 0, 85);
	SetMarkerPath(87, 1, 40);
	SetMarkerPath(87, 2, 106);
	SetMarkerPath(87, 3, 38);
	SetMarkerPath(87, 4, 108);
	SetMarkerPath(87, 5, 20);
	SetMarkerPath(88, 0, 18);
	SetMarkerPath(88, 1, 86);
	SetMarkerPath(88, 2, 123);
	SetMarkerPath(89, 0, 86);
	SetMarkerPath(89, 1, 90);
	SetMarkerPath(89, 2, 28);
	SetMarkerPath(9, 0, 21);
	SetMarkerPath(9, 1, 73);
	SetMarkerPath(9, 2, 81);
	SetMarkerPath(9, 3, 49);
	SetMarkerPath(90, 0, 89);
	SetMarkerPath(90, 1, 96);
	SetMarkerPath(91, 0, 92);
	SetMarkerPath(91, 1, 93);
	SetMarkerPath(91, 2, 2);
	SetMarkerPath(92, 0, 94);
	SetMarkerPath(92, 1, 91);
	SetMarkerPath(93, 0, 91);
	SetMarkerPath(93, 1, 95);
	SetMarkerPath(94, 0, 35);
	SetMarkerPath(94, 1, 92);
	SetMarkerPath(94, 2, 3);
	SetMarkerPath(94, 3, 58);
	SetMarkerPath(95, 0, 93);
	SetMarkerPath(95, 1, 3);
	SetMarkerPath(95, 2, 35);
	SetMarkerPath(95, 3, 58);
	SetMarkerPath(96, 0, 90);
	SetMarkerPath(96, 1, 98);
	SetMarkerPath(96, 2, 54);
	SetMarkerPath(96, 3, 103);
	SetMarkerPath(96, 4, 109);
	SetMarkerPath(96, 5, 116);
	SetMarkerPath(97, 0, 59);
	SetMarkerPath(97, 1, 29);
	SetMarkerPath(97, 2, 54);
	SetMarkerPath(97, 3, 104);
	SetMarkerPath(97, 4, 23);
	SetMarkerPath(97, 5, 115);
	SetMarkerPath(98, 0, 41);
	SetMarkerPath(98, 1, 29);
	SetMarkerPath(98, 2, 109);
	SetMarkerPath(98, 3, 22);
	SetMarkerPath(98, 4, 112);
	SetMarkerPath(99, 0, 110);
	SetMarkerPath(99, 1, 31);

	SetMarkerPathFlags(106, 4, ROCKET_JUMP);
	SetMarkerPathFlags(109, 4, DM6_DOOR);
	SetMarkerPathFlags(111, 2, ROCKET_JUMP);
	SetMarkerPathFlags(111, 3, ROCKET_JUMP);
	SetMarkerPathFlags(121, 0, JUMP_LEDGE);
	SetMarkerPathFlags(121, 3, DM6_DOOR);
	SetMarkerPathFlags(29, 5, DM6_DOOR);
	SetMarkerPathFlags(38, 5, ROCKET_JUMP);
	SetMarkerPathFlags(38, 6, ROCKET_JUMP);
	SetMarkerPathFlags(39, 4, ROCKET_JUMP);
	SetMarkerPathFlags(39, 5, ROCKET_JUMP);
	SetMarkerPathFlags(39, 6, ROCKET_JUMP);
	SetMarkerPathFlags(41, 0, JUMP_LEDGE);
	SetMarkerPathFlags(41, 1, JUMP_LEDGE);
	SetMarkerPathFlags(41, 2, JUMP_LEDGE);
	SetMarkerPathFlags(41, 3, JUMP_LEDGE);
	SetMarkerPathFlags(41, 4, JUMP_LEDGE);
	SetMarkerPathFlags(41, 5, DM6_DOOR);
	SetMarkerPathFlags(64, 6, ROCKET_JUMP);
	SetMarkerPathFlags(64, 7, ROCKET_JUMP);
	SetMarkerPathFlags(98, 4, DM6_DOOR);

	// special case...
	dm6_door = markers[41 - 1];
}

void DM6CampLogic() {
	qbool has_weapon = (int)self->s.v.items & (IT_ROCKET_LAUNCHER | IT_LIGHTNING);

	// Camp the red armor...
	if (numberofclients > 1) {
		if (has_weapon && !self->fb.bot_evade) {
			if ((self->s.v.health > 80) && (self->s.v.armorvalue > 100)) {
				if ((self->s.v.ammo_cells > 15) || (self->s.v.ammo_rockets > 3)) {
					search_entity = ez_find(world, "item_armorInv");
					if (search_entity) {
						if (search_entity->s.v.origin[2] <= self->s.v.origin[2] + 18) {
							if (VectorDistance(search_entity->s.v.origin, self->s.v.origin) < 200) {
								if (random() < 0.9) {
									self->fb.camp_state = self->fb.camp_state | CAMPBOT;
									linked_marker_ = touch_marker_;
								}
							}
							else  {
								self->fb.camp_state = self->fb.camp_state - (self->fb.camp_state & CAMPBOT);
							}
						}
					}
				}
			}
		}
	}
}

void DM6SelectWeaponToOpenDoor (gedict_t* self)
{
	if (self->fb.path_state & DM6_DOOR) {
		int items_ = (int) self->s.v.items;
		int desired_weapon = 0;
		if (self->s.v.ammo_shells) {
			desired_weapon = IT_SHOTGUN;
		}
		else if ((items_ & IT_NAILGUN) && (self->s.v.ammo_nails)) {
			desired_weapon = IT_NAILGUN;
		}
		else if ((items_ & IT_SUPER_NAILGUN) && (self->s.v.ammo_nails)) {
			desired_weapon = IT_SUPER_NAILGUN;
		}
		else if ((items_ & IT_LIGHTNING) && (self->s.v.ammo_cells)) {
			desired_weapon = IT_LIGHTNING;
		}

		self->fb.firing |= CheckNewWeapon( desired_weapon );
	}
}

qbool DM6DoorLogic (gedict_t* self, gedict_t* goal_entity)
{
	if (goal_entity->fb.Z_ == 1) {
		if (self->fb.touch_marker->fb.zones[0].task & DM6_DOOR) {
			if (dm6_door->s.v.takedamage) {
				if (self->s.v.enemy == NUM_FOR_EDICT(self->fb.look_object)) {
					if (!self->invincible_time) {
						if (self->fb.look_object->fb.firepower >= 50) {
							goal_entity->fb.saved_goal_desire = 0;
							return true;
						}
					}
				}
			}
		}
	}

	return false;
}

qbool DM6DoorClosed (fb_path_eval_t* eval)
{
	return (eval->test_marker == dm6_door && !dm6_door->s.v.takedamage) ||
		   ((eval->description & DM6_DOOR) && dm6_door->s.v.origin[0] > -64);
}

void DM6MarkerTouchLogic (gedict_t* self, gedict_t* goalentity_marker, gedict_t* touch_marker_)
{
	if (goalentity_marker && goalentity_marker->fb.Z_ == 1) {
		if (touch_marker_->fb.zones[0].task & DM6_DOOR) {
			if (dm6_door->s.v.takedamage) {
				vec3_t temp, src;
				VectorAdd(dm6_door->s.v.absmin, dm6_door->s.v.view_ofs, temp);
				VectorSubtract(temp, origin_, temp);
				temp[2] -= 40;
				normalize(temp, direction);
				VectorCopy(origin_, src);
				src[2] += 16;
				traceline(src[0], src[1], src[2], src[0] + direction[0] * 2048, src[1] + direction[1] * 2048, src[2] + direction[2] * 2048, FALSE, self);
				if (PROG_TO_EDICT(g_globalvars.trace_ent) == dm6_door) {
					self->fb.path_state |= DM6_DOOR;
				}
			}
			else  {
				self->fb.path_state &= ~DM6_DOOR;
			}
		}
	}
}

qbool DM6LookAtDoor (gedict_t* self)
{
	if (self->fb.path_state & DM6_DOOR) {
		self->fb.state |= NOTARGET_ENEMY;
		self->fb.look_object = look_object_ = dm6_door;
		return true;
	}

	return false;
}

qbool DM6FireAtDoor (gedict_t* self)
{
	if (self->fb.path_state & DM6_DOOR) {
		if (dm6_door->s.v.takedamage) {
			rel_pos[2] = rel_pos[2] - 38;
		}
		else {
			self->fb.path_state -= DM6_DOOR;
			self->fb.state &= NOT_NOTARGET_ENEMY;
		}

		return true;
	}

	return false;
}
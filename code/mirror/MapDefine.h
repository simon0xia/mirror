#ifndef _MAP_DEFINE_H
#define _MAP_DEFINE_H

const qint32 
	mapCount_major = 9,
	mapCount_minor = 10;

struct mirror_map {
	qint32 ID;
	quint32 imageID;
	QString name;
};

const mirror_map map_major[mapCount_major] = {
	{ 0, 0, QStringLiteral("新手村") },
	{ 1, 0, QStringLiteral("比奇") },
	{ 2, 0, QStringLiteral("盟重") },
	{ 3, 0, QStringLiteral("白日门") },
	{ 4, 0, QStringLiteral("苍月岛") },
	{ 5, 0, QStringLiteral("封魔谷") },
	{ 6, 0, QStringLiteral("魔龙城") },
	{ 7, 0, QStringLiteral("特色地图") },
	{ 8, 0, QStringLiteral("特权地图") }
};

const mirror_map map_minor[mapCount_major][mapCount_minor] = {
	{ 0, 0, QStringLiteral("新手村") },
	{ 0, 10, QStringLiteral("比奇城外") ,
	  1, 11, QStringLiteral("半兽人古墓") ,
	  2, 12, QStringLiteral("矿洞") ,
	  3, 13, QStringLiteral("沃玛神殿")
	},
	{ 0, 20, QStringLiteral("盟重城外") ,
	  1, 21, QStringLiteral("毒蛇山谷"),
	  2, 22, QStringLiteral("蜈蚣洞"),
	  3, 23, QStringLiteral("猪洞"),
	  4, 24, QStringLiteral("祖玛神殿")
	},
	{	0, 30, QStringLiteral("白日门城外"),
		1,31, QStringLiteral("丛林迷宫"),
		2,32, QStringLiteral("赤月峡谷"),
		3,33, QStringLiteral("峡谷广场"),
		4,34, QStringLiteral("抉择之地"),
		5,35, QStringLiteral("恶魔巢穴")
	},
	{ 0,40, QStringLiteral("尸魔洞") ,
	  1,41, QStringLiteral("骨魔洞") ,
	  2,42, QStringLiteral("牛魔寺庙") 
	},
	{ 0,50, QStringLiteral("封魔城外") ,
	  1,51, QStringLiteral("封魔矿区") ,
	  2,52, QStringLiteral("疾风殿") ,
	  3,53, QStringLiteral("魔魂殿") ,
	  4,54, QStringLiteral("烈焰殿") ,
	  5,55, QStringLiteral("霸者大厅") ,
	  6,56, QStringLiteral("封魔殿")
	},
	{ 0,60, QStringLiteral("魔龙郊外") ,
	  1,61, QStringLiteral("魔龙谷") ,
	  2,62, QStringLiteral("魔龙祭坛") ,
	  3,63, QStringLiteral("魔龙岭") ,
	  4,64, QStringLiteral("魔龙沼泽") ,
	  5,65, QStringLiteral("魔龙血域") 
	},
	{ 0,70, QStringLiteral("重装上阵")
	},
	{ 0,80, QStringLiteral("圣域幻镜1"),
	1,81, QStringLiteral("圣域幻镜2"),
	2,13, QStringLiteral("圣域幻镜3"),
	3,22, QStringLiteral("圣域幻镜4"),
	4,23, QStringLiteral("圣域幻镜5"),
	5,71, QStringLiteral("王者禁地"),
	6,24, QStringLiteral("圣域幻镜7"),
	7,55, QStringLiteral("圣域幻镜8"),
	8,33, QStringLiteral("圣域幻镜9"),
	9,42, QStringLiteral("圣域幻镜10")
	}
};

#endif
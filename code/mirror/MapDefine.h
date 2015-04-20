#ifndef _MAP_DEFINE_H
#define _MAP_DEFINE_H

const qint32 
	mapCount_major = 9,
	mapCount_minor = 10;

struct mirror_map {
	qint32 ID;
	QString name;
};

const mirror_map map_major[mapCount_major] = {
	{ 0, QString::fromLocal8Bit("新手村") },
	{ 1, QString::fromLocal8Bit("比奇") },
	{ 2, QString::fromLocal8Bit("盟重") },
	{ 3, QString::fromLocal8Bit("白日门") },
	{ 4, QString::fromLocal8Bit("苍月岛") },	
	{ 5, QString::fromLocal8Bit("封魔谷") },
	{ 6, QString::fromLocal8Bit("魔龙城") },
	{ 7, QString::fromLocal8Bit("特色地图") },
	{ 8, QString::fromLocal8Bit("特权地图") }
};

const mirror_map map_minor[mapCount_major][mapCount_minor] = {
	{ 0, QString::fromLocal8Bit("新手村") },
	{ 0, QString::fromLocal8Bit("比奇城外") ,
	  1, QString::fromLocal8Bit("半兽人古墓") ,
	  2, QString::fromLocal8Bit("矿洞") ,
	  3, QString::fromLocal8Bit("沃玛神殿")
	},
	{ 0, QString::fromLocal8Bit("盟重城外") ,
	  1, QString::fromLocal8Bit("毒蛇山谷"),
	  2, QString::fromLocal8Bit("蜈蚣洞"),
	  3, QString::fromLocal8Bit("猪洞"),
	  4, QString::fromLocal8Bit("祖玛神殿")
	},
	{	0, QString::fromLocal8Bit("白日门城外"),
		1, QString::fromLocal8Bit("丛林迷宫"),
		2, QString::fromLocal8Bit("赤月峡谷"),
		3, QString::fromLocal8Bit("峡谷广场"),
		4, QString::fromLocal8Bit("抉择之地"),
		5, QString::fromLocal8Bit("恶魔巢穴")
	},
	{ 0, QString::fromLocal8Bit("骨魔洞") ,
	  1, QString::fromLocal8Bit("尸魔洞") ,
	  2, QString::fromLocal8Bit("牛魔寺庙") 
	},
	{ 0, QString::fromLocal8Bit("封魔城外") ,
	  1, QString::fromLocal8Bit("封魔矿区") ,
	  2, QString::fromLocal8Bit("疾风殿") ,
	  3, QString::fromLocal8Bit("烈焰殿") ,
	  4, QString::fromLocal8Bit("魔魂殿") ,
	  5, QString::fromLocal8Bit("封魔殿") ,
	  6, QString::fromLocal8Bit("霸者大厅")
	},
	{ 0, QString::fromLocal8Bit("魔龙东郊") ,
	  1, QString::fromLocal8Bit("魔龙旧寨") ,
	  2, QString::fromLocal8Bit("魔龙祭坛") ,
	  3, QString::fromLocal8Bit("魔龙岭") ,
	  4, QString::fromLocal8Bit("魔龙沼泽") ,
	  5, QString::fromLocal8Bit("魔龙血域") 
	},
	{ 0, QString::fromLocal8Bit("重装上阵")
	},
	{ 0, QString::fromLocal8Bit("圣域幻镜1"),
	1, QString::fromLocal8Bit("圣域幻镜2"),
	2, QString::fromLocal8Bit("圣域幻镜3"),
	3, QString::fromLocal8Bit("圣域幻镜4"),
	4, QString::fromLocal8Bit("圣域幻镜5"),
	5, QString::fromLocal8Bit("王者禁地"),
	6, QString::fromLocal8Bit("圣域幻镜7"),
	7, QString::fromLocal8Bit("圣域幻镜8"),
	8, QString::fromLocal8Bit("圣域幻镜9"),
	9, QString::fromLocal8Bit("圣域幻镜10")
	}
};

#endif
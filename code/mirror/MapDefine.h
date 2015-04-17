#ifndef _MAP_DEFINE_H
#define _MAP_DEFINE_H

const qint32 
	mapCount_major = 7,
	mapCount_minor = 9;

struct mirror_map {
	qint32 ID;
	QString name;
};

const mirror_map map_major[mapCount_major] = {
    { 0, QStringLiteral("新手村") },
    { 1, QStringLiteral("比奇") },
    { 2, QStringLiteral("盟重") },
    { 3, QStringLiteral("苍月岛") },
    { 4, QStringLiteral("白日门") },
    { 5, QStringLiteral("封魔谷") },
    { 6, QStringLiteral("魔龙城") }
};

const mirror_map map_minor[mapCount_major][mapCount_minor] = {
    { 0, QStringLiteral("新手村") },
    { 0, QStringLiteral("比奇城外") ,
      1, QStringLiteral("半兽人古墓") ,
      2, QStringLiteral("矿洞") ,
      3, QStringLiteral("沃玛神殿")
	},
    { 0, QStringLiteral("盟重城外") ,
      1, QStringLiteral("毒蛇山谷"),
      2, QStringLiteral("蜈蚣洞"),
      3, QStringLiteral("猪洞"),
      4, QStringLiteral("祖玛神殿")
	},
    { 0, QStringLiteral("骨魔洞") ,
      1, QStringLiteral("尸魔洞") ,
      2, QStringLiteral("牛魔寺庙")
	},
    { 0, QStringLiteral("白日门城外") ,
      1, QStringLiteral("丛林迷宫"),
      2, QStringLiteral("赤月峡谷") ,
      3, QStringLiteral("峡谷广场") ,
      4, QStringLiteral("抉择之地") ,
      5, QStringLiteral("恶魔巢穴")
	},
    { 0, QStringLiteral("封魔城外") ,
      1, QStringLiteral("封魔矿区") ,
      2, QStringLiteral("疾风殿") ,
      3, QStringLiteral("烈焰殿") ,
      4, QStringLiteral("魔魂殿") ,
      5, QStringLiteral("封魔殿") ,
      6, QStringLiteral("霸者大厅")
	},
    { 0, QStringLiteral("魔龙东郊") ,
      1, QStringLiteral("魔龙旧寨") ,
      2, QStringLiteral("魔龙祭坛") ,
      3, QStringLiteral("魔龙岭") ,
      4, QStringLiteral("魔龙沼泽") ,
      5, QStringLiteral("魔龙血域")
	}
};

#endif

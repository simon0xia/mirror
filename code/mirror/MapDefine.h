#ifndef _MAP_DEFINE_H
#define _MAP_DEFINE_H

#include "def_DataType.h"

const qint32 
	mapCount_major = 9,
	mapCount_minor = 10;

struct mirror_map {
	mapID ID;
	quint32 imageID;
	QString name;
};

const mirror_map map_major[mapCount_major] = {
	{ 0, 0, QStringLiteral("ĞÂÊÖ´å") },
	{ 1, 0, QStringLiteral("±ÈÆæ") },
	{ 2, 0, QStringLiteral("ÃËÖØ") },
	{ 3, 0, QStringLiteral("°×ÈÕÃÅ") },
	{ 4, 0, QStringLiteral("²ÔÔÂµº") },
	{ 5, 0, QStringLiteral("·âÄ§¹È") },
	{ 6, 0, QStringLiteral("Ä§Áú³Ç") },
	{ 7, 0, QStringLiteral("ÌØÉ«µØÍ¼") },
	{ 8, 0, QStringLiteral("Ê¥Óò»Ã¾µ") }
};

const mirror_map map_minor[mapCount_major][mapCount_minor] = {
	{ 0, 0, QStringLiteral("ĞÂÊÖ´å") },
	{ 0, 10, QStringLiteral("±ÈÆæ³ÇÍâ") ,
	  1, 11, QStringLiteral("°ëÊŞÈË¹ÅÄ¹") ,
	  2, 12, QStringLiteral("¿ó¶´") ,
	  3, 13, QStringLiteral("ÎÖÂêÉñµî")
	},
	{ 0, 20, QStringLiteral("ÃËÖØ³ÇÍâ") ,
	  1, 21, QStringLiteral("¶¾ÉßÉ½¹È"),
	  2, 22, QStringLiteral("òÚò¼¶´"),
	  3, 23, QStringLiteral("Öí¶´"),
	  4, 24, QStringLiteral("×æÂêÉñµî")
	},
	{	0, 30, QStringLiteral("°×ÈÕÃÅ³ÇÍâ"),
		1,31, QStringLiteral("´ÔÁÖÃÔ¹¬"),
		2,32, QStringLiteral("³àÔÂÏ¿¹È"),
		3,33, QStringLiteral("Ï¿¹È¹ã³¡"),
		4,34, QStringLiteral("¾ñÔñÖ®µØ"),
		5,35, QStringLiteral("¶ñÄ§³²Ñ¨")
	},
	{ 0,40, QStringLiteral("Ê¬Ä§¶´") ,
	  1,41, QStringLiteral("¹ÇÄ§¶´") ,
	  2,42, QStringLiteral("Å£Ä§ËÂÃí") 
	},
	{ 0,50, QStringLiteral("·âÄ§³ÇÍâ") ,
	  1,51, QStringLiteral("·âÄ§¿óÇø") ,
	  2,52, QStringLiteral("¼²·çµî") ,
	  3,53, QStringLiteral("Ä§»êµî") ,
	  4,54, QStringLiteral("ÁÒÑæµî") ,
	  5,55, QStringLiteral("°ÔÕß´óÌü") ,
	  6,56, QStringLiteral("·âÄ§µî")
	},
	{ 0,60, QStringLiteral("Ä§Áú½¼Íâ") ,
	  1,61, QStringLiteral("Ä§Áú¹È") ,
	  2,62, QStringLiteral("Ä§Áú¼ÀÌ³") ,
	  3,63, QStringLiteral("Ä§ÁúÁë") ,
	  4,64, QStringLiteral("Ä§ÁúÕÓÔó") ,
	  5,65, QStringLiteral("Ä§ÁúÑªÓò") 
	},
	{ 0,70, QStringLiteral("ÖØ×°ÉÏÕó")
	},
	{ 0,80, QStringLiteral("Ê¥Óò»Ã¾µ1"),
	1,81, QStringLiteral("Ê¥Óò»Ã¾µ2"),
	2,13, QStringLiteral("Ê¥Óò»Ã¾µ3"),
	3,22, QStringLiteral("Ê¥Óò»Ã¾µ4"),
	4,23, QStringLiteral("Ê¥Óò»Ã¾µ5"),
	5,71, QStringLiteral("ÍõÕß½ûµØ"),
	6,24, QStringLiteral("Ê¥Óò»Ã¾µ7"),
	7,55, QStringLiteral("Ê¥Óò»Ã¾µ8"),
	8,33, QStringLiteral("Ê¥Óò»Ã¾µ9"),
	9,42, QStringLiteral("Ê¥Óò»Ã¾µ10")
	}
};

#endif
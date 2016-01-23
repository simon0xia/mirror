#include <stdlib.h>
#include <QFontDatabase>
#include <QStringList>

int GetRandomSequence(int Arr[], int total)
{
	if (total < 2)
	{
		return -1;
	}
	else if (total == 2)
	{
		Arr[0] = Arr[1] = 0;
		int num = 1.0 * rand() / RAND_MAX + 0.55;
		Arr[num] = 1;
		return total;
	}

	int *sequence = new int[total];

	for (int i = 0; i < total; i++)
	{
		sequence[i] = i;
	}

	int end = total - 1;
	for (int i = 0; i < total; i++)
	{
		int num = 1.0 * rand() / RAND_MAX * end;
		Arr[i] = sequence[num];
		sequence[num] = sequence[end];
		end--;
	}

	return total;
}

QString loadFontFamilyFromTTF_ygy()
{
	static QString font;
	static bool loaded = false;
	if (!loaded)
	{
		loaded = true;
		int loadedFontID = QFontDatabase::addApplicationFont("./font/ygyxsziti.TTF");
		QStringList loadedFontFamilies = QFontDatabase::applicationFontFamilies(loadedFontID);
		if (!loadedFontFamilies.empty())
			font = loadedFontFamilies.at(0);
	}
	return font;
}
#include "stdafx.h"
#include "Utility.h"


namespace SnickerDoodles
{
	Utility::Utility()
	{
	}




	float Utility::StandardDeviation(float tValues[], int tSize)
	{
		float aDeviation = 0.0;
		float aMean = 0;

		for (int x = 0; x < tSize; x++)
		{
			aMean += tValues[x];
		}
		aMean = aMean / tSize;

		for (int x = 0; x < tSize; x++)
		{
			aDeviation += (tValues[x] - aMean)*(tValues[x] - aMean);
		}

		aDeviation = sqrt(aDeviation / tSize);

		if (aDeviation == 0)
		{
			aDeviation = 1;
		}

		return aDeviation;
	}

	int Utility::ColorRange(float tValue, float tMinValue, float tMaxValue)
	{
		int aColor;
		float aDifference = (tMaxValue - tMinValue)/2;
		float aCenterValue = tMaxValue - aDifference;

		if (tValue > aCenterValue)
		{
			if (tValue > tMaxValue)
			{
				aColor = 3;
			}
			else
			{
				aColor = 2;
			}
		}
		else
		{
			if (tValue < tMinValue)
			{
				aColor = 0;
			}
			else
			{
				aColor = 1;
			}
		}


		return aColor;
	}
}
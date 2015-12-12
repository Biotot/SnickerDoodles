#pragma once


namespace SnickerDoodles
{

	ref class Utility
	{
	public:
		Utility();

		static float StandardDeviation(float tValues[], int tSize);
		static int ColorRange(float tValue, float tMinValue, float tMaxValue);
	};

}

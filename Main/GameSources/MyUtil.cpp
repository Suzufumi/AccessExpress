/*!
@file MyUtil.cpp
@brief BaseCrossにはないユーティリティ関数
@author Kyohei Okawa
*/

#include "stdafx.h"
#include "Project.h"

namespace basecross
{
	namespace MyUtil
	{
		float wstrToFloat(wstring wstr)
		{
			string str;
			Util::WStoMB(wstr, str);

			return std::stof(str);
		}

		int wstrToInt(wstring wstr)
		{
			string str;
			Util::WStoMB(wstr, str);

			return std::stoi(str);
		}

		bool wstrToBool(wstring wstr)
		{
			int i = wstrToInt(wstr);

			if (i == 0)
				return false;
			else
				return true;
		}

		float clamp(float minVal, float maxVal, float value)
		{
			return min(max(minVal, value), maxVal);
		}

		int getDigit(int number)
		{
			return std::to_string(number).length();
		}

		std::vector<int> extractTotalDigitsSpotCount(int number,int digit)
		{
			vector<int> result;

			for (int i = 0; i < digit; i++)
			{
				if (number == 0)
				{
					result.push_back(0);
					continue;
				}

				result.push_back(number % 10);
				number /= 10;
			}

			return result;
		}

		Vec3 rotateXZVec3(const Vec3 v,const float degRot)
		{
			Vec3 result;

			result.x = v.x * cosf((float)Deg2Rad(degRot)) + v.z * sinf((float)Deg2Rad(degRot));
			result.y = v.y;
			result.z = -v.x * sinf((float)Deg2Rad(degRot)) + v.z * cosf((float)Deg2Rad(degRot));

			return result;
		}

		Vec3 unityVec3StrToBCVec3(wstring str)
		{
			str.pop_back();
			str.erase(str.begin());

			vector<wstring> token;
			Util::WStrToTokenVector(token,str, L',');

			Vec3 result;
			result.x = wstrToFloat(token[0]);
			result.y = wstrToFloat(token[1]);
			result.z = wstrToFloat(token[2]);

			return result;
		}

		Quat unityQuatStrToBCQuat(wstring str)
		{
			str.pop_back();
			str.erase(str.begin());

			vector<wstring> token;
			Util::WStrToTokenVector(token, str, L',');

			Quat result;
			result.x = wstrToFloat(token[0]);
			result.y = wstrToFloat(token[1]);
			result.z = wstrToFloat(token[2]);
			result.w = wstrToFloat(token[3]);

			return result;
		}

		Quat createBillboard(Vec3 line)
		{
			Vec3 temp = line;
			Mat4x4 rotMatrix;
			Vec3 defUp(0, 1.0f, 0);
			Vec2 tempVec2(temp.x, temp.z);
			if (tempVec2.length() < 0.1f) {
				defUp = Vec3(0, 0, 1.0f);
			}
			temp.normalize();
			rotMatrix = XMMatrixLookAtLH(Vec3(0, 0, 0), temp, defUp);
			rotMatrix.inverse();
			Quat quat;
			quat = rotMatrix.quatInMatrix();
			quat.normalize();
			return quat;
		}
	}
}
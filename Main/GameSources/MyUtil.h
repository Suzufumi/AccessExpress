/*!
@file MyUtil.h
@brief BaseCrossにはないユーティリティ関数
@author Kyohei Okawa
*/

#pragma once
#include "stdafx.h"

namespace basecross
{
	namespace MyUtil
	{
		/*!
		 *@brief wstringからfloatへ変換する
		 *@param[in] wstr wstring
		 *@return 変換されたfloat
		 *@author Kyohei Okawa
		*/
		float wstrToFloat(wstring wstr);

		/*!
		 *@brief wstringからintへ変換する
		 *@param[in] wstr wstring
		 *@return 変換されたint
		 *@author Kyohei Okawa
		*/
		int wstrToInt(wstring wstr);

		/*!
		 *@brief wstringからboolへ変換する
		 *@param[in] wstr wstring
		 *@return 変換されたbool
		 *@details 0の時はfalseに変換されそれ以外はtrueに変換される
		 *@author Kyohei Okawa
		*/
		bool wstrToBool(wstring wstr);

		/*!
		 *@brief 最小値を下回る値を最小値に、最大値を超える値を最大値にする
		 *@param[in] minVal 最小値
		 *@param[in] maxVal 最大値
		 *@param[in] value 変換する値
		 *@return 変換後の値
		 *@author Kyohei Okawa
		*/
		float clamp(float minVal, float maxVal, float value);

		/*!
		 *@brief 桁数を数える
		 *@param[in] number 対象の整数
		 *@return numberの桁数
		 *@author Kyohei Okawa
		*/
		int getDigit(int number);

		/*!
		 *@brief 整数を一桁ずつわけvectorに収納する
		 *@param[in] number 整数
		 *@param[in] digit numberの桁数
		 *@return 一桁ずつの数字が入っているvector配列
		 *@details 0番目には１の位の数字、1番目には10の位の数字...
		 *@author Kyohei Okawa
		*/
		std::vector<int> extractTotalDigitsSpotCount(int number, int digit);

		///Vec3をyを軸にrotDegだけ回転させる
		Vec3 rotateXZVec3(const Vec3 v, const float rotDeg);

		///unityのVector3.ToString()で出力された文字をbasecrossのVec3に変換する
		Vec3 unityVec3StrToBCVec3(wstring str);

		///unityのQuaternion.ToString()で出力された文字をbasecrossのQuatに変換する
		Quat unityQuatStrToBCQuat(wstring str);

		///ビルボードするためのクオータニオンを作成する
		Quat createBillboard(Vec3 line);
	}

/*@}*/
}
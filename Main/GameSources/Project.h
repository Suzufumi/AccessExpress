/*!
@file Project.h
@brief コンテンツ用のヘッダをまとめる
*/

#pragma once

#define PI 3.141593

#define Deg2Rad(deg) (float)((deg * PI) / 180.0f)
#define Rad2Deg(rad) (float)((rad * 180.0f) / PI)


#include "ProjectShader.h"
#include "ProjectBehavior.h"
#include "Scene.h"
#include "GameStage.h"
#include "ProjectCamera.h"
#include "FixedGameObject.h"
#include "ShapeObject.h"
#include "PlayerSupport.h"
#include "Player.h"
#include "AccessObject.h"
#include "SearchObject.h"
#include "LinkObject.h"
#include "Timer.h"
#include "Ray.h"


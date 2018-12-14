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
#include "MyUtil.h"
#include "Scene.h"
#include "GameManager.h"
#include "GameStage.h"
#include "TitleStage.h"
#include "StageSelect.h"
#include "Result.h"
#include "GameOver.h"
#include "ProjectCamera.h"
#include "FixedGameObject.h"
#include "ShapeObject.h"
#include "Enemy.h"
#include "Sprite.h"
#include "PlayerSupport.h"
#include "PlayerAnimState.h"
#include "AccessObject.h"
#include "SearchObject.h"
#include "LinkObject.h"
#include "Player.h"
#include "MainUi.h"
#include "Timer.h"
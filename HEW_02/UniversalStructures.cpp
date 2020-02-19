//*****************************************************************************
// UniversalStructures.cpp
// ゲームの構造体と変数を管理する
//*****************************************************************************
#include "UniversalStructures.h"
#include "stdio.h"

//*****************************************************************************
//IsInCollision3D関数
//当たり判定を確認する
//引数：Hitbox3D, Hitbox3D
//戻：bool
//*****************************************************************************
bool IsInCollision3D(Hitbox3D a, Hitbox3D b)
{
	//check the X axis
	if (a.SizeX == 0 || a.SizeY == 0 || a.SizeZ == 0 ||
		b.SizeX == 0 || b.SizeY == 0 || b.SizeZ == 0)
		return false;
	float comX = a.x - b.x;
	if (comX < 0)
		comX *= -1;
	float comY = a.y - b.y;
	if (comY < 0)
		comY *= -1;
	float comZ = a.z - b.z;
	if (comZ < 0)
		comZ *= -1;
	if (comX < a.SizeX + b.SizeX)
	{
		//check the Y axis
		if (comY < a.SizeY + b.SizeY)
		{
			//check the Z axis
			if (comZ < a.SizeZ + b.SizeZ)
			{
				return true;
			}
		}
	}
	return false;
}

//*****************************************************************************
//IsInCollision2D関数
//2D当たり判定を確認する
//引数：Hitbox2D, Hitbox2D
//戻：bool
//*****************************************************************************
bool IsInCollision2D(Hitbox2D hbA, Hitbox2D hbB)
{
	if (hbA.h == 0 || hbA.w == 0 || hbB.w == 0 || hbB.h == 0)
		return false;
	Hitbox2D a = hbA;
	Hitbox2D b = hbB;
	a.x = a.x - (a.w*0.5f);
	b.x = b.x - (b.w*0.5f);
	a.y = a.y - (a.h*0.5f);
	b.y = b.y - (b.h*0.5f);
	return (((a.x + a.w) >= b.x) && (a.x) <= (b.x + b.w) && (a.y + a.h) >= b.y && a.y <= (b.y + b.h));
}

//*****************************************************************************
//IsVectorZero関数
//ベクトルの中身は0を確認する
//引数：XMFLOAT3
//戻：bool
//*****************************************************************************
bool IsVectorZero(XMFLOAT3 vec)
{
	if (vec.x != 0)
		return false;
	if (vec.y != 0)
		return false;
	if (vec.z != 0)
		return false;
	return true;
}

//*****************************************************************************
//CompVector関数
//ベクトルの中身は同じを確認する
//引数：XMFLOAT3, XMFLOAT3
//戻：bool
//*****************************************************************************
bool CompVector(XMFLOAT3 a, XMFLOAT3 b)
{
	if (a.x != b.x)
		return false;
	if (a.y != b.y)
		return false;
	if (a.z != b.z)
		return false;
	return true;
}

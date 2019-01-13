#include"Camera.h"
using namespace MathInterface;
ICamera::ICamera()
{
	mRotateX_Pitch = 0;
	mRotateY_Yaw = 0;
	mRotateZ_Roll = 0;
	mViewAngleY = 60.0f / 180.0f * MathInterface::PI;
	mAspectRatio = 4/3;
	mPosition = FLOAT3(0, 0, -8);
	mLookat = FLOAT3(0, 0, 0);
	mDirection = FLOAT3(0, 0, 1);
	mNearPlane = 1.0f;
	mFarPlane = 1000.0f;


	mMatrixProjection = MatrixPerspectiveFovLH(mViewAngleY, mAspectRatio, mNearPlane, mFarPlane);
	mMatrixView.Identity();
};
void ICamera::Destroy()
{
};
void	ICamera::SetLookAt(FLOAT3 vLookat)
{
	mLookat = vLookat;
	mFunction_UpdateRotation();
};

void	ICamera::SetLookAt(float x, float y, float z)
{
	FLOAT3 tmpLookat(x, y, z);
	mLookat = tmpLookat;
	mFunction_UpdateRotation();
};
FLOAT3	ICamera::GetLookAt()
{
	return  mLookat;
};

void	ICamera::SetPosition(FLOAT3 vPos)
{
	//lookat和位置不能重合啊
	mPosition = vPos;
	mFunction_UpdateRotation();
}

void	ICamera::SetPosition(float x, float y, float z)
{
	FLOAT3 tmpPos(x, y, z);
	mPosition = tmpPos;
	mFunction_UpdateRotation();
};

FLOAT3 ICamera::GetPosition()
{
	return mPosition;
}
FLOAT3 ICamera::GetDirection()
{
	return mLookat - mPosition;
};
void ICamera::Move(FLOAT3 vRelativePos)
{
	mPosition += vRelativePos;
	mLookat += vRelativePos;

};
void ICamera::Move(float relativeX, float relativeY, float relativeZ)
{
	mPosition += FLOAT3(relativeX, relativeY, relativeZ);
};

void ICamera::SetRotation(float RX_Pitch, float RY_Yaw, float RZ_Roll)//要更新Lookat
{
	SetRotationX_Pitch(RX_Pitch);
	SetRotationY_Yaw(RY_Yaw);
	SetRotationZ_Roll(RZ_Roll);
	mFunction_UpdateDirection();
};
void ICamera::SetRotationY_Yaw(float angleY)
{
	mRotateY_Yaw = angleY;
	mFunction_UpdateDirection();
};
void ICamera::SetRotationX_Pitch(float AngleX)
{
	//clamp to [-pi/2,pi/2]
	mRotateX_Pitch = Clamp(AngleX, -(PI / 2), (PI / 2));

	mFunction_UpdateDirection();
};
void ICamera::SetRotationZ_Roll(float AngleZ)
{
	//roll翻滚不需要更新lookat
	mRotateZ_Roll = AngleZ;
}
float ICamera::GetRotationY_Yaw()
{
	return mRotateY_Yaw;
}

float ICamera::GetRotationX_Pitch()
{
	return mRotateX_Pitch;
}

float ICamera::GetRotationZ_Roll()
{
	return mRotateZ_Roll;
}
void ICamera::RotateY_Yaw(float angleY)
{
	SetRotationY_Yaw(mRotateY_Yaw + angleY);
	mFunction_UpdateDirection();
};

void ICamera::RotateX_Pitch(float angleX)
{
	float newAngle = mRotateX_Pitch + angleX;
	SetRotationX_Pitch(newAngle);
	mFunction_UpdateDirection();
};

void ICamera::RotateZ_Roll(float angleZ)
{
	SetRotationZ_Roll(mRotateZ_Roll + angleZ);
	mFunction_UpdateDirection();
};
void ICamera::fps_MoveForward(float fSignedDistance, BOOL enableYAxisMovement)
{
	//...Yaw Angle Starts at Z axis ( left-handed system) 
	/*		Z
	|    / A
	|_ /
   O|/__________ X

	angle AOZ is the yaw angle

	*/

	FLOAT3 relativePos;

	//generate a direction first (later multiply it with fDist)
	if (enableYAxisMovement)
	{
		relativePos.x = sinf(mRotateY_Yaw) *cosf(mRotateX_Pitch);
		relativePos.z = cosf(mRotateY_Yaw) *cosf(mRotateX_Pitch);
		relativePos.y = sinf(mRotateX_Pitch);
	}
	else
	{
		relativePos.x = sinf(mRotateY_Yaw);
		relativePos.z = cosf(mRotateY_Yaw);
		relativePos.y = 0;
	}

	// length * unit_dir
	relativePos *= fSignedDistance;

	//relative movement
	Move(relativePos);
}

void ICamera::fps_MoveRight(float fSignedDistance, BOOL enableYAxisMovement)
{
	//...Yaw Angle Starts at Z axis ( left-handed system) 
	/*
	Z
	|    / A
	|_ /
	O	|/__________ X

	angle AOZ is the yaw angle

	*/

	FLOAT3 relativePos;

	//generate a direction first (later multiply it with fDist)
	if (enableYAxisMovement)
	{
		relativePos.x = cosf(mRotateY_Yaw) *cosf(mRotateX_Pitch);
		relativePos.z = -sinf(mRotateY_Yaw) *cosf(mRotateX_Pitch);
		relativePos.y = sinf(mRotateX_Pitch);
	}
	else
	{
		relativePos.x = cosf(mRotateY_Yaw);
		relativePos.z = -sinf(mRotateY_Yaw);
		relativePos.y = 0;
	}

	// length * unit_dir
	relativePos *= fSignedDistance;

	//relative movement
	Move(relativePos);
}
void ICamera::fps_MoveUp(float fSignedDistance)
{
	mPosition += FLOAT3(0, fSignedDistance, 0);
	mLookat += FLOAT3(0, fSignedDistance, 0);
}

void ICamera::GetViewMatrix(Matrix & outMat)
{
	mFunction_UpdateViewMatrix();
	outMat = mMatrixView;
}

void ICamera::GetProjMatrix(Matrix & outMat)
{
	mFunction_UpdateProjMatrix();
	outMat = mMatrixProjection;
};




void	ICamera::SetViewFrustumPlane(float iNearPlaneZ, float iFarPlaneZ)
{
	if ((iNearPlaneZ >0) && (iFarPlaneZ>iNearPlaneZ))
	{
		mNearPlane = iNearPlaneZ;
		mFarPlane = iFarPlaneZ;
	}

};

void ICamera::SetViewAngle(float iViewAngleY, float iAspectRatio)
{
	if (iViewAngleY>0 && (mViewAngleY <(PI / 2.0f))) { mViewAngleY = iViewAngleY; }
	if (iAspectRatio>0) { mAspectRatio = iAspectRatio; }
};
/************************************************************************

PRIVATE

************************************************************************/

void	ICamera::mFunction_UpdateProjMatrix()
{
	mMatrixProjection = MatrixPerspectiveFovLH(
		mViewAngleY,
		mAspectRatio,
		mNearPlane,
		mFarPlane);

};

void	ICamera::mFunction_UpdateViewMatrix()
{

	Matrix	tmpMatrixTranslation;
	Matrix	tmpMatrixRotation;
	//先对齐原点

	//为什么只要三个角度就能确定视角矩阵呢  不应该还有一个up向量吗  fuck?
	//因为原先的视角矩阵就是世界矩阵 Up向量已经确定拉!!!  fuck 第二条才想明白
	tmpMatrixTranslation = MatrixTranslate(-mPosition.x, -mPosition.y, -mPosition.z);
	//然后用 yawpitchroll的逆阵 转到view空间
	tmpMatrixRotation = MatrixPitchYawRoll(mRotateX_Pitch, mRotateY_Yaw, mRotateZ_Roll);
	//正交矩阵的转置是逆
	tmpMatrixRotation = MatrixTranspose(tmpMatrixRotation);
	//先平移，再旋转 (column vector)  逆变换就是先旋转再平移
	mMatrixView = tmpMatrixTranslation*tmpMatrixRotation;
};

void	ICamera::mFunction_UpdateRotation()
{
	//主要功能：这个函数主要是为了处理Direction改变带来的姿态角变化

	//更新direction
	FLOAT3	tmpDirection;
	//检查新direction是否为0
	tmpDirection = mLookat - mPosition;
	float mLength = tmpDirection.Length();
	//注意浮点数误差，视点和位置不能重合
	if (mLength<0.001f)
	{
		// 按道理来说这里不会出现视点和位置重合的情况 因为改变方向 视点也会动
		// 不会float误差一直减小这么坑爹吧
		mRotateX_Pitch = 0;
		mRotateY_Yaw = 0;
		mRotateZ_Roll = 0;
		mDirection = FLOAT3(0.0, 0, 1.0);
		mLookat = mPosition + mDirection;
		return;
	}
	else
		//视点和位置不重合 ，再赋值
	{
		mDirection = tmpDirection;
	}
	;

	//临时变量，储存的比值用来arctan获得Pitch角度
	float tmpRatio;
	//pitch角： tan = y/sqr(x^2+z^2))
	/*	注意：	atan取值范围是 [-pi/2,pi/2]
	atan2取值范围是 [-pi,pi] 		*/
	if ((mDirection.x == 0) && (mDirection.z == 0))
	{
		//是否在原点的正上下方
		if (mDirection.y >= 0)
		{
			mRotateX_Pitch = -PI / 2;
		}
		else
		{
			mRotateX_Pitch = PI / 2;
		}
	}
	else
	{
		//去你的 难道y负的时候是正角————对的没错，DX用左手系，左手系旋转正方向用左手决定
		//用大拇指指着旋转轴正向，四指指向正旋转方向
		//卧槽 原来左手系除了四指从X 指向Y 大拇指指向Z，还有这个用处~
		tmpRatio = -mDirection.y / sqrt(pow(mDirection.x, 2.0f) + pow(mDirection.z, 2.0f));
		mRotateX_Pitch = atan(tmpRatio);//返回[-0.5pi,0.5pi]
	}

	//yaw角： tan = -x/z
	mRotateY_Yaw = atan2(mDirection.x, mDirection.z);//俯视图yaw是顺时针正角

													 //roll角：更新direction不会改变roll角 
													 //roll逆时针转是正角
};

void	ICamera::mFunction_UpdateDirection()
{
	//主要功能：这个函数主要是为了处理姿态角改变带来的视线Direction变化

	//要更新Lookat
	float tmpDirectionLength = mDirection.Length();
	//直接用三角函数表达Direction	3dscanner的世界变换一章里面有
	mDirection.x = tmpDirectionLength* sin(mRotateY_Yaw)* cos(mRotateX_Pitch);
	mDirection.z = tmpDirectionLength* cos(mRotateY_Yaw)*cos(mRotateX_Pitch);
	mDirection.y = -tmpDirectionLength* sin(mRotateX_Pitch);
	mLookat = mPosition + mDirection;
};
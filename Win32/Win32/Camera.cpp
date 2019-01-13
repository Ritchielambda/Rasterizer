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
	//lookat��λ�ò����غϰ�
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

void ICamera::SetRotation(float RX_Pitch, float RY_Yaw, float RZ_Roll)//Ҫ����Lookat
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
	//roll��������Ҫ����lookat
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
	//�ȶ���ԭ��

	//ΪʲôֻҪ�����ǶȾ���ȷ���ӽǾ�����  ��Ӧ�û���һ��up������  fuck?
	//��Ϊԭ�ȵ��ӽǾ������������� Up�����Ѿ�ȷ����!!!  fuck �ڶ�����������
	tmpMatrixTranslation = MatrixTranslate(-mPosition.x, -mPosition.y, -mPosition.z);
	//Ȼ���� yawpitchroll������ ת��view�ռ�
	tmpMatrixRotation = MatrixPitchYawRoll(mRotateX_Pitch, mRotateY_Yaw, mRotateZ_Roll);
	//���������ת������
	tmpMatrixRotation = MatrixTranspose(tmpMatrixRotation);
	//��ƽ�ƣ�����ת (column vector)  ��任��������ת��ƽ��
	mMatrixView = tmpMatrixTranslation*tmpMatrixRotation;
};

void	ICamera::mFunction_UpdateRotation()
{
	//��Ҫ���ܣ����������Ҫ��Ϊ�˴���Direction�ı��������̬�Ǳ仯

	//����direction
	FLOAT3	tmpDirection;
	//�����direction�Ƿ�Ϊ0
	tmpDirection = mLookat - mPosition;
	float mLength = tmpDirection.Length();
	//ע�⸡�������ӵ��λ�ò����غ�
	if (mLength<0.001f)
	{
		// ��������˵���ﲻ������ӵ��λ���غϵ���� ��Ϊ�ı䷽�� �ӵ�Ҳ�ᶯ
		// ����float���һֱ��С��ô�ӵ���
		mRotateX_Pitch = 0;
		mRotateY_Yaw = 0;
		mRotateZ_Roll = 0;
		mDirection = FLOAT3(0.0, 0, 1.0);
		mLookat = mPosition + mDirection;
		return;
	}
	else
		//�ӵ��λ�ò��غ� ���ٸ�ֵ
	{
		mDirection = tmpDirection;
	}
	;

	//��ʱ����������ı�ֵ����arctan���Pitch�Ƕ�
	float tmpRatio;
	//pitch�ǣ� tan = y/sqr(x^2+z^2))
	/*	ע�⣺	atanȡֵ��Χ�� [-pi/2,pi/2]
	atan2ȡֵ��Χ�� [-pi,pi] 		*/
	if ((mDirection.x == 0) && (mDirection.z == 0))
	{
		//�Ƿ���ԭ��������·�
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
		//ȥ��� �ѵ�y����ʱ�������ǡ��������Ե�û��DX������ϵ������ϵ��ת�����������־���
		//�ô�Ĵָָ����ת��������ָָ������ת����
		//�Բ� ԭ������ϵ������ָ��X ָ��Y ��Ĵָָ��Z����������ô�~
		tmpRatio = -mDirection.y / sqrt(pow(mDirection.x, 2.0f) + pow(mDirection.z, 2.0f));
		mRotateX_Pitch = atan(tmpRatio);//����[-0.5pi,0.5pi]
	}

	//yaw�ǣ� tan = -x/z
	mRotateY_Yaw = atan2(mDirection.x, mDirection.z);//����ͼyaw��˳ʱ������

													 //roll�ǣ�����direction����ı�roll�� 
													 //roll��ʱ��ת������
};

void	ICamera::mFunction_UpdateDirection()
{
	//��Ҫ���ܣ����������Ҫ��Ϊ�˴�����̬�Ǹı����������Direction�仯

	//Ҫ����Lookat
	float tmpDirectionLength = mDirection.Length();
	//ֱ�������Ǻ������Direction	3dscanner������任һ��������
	mDirection.x = tmpDirectionLength* sin(mRotateY_Yaw)* cos(mRotateX_Pitch);
	mDirection.z = tmpDirectionLength* cos(mRotateY_Yaw)*cos(mRotateX_Pitch);
	mDirection.y = -tmpDirectionLength* sin(mRotateX_Pitch);
	mLookat = mPosition + mDirection;
};
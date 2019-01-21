#pragma once


class ICamera
{
public:

	ICamera();

	void				Destroy();

	void				SetLookAt(FLOAT3 vLookat);//要更新旋转角

	void				SetLookAt(float x, float y, float z);//要更新旋转角

	FLOAT3		GetLookAt();

	void				SetPosition(FLOAT3 vPos);

	void				SetPosition(float x, float y, float z);

	FLOAT3		GetPosition();

	FLOAT3		GetDirection();
	
	void				Move(FLOAT3 vRelativePos);//pos and lookat

	void				Move(float relativeX, float relativeY, float relativeZ);

	void				SetViewFrustumPlane(float iNearPlaneZ, float iFarPlaneZ);

	void				SetViewAngle(float iViewAngleY, float iAspectRatio);

	void				SetRotation(float RX_Pitch, float RY_Yaw, float RZ_Roll);//要更新Lookat

	void				SetRotationY_Yaw(float AngleX);//用setRotation呗

	void				SetRotationX_Pitch(float AngleY);

	void				SetRotationZ_Roll(float AngleZ);

	float				GetRotationY_Yaw();

	float				GetRotationX_Pitch();

	float				GetRotationZ_Roll();

	void				RotateY_Yaw(float angleY);

	void				RotateX_Pitch(float angleX);

	void				RotateZ_Roll(float angleZ);

	void				fps_MoveForward(float fSignedDistance, BOOL enableYAxisMovement = FALSE);

	void				fps_MoveRight(float fSignedDistance, BOOL enableYAxisMovement = FALSE);

	void				fps_MoveUp(float fSignedDistance);

	void				GetViewMatrix(Matrix& outMat);

	void				GetProjMatrix(Matrix& outMat);

private:

	void		mFunction_UpdateProjMatrix();
	void		mFunction_UpdateViewMatrix();
	void		mFunction_UpdateRotation();
	void		mFunction_UpdateDirection();
	float		mViewAngleY;
	float		mAspectRatio;
	float		mNearPlane;
	float		mFarPlane;

	FLOAT3		mPosition;
	FLOAT3		mLookat;
	FLOAT3		mDirection;

	Matrix      mMatrixView;
	Matrix	    mMatrixProjection;

	float			mRotateX_Pitch;
	float			mRotateY_Yaw;
	float			mRotateZ_Roll;

};